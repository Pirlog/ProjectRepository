#ifndef BAD_FLOWER_H
#define BAD_FLOWER_H

#include "monster.h"

class Bad_flower : public Monster{
public :
    Bad_flower(int l, int c, QVector<QPixmap*> ts, Container cntnr, int size, int spd);
    void set_character_animation();
    void move();
    ~Bad_flower(){}
private :
    int state_counter;
    int max_height;
    bool shift;
};


#endif // BAD_FLOWER_H
