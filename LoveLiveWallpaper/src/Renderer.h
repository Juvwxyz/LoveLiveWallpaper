#pragma once
#include "Component.h"

namespace LLWP
{
    class Renderer : public Component
    {
    public:
        Renderer(GameObject& o);

    protected:
        virtual void Render() = 0;
        friend class Application;
    };
}