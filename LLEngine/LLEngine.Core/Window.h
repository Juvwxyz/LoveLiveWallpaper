#pragma once

#include <Windows.h>

namespace LLEngine
{
    class Window 
    {
        static constexpr const wchar_t WndClassName[] = L"LLEngineWnd";
    public:
        Window() : Window("LLEngine Demo") {}
        Window(System::String ^ title);
        ~Window();

        bool ProcessMessage();

    protected:
        HWND hwnd;
        bool isRunning;
        static LRESULT CALLBACK WndProcSetup(HWND h, UINT msg, WPARAM wParam, LPARAM lParam);
        static LRESULT CALLBACK WndProc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam);
        virtual LRESULT wndproc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam);
    };

}