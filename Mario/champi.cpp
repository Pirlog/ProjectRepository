#include "champi.h"

Champi::Champi(int l, int c, QVector<QPixmap *> ts, Container cntnr) : Loot(l,c,ts,cntnr){
}

void Champi::effect(Hero *h){
    if(o_type == BIG_CHAMP){
        h->set_scale(1);
    }else{
        h->set_scale(0.6f);
        h->set_flower(false);
        h->set_tileset(h->get_initial_ts());
    }
}
