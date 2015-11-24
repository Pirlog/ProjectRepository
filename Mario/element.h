#ifndef ELEMENT_H
#define ELEMENT_H

#include <QVector>
#include <QPixmap>
#include "peach_graphics_item.h"

class Element{
public:
    Element(int l, int c, QVector<QPixmap*> ts, Container cntnr);
    inline unsigned int get_line() const{return line;}
    inline unsigned int get_column() const{return column;}
    inline int get_pos_x() const{return pos_x;}
    inline int get_pos_y() const{return pos_y;}
    inline float get_scale() const{return scale;}
    inline Container get_o_type() const {return o_type;}
    inline QVector<QPixmap*> get_tileset() const{return tileset;}
    inline unsigned int get_current_tile() const{return current_tile;}
    inline Peach_graphics_item* get_graphics() const{return graphics;}
    void set_graphics(int scene_height,int  element_size);
    void set_graphics_pixmap(int element_size);
    inline void set_pos_y_total(int new_val){pos_y = new_val;}
    inline void set_pos_x_total(int new_val){pos_x = new_val;}
    inline void set_pos_x(int shift){pos_x += shift;}
    inline void set_pos_y(int shift){pos_y += shift;}
    inline void set_current_tile(){current_tile = (current_tile+1) % tileset.size();}
    inline void set_scale(float s){scale = s;}
    inline void set_tileset(QVector<QPixmap*> new_ts){tileset = new_ts;}
    bool operator< (const Element& other) const {return column < other.column;}
    ~Element();
protected:
    unsigned int line;
    unsigned int column;
    int pos_x;
    int pos_y;
    Container o_type;
    float scale;
    int current_tile;
    QVector<QPixmap*> tileset;
    Peach_graphics_item* graphics;
};

#endif // ELEMENT_H
