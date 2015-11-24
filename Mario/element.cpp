#include "element.h"

Element::Element(int l, int c, QVector<QPixmap *> ts, Container cntnr) : line(l), column(c), tileset(ts){
    graphics = new Peach_graphics_item(cntnr, this);
    o_type = cntnr;
    current_tile= 0;
    if(cntnr == CASTLE){
        scale = 5;
    }else if (cntnr == CLOUD){
        scale = 2.5;
    }else{
        scale = 1;
    }
}

void Element::set_graphics(int scene_height, int element_size){
    pos_x = column * element_size;
    pos_y = scene_height -(line+1)*element_size;
    graphics->setPixmap(tileset[0]->scaled(element_size*scale,element_size*scale,Qt::KeepAspectRatio));
    graphics->setPos(pos_x, pos_y);
}


void Element::set_graphics_pixmap(int element_size){
    graphics->setPixmap(tileset[current_tile]->scaled(element_size*scale,element_size*scale,Qt::KeepAspectRatio));
}

Element::~Element()
{
    tileset.clear();
    delete graphics;
}
