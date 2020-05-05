#include "Mouse.h"
#include "MouseEventArg.h"
#include "Interactable.h"
#include "IDrag.h"

namespace LLWP
{
    POINT Mouse::currentPos_;
    POINT Mouse::delatPos;
    HitTestEventHandler Mouse::OnHitTestHandler;
    bool Mouse::isLButtonDonw = false;
    Event<HitTestEventHandler> Mouse::OnHitTest(Mouse::OnHitTestHandler);
    Interactable* Mouse::CurrentHitted = nullptr;
    Interactable* Mouse::LastHitted = nullptr;

    void Mouse::ProcessMouseEvent(WPARAM wPara, LPARAM lParam)
    {
        CurrentHitted = nullptr;
        MSLLHOOKSTRUCT* msdata = (MSLLHOOKSTRUCT *) lParam;

        delatPos = msdata->pt - currentPos_;
        currentPos_ = msdata->pt;

        MouseEventArg eventArgs(MouseEventArg::Type::None, currentPos_, delatPos);

        OnHitTestHandler(CurrentHitted, eventArgs);

        switch (wPara)
        {
        case WM_LBUTTONDOWN:
        {
            isLButtonDonw = true;
            LastHitted = CurrentHitted;
        }
        break;
        case WM_LBUTTONUP:
        {
            isLButtonDonw = false;
            LastHitted = CurrentHitted;
        }
        break;
        case WM_MOUSEMOVE:
        {
            if (isLButtonDonw)
            {
                if (LastHitted != nullptr)
                {
                    dynamic_cast<IDrag*>(LastHitted)->OnDrag(eventArgs);
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
    }
}