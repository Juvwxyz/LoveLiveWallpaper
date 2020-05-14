#pragma once
#include <utility>

namespace LLWP
{
    class MouseEventArg;

    class Interactable
    {
    public:
        virtual bool OnHitTest(Interactable*& hitted, const MouseEventArg& arg);
    protected:
        Interactable();
        virtual ~Interactable();

    private:

        friend class Mouse;
    };

}