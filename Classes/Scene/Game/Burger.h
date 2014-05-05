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
    Burger(vector<int> colors, bool isPotato):correctColors(colors),isPotato(isPotato){};
    static Burger* create(vector<int> colors, bool isPotato);
    void addMana(Mana* mana);
    bool validate();
    void jet();
    Point nextPoint();
    Mana* popMana(Node* parent);
    void drawPotatoCount();

    vector<Mana*> manas;
    vector<int> correctColors;
    vector<Sprite*> icons;
    Scale9Sprite* popup;
    int burgerId;
    bool isPotato;
};

#endif /* defined(__leadblow__Burger__) */
