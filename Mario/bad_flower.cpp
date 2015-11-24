#include "bad_flower.h"
#include <QDebug>


Bad_flower::Bad_flower(int l, int c, QVector<QPixmap *> ts, Container cntnr, int size, int spd) : Monster(l,c,ts,cntnr,size,spd)
{
    direction_x = STAND;
    state_counter = 60;
    max_height = 60;
}


void Bad_flower::set_character_animation(){
    graphics->setPos(pos_x,pos_y);
    current_tile = (current_tile + 1)%3;
    graphics->setPixmap(tileset[current_tile]->scaled(character_size*2*scale,character_size*scale,Qt::KeepAspectRatio));
}

void Bad_flower::move(){
    gravity_jump();
    if(state_counter< max_height){
        gravity_speed = -4;
        state_counter ++;
    }else{
       gravity_speed = 4;
       if(grounded == true){
           state_counter = 0;
       }

    }


}

