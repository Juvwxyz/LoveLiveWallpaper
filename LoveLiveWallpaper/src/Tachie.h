#pragma once
#include "Behaviour.h"
#include "IDrag.h"

namespace LLWP
{
    class Tachie : public Behaviour, public IDrag
    {
    public :
        Tachie(GameObject& o);

        virtual void Start() override;
        virtual void Update() override;

    protected:
        virtual void OnDrag(const MouseEventArg& e) override;
    };
}
