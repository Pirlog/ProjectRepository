#ifndef PEACH_GRAPHICS_ITEM_H
#define PEACH_GRAPHICS_ITEM_H

#include <QGraphicsPixmapItem>


class Element;

enum Container{BLOCK_B, BLOCK_M, BLOCK_N, FLOWER, COIN, HERO, PIPE, GROUND_UP, GROUND, BIG_CHAMP, SMALL_CHAMP, MONSTER, TURNIP, KOOPA, GOOMBA,
              BAD_FLOWER, CASTLE, CLOUD, HERO_FIRE};

class Peach_graphics_item : public QGraphicsPixmapItem{
public:
    Peach_graphics_item(Container c, Element* e);
    inline Container get_container_type() const{return container_type;}
    inline Element* get_container_object() const{return container_object;}
    ~Peach_graphics_item();
private:
    Container container_type;
    Element* container_object;
};

#endif // PEACH_GRAPHICS_ITEM_H
