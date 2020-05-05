#pragma once
#include "Interactable.h"


namespace LLWP
{
    class IWheelMove : public virtual Interactable
    {
    public:
        IWheelMove();

    protected:

        virtual void OnWheelMove(const MouseEventArg& e) = 0;
    };
}