
#ifndef WFC_H
#define WFC_H

#include <pokeagb/pokeagb.h>
#include "tileset.h"
#include "bitmap.h"

struct Wfc{
    void *addr;

    u8 width;
    u8 height;
    struct MapTile *output; //2d array of resulting tiles. points to struct MapTile *
    u32 *probs;  //2d of superpositions. points to u32 *
    bool *prob_calc_ctr; //2d array. points to bool *

};

struct Wfc init(u8 width, u8 height);
u8 start(struct Wfc wfc);
void recalc_prob_iterative(struct Wfc wfc, u8 start_x, u8 start_y);

#endif