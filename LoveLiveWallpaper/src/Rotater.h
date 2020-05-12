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
        float _angularSpeed;
    public:
        Rotater(GameObject& obj) : Behaviour(obj), _angularSpeed(0) {};

        void setSpeed(float s) { _angularSpeed = s; }

        void Start() override
        {
            last = ::std::chrono::system_clock::now();
        }

        void Update() override
        {
            auto delta = ::std::chrono::duration_cast<::std::chrono::nanoseconds>(::std::chrono::system_clock::now() - last).count();
            last = ::std::chrono::system_clock::now();
            this->transform().Rotate(_angularSpeed * delta * 1e-9f);
        }
    };
}