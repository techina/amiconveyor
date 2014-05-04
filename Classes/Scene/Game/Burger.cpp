#include "Burger.h"

Burger* Burger::create(const std::string& filename, vector<int> colors)
{
    auto *sprite = new Burger(colors);
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        auto popup = Scale9Sprite::create(Rect(2, 67, 192, 60), "img/game_recipe.png");
        auto cc = sprite->correctColors;
        int col = (cc.size() + 1) / 2;
        if (cc.size() == 2) {
            col = 2;
        }
        float wid = 26;
        float margin = 5;
        for (int i = 0; i < cc.size(); i++) {
            auto mana = Sprite::create(StringUtils::format("img/game_icon_mana_%d.png", cc[i]));
            float x = wid * ((i % col) + 0.5f) + margin;
            float y = i <= col ? 46 : 23;
            mana->setPosition(Point(x, y));
            popup->addChild(mana);
            auto num = LabelTTF::create(StringUtils::format("%02d", i + 1), "", 10);
            num->enableStroke(Color3B::BLACK, 3);
            num->setPosition(3, 3);
            num->setColor(Color3B::BLACK);
            mana->addChild(num);
            sprite->icons.push_back(mana);
        }
        popup->setContentSize(Size(wid * col + (margin + 2) * 2, popup->getContentSize().height));
        auto size = popup->getContentSize();
        auto arrow = Sprite::create("img/game_arrow.png");
        arrow->setPosition(Point(sprite->getContentSize().width / 2, size.height + arrow->getContentSize().height / 2 - 2));
        popup->addChild(arrow);
        popup->setPosition(Point(size.width / 2, -size.height / 2 - arrow->getContentSize().height / 2));
        sprite->addChild(popup);
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool Burger::validate()
{
    if (correctColors.size() != manas.size()) {
        return false;
    }
    for (int i = 0; i < manas.size(); i++) {
        if (correctColors[i] != manas[i]->color) {
            return false;
        }
    }
    return true;
}
