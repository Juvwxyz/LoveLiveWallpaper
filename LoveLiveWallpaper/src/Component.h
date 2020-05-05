#pragma once
#include <string>

namespace LLWP
{

    class Transform;
    class GameObject;

    class Component
    {
    public:
        Component(GameObject & o);

        virtual GameObject& gameobject();
        virtual Transform& transform();

        virtual ~Component();
    protected:
        GameObject& obj_;

    };
}