#pragma once
#include "Interactable.h"


namespace LLWP
{
    class IPointerMove : public virtual Interactable
    {
    public:
        IPointerMove();

    protected:

        virtual void OnPointerMove(const MouseEventArg& e) = 0;
    };
}