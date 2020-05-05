#pragma once
#include "EventSystem.h"
#include "typedef.h"
#include "winhead.h"
#include "MouseEventArg.h"

namespace LLWP
{
    class Interactable;

    using HitTestEventHandler = Delegate<void(Interactable*&, const MouseEventArg&)>;

    class Mouse
    {
    public:
        static void ProcessMouseEvent(WPARAM, LPARAM);
        static Event<HitTestEventHandler> OnHitTest;

    private:
        static POINT currentPos_;
        static POINT delatPos;

        static HitTestEventHandler OnHitTestHandler;

        static bool isLButtonDonw;

        static Interactable* CurrentHitted;
        static Interactable* LastHitted;
    };
}