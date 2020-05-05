#pragma once
#include "Component.h"

namespace LLWP
{
	class Sprite;

	class Image : public Component
	{
	public:
		Image(GameObject& obj);

		__declspec(property(get = getOverrideSprite, put = setOverrideSprite))
			Sprite* overrideSprite;

	public:
		Sprite* getOverrideSprite() const;
		Sprite* setOverrideSprite(Sprite* sprite);

	private:
		Sprite* _sprite;
		Sprite* _overrideSprite;
	};
}