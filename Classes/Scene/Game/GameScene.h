#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "UIButton.h"

using namespace std;
USING_NS_CC;
using namespace ui;

enum TILE {
    HIDE = 0,
    SHOWN,
    BOMB,
    SHOWN_BOMB
};

class GameScene : public LayerColor
{
public:
    static Scene* createScene(int playerNum);
    virtual bool initWithPlayerNum(int playerNum);
    void onTouchEnded(Touch* touch, Event* event);
    vector< pair<int, int> > ROUND;
    void onDropButtonTouch(Ref* target, TouchEventType type);
    void onRetryButtonTouch(Ref* target, TouchEventType type);
    void onTopButtonTouch(Ref* target, TouchEventType type);
    void onTweetButtonTouch(Ref* target, TouchEventType type);
    void onFacebookButtonTouch(Ref* target, TouchEventType type);
private:
    int row;
    int col;
    int mineNum;
    vector< vector<TILE> > tiles;
    vector< vector<Sprite*> > sprites;
    int openCount;
    Button* dropBtn;
    vector<int> scores;
    vector<LabelTTF*> scoreLabels;
    vector<bool> droped;
    int currentTurn;
    LabelTTF* levelLabel;
    int level;

    void resetTiles();
    int countMine(int i, int j) const;
    bool outObBounds(int i, int j) const;
    void parseTile(int i, int j);
    void adjustScoreLabel();
    void adjustDropBtn();
    void stepTurn();
    bool dropedAll();
};

#endif // __GAME_SCENE_H__
