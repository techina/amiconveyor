#include "Mana.h"

Mana::Mana(Node* home, int color)
:home(home)
,color(color)
{
}

Mana* Mana::create(Node* home, int color)
{
    auto *sprite = new Mana(home, color);
    auto filename = StringUtils::format("img/game_mana_%d.png", color);
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}
