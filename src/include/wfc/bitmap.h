
#ifndef BITMAP_H
#define BITMAP_H

#include <pokeagb/pokeagb.h>
#include "../agb_debug/debug.h"

u8 count_bits(u32 in);

void exec_for_tileids_in_bitmap(u32 bitmap, void (*f)(int)); //substitute of bitmap_to_tile_ids

u32 get_nth_set_bit(u32 bitmap, u16 pos);

#endif