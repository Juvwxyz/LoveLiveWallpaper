#include "MouseEventArg.h"

namespace LLWP
{
    MouseEventArg::MouseEventArg(MOUSE_EVENT_TYPE t, POINT p, POINT d) :
        type_(t),
        pos_(p),
        delta_(d)
    {
    }

    MOUSE_EVENT_TYPE MouseEventArg::type() const
    {
        return type_;
    }

    POINT MouseEventArg::position() const
    {
        return pos_;
    }

    POINT MouseEventArg::delta() const
    {
        return delta_;
    }

}
