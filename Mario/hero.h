#ifndef HERO_H
#define HERO_H

#include "character.h"
#include "player_info.h"

class Hero : public Character{
public :
    Hero(int l, int c, QVector<QPixmap*> ts, Container cntnr, int size, int spd);
    void move();
    void set_initial_param(int view_height);
    inline void set_enable_to_move(bool new_val){enable_to_move = new_val;}
    inline void set_flower(bool val){flower=val;}
    void check_color();
    inline bool get_flower() const{return flower;}
    inline bool get_invulnerable() const{return is_invulnerable;}
    inline bool get_wait_turnip() const{return wait_turnip;}
    inline bool get_enable_to_move() const{return enable_to_move;}
    inline void set_wait_turnip(bool new_val){wait_turnip = new_val;}
    inline void set_invulnerable(bool new_val){is_invulnerable = new_val;}
    inline QVector<QPixmap*> get_initial_ts() const{return initial_ts;}
    inline void set_coins_number(){coins_number++;}
    inline int get_coins_number(){return coins_number;}

    ~Hero();

private:
    int coins_number;
    QVector<QPixmap*> initial_ts;
    bool flower;
    bool is_invulnerable;
    bool enable_to_move;
    bool wait_turnip;
};

#endif // HERO_H
