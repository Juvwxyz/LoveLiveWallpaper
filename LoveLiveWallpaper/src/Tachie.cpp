#include "Tachie.h"
#include "Transform.h"
#include "MouseEventArg.h"

namespace LLWP
{
    Tachie::Tachie(GameObject& o) :
        Behaviour(o),
        Interactable(),
        IDrag(),
        canDrag(false)
    {}

    void Tachie::Start()
    {
        
    }

    void Tachie::Update()
    {
    }

    void Tachie::OnSettingButtonClick(GameObject& sender)
    {
        canDrag = !canDrag;
        if (canDrag)
        {
            renderer->setColor(1, 1, 1, 0.6f);
        }
        else
        {
            renderer->setColor(1, 1, 1, 1);
        }
    }

    void Tachie::OnDrag(const MouseEventArg& e)
    {
        if (canDrag)
        {
            this->transform().Move((float)e.delta().x, -(float)e.delta().y);
        }
    }

    bool Tachie::OnHitTest(Interactable*& hitted, const MouseEventArg& arg)
    {
        Interactable* dummy = nullptr;
        if (Interactable::OnHitTest(dummy, arg))
        {
            int x = arg.position().x;
            int y = arg.position().y;
            Vector tmp = Vector{ (float)x - 960, 540 - (float)y, 0 } * transform().worldToLocal();
            x = ((int)tmp.x() + 512);
            y = 512 - (int)tmp.y();

            if (renderer->getAlpha(x, y) > 0x08)
            {
                if (hitted == nullptr)
                {
                    hitted = dynamic_cast<Interactable*>(this);
                    return true;
                }
                if (dynamic_cast<Component*>(hitted)->transform().position().z() <= transform().position().z())
                {
                    hitted = dynamic_cast<Interactable*>(this);
                    return true;
                }
            }
        }
        return false;
    }

}
