#ifndef CHARACTER_H
#define CHARACTER_H

#include "element.h"

enum Direction{STAND, RIGHT, LEFT};
enum Stucked{NO, S_RIGHT, S_LEFT};
enum Turn_tile{T_NO, T_RIGHT, T_LEFT};

class Character : public Element{
public:
    Character(int l, int c, QVector<QPixmap*> ts, Container cntnr, int size, int spd);
    virtual void move() = 0;
    inline Direction get_direction_x() const{return direction_x;}
    inline int get_speed() const{return speed;}
    inline int get_character_size() const{return character_size;}
    inline int get_gravity_speed() const{return gravity_speed;}
    inline bool get_grounded() const{return grounded;}
    inline bool get_on_carapace() const{return on_carapace;}
    inline bool get_tile_flipped() const{return tile_flipped;}
    inline Stucked get_stucked()const{return stucked;}
    inline void set_character_size(float new_val){character_size = new_val;}
    inline void set_stucked(Stucked s){stucked =s;}
    inline void set_on_carapce(bool new_val){on_carapace = new_val;}
    inline void set_tile_flipped(bool flp){tile_flipped = flp;}
    inline void set_speed(int s){speed += s;}
    inline void set_gravity_speed(int s){gravity_speed += s;}
    inline void set_gravity_speed_total(int s){gravity_speed = s;}
    inline void set_grounded(bool grnd){grounded = grnd;}
    inline void set_direction_x(Direction dir){direction_x = dir;}
    void set_character_animation();
    void gravity_jump();
    virtual ~Character(){}
protected:
    Direction direction_x;
    int speed;
    int character_size;
    Stucked stucked;
    bool tile_flipped;
    int gravity_speed;
    bool grounded;
    Turn_tile tile_transformation;
    bool on_carapace;

};

#endif // CHARACTER_H
