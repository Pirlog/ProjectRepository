
#include "block.h"
#include <QDebug>

Block::Block(int l, int c, QVector<QPixmap *> ts, Container cntnr):  Element(l,c,ts, cntnr){
    hit= false;
    gravity_speed = 0;
    pos_y_ini = 0;
}

void Block::move_graphics(){
    graphics->setPos(pos_x, pos_y);
}

void Block::set_pos_y_ini(){
    if(pos_y_ini == 0){
        pos_y_ini = pos_y;
    }
}

