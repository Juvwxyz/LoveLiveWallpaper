#include "Mouse.h"
#include "WallpaperWindow.h"

namespace LLWP
{
    HWND WallpaperWindow::hWnd = nullptr;
    HWND WallpaperWindow::workerw = nullptr;
    HWND WallpaperWindow::Progman = nullptr;
    HHOOK WallpaperWindow::mHook = nullptr;

    WallpaperWindow::WallpaperWindow(HINSTANCE hInst, LPWSTR pArgs) :
        hInst(hInst),
        msg{ 0 }
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
            nullptr,
            nullptr,
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

        SetParent(hWnd, workerw);
        //SetParent(hWnd, Progman);

        SetWindowPos(hWnd, workerw, 0, 0, screenX, screenY, 0);
        //SetWindowPos(hWnd, Progman, 0, 0, screenX, screenY, 0);

        SetWindowRgn(workerw, CreateRectRgn(0, 0, screenX, screenY), true);

        ShowWindow(hWnd, SW_SHOWDEFAULT);
        UpdateWindow(hWnd);
        
        mHook = SetWindowsHookExW(WH_MOUSE_LL, (HOOKPROC)MouseProc, GetModuleHandleW(nullptr), 0);
    }

    WallpaperWindow::~WallpaperWindow()
    {
        //SetWindowRgn(workerw, CreateRectRgn(0,0,0,0), true);
        ShowWindow(workerw, SW_HIDE);
        UnregisterClassW(wndClassName, hInst);
        UnhookWindowsHookEx(mHook);
    }

    bool WallpaperWindow::ProcessMessage()
    {
        PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE);
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
        if (msg.message == WM_QUIT)
        {
            return false;
        }
        return true;
    }

    LRESULT CALLBACK WallpaperWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }

    void WallpaperWindow::GetWorkerW()
    {
        Progman = FindWindow(L"Progman", nullptr);
        PDWORD_PTR result = nullptr;
        SendMessageTimeoutW(Progman, 0x052c, 0x0, 0x0, SMTO_NORMAL, 5000, result);
        EnumWindows((WNDENUMPROC)EnumWindowsProc, 0x0);
        ShowWindow(workerw, SW_SHOW);
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
        case WM_MBUTTONDOWN:
            PostQuitMessage(0);
            break;
        }

        //mHook = SetWindowsHookExW(WH_MOUSE_LL, (HOOKPROC)MousHook, GetModuleHandleW(nullptr), 0);

        return CallNextHookEx(nullptr, nCode, wPara, lParam);
    }
}