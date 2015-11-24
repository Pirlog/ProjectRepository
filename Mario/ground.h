#ifndef GROUND_H
#define GROUND_H

#include "element.h"



enum TYPE_G{TOP, BOT};

class Ground : public Element{
public:
    Ground(int l, int c, QVector<QPixmap *> ts, Container cntnr, TYPE_G type);
    inline TYPE_G get_type_g() const{return type_g;}
    ~Ground(){}
private:
    TYPE_G type_g;
};
#endif // SETTING_H
