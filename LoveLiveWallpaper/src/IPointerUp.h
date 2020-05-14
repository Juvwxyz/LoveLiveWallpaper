#pragma once
#include "Interactable.h"


namespace LLWP
{
    class IPointerUp : public virtual Interactable
    {
    public:

    protected:
        IPointerUp();
        virtual void OnPointerUp(const MouseEventArg& e) = 0;

        friend class Mouse;
    };
}