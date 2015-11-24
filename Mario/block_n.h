#ifndef BLOCK_N_H
#define BLOCK_N_H

#include "block.h"

class Block_n : public Block{
public:
    Block_n(int l, int c, QVector<QPixmap *> ts, Container cntnr);
    ~Block_n(){}
};


#endif // BLOCK_N_H
