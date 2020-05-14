#pragma once
#include "Interactable.h"


namespace LLWP
{
    class IPointerDown : public virtual Interactable
    {
    public:

    protected:
        IPointerDown();
        virtual void OnPointerDown(const MouseEventArg& e) = 0;

        friend class Mouse;
    };
}