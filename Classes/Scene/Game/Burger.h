#ifndef __leadblow__Burger__
#define __leadblow__Burger__

#include "cocos2d.h"
#include "Mana.h"
#include "IRandom.h"

using namespace std;
USING_NS_CC;

class Burger : public Sprite
{
public:
    Burger(IRandom *rnd);
    static Burger* create(const string& filename, IRandom *rnd);
    vector<Mana*> manas;
    vector<int> correctColors;
};

#endif /* defined(__leadblow__Burger__) */
