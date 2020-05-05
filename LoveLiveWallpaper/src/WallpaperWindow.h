#pragma once

#include "winhead.h"

namespace LLWP
{
    class WallpaperWindow
    {
    public:

        WallpaperWindow(HINSTANCE, LPWSTR);
        ~WallpaperWindow();

        bool ProcessMessage();
        HWND hwnd() const { return hWnd; }

    private:

        HINSTANCE hInst;
        MSG msg;
        RECT wr;

        int screenX, screenY;

        static HWND hWnd;

        static constexpr wchar_t wndClassName[] = L"LLWP";
        static constexpr wchar_t wndTitle[] = L"LoveLive! Wallpaper";

        static HWND workerw;
        static HWND Progman;
        static HHOOK mHook;

        static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        static LRESULT CALLBACK MouseProc(int nCode, WPARAM wPara, LPARAM lParam);

        static BOOL CALLBACK EnumWindowsProc(HWND tophandle, HWND topparahandle);

        static void GetWorkerW();
    };

}