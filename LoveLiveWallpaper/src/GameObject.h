#pragma once
#include "Exception.h"
#include "Transform.h"

#include <list>
#include <memory>
#include <string>
#include <typeinfo>

namespace LLWP
{
    class Component;
    class MouseEventArg;

    class GameObject
    {
    public:
        GameObject();
        GameObject(const std::string& name);

        Transform& tranform();
        virtual ~GameObject();
    protected:

        std::string name_;
        Transform transform_;

        std::list<GameObject*>::iterator self_;
        std::list<Component*> componentList_;

        friend class Transform;
        template<class OBJECT, class... Args>
        friend OBJECT& CreateObject(Args&... args_);
        friend void DestroyObject(GameObject& obj);

        bool OnHitTest(const MouseEventArg& e);

    public:
        template<class COMPONENT, class... Args>
        COMPONENT& AddComponent(const Args&... args_)
        {
            COMPONENT* ptr = new COMPONENT(*this, args_...);
            componentList_.push_back(ptr);
            return *ptr;
        }

        template<class COMPONENT>
        COMPONENT* GetComponent()
        {
            for (auto com = componentList_.begin(); com != componentList_.end(); com++)
            {
                if (typeid(**com) == typeid(COMPONENT))
                {
                    return reinterpret_cast<COMPONENT*>(*com);
                }
            }
            return nullptr;
        }

    };

}