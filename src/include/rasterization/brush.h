
#ifndef BRUSH_H
#define BRUSH_H

#include <pokeagb/pokeagb.h>
#include "../agb_debug/debug.h"
#include "../wfc/wfc.h"

struct Brush{
    u32 superposition;
    u8 width;
    u8 softness;
};

void paint(struct Brush brush, struct Wfc wfc, u8 x, u8 y);

#endif