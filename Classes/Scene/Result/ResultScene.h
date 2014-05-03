#ifndef __gsweeper__ResultScene__
#define __gsweeper__ResultScene__

#include "cocos2d.h"
#include "UIButton.h"

USING_NS_CC;
using namespace ui;

class ResultScene : public LayerColor
{
public:
    static Scene* createScene(std::vector<int> scores);
    virtual bool initWithScores(std::vector<int> scores);
    CREATE_FUNC(ResultScene);
    void onTouchEnded(Touch* touch, Event* event);
    void onOkButtonTouch(Ref* target, TouchEventType type);
    void onTweetButtonTouch(Ref* target, TouchEventType type);
    void onFacebookButtonTouch(Ref* target, TouchEventType type);
    void onRankButtonTouch(Ref* target, TouchEventType type);
private:
    std::vector<int> scores;
};

#endif /* defined(__gsweeper__ResultScene__) */
