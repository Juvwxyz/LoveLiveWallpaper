#include "Interactable.h"

#include "Component.h"
#include "Mouse.h"
#include "MouseEventArg.h"
#include "Transform.h"

namespace LLWP
{
    Interactable::Interactable()
    {
        Mouse::OnHitTest += (*this, &Interactable::OnHitTest);
    }
    void Interactable::OnHitTest(Interactable*& hitted, const MouseEventArg& arg)
    {
        Component* thisComponent = dynamic_cast<Component*>(this);
        if (thisComponent->transform().HitTest(arg.position().x, arg.position().y))
        {
            if (hitted == nullptr)
            {
                hitted = this;
            }
            else if (dynamic_cast<Component*>(hitted)->transform().Position().z() <= thisComponent->transform().Position().z())
            {
                hitted = this;
            }
        }
    }
    Interactable::~Interactable()
    {
        Mouse::OnHitTest -= (*this, &Interactable::OnHitTest);
    }
}
