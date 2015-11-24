#ifndef BLOCK_B_H
#define BLOCK_B_H

#include "block.h"

class Block_b : public Block{
public:
    Block_b(int l, int c, QVector<QPixmap *> ts, Container cntnr);
    ~Block_b(){}
};

#endif // BLOCK_B_H
