#pragma once
#include "typedef.h"

namespace LLWP
{
    enum MOUSE_EVENT_TYPE
    {
        MOUSE_EVENT_NONE = 0,
        MOUSE_EVENT_POINTER_DOWN,
        MOUSE_EVENT_POINTER_UP,
        MOUSE_EVENT_POINTER_CLICK,
        MOUSE_EVENT_POINTER_ENTER,
        MOUSE_EVENT_POINTER_LEAVE,
        MOUSE_EVENT_POINTER_OVER,
        MOUSE_EVENT_WHEEL_UP,
        MOUSE_EVENT_WHEEL_DOWN,
        MOUSE_EVENT_DRAG,
        MOUSE_EVENT_HIT_TEST,
    };
    class MouseEventArg
    {
    public:
        MouseEventArg(MOUSE_EVENT_TYPE t, POINT p, POINT d);

        MOUSE_EVENT_TYPE type() const;
        POINT position()const;
        POINT delta()const;

    private:
        MOUSE_EVENT_TYPE type_;
        POINT pos_;
        POINT delta_;
    };

}