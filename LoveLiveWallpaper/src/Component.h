#pragma once
#include <memory>

namespace LLWP
{

    class Transform;
    class GameObject;

    class Component : public ::std::enable_shared_from_this<Component>
    {
    public:
        Component(GameObject & o);

        virtual GameObject& gameobject();
        virtual Transform& transform();

        virtual ~Component();
    protected:
        GameObject& obj_;
        ::std::weak_ptr<GameObject> _obj;

    };
}