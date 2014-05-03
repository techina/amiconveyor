#ifndef __leadblow__Mana__
#define __leadblow__Mana__

#include "cocos2d.h"

using namespace std;
USING_NS_CC;

class Mana : public Sprite
{
public:
    static Mana* create(const string& filename);
    Point velocity;
};

#endif /* defined(__leadblow__Mana__) */
