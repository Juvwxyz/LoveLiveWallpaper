#pragma once
#include "Behaviour.h"
#include "EventSystem.h"
#include "IPointerClick.h"
#include "IPointerDown.h"
#include "IPointerUp.h"
#include "IPointerEnter.h"
#include "IPointerLeave.h"
#include "CustomRenderer.h"

namespace LLWP
{
	class Sprite;

	class Button :
		public Behaviour,
		public IPointerClick,
		public IPointerDown,
		public IPointerUp,
		public IPointerEnter,
		public IPointerLeave
	{
	public:
		Button(GameObject& obj);

		Event<Delegate<void(GameObject&)>> OnClick;

		::std::shared_ptr<CustomRenderer> renderer;

	protected:
		virtual void OnPointerEnter(const MouseEventArg& e) override;
		virtual void OnPointerLeave(const MouseEventArg& e) override;
		virtual void OnPointerClick(const MouseEventArg& e) override;
		virtual void OnPointerDown(const MouseEventArg& e) override;
		virtual void OnPointerUp(const MouseEventArg& e) override;

		Delegate<void(GameObject&)> ClickEventHandler;

	private:
		bool pressed;
		bool left;
	};
}