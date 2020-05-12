#pragma once
#include <exception>

namespace LLWP
{
    class NullReferenceException : public ::std::exception
    {
    public:
        NullReferenceException() : ::std::exception("NullReferenceException", 1) {}
    };

    class IndexOutofRangeException : public ::std::exception
    {
    public:
        IndexOutofRangeException() : ::std::exception("IndexOutofRangeException", 1) {}
    };

    class GraphicException : public ::std::exception
    {
    public:
        GraphicException() : ::std::exception("GraphicException", 1) {}
    };
}