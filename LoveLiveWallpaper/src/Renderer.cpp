#include "Renderer.h"
#include "Application.h"
namespace LLWP
{
    LLWP::Renderer::Renderer(GameObject& o) :
        Component(o)
    {
        Application::RenderList_.push_back(this);
    }
}