
#ifndef TILESET_C
#define TILESET_C

#include "tileset.h"

u32 sum_left_rules_for_tileids(u32 superpos){
    //can probably be faster
    u32 out = 0;
    u8 i=0;
    while(superpos != 0){
        if((superpos & 1) == 1){
            out |= tileset[i].left;
        }
        i++;
        superpos >>= 1;
    }
    return out;
} 

u32 sum_right_rules_for_tileids(u32 superpos){
    //can probably be faster
    u32 out = 0;
    u8 i=0;
    while(superpos != 0){
        if((superpos & 1) == 1){
            out |= tileset[i].right;
        }
        i++;
        superpos >>= 1;
    }
    return out;
} 

u32 sum_up_rules_for_tileids(u32 superpos){
    //can probably be faster
    u32 out = 0;
    u8 i=0;
    while(superpos != 0){
        if((superpos & 1) == 1){
            out |= tileset[i].up;
        }
        i++;
        superpos >>= 1;
    }
    return out;
} 

u32 sum_down_rules_for_tileids(u32 superpos){
    //can probably be faster
    u32 out = 0;
    u8 i=0;
    while(superpos != 0){
        if((superpos & 1) == 1){
            out |= tileset[i].down;
        }
        i++;
        superpos >>= 1;
    }
    return out;
} 

#endif