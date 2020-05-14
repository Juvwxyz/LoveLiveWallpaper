#pragma once
#include "Behaviour.h"
#include "IDrag.h"
#include "CustomRenderer.h"

namespace LLWP
{
    class Tachie : public Behaviour, public IDrag
    {
    public :
        Tachie(GameObject& o);

        ::std::shared_ptr<CustomRenderer> renderer;

        virtual void Start() override;
        virtual void Update() override;

        void OnSettingButtonClick(GameObject& sender);
    protected:


        virtual void OnDrag(const MouseEventArg& e) override;
        virtual bool OnHitTest(Interactable*& hitted, const MouseEventArg& arg) override;

    private:
        bool canDrag;
    };
}
