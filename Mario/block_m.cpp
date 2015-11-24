#include "block_m.h"
#include "coin.h"

Block_m::Block_m(int l, int c, QVector<QPixmap *> ts, Container cntnr) : Block(l,c,ts, cntnr){
    bonus = S_COIN;
    hit = 0;
    gravity_speed = -5;
}

