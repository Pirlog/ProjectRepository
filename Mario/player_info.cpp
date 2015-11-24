#include "player_info.h"


Player_info::Player_info(QPixmap *t, QString s, QFont* f, int x_percent): tile(t), font(f), x_pos_percent(x_percent){
    score = new QGraphicsTextItem(s);
    graphics = new QGraphicsPixmapItem();
    score->setFont(*font);
    tile_scale = 1;
}

void Player_info::display_graphics(int element_size, int text_scale, int v_s){
    view_size=v_s;
    graphics->setPixmap(tile->scaled(element_size*tile_scale,element_size*tile_scale,Qt::KeepAspectRatio));
    graphics->setPos(view_size * x_pos_percent/100, 8);
    score->setPos((view_size * x_pos_percent/100) + 30, 0);
    score->setScale(text_scale);
}

void Player_info::move_x(int new_position, int window_width){
    if(new_position > window_width/2){
        graphics->setPos(new_position+(view_size * x_pos_percent/100) - view_size/2 , 8);
        score->setPos(new_position+(view_size * x_pos_percent/100)- view_size/2 + 30, 0);
    }else{
        graphics->setPos(view_size * x_pos_percent/100, 8);
        score->setPos((view_size * x_pos_percent/100) + 30, 0);
    }
}


void Player_info::reset_score(){
    score->setPlainText("0");
}

void Player_info::set_score(int add){
    int new_score = score->toPlainText().toInt() + add;
    score -> setPlainText(QString::number(new_score));
}

void Player_info::set_score_total(int new_val){
    score -> setPlainText(QString::number(new_val));
}

Player_info::~Player_info(){
    delete tile;
    delete graphics;
    delete score;
    delete font;
}
