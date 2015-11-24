#ifndef MODEL_H
#define MODEL_H

#include "hero.h"
#include "ground.h"
#include "block.h"
#include "block_n.h"
#include "block_b.h"
#include "block_m.h"
#include "flower.h"
#include "tile_factory.h"
#include "champi.h"
#include "turnip.h"
#include "bad_flower.h"
#include "pipe.h"
#include "coin.h"
#include "monster.h"
#include "goomba.h"
#include "koopa.h"
#include "player_info.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QString>
#include <QLinkedList>
#include <QFontDatabase>

enum Sound_t{MENU_S, GAME_S, TURNIP_S, MONSTER_S, COIN_S};

class Model{
public:
    Model();
    QJsonObject read_json_file(QString const file);
    void init_map();
    void init_monsters();
    void init_player_info();
    void init_items();
    void init_bonus();
    void reinit_game(int scene_height);
    void init_decor(QJsonArray decor_array);
    void init_coins(QJsonArray coins_array);
    void init_blocks(QJsonArray blocks_array);
    void init_pipes(QJsonArray pipes_array);
    void init_sounds();
    void add_loot(Block_m* block,  Character* c);
    void add_monster(QJsonArray monster_array, char type);
    void add_block(int const line, int const column, QString const type);
    void add_coin(int const line, int const column);
    void add_pipe(int const height, int const line,int const column);
    void add_ground(int const l_beg, int const l_end, int const c_beg, int const c_end);
    void add_turnip(Hero* h, int turnip_size);
    void add_block_hit(Block* b){block_hit.push_back(b);}

    void set_coin();
    void update_block(Block* b, Container c);

    void turnip_remove(int i);
    void monster_remove(int i);
    void remove_animated_element(int i);
    void remove_block_hit(){block_hit.pop_front();}

    inline Hero* get_hero()const{return peach;}
    inline QVector<Element*> get_monster_vector() const{return monster_vector;}
    inline QVector<Element*> get_block_n_vector() const{return block_n_vector;}
    inline QVector<Element*> get_elements() const{return elements;}
    inline QVector<Element*> get_animated_elements() const{return animated_elements;}
    inline QVector<Element*> get_turnip_vector() const{return turnip_vector;}
    inline QVector<Block*> get_bock_hit() const{return block_hit;}
    inline Player_info* get_player_coin() const{return player_coins;}
    inline Player_info* get_player_life() const{return player_life;}
    inline QString get_sound(Sound_t type) const{return sounds_vector[type];}
    inline QGraphicsTextItem* get_end_message()const{return end_message;}
    ~Model();

private:
    struct ElementCompare {
      bool operator()(const Element* l, const Element* r) {
        return *l < *r;
      }
    };
    Hero* peach;
    Tile_factory* tf;
    QFont* peach_font;
    QString path_background;
    QVector<Element*> animated_elements;
    QVector<Element*> elements;
    QVector<Element*> block_n_vector;
    QVector<Element*> monster_vector;
    QVector<Block*> block_hit;
    Player_info* player_coins;
    Player_info* player_life;
    QVector<Element*> turnip_vector;
    QVector<QString> sounds_vector;
    QGraphicsTextItem* end_message;
};

#endif // MODEL_H
