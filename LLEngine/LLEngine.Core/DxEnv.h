#pragma once

#include <Windows.h>

#define LL_IID_PPV_ARGS(x) __uuidof(**(x)), (void**)(x)

namespace LLEngine
{
    namespace DxEnv
    {
        void Initialize(HWND hwnd);
    };

}