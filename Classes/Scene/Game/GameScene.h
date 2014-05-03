#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "cocosbuilder/CCLayerLoader.h"
#include "cocosbuilder/CCBMemberVariableAssigner.h"
#include "UIButton.h"

using namespace std;
USING_NS_CC;
using namespace cocosbuilder;

class GameScene : public Layer, public CCBMemberVariableAssigner
{
    Node* laneA;
    Node* laneB;
public:
    static Scene* createScene();
    virtual bool init();
    ~GameScene(){};
    CREATE_FUNC(GameScene);
    void onTouchEnded(Touch* touch, Event* event);

    virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node* pNode);
};

class GameSceneLoader : public LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GameSceneLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GameScene);
};

#endif // __GAME_SCENE_H__
