#pragma once
#include "Interactable.h"

namespace LLWP
{
    class IPointerLeave : virtual public Interactable
    {
    protected:
        IPointerLeave();

        virtual void OnPointerLeave(const MouseEventArg& e) = 0;

        friend class Mouse;
    };
}