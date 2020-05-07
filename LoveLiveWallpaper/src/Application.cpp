#include "Application.h"

#include "Behaviour.h"
#include "Button.h"
#include "Graphics.h"
#include "GameObject.h"
#include "Image.h"
#include "Mouse.h"
#include "Raycaster.h"
#include "SpriteRenderer.h"
#include "Tachie.h"
#include "Rotater.h"

using namespace ::std::chrono;

namespace LLWP
{
    std::list<::std::shared_ptr<GameObject>> Application::ObjectList_;
    Action<> Application::startEventHandler;
    Action<> Application::updateEventHandler;
    Action<> Application::renderEventHandler;
    Event<Action<>> Application::OnStart(Application::startEventHandler);
    Event<Action<>> Application::OnUpdate(Application::updateEventHandler);
    Event<Action<>> Application::OnRender(Application::renderEventHandler);

    Application::Application(HINSTANCE hInst, LPWSTR pArgs) :
        mainTachie(CreateObject("MainTachie")),
        pairTachie(CreateObject("PairTachie")),
        settingButton(CreateObject("SettingButton")),
        deltaTime(0),
        lastTime(system_clock::now()),
        framecount(0),
        framerate(0),
        wnd(hInst, pArgs)
    {
        Graphics::Init(wnd);

        mainTachie->tranform().Size() = { 1024,1024 };

        mainTachie->AddComponent<SpriteRenderer>(L"Assets\\u_normal_navi_43001003.png");
        mainTachie->AddComponent<Raycaster>();
        mainTachie->AddComponent<Tachie>();
        mainTachie->tranform().Move(10, 28);

        pairTachie->tranform().Size() = { 1024,1024 };

        pairTachie->AddComponent<SpriteRenderer>(L"Assets\\u_normal_navi_43001003.png");
        pairTachie->AddComponent<Raycaster>();
        pairTachie->AddComponent<Tachie>();
        pairTachie->tranform().Move(900, 28);

        settingButton->tranform().Size() = { 50,50 };

        settingButton->AddComponent<SpriteRenderer>(L"Assets\\settingbutton.png");
        settingButton->AddComponent<Button>();
        settingButton->AddComponent<Rotater>();

        settingButton->tranform().Move(1850, 20);

        auto tachie = mainTachie->GetComponent<Tachie>();
    }

    void Application::Update()
    {
        updateEventHandler.invoke();
    }

    Application::~Application()
    {
        ObjectList_.clear();
    }

    void Application::Run()
    {
        while (wnd.ProcessMessage())
        {
            startEventHandler.invoke();
            Update();
            Render();
        }
    }
    void Application::Render()
    {
        Graphics::D2DContext->BeginDraw();
        Graphics::D2DContext->Clear(D2D1_COLOR_F{ 0.5f, 0.5f ,0.5f ,1.0f });

        renderEventHandler.invoke();
        //for (auto render = RenderList_.begin(); render !=   RenderList_.end(); render++)
        //{
        //    (*render)->Render();
        //}
        //Graphics::D2DContext->FillRectangle(
        //    { 1850, 20,1914,52 },
        //    Graphics::blackBrush.Get()
        //);
        auto a = std::to_wstring(framerate);
        Graphics::D2DContext->DrawTextW(
            a.c_str(),
            a.size(),
            Graphics::Dwriteformat.Get(),
            { 1853, 70,1914,102 },
            Graphics::whiteBrush.Get()
        );
        deltaTime = system_clock::now() - lastTime;
        framecount++;
        if (duration_cast<milliseconds>(deltaTime).count() >= 1000)
        {
            framerate = framecount;
            framecount = 0;
            lastTime = system_clock::now();
        }
        Graphics::D2DContext->EndDraw();
        Graphics::DXGISwapChain->Present(0, 0);
    }
    void DestroyObject(GameObject& obj)
    {
        auto iter = obj.self_;
        Application::ObjectList_.erase(iter);
    }
}