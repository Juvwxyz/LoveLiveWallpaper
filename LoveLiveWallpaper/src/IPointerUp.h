#pragma once
#include "Interactable.h"


namespace LLWP
{
    class IPointerUp : public virtual Interactable
    {
    public:
        IPointerUp();

    protected:

        virtual void OnPointerUp(const MouseEventArg& e) = 0;
    };
}