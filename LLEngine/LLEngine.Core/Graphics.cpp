#include "Graphics.h"
#include "DxEnv.h"
#include "Application.h"
#include "Window.h"

using namespace System;

namespace LLEngine
{
    void Graphics::Initialize(Application^ app)
    {
        Console::WriteLine("Initialize Graphcis...");

        DxEnv::Initialize(app->GetWnd()->GetHWND());
    }
}