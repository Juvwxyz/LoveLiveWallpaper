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

    class GameObject : public ::std::enable_shared_from_this<GameObject>
    {
    public:
        GameObject();
        GameObject(const std::string& name);

        Transform& tranform();
        virtual ~GameObject();
    protected:

        std::string name_;
        Transform transform_;

        std::list<::std::shared_ptr<GameObject>>::iterator self_;
        std::list<std::shared_ptr<Component>> componentList_;

        friend class Transform;
        template<class OBJECT, class... Args>
        friend ::std::shared_ptr<OBJECT> CreateObject(Args&&... args_);
        friend void DestroyObject(GameObject& obj);

        bool OnHitTest(const MouseEventArg& e);

    public:
        template<class COMPONENT, class... Args>
        ::std::shared_ptr<COMPONENT> AddComponent(const Args&... args_)
        {
            auto ptr = ::std::make_shared<COMPONENT>(*this, args_...);
            componentList_.push_back(ptr);
            return ptr;
        }

        template<class COMPONENT>
        ::std::shared_ptr<COMPONENT> GetComponent()
        {
            for (auto com = componentList_.begin(); com != componentList_.end(); com++)
            {
                if (typeid(**com) == typeid(COMPONENT))
                {
                    return ::std::dynamic_pointer_cast<COMPONENT, Component>(*com);
                }
            }
            return nullptr;
        }

    };

}