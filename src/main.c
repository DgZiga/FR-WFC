
#ifndef MAIN_C
#define MAIN_C

#include "main.h"

void wfc_entrypoint(struct MapHeader *mapHeader){
    
    struct Wfc wfc = init(5, 5);
    u8 res = start(wfc);
    dprintf("wfcs res: %x\n", res);

    //Copy mapdata and mapTile into RAM to edit them
    struct MapData *mapData = mapHeader->data;
    memcpy((void *)RAM_FREESPACE, mapData, sizeof(struct MapData));
    dprintf("og w h %x, %x\n", mapData->width, mapData->height);
    mapData = (struct MapData *)RAM_FREESPACE; 
    mapData->width = wfc.width;
    mapData->height = wfc.height;

    dprintf("done. setting %x to %x and %x to %x\n", mapData->data, wfc.output, mapHeader->data, mapData);
    mapData->data = (struct MapTile *) wfc.output;
    mapHeader->data = mapData;
    //free(wfc.addr);
}

#endif