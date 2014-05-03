#include "Mana.h"

Mana* Mana::create(const std::string& filename)
{
    auto *sprite = new Mana();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
