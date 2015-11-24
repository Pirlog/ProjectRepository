#include "separator.h"
#include <QDebug>

Separator::Separator(){

}

void Separator::set_elem(int b, int e){
    elem_beg = b;
    elem_end = e;
}

Map_separator::Map_separator(unsigned int s_width, unsigned int v_width){
    scene_width = s_width;
    separator_number = s_width/v_width;
    current_part = 0;
    previous_part = 0;
    next_part = 1;
}

void Map_separator::generate_separators_elem(QVector<Element *> elems){
    int elem_beg = 0;
    int elem_end = 0;
    for(int j = 0; j < separator_number ;j++){
        for(int i=elem_beg; i < elems.size(); i++){
            if(elems[i]->get_column() * 30 > (j+1) *(scene_width / separator_number)&& i!=elem_end){
                elem_end = i;
                set_separator(elem_beg, elem_end, j);
                elem_beg = i;
                i = elems.size();
            }
        }
    }
}

void Map_separator::set_separator(int beg, int end, int pos){
    separators.push_back(new Separator());
    separators[pos]->set_elem(beg, end);
}

void Map_separator::set_previous_part(int a){
    previous_part += a;
}

void Map_separator::set_next_part(int a){
    next_part += a;
}

Map_separator::~Map_separator(){
    separators.clear();
}

