#include "Application.h"

using namespace LLWP;

DWORD RenderThreadProc(LPVOID wnd);
void StartApplication(void* wnd);

HANDLE hApplication;
DWORD appThreadID;

int WINAPI wWinMain(
    _In_ HINSTANCE hInst,
    _In_opt_ HINSTANCE,
    _In_ LPWSTR pArgs,
    _In_ int)
{
    // 初始化
    Application app(hInst, pArgs);

    // 在另一个线程中启动渲染
    StartApplication(&app);

    // 消息循环
    while (app.wnd.ProcessMessage()) {}

    // 等待渲染线程退出
    WaitForSingleObject(hApplication, 5000);

    return 0;
}

void StartApplication(void* app)
{
    SECURITY_ATTRIBUTES sa{ 0 };
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = nullptr;
    sa.bInheritHandle = FALSE;
    hApplication = CreateThread(&sa, 0, RenderThreadProc, app, 0, &appThreadID);

    // 为了方便调试，暂时不捕获鼠标事件
    ((Application*)app)->wnd.CaptureMouse();
}

DWORD RenderThreadProc(LPVOID app)
{
    ((Application*)app)->Run();
    return 0;
}