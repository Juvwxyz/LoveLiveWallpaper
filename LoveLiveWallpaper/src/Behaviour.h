#pragma once
#include "Component.h"

namespace LLWP
{
    class Behaviour : public Component
    {
    public:
        Behaviour(GameObject& o);

        virtual void Start();
        
        virtual void Update();


    };
}