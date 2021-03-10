#include "Window.h"

#include <vcclr.h>

namespace LLEngine
{
    Window::Window(System::String^ title) :
        isRunning(true)
    {
        WNDCLASSEXW wnd_class
        {
            sizeof(WNDCLASSEXW),
            0U, (WNDPROC)Window::WndProcSetup,
            0, 0,
            GetModuleHandleW(nullptr),
            LoadIconW(nullptr, IDI_APPLICATION),
            LoadCursorW(nullptr, IDC_ARROW),
            CreateSolidBrush(0x808080),
            nullptr,
            WndClassName,
            nullptr
        };

        RegisterClassExW(&wnd_class);

        RECT wndRect{ 320, 180, 320 + 1280, 180 + 720 };
        AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, false);

        const wchar_t* title_str;
        if (!title)
        {
            title_str = L"LLEngine Demo";
        }
        else
        {
            pin_ptr<const wchar_t> pined = PtrToStringChars(title);
            title_str = (const wchar_t*)pined;
        }

        hwnd = CreateWindowExW(
            0u, WndClassName,
            title_str,
            WS_OVERLAPPEDWINDOW,
            320, 180, 1280, 720,
            nullptr, nullptr,
            GetModuleHandleW(nullptr),
            this
        );

        UpdateWindow(hwnd);
        ShowWindow(hwnd, SW_SHOW);
    }

    Window::~Window()
    {
        UnregisterClassW(WndClassName, GetModuleHandleW(nullptr));
    }

    bool Window::ProcessMessage()
    {
        MSG msg{};
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            DispatchMessageW(&msg);
        }
        return isRunning;
    }

    LRESULT CALLBACK Window::WndProcSetup(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (msg == WM_NCCREATE)
        {
            CREATESTRUCTW* create_info = reinterpret_cast<CREATESTRUCTW*>(lParam);
            LONG_PTR wnd_ptr = reinterpret_cast<LONG_PTR>(create_info->lpCreateParams);
            LONG_PTR wnd_proc = reinterpret_cast<LONG_PTR>(&Window::WndProc);

            SetWindowLongPtrW(h, GWLP_WNDPROC, wnd_proc);
            SetWindowLongPtrW(h, GWLP_USERDATA, wnd_ptr);
        }
        return DefWindowProcW(h, msg, wParam, lParam);
    }
    LRESULT CALLBACK Window::WndProc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        auto user_data = GetWindowLongPtrW(h, GWLP_USERDATA);
        Window* wnd_ptr = reinterpret_cast<Window*>(user_data);
        return wnd_ptr->wndproc(h, msg, wParam, lParam);
    }
    LRESULT Window::wndproc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            isRunning = false;
            return 0;
        default:
            return DefWindowProcW(h, msg, wParam, lParam);
        }
    }
}