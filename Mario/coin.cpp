
#include "coin.h"

Coin::Coin(int l, int c, QVector<QPixmap *> ts, Container cntnr): Loot(l,c,ts, cntnr){

}

void Coin::effect(Hero *h){
    h->set_coins_number();
}
