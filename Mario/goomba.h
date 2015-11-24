#ifndef GOOMBA_H
#define GOOMBA_H

#include "monster.h"

class Goomba : public Monster{
public :
    Goomba(int l, int c, QVector<QPixmap*> ts, Container cntnr, int size, int spd);
    void set_character_animation();
    void move();
    ~Goomba(){}
};

#endif // GOOMBA_H
