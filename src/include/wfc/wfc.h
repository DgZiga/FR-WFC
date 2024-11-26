
#ifndef WFC_H
#define WFC_H

#include <pokeagb/pokeagb.h>
#include "tileset.h"
#include "bitmap.h"

struct Wfc{
    void *addr;

    u8 width;
    u8 height;
    u32 *output; //2d array of resulting tiles. points to struct MapTile *
    u32 *probs;  //2d of superpositions. points to u32 *
    u32 *prob_calc_ctr; //2d array. points to bool *

};

void init(struct Wfc wfc);
u8 start(struct Wfc wfc);

#endif