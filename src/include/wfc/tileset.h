
#ifndef TILESET_H
#define TILESET_H

#include <pokeagb/pokeagb.h>

struct TileRule {
    u32 up;
    u32 down;
    u32 left;
    u32 right;
};

struct TileRule tileset[] = {
    {.up= 147, .down= 39, .left= 739, .right= 287},
    {.up= 147, .down= 39, .left= 739, .right= 287},
    {.up= 147, .down= 8, .left= 739, .right= 32},
    {.up= 260, .down= 272, .left= 739, .right= 64},
    {.up= 8, .down= 39, .left= 739, .right= 128},
    {.up= 147, .down= 64, .left= 4, .right= 287},
    {.up= 544, .down= 640, .left= 8, .right= 287},
    {.up= 64, .down= 39, .left= 16, .right= 287},
    {.up= 8, .down= 8, .left= 739, .right= 512},
    {.up= 64, .down= 64, .left= 256, .right= 287}
};

struct MapTile tileId_to_mapTile[] = {
   // /* 00 */{.tile=1, .permission=0},
   // /* 01 */{.tile=13, .permission=0},
   // /* 02 */{.tile=14, .permission=0},
   // /* 03 */{.tile=28, .permission=1},
   // /* 04 */{.tile=36, .permission=1},
   // /* 05 */{.tile=15, .permission=0},
   // /* 06 */{.tile=29, .permission=1},
   // /* 07 */{.tile=37, .permission=1},
   // /* 08 */{.tile=20, .permission=1},
   // /* 09 */{.tile=21, .permission=1},

    
    /* 00 */{.tile=1, .permission=0},
    /* 01 */{.tile=13, .permission=0},
    /* 02 */{.tile=14, .permission=0},
    /* 03 */{.tile=28, .permission=0},
    /* 04 */{.tile=36, .permission=0},
    /* 05 */{.tile=15, .permission=0},
    /* 06 */{.tile=29, .permission=0},
    /* 07 */{.tile=37, .permission=0},
    /* 08 */{.tile=20, .permission=0},
    /* 09 */{.tile=21, .permission=0},
};

u32 tileset_allow_all_tiles = (1 << sizeof(tileset) / sizeof(struct TileRule)) - 1;

u32 sum_left_rules_for_tileids (u32 superpos);
u32 sum_right_rules_for_tileids(u32 superpos);
u32 sum_up_rules_for_tileids   (u32 superpos);
u32 sum_down_rules_for_tileids (u32 superpos);

#endif