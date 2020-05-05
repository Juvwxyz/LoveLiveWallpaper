#include "Application.h"

using namespace LLWP;

int WINAPI wWinMain(
    _In_ HINSTANCE hInst,
    _In_opt_ HINSTANCE,
    _In_ LPWSTR pArgs,
    _In_ int)
{
    Application app(hInst, pArgs);

    app.Run();

    return 0;
}