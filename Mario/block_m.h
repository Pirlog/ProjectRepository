#ifndef BLOCK_M_H
#define BLOCK_M_H

#include "block.h"
#include "loot.h"

class Block_m : public Block{
public:
    Block_m(int l, int c, QVector<QPixmap*> ts, Container cntnr);
    inline bool get_hit() const{return hit;}
    inline int get_gravity_speed() const{return gravity_speed;}
    inline Surprise get_surprise(){return bonus;}
    inline void set_surprise(Surprise b){bonus = b;}
private:
    bool hit;
    int gravity_speed;
    Surprise bonus;
    ~Block_m(){}
};

#endif // BLOCK_M_H
