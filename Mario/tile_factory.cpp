#include "tile_factory.h"

Tile_factory::Tile_factory(){
    tileset_ground.push_back(new QPixmap(QString(":/settings/grass_beg.png"),0,Qt::AutoColor));
    tileset_ground.push_back(new QPixmap(QString(":/settings/grass_end.png"),0,Qt::AutoColor));
    tileset_ground.push_back(new QPixmap(QString(":/settings/ground_beg.png"),0,Qt::AutoColor));
    tileset_ground.push_back(new QPixmap(QString(":/settings/ground_end.png"),0,Qt::AutoColor));
    tileset_ground.push_back(new QPixmap(QString(":/settings/grass.png"),0,Qt::AutoColor));
    tileset_ground.push_back(new QPixmap(QString(":/settings/ground.png"),0,Qt::AutoColor));

    tileset_pipe.push_back(new QPixmap(QString(":/settings/pipe_beg_up.png"),0,Qt::AutoColor));
    tileset_pipe.push_back(new QPixmap(QString(":/settings/pipe_end_up.png"),0,Qt::AutoColor));
    tileset_pipe.push_back(new QPixmap(QString(":/settings/pipe_beg_down.png"),0,Qt::AutoColor));
    tileset_pipe.push_back(new QPixmap(QString(":/settings/pipe_end_down.png"),0,Qt::AutoColor));

    tileset_block.push_back(new QPixmap(QString(":/settings/block_b.png"),0,Qt::AutoColor));
    tileset_block.push_back(new QPixmap(QString(":/settings/block_n.png"),0,Qt::AutoColor));
    tileset_block_m.push_back(new QPixmap(QString(":/settings/block_m1.png"),0,Qt::AutoColor));
    tileset_block_m.push_back(new QPixmap(QString(":/settings/block_m2.png"),0,Qt::AutoColor));
    tileset_block_m.push_back(new QPixmap(QString(":/settings/block_m3.png"),0,Qt::AutoColor));
    tileset_block_m.push_back(new QPixmap(QString(":/settings/block_m4.png"),0,Qt::AutoColor));

    tileset_loot.push_back(new QPixmap(QString(":/settings/champi.png"),0,Qt::AutoColor));
    tileset_loot.push_back(new QPixmap(QString(":/settings/flower1.png"),0,Qt::AutoColor));
    tileset_loot.push_back(new QPixmap(QString(":/settings/flower2.png"),0,Qt::AutoColor));
    tileset_loot.push_back(new QPixmap(QString(":/settings/flower3.png"),0,Qt::AutoColor));
    tileset_loot.push_back(new QPixmap(QString(":/settings/coin1.png"),0,Qt::AutoColor));
    tileset_loot.push_back(new QPixmap(QString(":/settings/coin2.png"),0,Qt::AutoColor));
    tileset_loot.push_back(new QPixmap(QString(":/settings/coin3.png"),0,Qt::AutoColor));
    tileset_loot.push_back(new QPixmap(QString(":/settings/coin4.png"),0,Qt::AutoColor));

    tileset_peach.push_back(new QPixmap(QString(":/peach/peach_stand.png"),0,Qt::AutoColor));
    tileset_peach.push_back(new QPixmap(QString(":/peach/peach_run_dress0.png"),0,Qt::AutoColor));
    tileset_peach.push_back(new QPixmap(QString(":/peach/peach_run_dress1.png"),0,Qt::AutoColor));
    tileset_peach.push_back(new QPixmap(QString(":/peach/peach_run_dress2.png"),0,Qt::AutoColor));
    tileset_peach.push_back(new QPixmap(QString(":/peach/peach_run_dress3.png"),0,Qt::AutoColor));

    tileset_peach_fire.push_back(new QPixmap(QString(":/peach/peach_fire_stand.png"),0,Qt::AutoColor));
    tileset_peach_fire.push_back(new QPixmap(QString(":/peach/peach_fire_run0.png"),0,Qt::AutoColor));
    tileset_peach_fire.push_back(new QPixmap(QString(":/peach/peach_fire_run1.png"),0,Qt::AutoColor));
    tileset_peach_fire.push_back(new QPixmap(QString(":/peach/peach_fire_run2.png"),0,Qt::AutoColor));
    tileset_peach_fire.push_back(new QPixmap(QString(":/peach/peach_fire_run3.png"),0,Qt::AutoColor));

    tileset_goomba.push_back(new QPixmap(QString(":/enemies/GOOMBA_1.png"),0,Qt::AutoColor));
    tileset_goomba.push_back(new QPixmap(QString(":/enemies/GOOMBA_2.png"),0,Qt::AutoColor));
    tileset_goomba.push_back(new QPixmap(QString(":/enemies/GOOMBA_3.png"),0,Qt::AutoColor));
    tileset_goomba.push_back(new QPixmap(QString(":/enemies/GOOMBA_5.png"),0,Qt::AutoColor));
    tileset_goomba.push_back(new QPixmap(QString(":/enemies/GOOMBA_6.png"),0,Qt::AutoColor));
    tileset_goomba.push_back(new QPixmap(QString(":/enemies/GOOMBA_7.png"),0,Qt::AutoColor));
    tileset_goomba.push_back(new QPixmap(QString(":/enemies/GOOMBA_8.png"),0,Qt::AutoColor));
    tileset_goomba.push_back(new QPixmap(QString(":/enemies/GOOMBA_9.png"),0,Qt::AutoColor));
    tileset_goomba.push_back(new QPixmap(QString(":/enemies/GOOMBA_10.png"),0,Qt::AutoColor));
    tileset_goomba.push_back(new QPixmap(QString(":/enemies/GOOMBA_11.png"),0,Qt::AutoColor));
    tileset_goomba.push_back(new QPixmap(QString(":/enemies/GOOMBA_12.png"),0,Qt::AutoColor));
    tileset_goomba.push_back(new QPixmap(QString(":/enemies/GOOMBA_ECASE_1.png"),0,Qt::AutoColor));
    tileset_goomba.push_back(new QPixmap(QString(":/enemies/GOOMBA_ECASE_2.png"),0,Qt::AutoColor));

    tileset_turnip.push_back(new QPixmap(QString(":/settings/navet1.png"),0,Qt::AutoColor));
    tileset_turnip.push_back(new QPixmap(QString(":/settings/expl_1.png"),0,Qt::AutoColor));
    tileset_turnip.push_back(new QPixmap(QString(":/settings/expl_2.png"),0,Qt::AutoColor));
    tileset_turnip.push_back(new QPixmap(QString(":/settings/expl_3.png"),0,Qt::AutoColor));

    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_1.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_2.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_3.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_4.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_5.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_6.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_7.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_8.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_9.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_10.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_11.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_12.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_13.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_14.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_15.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_16.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_17.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_18.png"),0,Qt::AutoColor));
    tileset_koopa.push_back(new QPixmap(QString(":/enemies/koopa_19.png"),0,Qt::AutoColor));

    tileset_flower.push_back(new QPixmap(QString(":/enemies/bad_flower_2.png"),0,Qt::AutoColor));
    tileset_flower.push_back(new QPixmap(QString(":/enemies/bad_flower_1.png"),0,Qt::AutoColor));
    tileset_flower.push_back(new QPixmap(QString(":/enemies/bad_flower_3.png"),0,Qt::AutoColor));

    tileset_decor.push_back(new QPixmap(QString(":/settings/Castle.png"),0,Qt::AutoColor));
    tileset_decor.push_back(new QPixmap(QString(":/settings/cloud_thumb"),0,Qt::AutoColor));
}

QVector<QPixmap *> Tile_factory::tile_vector(Container c){
    switch(c){
    case FLOWER:
    case COIN:
    case BIG_CHAMP:
    case SMALL_CHAMP:
        return tileset_loot;
    case BAD_FLOWER :
        return tileset_flower;
        break;
    case CLOUD:
    case CASTLE:
        return tileset_decor;
        break;
    case KOOPA:
        return tileset_koopa;
        break;
    case GOOMBA:
        return tileset_goomba;
        break;
    case GROUND:
        return tileset_ground;
        break;
    case TURNIP:
        return tileset_turnip;
        break;
    case HERO:
        return tileset_peach;
        break;
    case HERO_FIRE:
        return tileset_peach_fire;
        break;
    case PIPE:
        return tileset_pipe;
        break;
    case BLOCK_B:
    case BLOCK_N:
        return tileset_block;
        break;
    case BLOCK_M:
        return tileset_block_m;
        break;
    default:
        return tileset_block_m;
        break;
    }
}

Tile_factory::~Tile_factory(){
    tileset_ground.clear();
    tileset_pipe.clear();
    tileset_block.clear();
    tileset_block_m.clear();
    tileset_loot.clear();
    tileset_peach.clear();
    tileset_peach_fire.clear();
    tileset_goomba.clear();
    tileset_turnip.clear();
    tileset_koopa.clear();
    tileset_flower.clear();
    tileset_decor.clear();
}
