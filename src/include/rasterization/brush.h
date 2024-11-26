
#ifndef BRUSH_H
#define BRUSH_H

#include <pokeagb/pokeagb.h>

struct Brush{
    u32 superposition;
    u8 width;
    u8 softness;
};

void paint(struct Brush brush, u32 *matrix, u8 x, u8 y);

#endif