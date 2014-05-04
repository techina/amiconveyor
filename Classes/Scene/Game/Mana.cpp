#include "Mana.h"

Mana::Mana(Node* home, int color)
:home(home)
,color(color)
{
}

Mana* Mana::create(const std::string& filename, Node* home, int color)
{
    auto *sprite = new Mana(home, color);
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
