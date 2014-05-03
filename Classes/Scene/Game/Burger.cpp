#include "Burger.h"

Burger* Burger::create(const std::string& filename)
{
    auto *sprite = new Burger();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
