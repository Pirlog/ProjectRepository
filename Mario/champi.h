#ifndef CHAMPI_H
#define CHAMPI_H

#include "loot.h"

class Champi : public Loot{
public:
    Champi(int l, int c, QVector<QPixmap *> ts, Container cntnr);
    void effect(Hero *h);
    ~Champi(){}
};

#endif // CHAMPI_H
