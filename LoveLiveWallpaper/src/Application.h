#pragma once

#include "WallpaperWindow.h"
#include "EventSystem.h"

#include <list>
#include <chrono>
#include <memory>

namespace LLWP
{
    class GameObject;
    class Renderer;

    class Application
    {
    public:
        Application(HINSTANCE hInst, LPWSTR pArgs);
        ~Application();

        static Event<Action<>> OnStart;
        static Event<Action<>> OnUpdate;
        static Event<Action<>> OnRender;

        void Run();

    private:
        ::std::shared_ptr<GameObject> mainTachie;
        ::std::shared_ptr<GameObject> pairTachie;

        ::std::shared_ptr<GameObject> settingButton;

        static Action<> startEventHandler;
        static Action<> updateEventHandler;
        static Action<> renderEventHandler;

        WallpaperWindow wnd;

        std::chrono::system_clock::duration deltaTime;
        std::chrono::system_clock::time_point lastTime;
        int framecount;
        int framerate;

        static std::list<::std::shared_ptr<GameObject>> ObjectList_;
        //static std::list<::std::shared_ptr<Renderer>> RenderList_;

        void Update();
        void Render();

        //friend class Renderer;

        template<class OBJECT, class... Args>
        friend ::std::shared_ptr<OBJECT> CreateObject(Args&&... args_);

        friend void DestroyObject(GameObject& obj);
    };

    template<class OBJECT = GameObject, class... Args>
    ::std::shared_ptr<OBJECT> CreateObject(Args&&... args_)
    {
        ::std::shared_ptr<OBJECT> obj = std::make_shared<OBJECT>(std::forward<Args&&>(args_)...);
        obj->self_ = Application::ObjectList_.insert(Application::ObjectList_.end(), obj);
        return obj;
    }

    void DestroyObject(GameObject& obj);
}

