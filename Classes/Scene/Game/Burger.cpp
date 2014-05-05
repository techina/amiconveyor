#include "Burger.h"

Burger* Burger::create(const std::string& filename, vector<int> colors)
{
    auto *sprite = new Burger(colors);
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        sprite->popup = Scale9Sprite::create(Rect(2, 67, 192, 60), "img/game_recipe.png");
        auto popup = sprite->popup;
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
            float y = i < col ? 46 : 23;
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

void Burger::addMana(Mana *mana)
{
    manas.push_back(mana);
    int idx = manas.size() - 1;
    if (idx < icons.size()) {
        auto icon = icons[idx];
        auto mark = Sprite::create(StringUtils::format("img/game_icon_%s.png", correctColors[idx] == mana->color ? "good" : "bad"));
        mark->setPosition(Point(icon->getContentSize()) / 2);
        icon->addChild(mark);
    }
    mana->setPosition(nextPoint());
    mana->setOrderOfArrival(manas.size());
    mana->retain();
    mana->removeFromParent();
    addChild(mana);
    mana->release();
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

void Burger::jet()
{
    popup->runAction(Spawn::create(ScaleTo::create(0.2f, 0), MoveBy::create(0.2f, Point(0, popup->getContentSize().height / 2)), NULL));
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto to = nextPoint();
    auto from = Point(to.x, visibleSize.height - getPositionY());

    auto bread = Sprite::create("img/game_bread.png");
    bread->setPosition(from);
    bread->runAction(MoveTo::create(0.2f, to));
    auto lid = Sprite::create("img/game_lid.png");
    lid->setPosition(from);
    lid->runAction(Sequence::create(DelayTime::create(0.1f), MoveTo::create(0.2f, to), NULL));
    auto comp = Sprite::create("img/game_compseal.png");
    comp->setPosition(from);
    comp->runAction(Sequence::create(DelayTime::create(0.2f), MoveTo::create(0.2f, to), NULL));
    addChild(bread);
    addChild(lid);
    addChild(comp);

    float vx = visibleSize.width;
    float vy = visibleSize.height / 2;
    auto delay = DelayTime::create(0.5f);
    auto move = MoveBy::create(0.5f, Point(-vx, vy));
    auto rotate = RotateBy::create(0.5f, -90);
    auto then = CallFunc::create([&]() {
        removeFromParent();
    });
    runAction(Sequence::create(delay, Spawn::create(move, rotate, NULL), then, NULL));
}

Point Burger::nextPoint()
{
    return Point(getContentSize()) / 2 + Point(-5, 7 * manas.size());
}
