#include "Component.h"
#include "GameObject.h"

namespace LLWP
{
    Component::Component(GameObject& o) :
        obj_(o),
        _obj(o.weak_from_this())
    {
    }

    GameObject& Component::gameobject()
    {
        return obj_;
    }

    const Transform& Component::transform() const
    {
        return obj_.transform();
    }

    Transform& Component::transform()
    {
        return obj_.transform();
    }

    Component::~Component()
    {
    }
}
