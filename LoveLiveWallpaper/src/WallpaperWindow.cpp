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
    bool WallpaperWindow::isExitting = false;

    // �������ʼ��
    WallpaperWindow::WallpaperWindow(HINSTANCE hInst, LPWSTR pArgs) :
        hInst(hInst),
        msg{ 0 }
    {
        DISPLAY_DEVICEW displayDevice{ sizeof(DISPLAY_DEVICEW) };

        // ö����ʾ�豸
        EnumDisplayDevicesW(nullptr, 0, &displayDevice, 0);

        DEVMODEW devMode{ 0 };

        // ö����ʾ�豸����
        EnumDisplaySettingsW(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode);

        // ��ȡ��Ļ����ֱ���
        screenX = devMode.dmPelsWidth;
        screenY = devMode.dmPelsHeight;

        // ��������Ϣ
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

        // ע�ᴰ����
        ATOM reg = RegisterClassExW(&wc);

        // ��������
        hWnd = CreateWindowExW
        (
            0L,
            wndClassName,
            wndTitle,
            WS_POPUP | WS_VISIBLE | WS_CHILD,
            0, 0, screenX, screenY,
            nullptr, nullptr, hInst, (void*)reg
        );

        // ��ȡ���汳�����ھ�� Progman��Windows����ı����ͷ������������
        Progman = FindWindow(L"Progman", nullptr);
        // ���ش��� 0x052c 
        SendMessageTimeoutW(Progman, 0x052c, 0x0, 0x0, SMTO_NORMAL, 5000, nullptr);
        EnumWindows((WNDENUMPROC)EnumWindowsProc, 0x0);

        // ����Win10��Win7��Ҫ����ֽ���ڷ��ڲ�ͬ��λ��
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
            MessageBoxW(NULL, L"����֧�ֵĲ���ϵͳ", L"LLWP Error", 0);
            exit(0);
        }

        // ��ʾ�����´���
        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);

        // ϵͳ����ͼ����Ϣ
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

        // ����ϵͳ����ͼ����Ҽ��˵�
        hMenu = CreatePopupMenu();

        AppendMenuW(hMenu, MF_STRING, 0x0061, L"�˳�");
        AppendMenuW(hMenu, MF_STRING, 0x0062, L"����");

        // ��ʾϵͳ����ͼ��
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

    // ��Ϣ����
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

    // ��Ϣ����ص�������ֻ��Ҫ��������ͼ����Ϣ���ɣ������Ϣ�ڹ��ӻص��д���
    LRESULT CALLBACK WallpaperWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        // �Զ�������ͼ����Ϣ
        case 0x1234:
        {
            switch (lParam)
            {
            case WM_RBUTTONUP:
            {
                POINT pt;
                GetCursorPos(&pt);

                SetForegroundWindow(WallpaperWindow::hWnd);

                // TrackPopupMenu��ȴ�����ֵ�������Ⱦ��Ҫ�ŵ���һ���߳��У����򵯳��˵�����Ῠ��
                int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hWnd, NULL);

                // �Զ���˵�����
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

    // ��װ��깳�ӡ�����������ڳ�ʼ��DirectX�Լ�������ͼ���֮�󱻵���
    // �����ʼ��DirectX�Լ�������ͼ�Ĺ��������Ῠ��
    void WallpaperWindow::CaptureMouse()
    {
        mHook = SetWindowsHookExW(WH_MOUSE_LL, (HOOKPROC)MouseProc, GetModuleHandleW(nullptr), 0);
    }

    // ����ö�ٻص�
    // ��Progman����0x052c��Ϣ֮�󣬻��������WorkerW����
    // ����һ����������ͼ�꣬��һ���ǿյģ��������л���ֽʱʵ�ֵ���Ч��
    // ������Ҫ�����ǵı�ֽ���ڷŵ�����յ�WorkerW��
    BOOL WallpaperWindow::EnumWindowsProc(HWND tophandle, HWND topparahandle)
    {
        HWND p = FindWindowExW(tophandle, nullptr, L"SHELLDLL_DefView", nullptr);
        if (p != nullptr) {
            workerw = FindWindowExW(nullptr, tophandle, L"WorkerW", nullptr);
            return false;
        }
        return true;
    }

    // ��깳�ӻص�����
    LRESULT WallpaperWindow::MouseProc(int nCode, WPARAM wPara, LPARAM lParam)
    {
        if (isExitting)
        {
            return CallNextHookEx(nullptr, nCode, wPara, lParam);
        }
#ifdef _DEBUG

        UnhookWindowsHookEx(mHook);

#endif // _DEBUG

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
#ifdef _DEBUG

        mHook = SetWindowsHookExW(WH_MOUSE_LL, (HOOKPROC)MouseProc, GetModuleHandleW(nullptr), 0);

#endif // DEBUG

        return CallNextHookEx(nullptr, nCode, wPara, lParam);
    }
}