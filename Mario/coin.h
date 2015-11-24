#ifndef COIN_H
#define COIN_H

#include "loot.h"

class Coin : public Loot{
public:
    Coin(int l, int c, QVector<QPixmap*> ts, Container cntnr);
    void effect(Hero *h);
    ~Coin(){}
};
#endif // COIN_H
