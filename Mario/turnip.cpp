
#include "turnip.h"
#include <QDebug>

Turnip::Turnip(int l, int c, QVector<QPixmap *> ts, Container cntnr, int t_size, int speed, int x, int y,  bool is_left) :
    Character(l,c,ts,cntnr, t_size, speed){
    pos_x = x;
    pos_y = y;;
    touch = false;
    up = true;
    max_incr = 6;
    incr = max_incr;
    left = is_left;
    grounded_once = false;
}

void Turnip::move(){
    if(!grounded){
        if(stucked == NO){
            if(left){
                pos_x -= speed;
            }else{
                pos_x += speed;
            }
            if(up && incr > 0){
                incr -= 1;
                pos_y -= 2 *incr;
            }else{
                if(incr < 3){
                    up = false;
                    incr += 1;
                }
                pos_y += 2 * incr;
            }
        }else{
            grounded_once = true;
        }
    }else{
        grounded_once = true;
    }
}

void Turnip::set_graphics(){
    graphics->setPixmap(tileset[0]->scaled(character_size*scale,character_size*scale,Qt::KeepAspectRatio));
    graphics->setPos(pos_x, pos_y);
}

void Turnip::set_character_animation(){  
    if(grounded_once){
        character_size = 90;
        gravity_speed = 0;
        if(current_tile == 0){
            pos_y -= 50;
            pos_x -= 30;
        }
        current_tile = (current_tile %3) +1;
        if(current_tile == 3){
            touch = true;
        }
        stucked = S_LEFT;
        grounded = true;
    }

    graphics->setPos(pos_x,pos_y);
    graphics->setPixmap(tileset[current_tile]->scaled(character_size*scale,character_size*scale,Qt::KeepAspectRatio));
}
