#ifndef PIPE_H
#define PIPE_H

#include "element.h"

class Pipe : public Element{
public:
    Pipe(int l, int c, QVector<QPixmap *> ts, Container cntnr);
    ~Pipe(){}
};

#endif // PIPE_H
