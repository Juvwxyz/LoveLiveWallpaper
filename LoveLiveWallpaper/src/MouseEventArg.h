#pragma once
#include "typedef.h"

namespace LLWP
{
    class MouseEventArg
    {
    public:
        enum class Type
        {
            None,
            LButtonDown,
            LButtonUp,
            LButtonClick,
            WheelUp,
            WhellDown,
            Drag
        };
        MouseEventArg(Type t, POINT p, POINT d);

        Type type() const;
        POINT position()const;
        POINT delta()const;

    private:
        Type type_;
        POINT pos_;
        POINT delta_;
    };

}