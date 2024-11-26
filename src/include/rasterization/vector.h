
#ifndef VECTOR_H
#define VECTOR_H

#include "rasterization.h"

struct Vector{
    u8 fromX;
    u8 fromY;
    u8 toX;
    u8 toY;
};

void rasterizeVector(struct Vector v, u32 *matrix, struct Brush brush);

#endif