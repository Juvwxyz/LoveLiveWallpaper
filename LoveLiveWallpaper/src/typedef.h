#pragma once
#include "winhead.h"
#include <windef.h>


namespace LLWP
{
    typedef tagPOINT SIZE;

    inline POINT operator-(POINT l, POINT r)
    {
        return { l.x - r.x,l.y - r.y };
    }
}
