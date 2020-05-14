#include "Mouse.h"
#include "MouseEventArg.h"
#include "Interactable.h"
#include "IDrag.h"
#include "IPointerDown.h"
#include "IPointerUp.h"
#include "IPointerClick.h"
#include "IPointerEnter.h"
#include "IPointerLeave.h"
#include "IPointerOver.h"

namespace LLWP
{
    POINT Mouse::_currentPos;
    POINT Mouse::_delatPos;
    HitTestEventHandler Mouse::OnHitTestHandler;
    bool Mouse::isPointerDonw = false;
    Event<HitTestEventHandler> Mouse::OnHitTest(Mouse::OnHitTestHandler);
    Interactable* Mouse::CurrentHitted = nullptr;
    Interactable* Mouse::LastHitted = nullptr;
    Interactable* Mouse::LastPointerDown = nullptr;

    void Mouse::ProcessMouseEvent(WPARAM wPara, LPARAM lParam)
    {
        CurrentHitted = nullptr;
        MSLLHOOKSTRUCT* msdata = (MSLLHOOKSTRUCT *) lParam;

        _delatPos = msdata->pt - _currentPos;
        _currentPos = msdata->pt;

        OnHitTestHandler(CurrentHitted, { MOUSE_EVENT_HIT_TEST, _currentPos, _delatPos });

        switch (wPara)
        {
        case WM_LBUTTONDOWN:
        {
            isPointerDonw = true;
            if (IPointerDown* p = dynamic_cast<IPointerDown*>(CurrentHitted))
            {
                p->OnPointerDown({ MOUSE_EVENT_POINTER_DOWN, _currentPos, _delatPos });
            }
            LastPointerDown = CurrentHitted;
        }
        break;
        case WM_LBUTTONUP:
        {
            isPointerDonw = false;
            if (IPointerUp* p = dynamic_cast<IPointerUp*>(LastPointerDown))
            {
                p->OnPointerUp({ MOUSE_EVENT_POINTER_UP, _currentPos, _delatPos });
            }
            if (LastPointerDown == CurrentHitted)
            {
                if (IPointerClick* p = dynamic_cast<IPointerClick*>(LastPointerDown))
                {
                    p->OnPointerClick({ MOUSE_EVENT_POINTER_CLICK, _currentPos, _delatPos });
                }
            }
        }
        break;
        case WM_MOUSEMOVE:
        {
            if (LastHitted != CurrentHitted)
            {
                if (IPointerLeave* p = dynamic_cast<IPointerLeave*>(LastHitted))
                {
                    p->OnPointerLeave({ MOUSE_EVENT_POINTER_LEAVE, _currentPos, _delatPos });
                }
                if (IPointerEnter* p = dynamic_cast<IPointerEnter*>(CurrentHitted))
                {
                    p->OnPointerEnter({ MOUSE_EVENT_POINTER_ENTER, _currentPos, _delatPos });
                }
            }
            else
            {
                if (isPointerDonw)
                {
                    if (LastPointerDown != nullptr)
                    {
                        if (IDrag* p = dynamic_cast<IDrag*>(LastPointerDown))
                        {
                            p->OnDrag({ MOUSE_EVENT_DRAG, _currentPos, _delatPos });
                        }
                    }
                }
                else if (IPointerOver* p = dynamic_cast<IPointerOver*>(LastHitted))
                {
                    p->OnPointerOver({ MOUSE_EVENT_POINTER_OVER, _currentPos, _delatPos });
                }

            }
        }
        break;
        case WM_MOUSEWHEEL:
        {

        }
        break;
        default:
        break;
        }
        LastHitted = CurrentHitted;
    }
}