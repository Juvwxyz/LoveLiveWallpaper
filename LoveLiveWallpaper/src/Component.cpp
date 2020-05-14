#include "Component.h"
#include "GameObject.h"

namespace LLWP
{
    Component::Component(GameObject& o) :
        _obj(o.weak_from_this())
    {
    }

    GameObject& Component::gameobject()
    {
        return *_obj.lock();
    }

    const Transform& Component::transform() const
    {
        return _obj.lock()->transform();
    }

    Transform& Component::transform()
    {
        return _obj.lock()->transform();
    }

    Component::~Component()
    {
    }
}
