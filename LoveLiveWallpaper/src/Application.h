#pragma once

#include "WallpaperWindow.h"
#include "EventSystem.h"

#include <list>

namespace LLWP
{
    using StartEventHandler = Delegate<void()>;

    class GameObject;
    class Renderer;

    class Application
    {
    public:
        Application(HINSTANCE hInst, LPWSTR pArgs);
        ~Application();

        void Run();

    private:
        GameObject& mainTachie;
        GameObject& pairTachie;

        GameObject& settingButton;

        StartEventHandler startEventHandler;

        WallpaperWindow wnd;

        static std::list<GameObject*> ObjectList_;
        static std::list<Renderer*> RenderList_;

        void Update();
        void Render();

        friend class Renderer;

        template<class OBJECT, class... Args>
        friend OBJECT& CreateObject(Args&... args_);

        friend void DestroyObject(GameObject& obj);
    };

    template<class OBJECT = GameObject, class... Args>
    OBJECT& CreateObject(Args&... args_)
    {
        OBJECT* obj = new OBJECT(std::move(args_)...);
        obj->self_ = Application::ObjectList_.insert(Application::ObjectList_.end(), obj);
        return *obj;
    }

    void DestroyObject(GameObject& obj);
}

