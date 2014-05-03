#include "ResultScene.h"
#include "AppDelegate.h"
#include "../Title/TitleScene.h"
#include "../../Common/GamePlatform.h"
#include "../../Common/LBSocial.h"
#include "../../Common/LBAnalytics.h"

USING_NS_CC;
using namespace ui;

Scene* ResultScene::createScene(std::vector<int> scores)
{
    auto scene = Scene::create();
    ResultScene *pRet = new ResultScene();
    if (pRet && pRet->initWithScores(scores))
    {
        pRet->autorelease();
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
    scene->addChild(pRet);
    return scene;
}

bool ResultScene::initWithScores(std::vector<int> scores)
{
    if ( !LayerColor::initWithColor(Color4B(255, 255, 255, 255)) )
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    visibleSize.height -= 100;
    Point origin = Director::getInstance()->getVisibleOrigin();
    origin.y += 100;

    auto ud = UserDefault::getInstance();
    int highScore = ud->getIntegerForKey("highScore");
    int scoreMax = *(std::max_element(scores.begin(), scores.end()));
    int maxCount = static_cast<int>(std::count(scores.begin(), scores.end(), scoreMax));
    float scoreBottom = 0;
    for (int i=0; i<scores.size(); i++) {
        auto prefix = "";
        if (scores.size() > 1) {
            prefix = StringUtils::format("%dP ", i + 1).c_str();
        }
        auto scoreLabel = LabelTTF::create(StringUtils::format("%sSCORE: %d$", prefix, scores[i]), "Arial", 48);
        scoreLabel->setColor(Color3B(0, 0, 0));
        float labHei = scoreLabel->getContentSize().height;
        float top = (visibleSize.height - labHei * scores.size()) / 2;
        scoreLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height - top - labHei * (i + 0.5f)) + origin);
        addChild(scoreLabel);
        if (scores.size() == 1 && scores[i] >= highScore) {
            auto badge = Sprite::create("badge_score.png");
            badge->setPosition(scoreLabel->getPosition() + Point(scoreLabel->getContentSize()) / 2 + Point(10, 10));
            addChild(badge);
        }
        if (scores.size() > 1 && maxCount == 1 && scores[i] == scoreMax) {
            auto badge = Sprite::create("badge_win.png");
            badge->setPosition(scoreLabel->getPosition() + Point(-(scoreLabel->getContentSize().width + badge->getContentSize().width) / 2, 0));
            addChild(badge);
        }
        scoreBottom = scoreLabel->getPositionY() - labHei / 2;
    }

    auto tweetButton = AppDelegate::createButton("icon_tw.png", "");
    tweetButton->addTouchEventListener(this, toucheventselector(ResultScene::onTweetButtonTouch));
    auto facebookButton = AppDelegate::createButton("icon_fb.png", "");
    facebookButton->addTouchEventListener(this, toucheventselector(ResultScene::onFacebookButtonTouch));
    auto rankButton = AppDelegate::createButton("icon_rank.png", "");
    rankButton->addTouchEventListener(this, toucheventselector(ResultScene::onRankButtonTouch));

    facebookButton->setPosition(Point(visibleSize.width / 2, scoreBottom - facebookButton->getContentSize().height));
    tweetButton->setPosition(facebookButton->getPosition() + Point(-facebookButton->getContentSize().width - 10, 0));
    rankButton->setPosition(facebookButton->getPosition() + Point(facebookButton->getContentSize().width + 10, 0));

    addChild(tweetButton);
    addChild(facebookButton);
    addChild(rankButton);

    auto okButton = AppDelegate::createButton("button_primary.png", "OK");
    okButton->setPosition(Point(visibleSize.width / 2, okButton->getContentSize().height) + origin);
    okButton->addTouchEventListener(this, toucheventselector(ResultScene::onOkButtonTouch));
    addChild(okButton);

    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch* touch, Event* event) { return true; };
    listener->onTouchMoved = [](Touch* touch, Event* event) {};
    listener->onTouchEnded = CC_CALLBACK_2(ResultScene::onTouchEnded, this);
    listener->onTouchCancelled = [](Touch* touch, Event* event) {};
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void ResultScene::onTouchEnded(Touch* touch, Event* event) {
}

void ResultScene::onOkButtonTouch(Ref* target, TouchEventType type)
{
    if (type != TouchEventType::TOUCH_EVENT_ENDED) {
        return;
    }
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, TitleScene::createScene(), Color3B(255, 255, 255)));
}

void ResultScene::onTweetButtonTouch(Ref* target, TouchEventType type)
{
    if (type != TouchEventType::TOUCH_EVENT_ENDED) {
        return;
    }
    AppDelegate::screenShot("screenshot.jpg", [](std::string filePath) {
        LBSocial::tweet("GREEEED!!!! http://goo.gl/x5iI8f", filePath.c_str());
    });
    LBAnalytics::logEvent("share", {{"sns", "tw"}, {"result", "true"}});
}

void ResultScene::onFacebookButtonTouch(Ref* target, TouchEventType type)
{
    if (type != TouchEventType::TOUCH_EVENT_ENDED) {
        return;
    }
    AppDelegate::screenShot("screenshot.jpg", [](std::string filePath) {
        LBSocial::facebook("GREEEED!!!! http://goo.gl/x5iI8f", filePath.c_str());
    });
    LBAnalytics::logEvent("share", {{"sns", "fb"}, {"result", "true"}});
}

void ResultScene::onRankButtonTouch(Ref* target, TouchEventType type)
{
    if (type != TouchEventType::TOUCH_EVENT_ENDED) {
        return;
    }
    GamePlatform::show();
}
