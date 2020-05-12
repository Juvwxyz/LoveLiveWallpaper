#include "GameObject.h"

#include "Application.h"
#include "Raycaster.h"

namespace LLWP
{

    GameObject::GameObject() : GameObject("NewObject")
    {}
    GameObject::GameObject(const std::string& name) :
        name_(name),
        transform_(*this),
        componentList_()
    {}
    const Transform& GameObject::transform() const
    {
        return transform_;
    }

    Transform& GameObject::transform()
    {
        return transform_;
    }

    GameObject::~GameObject()
    {
    }

    bool GameObject::OnHitTest(const MouseEventArg& e)
    {
        auto r = GetComponent<Raycaster>();
        if (r == nullptr)
        {
            return false;
        }
        return false;
    }
}