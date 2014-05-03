#ifndef __leadblow__Burger__
#define __leadblow__Burger__

#include "cocos2d.h"
#include "Mana.h"

using namespace std;
USING_NS_CC;

class Burger : public Sprite
{
public:
    static Burger* create(const string& filename);
    vector<Mana*> manas;
};

#endif /* defined(__leadblow__Burger__) */
