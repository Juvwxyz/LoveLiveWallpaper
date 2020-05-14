#pragma once
#include "EventSystem.h"
#include "typedef.h"
#include "winhead.h"
#include "MouseEventArg.h"

namespace LLWP
{
    class Interactable;

    using HitTestEventHandler = Delegate<bool(Interactable*&, const MouseEventArg&)>;

    class Mouse
    {
    public:
        static void ProcessMouseEvent(WPARAM, LPARAM);
        static Event<HitTestEventHandler> OnHitTest;

    private:
        static POINT _currentPos;
        static POINT _delatPos;

        static HitTestEventHandler OnHitTestHandler;

        static bool isPointerDonw;

        static Interactable* CurrentHitted;
        static Interactable* LastHitted;
        static Interactable* LastPointerDown;
    };
}