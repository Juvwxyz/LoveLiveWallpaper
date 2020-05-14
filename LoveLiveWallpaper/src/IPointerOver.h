#pragma once
#include "Interactable.h"


namespace LLWP
{
    class IPointerOver : public virtual Interactable
    {
    public:
        IPointerOver();

    protected:

        virtual void OnPointerOver(const MouseEventArg& e) = 0;

        friend class Mouse;
    };
}