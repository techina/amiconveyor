#include "TitleScene.h"
#include "../Game/GameScene.h"
#include "AppDelegate.h"
#include "../../Common/Store.h"
#include "../../Common/LBAnalytics.h"

USING_NS_CC;
using namespace ui;

Scene* TitleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = TitleScene::create();
    scene->addChild(layer);
    return scene;
}

bool TitleScene::init()
{
    if ( !LayerColor::initWithColor(Color4B(255, 255, 255, 255)) )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    visibleSize.height -= 100;
    Point origin = Director::getInstance()->getVisibleOrigin();
    origin.y += 100;

    auto label = LabelTTF::create("MINE SWEEPER G", "Arial", 64);
    label->setColor(Color3B(0, 0, 0));
    label->setPosition(Point(origin.x + visibleSize.width/2,
                             origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label);

    auto tutorial = Sprite::create("tutorial.png");
    tutorial->setPosition(Point(visibleSize) / 2 + origin);
    addChild(tutorial);

    auto startButton = AppDelegate::createButton("button_dollar.png", "");
    startButton->addTouchEventListener(this, toucheventselector(TitleScene::onStartButtonTouch));
    addChild(startButton);

    auto ud = UserDefault::getInstance();
    int highScore = ud->getIntegerForKey("highScore", -1);
    if (highScore > 0) {
        auto scoreLabel = LabelTTF::create(StringUtils::format("HIGH SCORE: %d$", highScore), "Arial", 48);
        scoreLabel->setColor(Color3B(0, 0, 0));
        scoreLabel->setPosition(label->getPosition() + Point(0, -label->getContentSize().height));
        addChild(scoreLabel);
        
        auto multiButton = AppDelegate::createButton("button_multi.png", "");
        multiButton->addTouchEventListener(this, toucheventselector(TitleScene::onMultiButtonTouch));
        addChild(multiButton);
        if (ud->getBoolForKey("showRate", false)) {
            auto rateBtn = AppDelegate::createButton("button_primary.png", "RATE");
            rateBtn->addTouchEventListener(this, toucheventselector(TitleScene::onRateButtonTouch));
            addChild(rateBtn);
            multiButton->setPosition(Point(visibleSize.width / 2, (visibleSize.height - tutorial->getContentSize().height) / 4) + origin);
            startButton->setPosition(multiButton->getPosition() + Point(0, 72));
            rateBtn->setPosition(multiButton->getPosition() + Point(0, -72));
        } else {
            startButton->setPosition(Point(visibleSize.width / 2, (visibleSize.height - tutorial->getContentSize().height) / 4 + 36) + origin);
            multiButton->setPosition(startButton->getPosition() + Point(0, -72));
        }
    } else {
        startButton->setPosition(Point(visibleSize.width / 2, (visibleSize.height - tutorial->getContentSize().height) / 4) + origin);
    }

    return true;
}

void TitleScene::onStartButtonTouch(Ref* target, TouchEventType type)
{
    if (type == TouchEventType::TOUCH_EVENT_ENDED) {
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, GameScene::createScene(1), Color3B(255, 255, 255)));
    }
}

void TitleScene::onMultiButtonTouch(Ref* target, TouchEventType type)
{
    if (type == TouchEventType::TOUCH_EVENT_ENDED) {
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, GameScene::createScene(2), Color3B(255, 255, 255)));
    }
}

void TitleScene::onRateButtonTouch(Ref* target, TouchEventType type)
{
    if (type == TouchEventType::TOUCH_EVENT_ENDED) {
        LBAnalytics::logEvent("rate", map<string, string>());
        Store::openReviewPage();
    }
}
