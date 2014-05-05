#include "GameScene.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "cocosbuilder/CCNodeLoaderLibrary.h"
#include "../Result/ResultScene.h"
#include "spine/Json.h"

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

    rnd = new RandomImpl();
    score = 0;
    auto str = FileUtils::getInstance()->getStringFromFile("level.json");
    auto json = Json_create(str.c_str());
    coolDown = Json_getFloat(json, "coolDown", 0.5f);
    auto events = Json_getItem(json, "events");
    auto it = events->child;
    while (it) {
        Level l;
        l.delta = Json_getFloat(it, "delta", -1);
        l.height = Json_getInt(it, "height", -1);
        l.speed = Json_getFloat(it, "speed", -1);
        l.freq = Json_getFloat(it, "freq", -1);
        l.potato = Json_getInt(it, "potato", 0);
        l.lane = Json_getInt(it, "lane", 0);
        levels.push_back(l);
        it = it->next;
    }
    levelCounter = 0;
    checkLevel(0);
    spawnCounter = currentLevel.freq;

    /*
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->preloadEffect("sound/se_coin.mp3");
    audio->preloadEffect("sound/se_ob.mp3");
     */

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
        auto mana = Mana::create(manas[i], i);
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
    for (auto mana : flyingManas) {
        if (mana->velocity.getLengthSq() <= 0 && mana->getBoundingBox().containsPoint(touchBegan)) {
            mana->setScale(1.2);
        }
    }
    return true;
}

void GameScene::onTouchEnded(Touch* touch, Event* event)
{
    for (auto mana : flyingManas) {
        if (mana->getBoundingBox().containsPoint(touchBegan)) {
            auto touchEnd = touch->getLocation();
            mana->velocity = (touchEnd - touchBegan).normalize() * 100 / flickCounter;
            mana->setScale(1);
            break;
        }
    }
}

void GameScene::update(float dt)
{
    flickCounter += dt;
    checkLevel(dt);
    updateManas(dt);
    updateBurgers(dt);
    spawnCounter -= dt;
    if (spawnCounter < 0) {
        spawnCounter = currentLevel.freq;
        vector<int> correctColors;
        for (int i = 0; i < currentLevel.height; i++) {
            correctColors.push_back(rnd->next() % 3);
        }
        auto b = Burger::create("img/game_bread_under.png", correctColors);
        auto lane = currentLevel.lane && rnd->next() % 2 == 0 ? laneB : laneA;
        b->setPosition(lane->getPosition());
        addChild(b);
        burgers.push_back(b);
    }
}

void GameScene::checkLevel(float dt)
{
    if (levels.empty()) {
        return;
    }
    levelCounter += dt;
    auto nextLevel = levels.front();
    if (levelCounter >= nextLevel.delta) {
        levelCounter = 0;
        currentLevel.height = nextLevel.height != -1 ? nextLevel.height : currentLevel.height;
        currentLevel.speed = nextLevel.speed != -1 ? nextLevel.speed : currentLevel.speed;
        currentLevel.freq = nextLevel.freq != -1 ? nextLevel.freq : currentLevel.freq;
        currentLevel.potato |= nextLevel.potato;
        currentLevel.lane |= nextLevel.lane;
        levels.pop_front();
    }
}

void GameScene::updateManas(float dt)
{
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
}

void GameScene::updateBurgers(float dt)
{
    for (auto it = burgers.begin(); it != burgers.end();) {
        auto burger = *it;
        auto vec = Point(-currentLevel.speed * dt, 0);
        burger->setPosition(burger->getPosition() + vec);
        for (auto itt = flyingManas.begin(); itt != flyingManas.end(); itt++) {
            auto fm = *itt;
            if (fm->getBoundingBox().intersectsRect(burger->getBoundingBox())) {
                burger->addMana(fm);
                auto mana = Mana::create(fm->home, fm->color);
                spawnMana(mana);
                addChild(mana);
                flyingManas.erase(itt);
                break;
            }
        }
        if (burger->validate()) {
            burger->jet();
            score++;
            drawScore();
            it = burgers.erase(it);
        } else if (burger->getBoundingBox().getMaxX() < 0) {
            Director::getInstance()->replaceScene(ResultScene::createScene());
            it = burgers.erase(it);
        } else {
            it++;
        }
    }
}

void GameScene::spawnMana(Mana *e)
{
    e->setPosition(Point(-1000, -1000));
    e->runAction(Sequence::create(DelayTime::create(coolDown), CallFuncN::create([&](Node* e) {
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
