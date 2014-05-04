#include "ResultScene.h"
#include "AppDelegate.h"
#include "../Title/TitleScene.h"
#include "../../Common/GamePlatform.h"
#include "../../Common/LBSocial.h"
#include "../../Common/LBAnalytics.h"

USING_NS_CC;

Scene* ResultScene::createScene()
{
    auto scene = Scene::create();
    ResultScene *pRet = new ResultScene();
    if (pRet && pRet->init())
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

bool ResultScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    auto vSize = Director::getInstance()->getVisibleSize();

    auto lab = LabelTTF::create("out", "", 48);
    lab->setPosition(Point(vSize) / 2);
    addChild(lab);

    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch* touch, Event* event) { return true; };
    listener->onTouchMoved = [](Touch* touch, Event* event) {};
    listener->onTouchEnded = CC_CALLBACK_2(ResultScene::onTouchEnded, this);
    listener->onTouchCancelled = [](Touch* touch, Event* event) {};
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void ResultScene::onTouchEnded(Touch* touch, Event* event)
{
}
