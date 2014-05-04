#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "cocosbuilder/CCLayerLoader.h"
#include "cocosbuilder/CCBMemberVariableAssigner.h"
#include "UIButton.h"
#include "Burger.h"
#include "Mana.h"
#include "../../Common/RandomImpl.h"

using namespace std;
USING_NS_CC;
using namespace cocosbuilder;

class Level
{
public:
    float delta;
    int height;
    float speed;
    float freq;
    bool potato;
    bool lane;
};

class GameScene : public Layer, public CCBMemberVariableAssigner
{
    Node* laneA;
    Node* laneB;
    float flickCounter;
    Point touchBegan;
    Node* manaA;
    Node* manaB;
    Node* manaC;
    list<Mana*> flyingManas;
    list<Burger*> burgers;
    float spawnCounter;
    RandomImpl* rnd;
    LabelTTF* scoreLabel;
    int score;
    list<Level> levels;
    float levelCounter;
    Level currentLevel;
    float coolDown;
public:
    static Scene* createScene();
    virtual bool init();
    void initManas();
    ~GameScene(){};
    CREATE_FUNC(GameScene);
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node* pNode);
    void update(float dt);
    void checkLevel(float dt);
    void updateManas(float dt);
    void updateBurgers(float dt);
    void spawnMana(Mana* mana);
    void drawScore();
};

class GameSceneLoader : public LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GameSceneLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GameScene);
};

#endif // __GAME_SCENE_H__
