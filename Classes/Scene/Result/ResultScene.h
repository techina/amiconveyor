#ifndef __gsweeper__ResultScene__
#define __gsweeper__ResultScene__

#include "cocos2d.h"

USING_NS_CC;

class ResultScene : public Layer
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ResultScene);
    void onTouchEnded(Touch* touch, Event* event);
};

#endif /* defined(__gsweeper__ResultScene__) */
