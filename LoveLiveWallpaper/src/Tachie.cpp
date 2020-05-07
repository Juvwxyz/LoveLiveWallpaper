#include "Tachie.h"
#include "Transform.h"
#include "MouseEventArg.h"

namespace LLWP
{
    Tachie::Tachie(GameObject& o) :
        Behaviour(o),
        Interactable(),
        IDrag()
    {}

    void Tachie::Start()
    {
        
    }

    void Tachie::Update()
    {
    }



    void Tachie::OnDrag(const MouseEventArg& e)
    {
        this->transform().Move((float)e.delta().x, (float)e.delta().y);
    }

}
