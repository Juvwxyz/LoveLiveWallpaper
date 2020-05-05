#pragma once
#include "Interactable.h"


namespace LLWP
{
    class IPointerDown : public virtual Interactable
    {
    public:
        IPointerDown();

    protected:

        virtual void OnPointerDown(const MouseEventArg& e) = 0;
    };
}