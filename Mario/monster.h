#ifndef MONSTER_H
#define MONSTER_H

#include "character.h"

class Monster : public Character{
public :
    Monster(int l, int c, QVector<QPixmap*> ts, Container cntnr, int size, int spd);
    virtual void move()=0;
    virtual void set_character_animation()=0;
    inline void set_alive(bool new_val){alive = new_val;}
    inline bool get_alive()const{return alive;}
    ~Monster(){}
protected :
    bool alive;
};


#endif // MONSTER_H
