#include <QDebug>
#include "character.h"


Character::Character(int l, int c, QVector<QPixmap *> ts, Container cntnr, int size, int spd) :
    Element(l,c,ts, cntnr), speed(spd)
{
    character_size = size;
    tile_flipped = false;
    direction_x = STAND;
    stucked = NO;
    gravity_speed = 0;
    grounded = false;
    tile_transformation = T_NO;
    on_carapace = false;
}

void Character::gravity_jump()
{
    if(grounded == false && gravity_speed <= 14){
        gravity_speed += 3;
    }
    pos_y += gravity_speed;
}

void Character::set_character_animation()
{
    graphics->setPos(pos_x,pos_y);
    if(direction_x == STAND){
        current_tile = 0;
    }else{
        current_tile = ((current_tile) % (tileset.size()-1)) +1;
        switch(tile_transformation){
        case T_RIGHT:
            graphics->setTransform(QTransform::fromScale(1,1));
            break;
        case T_LEFT:
            graphics->setTransform(QTransform().translate(graphics->boundingRect().width(),0).scale(-1,1));
            break;
        default: break;
        }
    }
    graphics->setPixmap(tileset[current_tile]->scaled(character_size*scale,character_size*scale,Qt::KeepAspectRatio));
}
