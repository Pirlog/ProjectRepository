#include "koopa.h"

Koopa::Koopa(int l, int c, QVector<QPixmap *> ts, Container cntnr, int size, int spd) : Monster(l,c,ts,cntnr,size,spd){
    carapace = false;
    speed_max = 2;
    counter_carapace_active = 0;
    moving_carapace = false;
}


void Koopa::set_character_animation(){
    graphics->setPos(pos_x,pos_y);
    if(carapace == false){
        current_tile = (current_tile +1) % 8 ;
    }else if(carapace == true && moving_carapace == false){
        speed_max = 10;
        character_size = 30;
        if(current_tile != 12){current_tile = (current_tile % 5 + 1) + 8;}
        else{direction_x = STAND; counter_carapace_active =0;}
    }else if(carapace == true && moving_carapace == true){
        speed_max = 10;
        current_tile = (current_tile % 5 + 1) + 12;
        counter_carapace_active ++;
    }
    switch(tile_transformation){
    case T_LEFT:
        graphics->setTransform(QTransform::fromScale(1,1));
        break;
    case T_RIGHT:
        graphics->setTransform(QTransform().translate(graphics->boundingRect().width(),0).scale(-1,1));
        break;
    default: break;
    }

    graphics->setPixmap(tileset[current_tile]->scaled(character_size*scale,character_size*scale,Qt::KeepAspectRatio));
}

void Koopa::move()
{
   // if(direction_x != STAND)
    gravity_jump();
    if(stucked == S_LEFT){
        direction_x = RIGHT;
    }else if(stucked == S_RIGHT){
        direction_x = LEFT;
    }
    if(direction_x == LEFT){
        stucked= NO;
        pos_x -= speed_max;
        if(!tile_flipped){
            tile_transformation = T_LEFT;
            tile_flipped = true;
        }else{
            tile_transformation=T_NO;
        }
    }
    if(direction_x == RIGHT){
        stucked = NO;
        pos_x += speed_max;
        if(tile_flipped){
            tile_transformation = T_RIGHT;
            tile_flipped = false;
        }else{
            tile_transformation=T_NO;
        }
    }
}
