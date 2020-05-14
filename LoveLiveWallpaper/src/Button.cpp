#include "Button.h"

#include "GameObject.h"
#include "MouseEventArg.h"
#include "Transform.h"

namespace LLWP
{

    Button::Button(GameObject& obj) :
        Behaviour(obj),
        Interactable(),
        IPointerClick(),
        IPointerDown(),
        IPointerUp(),
        IPointerEnter(),
        IPointerLeave(),
        OnClick(ClickEventHandler),
        renderer(nullptr),
        pressed(false)
    {
    }

    void Button::OnPointerEnter(const MouseEventArg& e)
    {
        left = false;
        transform().ScaleBy(1.1f);
    }

    void Button::OnPointerLeave(const MouseEventArg& e)
    {
        left = true;
        if (!pressed)
            transform().ScaleBy(1 / 1.1f);
    }

    void Button::OnPointerClick(const MouseEventArg& e)
    {
        ClickEventHandler.invoke(gameobject());
    }

    void Button::OnPointerDown(const MouseEventArg& e)
    {
        pressed = true;
        if (renderer)
            renderer->setColor(0.75f, 0.75f, 0.75f, 1);
    }

    void Button::OnPointerUp(const MouseEventArg& e)
    {
        pressed = false;
        if (left) transform().ScaleBy(1 / 1.1f);
        if (renderer)
            renderer->setColor(1, 1, 1, 1);
    }
    
}