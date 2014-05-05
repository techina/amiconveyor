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
    void addMana(Mana* mana);
    bool validate();
    void jet();
    Point nextPoint();

    vector<Mana*> manas;
    vector<int> correctColors;
    vector<Sprite*> icons;
    Scale9Sprite* popup;
};

#endif /* defined(__leadblow__Burger__) */
