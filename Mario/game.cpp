#include "game.h"
#include "element.h"
#include <typeinfo>
#include <QDebug>


Game::Game(Model *m, View *v){
    model = m;
    view = v;
    mp = new Map_separator(v->get_scene_width(), 4*v->get_view_width()/6);
    QTimer *timer_animation = new QTimer();
    connect(timer_animation, SIGNAL(timeout()), this, SLOT(display_animation()));
    timer_animation->start(150);
    connect(view, SIGNAL(key_event(Keys,Event)), this, SLOT(reaction_to_key_event(Keys,Event)));
    connect(view, SIGNAL(hero_died(Hero*)), this, SLOT(reinit_game(Hero*)));
    connect(view, SIGNAL(monster_died(Monster*)), this, SLOT(monster_dead(Monster*)));
    connect(view, SIGNAL(emission_detected(Character*,Peach_graphics_item*,int,int,int,int)),this,
            SLOT(collision_reaction(Character*,Peach_graphics_item*,int,int,int,int)));
    QTimer *timer_character = new QTimer();
    connect(timer_character, SIGNAL(timeout()), this, SLOT(animate_world()));
    timer_character->start(32);
    hero_collision = false;
    end = false;
}

void Game::send_elem_to_map(){
    mp->generate_separators_elem(model->get_elements());
    view->add_elem_to_scene(model->get_block_n_vector());
    view->add_elem_to_scene(model->get_elements().mid(mp->get_previous_part()->get_e_beg(), mp->get_next_part()->get_e_end()-mp->get_previous_part()->get_e_beg()));
    view->add_animated_elem_to_scene(model->get_animated_elements());
    view->add_elem_to_scene(model->get_monster_vector());
    view->add_hero(model->get_hero());
    view->add_player_info(model->get_player_coin());
    view->add_player_info(model->get_player_life());
    view->show_scene(MENU);
    view->play_sounds(model->get_sound(MENU_S),'m');
}

void Game::collision_reaction(Character* ch,Peach_graphics_item* cld_i,int cld_x,int cld_y,int ch_width,int ch_height){
    if (dynamic_cast<Hero*>(ch)) {
        hero_collision = true;
    }
    if(ch->get_o_type() == KOOPA && static_cast<Koopa*>(ch)->get_moving_carapace()){
        if(cld_i->get_container_type() == GOOMBA || cld_i->get_container_type() == KOOPA || cld_i->get_container_type() == BAD_FLOWER){
            view->play_sounds(model->get_sound(MONSTER_S),'s');
            view->remove_item(cld_i->get_container_object());
            monster_dead(static_cast<Monster*>(cld_i->get_container_object()));
        }
    }
    if((ch->get_o_type() == GOOMBA || ch->get_o_type() == KOOPA || ch->get_o_type() == BAD_FLOWER) &&
            cld_i ->get_container_type() == TURNIP){
        int pos_monster = model->get_monster_vector().indexOf(static_cast<Monster*>(ch));
        view->remove_item(ch);
        static_cast<Turnip*>(cld_i->get_container_object())->set_grounded(true);
        model->monster_remove(pos_monster);
    }else if(ch->get_o_type() == HERO){
        touch_bonus(ch, cld_i);
        touch_monster(ch, cld_i);
        if(cld_i->get_container_type() == BLOCK_B || cld_i->get_container_type() == BLOCK_N||
                cld_i->get_container_type() == BLOCK_M){
            touch_blocks(ch, cld_i, cld_y, ch_height);
        }
        if(cld_i->get_container_type() == CASTLE){
            static_cast<Hero*>(ch)->set_enable_to_move(false);
            view->display_text_item(model->get_end_message(), cld_x - cld_i->boundingRect().width(), cld_y - 100);
            end = true;
            QTimer::singleShot(2000,this,SLOT(exit_game()));
        }
    }
    stuck_check(ch, cld_i, cld_x, cld_y, ch_width, ch_height);
    if(ch->get_gravity_speed() >= 0){
        character_ground_check(ch,cld_i, cld_y, ch_height);
    }
}

void Game::monster_dead(Monster *m){
    int pos_monster = model->get_monster_vector().indexOf(m);
    model->monster_remove(pos_monster);
}

void Game::reinit_game(Hero *h){
    model->get_player_life()->set_score(-1);
    if(model->get_player_life()->get_score_number() > 0){
        h->set_initial_param(view->get_window()->height());
        view->update_character(h);
    }else{
        QCoreApplication::quit();
    }
}

void Game::invulnerability_off(){
    model->get_hero()->set_invulnerable(false);
}

void Game::turnip_on()
{
    model->get_hero()->set_wait_turnip(false);
}

void Game::handle_collision(){
    Hero* h = model->get_hero();
    h->set_grounded(false);
    for(int i = 0; i< model->get_monster_vector().size();i++){
        static_cast<Monster*>(model->get_monster_vector()[i])->set_grounded(false);
        view->check_collision(static_cast<Monster*>(model->get_monster_vector()[i]));
    }
    for(int i = 0; i< model->get_turnip_vector().size();i++){
        static_cast<Turnip*>(model->get_turnip_vector()[i])->set_grounded(false);
        view->check_collision(static_cast<Turnip*>(model->get_turnip_vector()[i]));
    }
    view->check_collision(h);
}


void Game :: character_ground_check(Character* ch, Peach_graphics_item* cld_i, int cld_y, int ch_height){
    if(((cld_i->get_container_type() == GROUND_UP ||
         cld_i->get_container_type() == BLOCK_B ||
         cld_i->get_container_type() == BLOCK_N ||
         cld_i->get_container_type() == BLOCK_M ||
         cld_i->get_container_type() == PIPE) &&
        cld_y + 15 >= ch->get_pos_y() + ch->get_scale() * ch->get_character_size())&&
            ch->get_pos_y() + ch->get_scale() * ch->get_character_size() >= cld_y &&
            ch->get_pos_x() < cld_i->x()+ cld_i->boundingRect().width()&&
            ch->get_pos_x() + ch->get_graphics()->boundingRect().width() > cld_i->x()
            ){
        ch->set_pos_y_total(cld_y +2 - ch_height);
        ch->set_gravity_speed_total(0);
        ch->set_grounded(true);
    }
}

void Game :: stuck_check(Character* ch, Peach_graphics_item* cld_i, int cld_x, int cld_y, int ch_width, int ch_height){
    if(cld_i->get_container_type() == BLOCK_B || cld_i->get_container_type() == BLOCK_M
            ||cld_i->get_container_type() == BLOCK_N || cld_i->get_container_type() == PIPE ){
        if(ch->get_pos_x() + ch_width - 15 <= cld_x
                && ch->get_pos_y()+ch_height > cld_y+15){
            ch->set_stucked(S_RIGHT);
        }
        if(ch->get_pos_x() + 15 >= cld_x + ch_width
                && ch->get_pos_y() + ch_height > cld_y+15){
            ch->set_stucked(S_LEFT);
        }
    }
}

void Game::update_blocks_hit(){
    for(int i=0; i < model->get_bock_hit().size(); i++){
        if(!model->get_bock_hit()[i]->get_hit()){
            model->remove_block_hit();
            break;
        }
        if(model->get_bock_hit()[i]->get_gravity_speed() > -4){
            model->get_bock_hit()[i]->set_gravity_speed(-2);
            model->get_bock_hit()[i]->set_pos_y(model->get_bock_hit()[i]->get_gravity_speed());
        }else{
            model->get_bock_hit()[i]->set_pos_y_total(model->get_bock_hit()[i]->get_pos_y_ini());
            model->get_bock_hit()[i]->set_hit(false);
            model->get_bock_hit()[i]->set_gravity_speed_total(0);
        }
    }
}

void Game::update_turnips(){
    for(int i = 0; i< model->get_turnip_vector().size(); i++){
        if(static_cast<Turnip*>(model->get_turnip_vector()[i])->get_grounded_once()){
            view->play_sounds(model->get_sound(TURNIP_S),'s');
        }
        if(static_cast<Turnip*>(model->get_turnip_vector()[i])->get_touch()){
            view->remove_item(model->get_turnip_vector()[i]);
            model->turnip_remove(i);
        }else{
            static_cast<Turnip*>(model->get_turnip_vector()[i])->set_character_animation();
            static_cast<Turnip*>(model->get_turnip_vector()[i])->move();
        }
    }
}

void Game::touch_blocks(Character *ch, Peach_graphics_item *cld_i, int cld_y, int ch_height){
    Block* b = static_cast<Block*>(cld_i->get_container_object());
    if(ch->get_pos_y() - 3 <= cld_y + ch_height + 3 && cld_y < ch->get_pos_y() &&
            ch->get_gravity_speed()<0){
        ch->set_gravity_speed_total(0);
    }
    if(cld_y + view->get_element_size() - 10 <= ch->get_pos_y() && ch->get_gravity_speed()>0
            && ch->get_pos_x() < cld_i->x()+ cld_i->boundingRect().width() - 10&&
            ch->get_pos_x() + ch->get_graphics()->boundingRect().width() > cld_i->x()+ 10){
        if(!b->get_hit()){
            b->set_pos_y_ini();
            b->set_hit(true);
            if(cld_i->get_container_type() == BLOCK_B){
                block_b_hit(ch, b);
            }
            if(cld_i->get_container_type() == BLOCK_M){
                block_m_hit(ch,b);
            }
            if(cld_i->get_container_type() == BLOCK_N){
                model->add_block_hit(b);
            }
        }
    }
}

void Game::block_b_hit(Character*c, Block* b){
    if(c->get_scale() == 1){
        model->update_block(b, BLOCK_B);
        view->remove_item(static_cast<Element*>(b));
    }else{
        model->add_block_hit(b);
    }
}

void Game::block_m_hit(Character*c, Block* b){
    Block_m* current_block = static_cast<Block_m*>(b);
    model->add_loot(current_block, c);
    view->add_bonus(model->get_animated_elements());
    model->update_block(b, BLOCK_M);
    view->update_block(b, model->get_block_n_vector());
    model->add_block_hit(static_cast<Block*>(model->get_block_n_vector().last()));
}

void Game::update_map(){
    int next_pos_beg = model->get_elements()[mp->get_current_part()->get_e_end()]->get_column() * view->get_element_size();
    int curr_pos_beg = model->get_elements()[mp->get_current_part()->get_e_beg()]->get_column() * view->get_element_size();
    if(model->get_hero()->get_pos_x() > next_pos_beg){
        if(mp->get_current_index() != 0 && mp->get_current_index() < mp->get_separator_number()){
            remove_elems(mp->get_previous_part());
            mp->set_previous_part(1);
        }
        if(mp->get_next_index() < mp->get_separator_number() ){
            mp->set_next_part(1);
            if(mp->get_next_index() != 1){
                add_elems(mp->get_next_part());
            }
            mp->set_current_part(1);
        }
    }else if(curr_pos_beg != 0 && model->get_hero()->get_pos_x() < curr_pos_beg){
        if(mp->get_current_index() != mp->get_separator_number()-1){
            remove_elems(mp->get_next_part());
            mp->set_next_part(-1);
        }
        if(mp->get_previous_index() != 0){
            mp->set_previous_part(-1);
            add_elems(mp->get_previous_part());
        }

        mp->set_current_part(-1);

    }

}

void Game::remove_elems(Separator *s){
    view->remove_elements(model->get_elements().mid(s->get_e_beg(), (s->get_e_end()-s->get_e_beg())));
}

void Game::add_elems(Separator *s){
    view->add_elem_to_scene(model->get_elements().mid(s->get_e_beg(), (s->get_e_end()-s->get_e_beg())));
}

void Game::touch_bonus(Character *ch, Peach_graphics_item *cld_i){
    if((cld_i->get_container_type() == COIN || cld_i->get_container_type() == BIG_CHAMP ||
        cld_i->get_container_type() == SMALL_CHAMP ||cld_i->get_container_type() == FLOWER) &&
            ch->get_o_type() == HERO){
        if(cld_i->get_container_type() == SMALL_CHAMP || cld_i->get_container_type() == BIG_CHAMP){
            static_cast<Champi*> (cld_i->get_container_object())->effect( static_cast<Hero*>(ch));
        }else if (cld_i->get_container_type() == FLOWER){
            static_cast<Flower*> (cld_i->get_container_object())->effect( static_cast<Hero*>(ch));
        }else{
            static_cast<Coin*> (cld_i->get_container_object())->effect( static_cast<Hero*>(ch));
            view->play_sounds(model->get_sound(COIN_S),'s');
            model->set_coin();
        }
        view->remove_item_from_scene(cld_i);
        int pos = model->get_animated_elements().indexOf(static_cast<Loot*>(cld_i->get_container_object()));
        model->remove_animated_element(pos);
    }
}

void Game::touch_monster(Character *ch, Peach_graphics_item *cld_i){
    if(cld_i->get_container_type()==BAD_FLOWER && (!static_cast<Hero*>(ch)->get_invulnerable())){
        peach_died(static_cast<Hero*>(ch));
        static_cast<Hero*>(ch)->set_invulnerable(true);
        QTimer::singleShot(1000,this,SLOT(invulnerability_off()));
    }else if(cld_i->get_container_type()== GOOMBA){
        goomba_collision(static_cast<Hero*>(ch), static_cast<Goomba*>(cld_i->get_container_object()));
    }else if(cld_i->get_container_type() == KOOPA){
        koopa_collision(static_cast<Hero*>(ch), static_cast<Koopa*>(cld_i->get_container_object()));
    }
}

void Game::goomba_collision(Hero *h, Goomba *g){
    if(h->get_pos_y()+ h->get_graphics()->boundingRect().height() - 10< g->get_pos_y()+ g->get_graphics()->boundingRect().height()
            && h->get_gravity_speed()>0){
        g->set_alive(false);
        view->play_sounds(model->get_sound(MONSTER_S),'s');
    }else if(g->get_alive() &&(!h->get_invulnerable())){
        peach_died(h);
    }
}

void Game::peach_died(Hero* h){
    if(h->get_scale() == 1){
        h->set_tileset(h->get_initial_ts());
        h->set_scale(0.8);
        h->set_flower(false);
    }else{
        model->get_player_life()->set_score(-1);
    }
    h->set_invulnerable(true);
    QTimer::singleShot(1000,this,SLOT(invulnerability_off()));
    if(model->get_player_life()->get_score_number() < 0){
        view->show_scene(MENU);
        view->play_sounds(model->get_sound(MENU_S),'m');
        model->reinit_game(view->get_scene_height());
        // QCoreApplication::quit();
    }
}

void Game::koopa_collision(Hero *h, Koopa *k){
    if(k->get_pos_y() + 15 >= h->get_pos_y() + h->get_scale() * h->get_character_size()&&
            h->get_pos_y() + h->get_scale() * h->get_character_size() >= k->get_pos_y() &&
            h->get_pos_x() < k->get_pos_x()+ k->get_graphics()->boundingRect().width()&&
            h->get_pos_x() + h->get_graphics()->boundingRect().width() > k->get_pos_x()
            ){
        if(!k->get_carapace()){
            view->play_sounds(model->get_sound(MONSTER_S),'s');
        }
        k->set_moving_carapace(false);
        k->set_carapace(true);
        h->set_pos_y_total(k->get_pos_y() +2- h->get_graphics()->boundingRect().height());
        h->set_grounded(true);
        h->set_gravity_speed_total(0);
    }else if(k->get_carapace() && h->get_pos_x() < k->get_pos_x()+ k->get_graphics()->boundingRect().width() - 10&&
             h->get_pos_x() + h->get_graphics()->boundingRect().width() > k->get_pos_x()+ 10){
        if(h->get_pos_x() > k->get_pos_x() + 10 && !k->get_moving_carapace()){
            k->set_direction_x(LEFT);
        }else if(h->get_pos_x()  < k->get_pos_x() && !k->get_moving_carapace()){
            k->set_direction_x(RIGHT);
        }
        k->set_moving_carapace(true);
    }else if(k->get_moving_carapace() == true && !h->get_invulnerable()){
        if(k->get_counter_carapace() > 15){peach_died(h);}
    }else if(!k->get_carapace()&& !h->get_invulnerable()){peach_died(h);}
}

void Game::display_animation(){
    for(int i=0; i<model->get_animated_elements().size(); i++){
        model->get_animated_elements()[i]->set_current_tile();
    }
    view->update_animated_elem(model->get_animated_elements());
}

void Game::animate_world(){
    animated_charaters();
    animate_items();
    update_score();
    update_map();
    view->update_character(model->get_hero());
    view->update_blocks(model->get_bock_hit());
    view->center_view(model->get_hero()->get_pos_x(), model->get_hero()->get_pos_y());
}

void Game::animated_charaters(){
    if(!end){
        hero_collision = false;
        handle_collision();
        for(int i = 0; i< model->get_monster_vector().size();i++){
            Monster *m =  static_cast<Monster*>(model->get_monster_vector()[i]);
            if(model->get_hero()->get_pos_x()> m->get_pos_x() - static_cast<int>(view->get_view_width()/2)
                    && model->get_hero()->get_pos_x() - m->get_character_size() < m->get_pos_x() + static_cast<int>(view->get_view_width()/2)){
                m->move();
                view->update_character(m);
            }
        }
        model->get_hero()->move();
        if(!hero_collision){
            model->get_hero()->set_stucked(NO);
            for(int i = 0; i< model->get_monster_vector().size();i++){
                static_cast<Monster*>(model->get_monster_vector()[i])->set_stucked(NO);
            }
        }
    }
}

void Game::animate_items(){
    update_blocks_hit();
    update_turnips();
}

void Game::update_score(){
    view->update_info(model->get_hero()->get_pos_x(),model->get_player_coin());
    view->update_info(model->get_hero()->get_pos_x(),model->get_player_life());
}

void Game::reaction_to_key_event(Keys k, Event e){
    if(e == PRESS){
        key_pressed(k);
    }else{
        key_released(k);
    }
}

void Game::key_pressed(Keys k){
    switch(k){
    case K_ENTER:
        if(view->get_actual_scene()== MENU){
            view->show_scene(MAP);
            view->play_sounds(model->get_sound(GAME_S),'m');
        }
        break;
    case K_LEFT:
        if(model->get_hero()->get_graphics()->x() > 0)
            model->get_hero()->set_direction_x(LEFT);
        break;
    case K_RIGHT:
        if(model->get_hero()->get_graphics()->x() +model->get_hero()->get_graphics()->boundingRect().width() <
                view->get_scene_width())
            model->get_hero()->set_direction_x(RIGHT);
        break;
    case K_UP:
        if(model->get_hero()->get_grounded() == true)
            model->get_hero()->set_gravity_speed(-30);
        model->get_hero()->set_grounded(false);
        break;
    case K_SPACE:
        if(model->get_hero()->get_flower() == true){
            if(!model->get_hero()->get_wait_turnip()){
                model->add_turnip(model->get_hero(), 25);
                view->add_turnip(static_cast<Turnip*>(model->get_turnip_vector().last()));
                model->get_hero()->set_wait_turnip(true);
                QTimer::singleShot(500,this,SLOT(turnip_on()));
            }
        }
        break;
    default:break;
    }
}

void Game::key_released(Keys k){
    switch(k){
    case K_LEFT:
        if(model->get_hero()->get_direction_x() != RIGHT)
            model->get_hero()->set_direction_x(STAND);
        break;
    case K_RIGHT:
        if(model->get_hero()->get_direction_x() != LEFT)
            model->get_hero()->set_direction_x(STAND);
        break;
    default: break;
    }
}

void Game::exit_game(){
    QCoreApplication::quit();
}

Game::~Game(){
    delete model;
    delete view;
}

