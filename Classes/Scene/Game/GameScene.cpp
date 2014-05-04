#include "GameScene.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "cocosbuilder/CCNodeLoaderLibrary.h"
#include "../Result/ResultScene.h"

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto nodeLoaderLibrary = NodeLoaderLibrary::getInstance();
    nodeLoaderLibrary->registerNodeLoader("GameScene", GameSceneLoader::loader());
    auto ccbReader = new CCBReader(nodeLoaderLibrary);
    ccbReader->autorelease();
    auto layer = ccbReader->readNodeGraphFromFile("ccbi/GameScene.ccbi");
    static_cast<GameScene*>(layer)->initManas();
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
    rnd = new RandomImpl();
    score = 0;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->preloadEffect("sound/se_coin.mp3");
    audio->preloadEffect("sound/se_ob.mp3");

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
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "manaB", Node*, manaB);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "manaC", Node*, manaC);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "scoreLabel", LabelTTF*, scoreLabel);
    return true;
}

void GameScene::initManas()
{
    vector<Node*> manas = {manaA, manaB, manaC};
    for (int i = 0; i < manas.size(); i++) {
        auto filename = StringUtils::format("img/cookie%d.png", i);
        auto mana = Mana::create(filename, manas[i], i);
        mana->setPosition(manas[i]->getPosition());
        addChild(mana);
        flyingManas.push_back(mana);
    }
    drawScore();
}

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
    flickCounter = 0;
    touchBegan = touch->getLocation();
    return true;
}

void GameScene::onTouchEnded(Touch* touch, Event* event)
{
    for (auto mana : flyingManas) {
        if (mana->getBoundingBox().containsPoint(touchBegan)) {
            auto touchEnd = touch->getLocation();
            mana->velocity = (touchEnd - touchBegan).normalize() * 100 / flickCounter;
            break;
        }
    }
}

void GameScene::update(float dt)
{
    flickCounter += dt;
    for (auto it = flyingManas.begin(); it != flyingManas.end();) {
        auto e = *it;
        e->setPosition(e->getPosition() + e->velocity * dt);
        if (!getBoundingBox().intersectsRect(e->getBoundingBox())) {
            spawnMana(e);
            it = flyingManas.erase(it);
        } else {
            it++;
        }
    }
    updateBurgers(dt);
    spawnCounter -= dt;
    if (spawnCounter < 0) {
        spawnCounter = 3;
        auto b = Burger::create("img/plate.png", rnd);
        b->setPosition(laneA->getPosition());
        addChild(b);
        burgers.push_back(b);
    }
}

void GameScene::updateBurgers(float dt)
{
    for (auto it = burgers.begin(); it != burgers.end();) {
        auto e = *it;
        auto vec = Point(-50 * dt, 0);
        e->setPosition(e->getPosition() + vec);
        for (auto m : e->manas) { m->setPosition(m->getPosition() + vec); }

        for (auto itt = flyingManas.begin(); itt != flyingManas.end(); itt++) {
            auto fm = *itt;
            if (fm->getBoundingBox().intersectsRect(e->getBoundingBox())) {
                e->manas.push_back(fm);
                fm->setPosition(e->getPosition() + Point(0, 10 * e->manas.size()));
                fm->setOrderOfArrival(e->manas.size());
                auto mana = Mana::create(StringUtils::format("img/cookie%d.png", fm->color), fm->home, fm->color);
                spawnMana(mana);
                addChild(mana);
                flyingManas.erase(itt);
                break;
            }
        }
        if (!getBoundingBox().intersectsRect(e->getBoundingBox())) {
            if (e->validate()) {
                score++;
                drawScore();
                for (auto m : e->manas) { m->removeFromParent(); }
                e->removeFromParent();
            } else {
                Director::getInstance()->replaceScene(ResultScene::createScene());
            }
            it = burgers.erase(it);
        } else {
            it++;
        }
    }
}

void GameScene::spawnMana(Mana *e)
{
    e->setPosition(Point(-1000, -1000));
    e->runAction(Sequence::create(DelayTime::create(1.0f), CallFuncN::create([&](Node* e) {
        auto mana = static_cast<Mana*>(e);
        mana->setPosition(mana->home->getPosition());
        mana->velocity = Point::ZERO;
        flyingManas.push_back(mana);
    }), NULL));
}

void GameScene::drawScore()
{
    scoreLabel->setString(StringUtils::format("%04d", score));
}
