#pragma once
#include <exception>

namespace LLWP
{
    class bad_json_convert : public std::exception
    {
        const char* s;

    public:
        bad_json_convert(const char* ss) : s(ss) {}
        inline virtual const char* what() const noexcept
        {
            return s;
        }
    };

    class json_exception : public std::exception
    {
        const char* s;

    public:
        json_exception(const char* msg) : s(msg) {}
        inline virtual const char* what() const noexcept
        {
            return s;
        }
    };
}