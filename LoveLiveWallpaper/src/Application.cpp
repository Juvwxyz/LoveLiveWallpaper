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

namespace LLWP
{
    std::list<GameObject*> Application::ObjectList_;
    std::list<Renderer*> Application::RenderList_;

    Application::Application(HINSTANCE hInst, LPWSTR pArgs) :
        mainTachie(CreateObject("MainTachie")),
        pairTachie(CreateObject("PairTachie")),
        settingButton(CreateObject("SettingButton")),
        wnd(hInst, pArgs)
    {
        Graphics::Init(wnd);

        mainTachie.tranform().Size() = { 1024,1024 };

        mainTachie.AddComponent<SpriteRenderer>(L"Assets\\u_normal_navi_43001003.png");
        mainTachie.AddComponent<Raycaster>();
        mainTachie.AddComponent<Tachie>();

        pairTachie.tranform().Size() = { 1024,1024 };

        pairTachie.AddComponent<SpriteRenderer>(L"Assets\\u_normal_navi_43001003.png");
        pairTachie.AddComponent<Raycaster>();
        pairTachie.AddComponent<Tachie>();
        pairTachie.tranform().Move(1000, 0);

        settingButton.tranform().Size() = { 50,50 };

        settingButton.AddComponent<SpriteRenderer>(L"Assets\\settingbutton.png");
        settingButton.AddComponent<Button>();

        settingButton.tranform().Move(1850, 20);
    }

    void Application::Update()
    {
    }

    Application::~Application() {}

    void Application::Run()
    {
        while (wnd.ProcessMessage())
        {
            Update();
            Render();
        }
    }
    void Application::Render()
    {
        Graphics::D2DContext->BeginDraw();
        Graphics::D2DContext->Clear(D2D1_COLOR_F{ 0.5f, 0.5f ,0.5f ,1.0f });
        for (auto render = RenderList_.begin(); render !=   RenderList_.end(); render++)
        {
            (*render)->Render();
        }
        Graphics::D2DContext->EndDraw();
        Graphics::DXGISwapChain->Present(1, 0);
    }
    void DestroyObject(GameObject& obj)
    {
        auto iter = obj.self_;
        delete (*iter);
        Application::ObjectList_.erase(iter);
    }
}