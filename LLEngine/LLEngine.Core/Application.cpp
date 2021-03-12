#include "Application.h"
#include "Window.h"
#include "Graphics.h"

#include <vcclr.h>

using namespace System;
using namespace System::Runtime;

namespace LLEngine
{
    Application::Application()
    {
        if (Application::app)
        {
            throw gcnew Exception("Do not create two Application instances!");
        }
        Application::app = this;
        title = "LLEngine Demo";
    }

    void Application::Run()
    {
        wnd = new Window(title);

        Graphics::Initialize(this);

        Console::WriteLine("{0} starting up...", title);
        this->StartUp();

        Console::WriteLine("{0} is running...", title);
        while (wnd->ProcessMessage())
        {
            this->Update();
            this->UpdateEvent();
        }

        Console::WriteLine("{0} cleaning up...", title);
        this->CleanUp();
    }

    Application::~Application()
    {
        delete wnd;
    }
}