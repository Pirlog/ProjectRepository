#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QFont>
#include <QPixmap>


class Player_info{
public:
    Player_info(QPixmap* t, QString s, QFont* f, int x_percent);
    inline QGraphicsPixmapItem* get_graphics()const {return graphics;}
    inline QGraphicsTextItem* get_score()const {return score;}
    void display_graphics(int element_size, int text_scale, int view_size);
    void move_x(int new_position, int window_width);
    void reset_score();
    void set_score(int add);
    void set_score_total(int new_val);
    inline int get_score_number() const{return score->toPlainText().toInt();}
    ~Player_info();
private:
    QPixmap* tile;
    QGraphicsPixmapItem* graphics;
    QGraphicsTextItem* score;
    QFont* font;
    int x_pos_percent;
    float tile_scale;
    int view_size;
};

#endif // PLAYER_INFO_H
