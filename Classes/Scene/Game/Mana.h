#ifndef __leadblow__Mana__
#define __leadblow__Mana__

#include "cocos2d.h"

using namespace std;
USING_NS_CC;

class Mana : public Sprite
{
public:
    Mana(Node* home, int color);
    static Mana* create(const string& filename, Node* home, int color);
    Point velocity;
    Node* home;
    int color;
};

#endif /* defined(__leadblow__Mana__) */
