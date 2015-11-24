#ifndef LOOT_H
#define LOOT_H
#include "hero.h"

enum Surprise{S_BIG_CHAMP, S_SMALL_CHAMP, S_COIN, S_FLOWER};


class Loot : public Element{
public:
    Loot(int l, int c, QVector<QPixmap *> ts, Container cntnr);
    virtual void effect(Hero *h)=0;
    ~Loot(){}
};


#endif // LOOT_H
