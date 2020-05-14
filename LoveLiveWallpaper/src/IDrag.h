#pragma once
#include "Interactable.h"

namespace LLWP
{
    class IDrag : virtual public Interactable
    {
    public :

    protected:
        IDrag();
        virtual void OnDrag(const MouseEventArg& e) = 0;
        virtual ~IDrag();

        friend class Mouse;
    };
}
