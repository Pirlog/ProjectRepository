#ifndef GAME_H
#define GAME_H

#include "model.h"
#include "view.h"
#include "separator.h"
#include <QObject>
#include <QTimer>


class Game : public QObject{
    Q_OBJECT
public:
    Game(Model* m, View* v);
    void send_elem_to_map();
    void handle_collision();
    void character_ground_check(Character* ch, Peach_graphics_item* cld_i, int cld_y, int ch_height);
    void update_blocks_hit();
    void update_turnips();
    void peach_died(Hero* h);
    void goomba_collision(Hero *, Goomba* g);
    void koopa_collision(Hero *, Koopa* k);
    void stuck_check(Character* ch, Peach_graphics_item* cld_i, int cld_x, int cld_y, int ch_width, int ch_height);
    void touch_blocks(Character *ch, Peach_graphics_item *cld_i, int cld_y, int ch_height);
    void touch_bonus(Character *ch, Peach_graphics_item *cld_i);
    void touch_monster(Character *ch, Peach_graphics_item *cld_i);
    void character_touch_item(Character* c, Peach_graphics_item *c_i, int cld_x,int cld_y, int ch_width, int ch_height);
    void key_pressed(Keys k);
    void key_released(Keys k);
    void block_b_hit(Character*c, Block* b);
    void block_m_hit(Character*c, Block* b);
    void update_map();
    void remove_elems(Separator *s);
    void add_elems(Separator* s);
    void animated_charaters();
    void animate_items();
    void update_score();
    ~Game();
public slots :
    void reaction_to_key_event(Keys k, Event e);
    void display_animation();
    void animate_world();
    void collision_reaction(Character* ch,Peach_graphics_item* cld_i,int cld_x,int cld_y,int ch_width,int ch_height);
    void monster_dead(Monster* m);
    void reinit_game(Hero* h);
    void invulnerability_off();
    void turnip_on();
    void exit_game();
private :
    Model *model;
    View *view;
    Map_separator* mp;
    bool hero_collision;
    bool end;
};

#endif // GAME_H
