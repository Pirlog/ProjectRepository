#include "model.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <typeinfo>
#include <QDebug>
#include <algorithm>
#include <iostream>

Model::Model(){
    tf = new Tile_factory();
    peach = new Hero(6,2,tf->tile_vector(HERO), HERO, 53, 4);
    init_map();
    init_items();
    init_player_info();
    init_bonus();
    init_monsters();
    init_sounds();
    std::sort(elements.begin(), elements.end(), ElementCompare());
    end_message = new QGraphicsTextItem("Congratulations!");
    end_message->setFont(*peach_font);

}

QJsonObject Model::read_json_file(QString const file){
    QFile f(file);
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QString json_string = QString::fromUtf8(f.readAll()).simplified();
    QJsonDocument json_content = QJsonDocument::fromJson(json_string.toUtf8());
    f.close();
    return json_content.object();
}

void Model::init_player_info(){
    int id = QFontDatabase::addApplicationFont(":/score/MarioLuigi2.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    peach_font = new QFont(family);
    player_coins = new Player_info(tf->tile_vector(COIN)[4], "0", peach_font, 3);
    player_life = new Player_info(new QPixmap(QString(":/settings/champi_life.png")), "3", peach_font, 85);
}

void Model::init_bonus(){
    static_cast<Block_m*>(animated_elements[7])->set_surprise(S_BIG_CHAMP);
    static_cast<Block_m*>(animated_elements[5])->set_surprise(S_FLOWER);
    static_cast<Block_m*>(animated_elements[2])->set_surprise(S_BIG_CHAMP);
    static_cast<Block_m*>(animated_elements[0])->set_surprise(S_FLOWER);
}

void Model::reinit_game(int scene_height)
{
    peach->set_initial_param(scene_height);
    player_coins->set_score_total(0);
    player_life->set_score_total(3);
}

void Model::init_decor(QJsonArray decor_array){
    QJsonObject decor_obj;
    for(int i=0; i<decor_array.size();i++){
        decor_obj=decor_array[i].toObject();
        if(decor_obj["type"] == "castle"){
            Element* e = new Element(decor_obj["line"].toInt(),decor_obj["column"].toInt(),tf->tile_vector(CASTLE).mid(0,1),CASTLE);
             elements.push_back(e);
        }else if(decor_obj["type"] == "cloud"){
            Element* e = new Element(decor_obj["line"].toInt(),decor_obj["column"].toInt(),tf->tile_vector(CLOUD).mid(1,1),CLOUD);
            elements.push_back(e);
        }
    }
}


void Model::init_map(){
    QJsonObject json_obj= read_json_file(":/map/map_1.json");
    QJsonArray ground_array = json_obj.value(QString("ground")).toArray();
    QJsonObject ground_obj;
    for(int i = 0; i < ground_array.size(); i++){
        ground_obj = ground_array[i].toObject();
        add_ground(ground_obj["line_beg"].toInt(), ground_obj["line_end"].toInt(),
                ground_obj["column_beg"].toInt(), ground_obj["colum_end"].toInt());
    }
}


void Model::init_items(){
    QJsonObject json_obj= read_json_file(":/map/items_1.json");
    QJsonArray blocks_array = json_obj.value(QString("items")).toObject()["block"].toArray();
    init_blocks(blocks_array);
    QJsonArray coins_array = json_obj.value(QString("items")).toObject()["coins"].toArray();
    init_coins(coins_array);
    QJsonArray pipes_array = json_obj.value(QString("items")).toObject()["pipes"].toArray();
    init_pipes(pipes_array);
    QJsonArray decor_array = json_obj.value(QString("items")).toObject()["decor"].toArray();
    init_decor(decor_array);
}

void Model::init_monsters(){
    QJsonObject json_obj= read_json_file(":/map/monster_1.json");
    QJsonArray goomba_array = json_obj.value(QString("monster")).toObject()["goomba"].toArray();
    add_monster(goomba_array,'g');
    QJsonArray koopa_array = json_obj.value(QString("monster")).toObject()["koopa"].toArray();
    add_monster(koopa_array,'k');
    QJsonArray flower_array = json_obj.value(QString("monster")).toObject()["bad_flower"].toArray();
    add_monster(flower_array,'f');
}


void Model::init_coins(QJsonArray coins_array){
    QJsonObject coin_obj;
    for(int i=0; i<coins_array.size();i++){
        coin_obj=coins_array[i].toObject();
        add_coin(coin_obj["line"].toInt(), coin_obj["column"].toInt());
    }
}

void Model::init_blocks(QJsonArray blocks_array){
    QJsonObject block_obj;
    for(int i=0; i<blocks_array.size();i++){
        block_obj=blocks_array[i].toObject();
        add_block(block_obj["line"].toInt(), block_obj["column"].toInt(),
                block_obj["type"].toString());
    }
}

void Model::init_pipes(QJsonArray pipes_array){
    QJsonObject pipe_obj;
    for(int i=0; i<pipes_array.size();i++){
        pipe_obj=pipes_array[i].toObject();
        add_pipe(pipe_obj["height"].toInt(),pipe_obj["line"].toInt(),
                pipe_obj["column"].toInt());
    }
}

void Model::init_sounds()
{
    sounds_vector.push_back("qrc:/musics/NSMB_Ecran_titre.mp3");
    sounds_vector.push_back("qrc:/musics/NSMB_Monde1.mp3");
    sounds_vector.push_back("qrc:/sounds/Bomb.mp3");
    sounds_vector.push_back("qrc:/sounds/Enemie_Died.wav");
    sounds_vector.push_back("qrc:/sounds/Coin.wav");
}

void Model::add_monster(QJsonArray monster_array, char type){
    QJsonObject monster_obj;
    for(int i=0; i<monster_array.size();i++){
        monster_obj=monster_array[i].toObject();
        if(type == 'g'){
            Goomba* m = new Goomba(monster_obj["line"].toInt(),monster_obj["column"].toInt(),tf->tile_vector(GOOMBA),GOOMBA, 30, 2);
            monster_vector.push_back(m);
        }else if(type == 'k'){
            Koopa* m = new Koopa(monster_obj["line"].toInt(),monster_obj["column"].toInt(),tf->tile_vector(KOOPA),KOOPA, 40, 2);
            monster_vector.push_back(m);
        }else if(type == 'f'){
            Bad_flower* m = new Bad_flower(monster_obj["line"].toInt(),monster_obj["column"].toInt(),tf->tile_vector(BAD_FLOWER),BAD_FLOWER, 30, 2);
            monster_vector.push_back(m);
        }
    }
}

void Model::add_loot(Block_m *block, Character* c){
    Loot* l;
    if(block->get_surprise() == S_BIG_CHAMP){
        l = new Champi(block->get_line()+1, block->get_column(), tf->tile_vector(BIG_CHAMP).mid(0, 1),BIG_CHAMP);
    }else if(block->get_surprise() == S_SMALL_CHAMP){
        l = new Champi(block->get_line()+1, block->get_column(), tf->tile_vector(SMALL_CHAMP).mid(0, 1),SMALL_CHAMP);
    }else if(block->get_surprise() == S_FLOWER && c->get_scale() == 1){
        l = new Flower(block->get_line()+1, block->get_column(), tf->tile_vector(FLOWER).mid(1, 3),FLOWER, tf->tile_vector(HERO_FIRE));
    }else{
        l = new Coin(block->get_line()+1, block->get_column(), tf->tile_vector(COIN).mid(4, -1),COIN);
    }
    animated_elements.push_back(l);
}

void Model::add_block( int const line, int const column, QString const type){
    if(type =="break"){
        animated_elements.push_back(new Block_b(line, column, tf->tile_vector(BLOCK_B).mid(0,1), BLOCK_B));
    }
    else if(type=="normal"){
        block_n_vector.push_back(new Block_n(line, column, tf->tile_vector(BLOCK_N).mid(1,1), BLOCK_N));
    }
    else if(type =="bonus"){
        animated_elements.push_front(new Block_m(line, column, tf->tile_vector(BLOCK_M), BLOCK_M));
    }
}

void Model::add_coin(int const line, int const column){
    Coin *c = new Coin(line, column, tf->tile_vector(COIN).mid(4, -1), COIN);
    animated_elements.push_back(c);
}

void Model::add_pipe(int const height, int const line, int const column){
    for(int i=line; i<=line+height -1; i++){
        for(int j= column; j <= column +1; j++){
            Pipe *p;
            QVector<QPixmap *> ts;
            if(i == line+height-1 && j==column){
                ts.push_back(tf->tile_vector(PIPE)[0]);
            }else if(i == line+height-1 && j==column+1){
                ts.push_back(tf->tile_vector(PIPE)[1]);
            }else if(j==column){
                ts.push_back(tf->tile_vector(PIPE)[2]);
            }else{
                ts.push_back(tf->tile_vector(PIPE)[3]);
            }
            p = new Pipe(i, j, ts, PIPE);
            elements.push_back(p);
        }
    }
}

void Model::add_turnip(Hero *h, int turnip_size){
    Turnip* t = new Turnip(1,1,tf->tile_vector(TURNIP), TURNIP,turnip_size,8, h->get_pos_x(), h->get_pos_y(),  h->get_tile_flipped());
    turnip_vector.push_back(t);
}

void Model::add_ground(int const l_beg, int const l_end, int const c_beg, int const c_end){
    for(int i = l_beg; i <= l_end; i++){
        for(int j = c_beg; j <= c_end; j++){
            Ground* g;
            QVector<QPixmap *> ts;
            if(i== l_end && j == c_beg){
                ts.push_back(tf->tile_vector(GROUND)[0]);
                g = new Ground(i, j, ts, GROUND_UP, TOP);
            }
            else if(i== l_end && j == c_end){
                ts.push_back(tf->tile_vector(GROUND)[1]);
                g = new Ground(i,j,ts, GROUND_UP, TOP);
            }
            else if(j == c_beg){
                ts.push_back(tf->tile_vector(GROUND)[2]);
                g = new Ground(i, j, ts, GROUND, BOT);
            }
            else if(j == c_end){
                ts.push_back(tf->tile_vector(GROUND)[3]);
                g = new Ground(i, j, ts, GROUND, BOT);
            }
            else if(i == l_end){
                ts.push_back(tf->tile_vector(GROUND)[4]);
                g = new Ground(i, j, ts, GROUND_UP, TOP);
            }else{
                ts.push_back(tf->tile_vector(GROUND)[5]);
                g = new Ground(i,j,ts, GROUND, BOT);
            }
            elements.push_back(g);
        }
    }
}

void Model::update_block(Block *e, Container c){
    int pos, line, column;
    QVector<QPixmap *> ts;
    switch(c){
    case BLOCK_B :
        pos = animated_elements.indexOf(static_cast<Block_b*>(e));
        animated_elements.erase(animated_elements.begin()+pos);
        break;
    case BLOCK_M:
        pos = animated_elements.indexOf(static_cast<Block_m*>(e));
        ts.push_back(tf->tile_vector(BLOCK_N)[1]);
        column = e->get_column();
        line = e->get_line();
        block_n_vector.push_back(new Block_n(line, column, ts, BLOCK_N));
        animated_elements.erase(animated_elements.begin()+pos);
        break;
    default: break;
    }
}

void Model::set_coin(){
    player_coins->set_score_total(peach->get_coins_number());
}


void Model::remove_animated_element(int i){
    animated_elements.erase(animated_elements.begin()+i);
}

void Model::monster_remove(int i){
    monster_vector.erase(monster_vector.begin()+i);
}

void Model::turnip_remove(int i){
    turnip_vector.erase(turnip_vector.begin()+i);
}

Model::~Model(){
    delete peach;
    delete peach_font;
    block_n_vector.clear();
    animated_elements.clear();
    monster_vector.clear();
    elements.clear();
    block_hit.clear();
    delete player_coins;
    delete player_life;
    turnip_vector.clear();
    delete tf;
    delete end_message;
}
