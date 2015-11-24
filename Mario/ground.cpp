#include "ground.h"
#include <QString>


Ground::Ground(int l, int c, QVector<QPixmap *> ts, Container cntnr, TYPE_G type):  Element(l,c,ts, cntnr), type_g(type){
}


