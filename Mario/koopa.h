#ifndef KOOPA_H
#define KOOPA_H

#include "monster.h"

class Koopa : public Monster{
public :
    Koopa(int l, int c, QVector<QPixmap*> ts, Container cntnr, int size, int spd);
    void set_character_animation();
    inline void set_carapace(int new_val){carapace = new_val;}
    inline void set_moving_carapace(int new_val){moving_carapace = new_val;}
    inline bool get_carapace() const{return carapace;}
    inline bool get_moving_carapace() const{return moving_carapace;}
    inline int get_counter_carapace() const{return counter_carapace_active;}
    void move();
    ~Koopa(){}
private:
    bool carapace;
    bool moving_carapace;
    int speed_max;
    int counter_carapace_active;
};

#endif // KOOPA_H
