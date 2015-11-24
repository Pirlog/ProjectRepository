#include "view.h"
#include "QDebug"
#include <QGraphicsPixmapItem>
#include <QMovie>
#include <QPoint>
#include <QColor>
#include <QTextEdit>
#include <QFontDatabase>
#include <QGraphicsTextItem>


View::View(unsigned int s_wdth, unsigned int v_wdth, unsigned int height, unsigned int e_size):
    scene_width(s_wdth), view_width(v_wdth), view_height(height), element_size(e_size){
    scene_height = height;
    menu = new StartMenu(view_width, view_height);
    map = new QGraphicsScene();
    window = new QGraphicsView();
    map -> setSceneRect(0,0,scene_width, scene_height);
    map->setBackgroundBrush(QBrush(QColor(82,142,233)));
    window->setFixedSize(view_width, view_height);
    window->move(0,0);
    window->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    window->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    actual_scene= MENU;
    bg_music = new QMediaPlayer();
    sounds = new QMediaPlayer();
    playlist = new QMediaPlaylist(bg_music);
    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
}

void View::show_scene(Scene s){
    if(s == MENU){
        window->setScene(menu);
        menu->start_movie();
        actual_scene = MENU;
        menu -> addItem(this);
        this->setFocus();
    }else{
        menu->stop_movie();
        window->setScene(map);
        actual_scene = MAP;
        map->addItem(this);
        this->setFocus();
    }
    window->show();
}

void View::update_animated_elem(const QVector<Element*> elem){
    for(int i=0; i<elem.size();i++){
        elem[i]->set_graphics_pixmap(element_size);
    }
}

void View::update_character(Character *c){
    if(c->get_o_type()== GOOMBA){
        Goomba* g = static_cast<Goomba*>(c);
        g->set_character_animation();
    }else if(c->get_o_type()== KOOPA){
        Koopa* k = static_cast<Koopa*>(c);
        k->set_character_animation();
    }else if(c->get_o_type()== BAD_FLOWER){
        Bad_flower* f = static_cast<Bad_flower*>(c);
        f->set_character_animation();
    }else{
        c->set_character_animation();
    }
    if(c->get_pos_y()>map->height()){
        if(Hero* derived = dynamic_cast<Hero*>(c)){
            emit hero_died(derived);
        }else{
            emit monster_died(static_cast<Monster*>(c));
        }
    }
}

void View::update_blocks(QVector<Block *> block_hit){
    for(int i= 0; i < block_hit.size(); i++ ){
        block_hit[i]->move_graphics();
    }
}

void View::update_info(int hero_x, Player_info *pi){
    pi->move_x(hero_x, window->width());
}


void View::add_player_info(Player_info* pi){
    pi->display_graphics(element_size, 2, window->width());
    map->addItem(pi->get_graphics());
    map->addItem(pi->get_score());

}

void View::add_hero(Hero *h){
    h->set_graphics(scene_height, element_size);
    map->addItem(h->get_graphics());
}

void View::add_turnip(Turnip *t){
    t->set_graphics();
    map->addItem(t->get_graphics());

}

void View::add_elem_to_scene(const QVector<Element *> elems){
    for(int i=0 ; i< elems.size(); i++){
        if(elems[i]->get_o_type()==CASTLE){
            elems[i]->set_graphics(scene_height,element_size);
        }else{
            elems[i]->set_graphics(scene_height,element_size);
        }
        map->addItem(elems[i]->get_graphics());
    }
}

void View::add_elem_to_scene(const QVector<Monster *> elems){
    for(int i=0 ; i< elems.size(); i++){
        elems[i]->set_graphics(scene_height,element_size);
        map->addItem(elems[i]->get_graphics());
    }
}


void View::add_animated_elem_to_scene(const QVector<Element *> elems){
    for(int i=0 ; i< elems.size(); i++){
        elems[i]->set_graphics(scene_height,element_size);
        map->addItem(elems[i]->get_graphics());
    }
}

void View::keyPressEvent(QKeyEvent *event){
    switch(event -> key()){
    case Qt::Key_Left :
        emit key_event(K_LEFT, PRESS);
        break;
    case Qt::Key_Right :
        emit key_event(K_RIGHT, PRESS);
        break;
    case Qt::Key_Up :
        emit key_event(K_UP, PRESS);
        break;
    case Qt::Key_Return:
        emit key_event(K_ENTER, PRESS);
        break;
    case Qt::Key_Space:
        emit key_event(K_SPACE, PRESS);
        break;
    default:break;
    }
}

void View::keyReleaseEvent(QKeyEvent *event){
    if(!(event ->isAutoRepeat())){
        switch(event -> key()){
        case Qt::Key_Left :
            emit key_event(K_LEFT, RELEASE);
            break;
        case Qt::Key_Right :
            emit key_event(K_RIGHT, RELEASE);
            break;
        default:break;
        }
    }
}

void View::center_view(int x, int y){
    window->centerOn(QPoint(x,y));
}

void View::check_collision(Character *c){
    Peach_graphics_item* ch_g = c->get_graphics();
    QList<QGraphicsItem*> colliding_items = ch_g->collidingItems();
    for(int i = 0, n = colliding_items.size(); i < n; ++i){
        Peach_graphics_item *c_i= static_cast<Peach_graphics_item*>(colliding_items[i]);
        emit emission_detected(c, c_i, c_i->x(), c_i->y(), ch_g->boundingRect().width(), ch_g->boundingRect().height());
    }
}

void View::remove_item_from_scene(QGraphicsItem *gi){
    map->removeItem(gi);
}

void View::update_block(Block *e, QVector<Element*> v){
    map->removeItem(e->get_graphics());
    v.last()->set_graphics(scene_height,element_size);
    map->addItem(v.last()->get_graphics());
}

void View::remove_item(Element *e){
    map->removeItem(e->get_graphics());
}

void View::remove_elements(QVector<Element*> e)
{
    for(int i = 0; i< e.size(); i++){
        map->removeItem(e[i]->get_graphics());
    }
}


void View::add_bonus(QVector<Element *> vector){
    vector.last()->set_graphics(scene_height,element_size);
    map->addItem(vector.last()->get_graphics());
}

void View::play_sounds(QString sound, char type)
{
    if (type == 's'){
        sounds->setMedia(QUrl(sound));
        sounds->play();
    }
    else
    {
        if(!playlist->isEmpty()){
            playlist->removeMedia(0);
        }
        playlist->addMedia(QUrl(sound));
        playlist->setCurrentIndex(0);
        bg_music->setPlaylist(playlist);
        bg_music->play();
    }
}

void View::display_text_item(QGraphicsTextItem *ti, int x, int y){
    ti->setPos(x,y);
    ti->setScale(3);
    map->addItem(ti);
}

View::~View(){
    delete menu;
    delete window;
    delete map;
    delete sounds;
    delete bg_music;
    delete playlist;
}
