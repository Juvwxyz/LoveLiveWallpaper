#include "Mouse.h"
#include "WallpaperWindow.h"
#include <VersionHelpers.h>
namespace LLWP
{
    HWND WallpaperWindow::hWnd = nullptr;
    HMENU WallpaperWindow::hMenu = nullptr;
    HWND WallpaperWindow::workerw = nullptr;
    HWND WallpaperWindow::Progman = nullptr;
    HHOOK WallpaperWindow::mHook = nullptr;

    // 窗口类初始化
    WallpaperWindow::WallpaperWindow(HINSTANCE hInst, LPWSTR pArgs) :
        hInst(hInst),
        msg{ 0 },
        isExitting(false)
    {
        DISPLAY_DEVICEW displayDevice{ sizeof(DISPLAY_DEVICEW) };

        // 枚举显示设备
        EnumDisplayDevicesW(nullptr, 0, &displayDevice, 0);

        DEVMODEW devMode{ 0 };

        // 枚举显示设备设置
        EnumDisplaySettingsW(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode);

        // 获取屏幕物理分辨率
        screenX = devMode.dmPelsWidth;
        screenY = devMode.dmPelsHeight;

        // 窗口类信息
        WNDCLASSEX wc =
        {
            sizeof(WNDCLASSEX),
            CS_CLASSDC,
            WndProc,
            0,
            0,
            hInst,
            LoadIconW(nullptr, IDI_APPLICATION),
            LoadCursorW(nullptr, IDC_ARROW),
            CreateSolidBrush(0xffffff),
            nullptr,
            wndClassName,
            nullptr
        };

        // 注册窗口类
        ATOM reg = RegisterClassExW(&wc);

        // 创建窗口
        hWnd = CreateWindowExW
        (
            0L,
            wndClassName,
            wndTitle,
            WS_POPUP | WS_VISIBLE | WS_CHILD,
            0, 0, screenX, screenY,
            nullptr, nullptr, hInst, (void*)reg
        );

        // 获取桌面背景窗口句柄 Progman，Windows桌面的背景就放在这个窗口中
        Progman = FindWindow(L"Progman", nullptr);
        // 神秘代码 0x052c 
        SendMessageTimeoutW(Progman, 0x052c, 0x0, 0x0, SMTO_NORMAL, 5000, nullptr);
        EnumWindows((WNDENUMPROC)EnumWindowsProc, 0x0);

        // 对于Win10和Win7，要将壁纸窗口放在不同的位置
        if (IsWindows10OrGreater())
        {
            SetParent(hWnd, workerw);
            SetWindowPos(hWnd, workerw, 0, 0, screenX, screenY, 0);
            ShowWindow(workerw, SW_SHOW);
            SetWindowRgn(workerw, CreateRectRgn(0, 0, screenX, screenY), true);
        }
        else if (IsWindows7SP1OrGreater())
        {
            SetParent(hWnd, Progman);
            SetWindowPos(hWnd, Progman, 0, 0, screenX, screenY, 0);
            ShowWindow(workerw, SW_HIDE);
        }
        else
        {
            MessageBoxW(NULL, L"不受支持的操作系统", L"LLWP Error", 0);
            exit(0);
        }

        // 显示并更新窗口
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        // 系统托盘图标信息
        nid =
        {
            sizeof(NOTIFYICONDATAW),
            hWnd,
            0x0060,
            NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_INFO,
            0x1234,
            LoadIconW(nullptr, IDI_APPLICATION),
            L"LoveLive! Wallpaper"
        };

        // 创建系统托盘图标的右键菜单
        hMenu = CreatePopupMenu();

        AppendMenuW(hMenu, MF_STRING, 0x0061, L"退出");
        AppendMenuW(hMenu, MF_STRING, 0x0062, L"关于");

        // 显示系统托盘图标
        Shell_NotifyIconW(NIM_ADD, &nid);
    }

    WallpaperWindow::~WallpaperWindow()
    {
        if (IsWindows10OrGreater())
        {
            SetWindowRgn(workerw, CreateRectRgn(0, 0, 0, 0), true);
        }
        UnregisterClassW(wndClassName, hInst);
        UnhookWindowsHookEx(mHook);
    }

    // 消息处理
    bool WallpaperWindow::ProcessMessage()
    {
        if (GetMessageW(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            return true;
        }
        else
        {
            Shell_NotifyIconW(NIM_DELETE, &nid);
            isExitting = true;
            return false;
        }
    }

    // 消息处理回调，这里只需要处理托盘图标消息即可，鼠标消息在钩子回调中处理
    LRESULT CALLBACK WallpaperWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        // 自定义托盘图标消息
        case 0x1234:
        {
            switch (lParam)
            {
            case WM_RBUTTONUP:
            {
                POINT pt;
                GetCursorPos(&pt);

                SetForegroundWindow(WallpaperWindow::hWnd);

                // TrackPopupMenu会等待返回值，因此渲染需要放到另一个线程中，否则弹出菜单后画面会卡主
                int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hWnd, NULL);

                // 自定义菜单命令
                if (cmd == 0x0061)
                {
                    PostQuitMessage(0);
                }
                else if (cmd == 0x0062)
                {
                    MessageBoxW(WallpaperWindow::hWnd, L"LoveLive! Wallpaper\nversion 0.0.0.1", L"About LoveLive! Wallpaper", 0);
                }
            }
            break;
            default:
                break;
            }
        }
            break;
        default:
            break;
        }
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }

    // 安装鼠标钩子。这个函数会在初始化DirectX以及载入贴图完毕之后被调用
    // 否则初始化DirectX以及载入贴图的过程中鼠标会卡顿
    void WallpaperWindow::CaptureMouse()
    {
        mHook = SetWindowsHookExW(WH_MOUSE_LL, (HOOKPROC)MouseProc, GetModuleHandleW(nullptr), 0);
    }

    // 窗口枚举回调
    // 向Progman发送0x052c消息之后，会出现两个WorkerW窗口
    // 其中一个放着桌面图标，另一个是空的，用于在切换壁纸时实现淡出效果
    // 我们需要把我们的壁纸窗口放到这个空的WorkerW下
    BOOL WallpaperWindow::EnumWindowsProc(HWND tophandle, HWND topparahandle)
    {
        HWND p = FindWindowExW(tophandle, nullptr, L"SHELLDLL_DefView", nullptr);
        if (p != nullptr) {
            workerw = FindWindowExW(nullptr, tophandle, L"WorkerW", nullptr);
            return false;
        }
        return true;
    }

    // 鼠标钩子回调函数
    LRESULT WallpaperWindow::MouseProc(int nCode, WPARAM wPara, LPARAM lParam)
    {
        //UnhookWindowsHookEx(mHook);

        MSLLHOOKSTRUCT* mshook = (MSLLHOOKSTRUCT*)lParam;
        
        switch (wPara)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MOUSEMOVE:
        case WM_MOUSEWHEEL:
            Mouse::ProcessMouseEvent(wPara, lParam);
            break;
        default:
            break;
        }

        //mHook = SetWindowsHookExW(WH_MOUSE_LL, (HOOKPROC)MousHook, GetModuleHandleW(nullptr), 0);

        return CallNextHookEx(nullptr, nCode, wPara, lParam);
    }
}