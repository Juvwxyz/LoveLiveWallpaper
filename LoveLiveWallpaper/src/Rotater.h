#pragma once

#include "Behaviour.h"
#include "GameObject.h"
#include "Transform.h"

#include <chrono>

namespace LLWP
{
    class Rotater : public Behaviour
    {
        ::std::chrono::system_clock::time_point last;
    public:
        Rotater(GameObject& obj) : Behaviour(obj) {};

        void Start() override
        {
            last = ::std::chrono::system_clock::now();
        }

        void Update() override
        {
            auto delta = ::std::chrono::duration_cast<::std::chrono::microseconds>(::std::chrono::system_clock::now() - last).count();
            this->transform().Rotate((float)delta / 100);
        
        }
    };
}