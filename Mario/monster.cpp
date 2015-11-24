#include "monster.h"
#include <QDebug>


Monster::Monster(int l, int c, QVector<QPixmap *> ts, Container cntnr, int size, int spd): Character(l,c,ts, cntnr, size, spd){
    direction_x = LEFT;
    alive = true;
}
