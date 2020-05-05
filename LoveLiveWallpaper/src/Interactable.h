#pragma once
#include <utility>

namespace LLWP
{
    class MouseEventArg;

    class Interactable
    {
    protected:
        Interactable();
        virtual void OnHitTest(Interactable*& hitted, const MouseEventArg& arg);
        virtual ~Interactable();

    private:

        friend class Mouse;
    };

}