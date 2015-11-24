#ifndef TURNIP_H
#define TURNIP_H

#include "character.h"

class Turnip : public Character{


public:
    Turnip(int l, int c, QVector<QPixmap *> ts, Container cntnr, int t_size, int speed, int x, int y, bool is_left);
    void set_graphics();
    void set_character_animation();
    void move();
    inline void set_touch(bool new_value){touch = new_value;}
    inline bool get_touch()const{return touch;}
    inline bool get_grounded_once() const{return grounded_once;}
    ~Turnip(){}
private:
    bool touch;
    bool up;
    bool left;
    bool grounded_once;
    int max_incr;
    int incr;
};

#endif // TURNIP_H
