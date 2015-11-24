#include "flower.h"


Flower::Flower(int l, int c, QVector<QPixmap *> ts, Container cntnr, QVector<QPixmap*> ts_h) : Loot(l,c,ts,cntnr){
    new_ts = ts_h;
}

void Flower::effect(Hero *h){
    h->set_tileset(new_ts);
    h->set_flower(true);
}

Flower::~Flower(){
    new_ts.clear();
}

