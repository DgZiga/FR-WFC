
#ifndef BITMAP_H
#define BITMAP_H

#include <pokeagb/pokeagb.h>
#include "../agb_debug/debug.h"

u8 count_bits(u32 in);

u32 get_nth_set_bit(u32 bitmap, u16 pos);

#endif