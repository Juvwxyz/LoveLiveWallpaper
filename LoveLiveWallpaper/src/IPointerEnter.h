#pragma once
#include "Interactable.h"

namespace LLWP
{
    class IPointerEnter : virtual public Interactable
    {
    protected:
        IPointerEnter();

        virtual void OnPointerEnter(const MouseEventArg& e) = 0;

        friend class Mouse;
    };
}