#pragma once
#include "Json.h"

#include <string.h>

namespace LLWP
{
    class UserSettings
    {
        Json data;

        static ::std::wstring configPath;

    public:
        UserSettings();
    };
}