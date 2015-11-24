#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "peach_graphics_item.h"
#include <QVector>
#include <QKeyEvent>
#include "ground.h"
#include "block_b.h"
#include "block_n.h"
#include "pipe.h"
#include "coin.h"
#include "turnip.h"
#include "goomba.h"
#include "koopa.h"
#include "bad_flower.h"
#include "monster.h"
#include "block_m.h"
#include "hero.h"
#include "character.h"
#include "player_info.h"
#include "start.h"
#include <QObject>
#include <QGraphicsRectItem>
#include <QMediaPlayer>
#include <QMediaPlaylist>

enum Keys{K_LEFT, K_RIGHT, K_UP, K_ENTER, K_SPACE};
enum Scene{MENU, MAP};
enum Event{PRESS, RELEASE};

class View: public QObject, public QGraphicsRectItem{
    Q_OBJECT
public:
    View(unsigned int s_wdth, unsigned int v_wdth, unsigned int height,unsigned int e_size);
    void init_view();
    inline QGraphicsView* get_window()  const {return window;}
    inline QGraphicsScene* get_menu()  const {return menu;}
    inline QGraphicsScene* get_map()  const {return map;}
    inline unsigned int get_view_width() const {return view_width;}
    inline unsigned int get_view_height()  const {return view_height;}
    inline unsigned int get_scene_width() const {return scene_width;}
    inline unsigned int get_scene_height()  const {return scene_height;}
    inline int get_element_size()  const {return element_size;}
    inline Scene get_actual_scene()  const {return actual_scene;}
    void add_elem_to_scene(QVector<Element*> const elems);
    void add_elem_to_scene(QVector<Monster*> const elems);
    void add_animated_elem_to_scene(QVector<Element*> const elems);
    void add_player_info(Player_info* pi);
    void add_hero(Hero* h);
    void add_turnip(Turnip* t);
    void show_scene(Scene s);
    void update_animated_elem(QVector<Element*> elem);
    void update_character(Character* c);
    void update_blocks(QVector<Block*> block_hit);
    void update_info(int hero_x, Player_info* pi);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void center_view(int x, int y);
    void check_collision(Character* c);
    void remove_item_from_scene(QGraphicsItem* gi);
    void update_block(Block* e, QVector<Element*> v);
    void remove_item(Element* e);
    void remove_elements(QVector<Element*> e);
    void add_bonus(QVector<Element*> vector);
    void play_sounds(QString sound, char type);
    void display_text_item(QGraphicsTextItem* ti, int x, int y);
    ~View();

signals:
    void emission_detected(Character* c, Peach_graphics_item* cld_obj, int cld_x, int cld_y, int ch_width, int ch_height);
    void key_event(Keys k,Event e);
    void hero_died(Hero* h);
    void monster_died(Monster* m);
private:
    QGraphicsView *window;
    StartMenu* menu;
    QGraphicsScene* map;
    unsigned int scene_width;
    unsigned int view_width;
    unsigned int view_height;
    unsigned int scene_height;
    int element_size;
    Scene actual_scene;
    QMediaPlayer *bg_music;
    QMediaPlayer *sounds;
    QMediaPlaylist *playlist;
};

#endif // VIEW_H
