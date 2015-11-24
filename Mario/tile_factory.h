#include "peach_graphics_item.h"

#ifndef TILE_FACTORY
#define TILE_FACTORY



class Tile_factory{
public:
    Tile_factory();
    QVector<QPixmap*> tile_vector(Container c);
    ~Tile_factory();
private:
    QVector<QPixmap*> tileset_ground;
    QVector<QPixmap*> tileset_pipe;
    QVector<QPixmap*> tileset_block;
    QVector<QPixmap*> tileset_block_m;
    QVector<QPixmap*> tileset_loot;
    QVector<QPixmap*> tileset_peach;
    QVector<QPixmap*> tileset_peach_fire;
    QVector<QPixmap*> tileset_goomba;
    QVector<QPixmap*> tileset_turnip;
    QVector<QPixmap*> tileset_koopa;
    QVector<QPixmap*> tileset_flower;
    QVector<QPixmap*> tileset_decor;
};

#endif // TILE_FACTORY

