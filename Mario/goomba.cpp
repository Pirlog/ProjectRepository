
#include "goomba.h"


Goomba::Goomba(int l, int c, QVector<QPixmap *> ts, Container cntnr, int size, int spd) : Monster(l,c,ts,cntnr,size,spd){
}


void Goomba::set_character_animation(){
    graphics->setPos(pos_x,pos_y);
    if(alive == true){
        current_tile = (current_tile +1) % 11 ;
    }else{
        current_tile = (current_tile % 2) + 11;
    }
    switch(tile_transformation){
    case T_RIGHT:
        graphics->setTransform(QTransform::fromScale(1,1));
        break;
    case T_LEFT:
        graphics->setTransform(QTransform().translate(graphics->boundingRect().width(),0).scale(-1,1));
        break;
    default: break;
    }

    graphics->setPixmap(tileset[current_tile]->scaled(character_size*scale,character_size*scale,Qt::KeepAspectRatio));
}

void Goomba::move()
{
 //   if(direction_x != STAND)
    gravity_jump();
    if(stucked == S_LEFT){
        direction_x = RIGHT;
    }else if(stucked == S_RIGHT){
        direction_x = LEFT;
    }
    if(direction_x == LEFT){
        stucked= NO;
        if(speed >= -2) {speed = speed - 1;}
        pos_x += speed;
        if(!tile_flipped){
            tile_transformation = T_LEFT;
            tile_flipped = true;
        }else{
            tile_transformation=T_NO;
        }
    }
    if(direction_x == RIGHT){
        stucked = NO;
        if(speed <= 2) {speed = speed + 1;}
        pos_x += speed;
        if(tile_flipped){
            tile_transformation = T_RIGHT;
            tile_flipped = false;
        }else{
            tile_transformation=T_NO;
        }
    }
}
