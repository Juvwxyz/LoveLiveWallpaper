#include "Application.h"

#include <shlobj_core.h>

#include "Behaviour.h"
#include "Button.h"
#include "Exception.h"
#include "Graphics.h"
#include "GameObject.h"
#include "Image.h"
#include "Mouse.h"
#include "Raycaster.h"
#include "CustomRenderer.h"
#include "SpriteRenderer.h"
#include "Tachie.h"
#include "Rotater.h"
#include "UserSettings.h"

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

    ::std::wstring Application::ProfilePath;

    Application::Application(HINSTANCE hInst, LPWSTR pArgs) :
        mainTachie(CreateObject("MainTachie")),
        pairTachie(CreateObject("PairTachie")),
        innerCircle(CreateObject("InnerCircle")),
        outerCircle(CreateObject("OuterCircle")),
        settingButton(CreateObject("SettingButton")),
        deltaTime(0),
        lastTime(system_clock::now()),
        framecount(0),
        framerate(0),
        wnd(hInst, pArgs)
    {
        ProfilePath.resize(MAX_PATH);
        SHGetFolderPathW(wnd.hWnd, CSIDL_LOCAL_APPDATA, nullptr, 0, ProfilePath.data());
        ProfilePath.resize(lstrlenW(ProfilePath.data()));
        ProfilePath += L"\\LoveLiveWallpaper";

        int result = SHCreateDirectory(wnd.hWnd, ProfilePath.c_str());

        UserSettings userSettings;

        // 初始化DirectX环境
        HRESULT hr = Graphics::Init(wnd);
        if (hr != S_OK) throw GraphicException();

        auto sr = innerCircle->AddComponent<CustomRenderer>(L"Assets\\InnerCircle.png", DirectX::XMFLOAT4{ .25f,.25f,.25f,1});
        auto r = innerCircle->AddComponent<Rotater>();
        r->setSpeed(-30);
        innerCircle->transform().Move(480, 0);
        innerCircle->transform().ScaleBy(0.75f);

        sr = outerCircle->AddComponent<CustomRenderer>(L"Assets\\OuterCircle.png", DirectX::XMFLOAT4{ .25f,.25f,.25f,1 });
        r = outerCircle->AddComponent<Rotater>();
        r->setSpeed(30);
        outerCircle->transform().Move(480, 0);
        outerCircle->transform().ScaleBy(0.75f);

        auto renderer = mainTachie->AddComponent<CustomRenderer>(L"Assets\\u_normal_navi_43001003.png");
        mainTachie->AddComponent<Raycaster>();
        auto mtachie = mainTachie->AddComponent<Tachie>();
        mainTachie->transform().Move(-462, -8);
        mainTachie->transform().size() = { 1024,1024 };

        mtachie->renderer = renderer;

        renderer = pairTachie->AddComponent<CustomRenderer>(L"Assets\\u_normal_navi_32009012.png");
        pairTachie->AddComponent<Raycaster>();
        auto ptachie = pairTachie->AddComponent<Tachie>();
        pairTachie->transform().Move(462, 0);
        pairTachie->transform().size() = { 1024,1024 };

        ptachie->renderer = renderer;

        renderer = settingButton->AddComponent<CustomRenderer>(L"Assets\\settingbutton.png");
        auto button = settingButton->AddComponent<Button>();
        settingButton->transform().size() = { 50,50 };
        settingButton->transform().Move(915, 495);

        button->OnClick += (*ptachie, &Tachie::OnSettingButtonClick);
        button->OnClick += (*mtachie, &Tachie::OnSettingButtonClick);
        button->renderer = renderer;
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
        startEventHandler.invoke();
        while (!wnd.isExitting)
        {
            Update();
            Render();
        }
    }

    void Application::Render()
    {
        HRESULT hr = S_OK;
        Graphics::D3DContext->OMSetRenderTargets(1, Graphics::D3DRenderTargetView.GetAddressOf(), nullptr);
        float background[4] = { 0.5f, 0.5f ,0.5f ,1.f };
        Graphics::D3DContext->ClearRenderTargetView(Graphics::D3DRenderTargetView.Get(), background);

        //Graphics::D2DContext->BeginDraw();
        //Graphics::D2DContext->Clear({ 0.5f, 0.5f ,0.5f ,1.f });
        //hr = Graphics::D2DContext->EndDraw();

        renderEventHandler.invoke();

        Graphics::D2DContext->BeginDraw();
        auto a = std::to_wstring(framerate);
        Graphics::D2DContext->DrawTextW(
            a.c_str(),
            (UINT32)a.size(),
            Graphics::Dwriteformat.Get(),
            {
                1853 * Graphics::getScaleFactor(),
                70 * Graphics::getScaleFactor(),
                1914 * Graphics::getScaleFactor(),
                102 * Graphics::getScaleFactor()
            },
            Graphics::whiteBrush.Get()
        );
        hr = Graphics::D2DContext->EndDraw();

        deltaTime = system_clock::now() - lastTime;
        framecount++;
        if (duration_cast<milliseconds>(deltaTime).count() >= 1000)
        {
            framerate = framecount;
            framecount = 0;
            lastTime = system_clock::now();
        }

        Graphics::DXGISwapChain->Present(1, 0);
    }
    void DestroyObject(GameObject& obj)
    {
        auto iter = obj.self_;
        Application::ObjectList_.erase(iter);
    }
}