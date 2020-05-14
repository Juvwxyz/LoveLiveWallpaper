#pragma once
#include "Component.h"

namespace LLWP
{
	class Sprite;

	class Image : public Component
	{
	public:
		Image(GameObject& obj);

	private:
		::std::shared_ptr<Sprite> _sprite;
	};
}