#pragma once
#include "Interactable.h"


namespace LLWP
{
    class IPointerClick : public virtual Interactable
    {
    public:
        IPointerClick();

    protected:

        virtual void OnPointerClick(const MouseEventArg& e) = 0;

        friend class Mouse;
    };
}