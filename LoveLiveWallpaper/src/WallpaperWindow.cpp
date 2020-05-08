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

    WallpaperWindow::WallpaperWindow(HINSTANCE hInst, LPWSTR pArgs) :
        hInst(hInst),
        msg{ 0 },
        isExitting(false)
    {
        DISPLAY_DEVICEW displayDevice{ sizeof(DISPLAY_DEVICEW) };

        EnumDisplayDevicesW(nullptr, 0, &displayDevice, 0);

        DEVMODEW devMode{ 0 };

        EnumDisplaySettingsW(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode);

        screenX = devMode.dmPelsWidth;
        screenY = devMode.dmPelsHeight;

        WNDCLASSEX wc =
        {
            sizeof(WNDCLASSEX),
            CS_CLASSDC,
            WndProc,
            0,
            0,
            hInst,
            LoadIconW(nullptr, MAKEINTRESOURCEW(IDI_APPLICATION)),
            LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_ARROW)),
            CreateSolidBrush(0xffffff),
            nullptr,
            wndClassName,
            nullptr
        };

        ATOM reg = RegisterClassExW(&wc);

        hWnd = CreateWindowExW
        (
            0L,
            wndClassName,
            wndTitle,
            WS_POPUP | WS_VISIBLE | WS_CHILD,
            0, 0, screenX, screenY,
            nullptr, nullptr, hInst, (void*)reg
        );

        GetWorkerW();

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

        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        nid =
        {
            sizeof(NOTIFYICONDATAW),
            hWnd,
            0x0060,
            NIF_TIP | NIF_ICON | NIF_MESSAGE | NIF_INFO,
            0x1234,
            LoadIconW(nullptr, MAKEINTRESOURCEW(IDI_APPLICATION)),
            L"LoveLive! Wallpaper"
        };

        hMenu = CreatePopupMenu();

        AppendMenuW(hMenu, MF_STRING, 0x0061, L"退出");
        AppendMenuW(hMenu, MF_STRING, 0x0062, L"关于");

        Shell_NotifyIconW(NIM_ADD, &nid);
        
        //mHook = SetWindowsHookExW(WH_MOUSE_LL, (HOOKPROC)MouseProc, GetModuleHandleW(nullptr), 0);
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

    LRESULT CALLBACK WallpaperWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case 0x1234:
        {
            switch (lParam)
            {
            case WM_RBUTTONUP:
            {
                POINT pt;
                GetCursorPos(&pt);

                SetForegroundWindow(WallpaperWindow::hWnd);

                int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hWnd, NULL);

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

    void WallpaperWindow::GetWorkerW()
    {
        Progman = FindWindow(L"Progman", nullptr);
        PDWORD_PTR result = nullptr;
        SendMessageTimeoutW(Progman, 0x052c, 0x0, 0x0, SMTO_NORMAL, 5000, result);
        EnumWindows((WNDENUMPROC)EnumWindowsProc, 0x0);
    }

    void WallpaperWindow::CaptureMouse()
    {
        mHook = SetWindowsHookExW(WH_MOUSE_LL, (HOOKPROC)MouseProc, GetModuleHandleW(nullptr), 0);
    }

    BOOL WallpaperWindow::EnumWindowsProc(HWND tophandle, HWND topparahandle)
    {
        HWND p = FindWindowExW(tophandle, nullptr, L"SHELLDLL_DefView", nullptr);
        if (p != nullptr) {
            workerw = FindWindowExW(nullptr, tophandle, L"WorkerW", nullptr);
            return false;
        }
        return true;
    }

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