#include "MouseEventArg.h"

namespace LLWP
{
    MouseEventArg::MouseEventArg(Type t, POINT p, POINT d) :
        type_(t),
        pos_(p),
        delta_(d)
    {
    }

    MouseEventArg::Type MouseEventArg::type() const
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
