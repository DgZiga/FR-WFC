
#ifndef MAIN_C
#define MAIN_C

#include "main.h"

void wfc_entrypoint(struct MapHeader *mapHeader){

    struct Wfc wfc = {.height=10, .width=10};
    init(wfc);
    u8 res = start(wfc);
    dprintf("wfcs res: %x", res);

    //Copy mapdata and mapTile into RAM to edit them
    struct MapData *mapData = mapHeader->data;
    memcpy((void *)RAM_FREESPACE, mapData, sizeof(struct MapData));
    struct MapTile *data = (struct MapTile *) (RAM_FREESPACE + sizeof(struct MapData) + 0x10);
    memcpy((void *)data, mapData->data, mapData->width * mapData->height * 2);

    mapData = (struct MapData *)RAM_FREESPACE; 
    if(mapData->width == 24){
        data[0].tile=0xE;
    }   

    mapData->data = (struct MapTile *) wfc.output;
    mapHeader->data = mapData;
    
    //free(wfc.addr);
}

#endif