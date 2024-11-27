
#ifndef MAIN_C
#define MAIN_C

#include "main.h"

void wfc_entrypoint(struct MapHeader *mapHeader){
    
    struct Wfc wfc = init(18, 18);

    dprintf("Starting rasterization\n");
    struct Vector v = {.fromX=0, .fromY=0, .toX=100, .toY=100};
    struct Brush b = {.superposition= tileset_walkable_superpos, .width= 2, .softness=2};
    rasterizeVector(v, wfc, b);
    dprintf("Ending rasterization\n");

    recalc_prob_iterative(wfc, v.fromX, v.fromY);

    u8 res = start(wfc);
    dprintf("wfcs res: %x\n", res);

    //Copy mapdata and mapTile into RAM to edit them
    struct MapData *mapData = mapHeader->data;
    memcpy((void *)RAM_FREESPACE, mapData, sizeof(struct MapData));
    mapData = (struct MapData *)RAM_FREESPACE; 
    mapData->width = wfc.width;
    mapData->height = wfc.height;

    dprintf("done. setting %x to %x and %x to %x\n", mapData->data, wfc.output, mapHeader->data, mapData);
    mapData->data = (struct MapTile *) wfc.output;
    mapHeader->data = mapData;
    //free(wfc.addr);
}

#endif