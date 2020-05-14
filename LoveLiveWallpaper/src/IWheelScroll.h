#pragma once
#include "Interactable.h"


namespace LLWP
{
    class IWheelScroll : public virtual Interactable
    {
    public:

    protected:
        IWheelScroll();

        virtual void OnWheelScrool(const MouseEventArg& e) = 0;
    };
}