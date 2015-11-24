#include <QVector>
#include "element.h"

#ifndef SEPARATOR
#define SEPARATOR

class Separator{
public:
    Separator();
    void set_elem(int b, int e);
    void set_anim_e(int b, int e);
    void set_monster_e(int b, int e);
    inline int get_e_beg()const{return elem_beg;}
    inline int get_e_end()const{return elem_end;}
private:
    int elem_beg;
    int elem_end;
};


class Map_separator{
public:
    Map_separator(unsigned int s_width, unsigned int v_width);
    void generate_separators_elem(QVector<Element*> elems);
    void set_separator(int beg, int end, int pos);
    inline Separator* get_current_part()const{return separators[current_part];}
    inline Separator* get_previous_part()const{return separators[previous_part];}
    inline Separator* get_next_part()const{return separators[next_part];}
    inline int get_current_index()const{return current_part;}
    inline int get_previous_index()const{return previous_part;}
    inline int get_next_index()const{return next_part;}
    inline int get_separator_number()const{return separator_number;}
    inline void set_current_part(int a){current_part += a;}
    void set_previous_part(int a);
    void set_next_part(int a);
    inline QVector<Separator*> get_separators() const{return separators;}
    ~Map_separator();
private:
    int separator_number;
    int current_part;
    int previous_part;
    int next_part;
    int scene_width;
    QVector<Separator*> separators;
};

#endif // SEPARATOR

