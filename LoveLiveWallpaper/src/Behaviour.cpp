#include "Behaviour.h"

#include "Application.h"

namespace LLWP
{
    Behaviour::Behaviour(GameObject& o) :
        Component(o)
    {
        Application::OnStart += (*this, &Behaviour::Start);
        Application::OnUpdate += (*this, &Behaviour::Update);
    }

	void Behaviour::Start() {}

    void Behaviour::Update() {}

}
