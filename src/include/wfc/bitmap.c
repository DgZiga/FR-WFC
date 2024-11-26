
#ifndef BITMAP_C
#define BITMAP_C

#include "bitmap.h"

const u8 BIT_LOOKUP_TBL[] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};

u8 count_bits(u32 in){
    u8 res = 0;
    while(in>0){
        res += BIT_LOOKUP_TBL[in & 15];
        in >>= 4;
    }
    return res;
}

u8 get_nth_set_bit(u32 bitmap, u16 pos){
    u8 i=0;
    while (bitmap != 0){
        if ((bitmap & 1) == 1){
            if(pos == 0){
                return i;
            }
            pos--;
        }
        i++;
        bitmap >>= 1;
    }
    dprintf("get_nth_set_bit ERROR!!");
    return 0;
}

#endif