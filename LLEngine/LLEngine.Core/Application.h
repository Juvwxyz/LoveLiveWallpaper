#pragma once

namespace LLEngine
{
    using namespace System;

    class Window;
    public ref class Application abstract
    {
    public:
        Application();
        virtual ~Application();

        void Run();

    protected:

        virtual void StartUp() = 0;

        virtual void Update() = 0;

        virtual void CleanUp() = 0;

        String^ title;

    internal:
        delegate void UpdateHandler();
        static event UpdateHandler^ UpdateEvent;
        delegate void StartUpHandler();
        static event StartUpHandler^ StartUpEvent;

        inline Window* GetWnd() { return wnd; }
    private:
        static Application^ app = nullptr;
        Window* wnd;
    };
}
