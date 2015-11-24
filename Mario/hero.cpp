#include "hero.h"
#include "champi.h"
#include "pipe.h"
#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QGraphicsColorizeEffect>


Hero::Hero(int l, int c, QVector<QPixmap *> ts, Container cntnr, int size, int spd): Character(l,c,ts, cntnr, size, spd){
    scale=0.8;
    initial_ts = ts;
    flower = false;
    is_invulnerable = false;
    coins_number = 0;
    wait_turnip = false;
    enable_to_move = true;
}


void Hero::set_initial_param(int view_height){
    pos_x = column * 30;
    pos_y = view_height/2;
    scale = 0.8;
    set_tileset(initial_ts);
    flower = false;
}

void Hero::move(){
    if(enable_to_move){
        check_color();
        gravity_jump();
        if(direction_x == LEFT){
            if(stucked != S_LEFT){
                stucked= NO;
                if(speed >= -6) {speed = speed - 3;}
                pos_x += speed;
                if(!tile_flipped){
                    tile_transformation = T_LEFT;
                    tile_flipped = true;
                }else{
                    tile_transformation=T_NO;
                }
            }
        }
        if(direction_x == RIGHT){
            if(stucked!= S_RIGHT){
                stucked = NO;
                if(speed <= 6) {speed = speed + 3;}
                pos_x += speed;
                if(tile_flipped){
                    tile_transformation = T_RIGHT;
                    tile_flipped = false;
                }else{
                    tile_transformation=T_NO;
                }
            }
        }
    }
}

void Hero::check_color(){
    QGraphicsColorizeEffect* colEffect = new QGraphicsColorizeEffect;
    QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect;
    if(is_invulnerable){

        colEffect->setColor(Qt::magenta);
        graphics->setGraphicsEffect(colEffect);
    }else{
        opacity->setOpacity(1);
        graphics->setGraphicsEffect(opacity);
    }
}

Hero::~Hero(){
    initial_ts.clear();
}

