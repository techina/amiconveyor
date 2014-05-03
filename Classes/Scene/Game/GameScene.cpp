#include "GameScene.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "cocosbuilder/CCNodeLoaderLibrary.h"

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto nodeLoaderLibrary = NodeLoaderLibrary::getInstance();
    nodeLoaderLibrary->registerNodeLoader("GameScene", GameSceneLoader::loader());
    auto ccbReader = new CCBReader(nodeLoaderLibrary);
    ccbReader->autorelease();
    auto layer = ccbReader->readNodeGraphFromFile("ccbi/GameScene.ccbi");
    //static_cast<PuzzleScene*>(layer)->initStoneLayer();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    spawnCounter = 3;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->preloadEffect("se_coin.mp3");
    audio->preloadEffect("se_ob.mp3");

    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = [](Touch* touch, Event* event) {};
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    listener->onTouchCancelled = [](Touch* touch, Event* event) {};
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    scheduleUpdate();
    return true;
}

bool GameScene::onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "laneA", Node*, laneA);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "laneB", Node*, laneB);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "manaA", Node*, manaA);
    return true;
}

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
    flickCounter = 0;
    touchBegan = touch->getLocation();
    return true;
}

void GameScene::onTouchEnded(Touch* touch, Event* event)
{
    auto dollar = Mana::create("img/cookie.png");
    dollar->setPosition(manaA->getPosition());
    addChild(dollar);
    auto touchEnd = touch->getLocation();
    dollar->velocity = (touchEnd - touchBegan).normalize() * 100 / flickCounter;
    flyingManas.push_back(dollar);
}

void GameScene::update(float dt)
{
    flickCounter += dt;
    for (auto it = flyingManas.begin(); it != flyingManas.end();) {
        auto e = *it;
        e->setPosition(e->getPosition() + e->velocity * dt);
        if (!getBoundingBox().intersectsRect(e->getBoundingBox())) {
            e->removeFromParent();
            it = flyingManas.erase(it);
        } else {
            it++;
        }
    }
    for (auto it = burgers.begin(); it != burgers.end();) {
        auto e = *it;
        auto vec = Point(-50 * dt, 0);
        e->setPosition(e->getPosition() + vec);
        for (auto m : e->manas) { m->setPosition(m->getPosition() + vec); }

        for (auto itt = flyingManas.begin(); itt != flyingManas.end(); itt++) {
            auto fm = *itt;
            if (fm->getBoundingBox().intersectsRect(e->getBoundingBox())) {
                fm->setPosition(e->getPosition() + Point(0, 20 * e->manas.size()));
                e->manas.push_back(fm);
                flyingManas.erase(itt);
                break;
            }
        }

        if (!getBoundingBox().intersectsRect(e->getBoundingBox())) {
            for (auto m : e->manas) { m->removeFromParent(); }
            e->removeFromParent();
            it = burgers.erase(it);
        } else {
            it++;
        }
    }
    spawnCounter -= dt;
    if (spawnCounter < 0) {
        spawnCounter = 3;
        auto b = Burger::create("img/plate.png");
        b->setPosition(laneA->getPosition());
        addChild(b);
        burgers.push_back(b);
    }
}
