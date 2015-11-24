#ifndef BLOCK_H
#define BLOCK_H
#include "element.h"

class Block: public Element{
public:
    Block(int l, int c, QVector<QPixmap *> ts, Container cntnr);
    void move_graphics();
    void set_pos_y_ini();
    void set_gravity_speed(int shift){gravity_speed+= shift;}
    inline void set_gravity_speed_total(int new_val){gravity_speed= new_val;}
    inline int get_gravity_speed(){return gravity_speed;}
    inline void set_hit(bool val){hit = val;}
    inline bool get_hit(){return hit;}
    inline int get_pos_y_ini(){return pos_y_ini;}
    ~Block(){}

protected:
    bool hit;
    int gravity_speed;
    int pos_y_ini;
};

#endif // BLOCK_H
