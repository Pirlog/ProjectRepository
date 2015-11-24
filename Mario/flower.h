#ifndef FLOWER_H
#define FLOWER_H
#include "loot.h"


class Flower : public Loot{
public:
    Flower(int l, int c, QVector<QPixmap *> ts, Container cntnr, QVector<QPixmap*> ts_h);
    void effect(Hero *h);
    ~Flower();
private:
    QVector<QPixmap*> new_ts;
};

#endif // FLOWER_H
