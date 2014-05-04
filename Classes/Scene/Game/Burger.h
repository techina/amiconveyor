#ifndef __leadblow__Burger__
#define __leadblow__Burger__

#include "cocos2d.h"
#include "Mana.h"
#include "extensions/GUI/CCControlExtension/CCScale9Sprite.h"

using namespace std;
USING_NS_CC;
USING_NS_CC_EXT;

class Burger : public Sprite
{
public:
    Burger(vector<int> colors):correctColors(colors){};
    static Burger* create(const string& filename, vector<int> colors);
    vector<Mana*> manas;
    vector<int> correctColors;
    bool validate();
};

#endif /* defined(__leadblow__Burger__) */
