#include "Image.h"

#include "Sprite.h"

namespace LLWP
{
	Image::Image(GameObject& obj) :
		Component(obj),
        _sprite(nullptr),
        _overrideSprite(nullptr)

	{
	}

    Sprite* Image::getOverrideSprite() const
    {
        return _overrideSprite;
    }

    Sprite* Image::setOverrideSprite(Sprite* sprite)
    {
        _overrideSprite = sprite;
        return _overrideSprite;
    }
}