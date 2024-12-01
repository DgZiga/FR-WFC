
#ifndef MAIN_C
#define MAIN_C

#include "main.h"

u8 randUpTo(u8 max){
    return ((u32)rand() * max / 0xFFFF);
}

#define ROOMS_NO 3

void wfc_entrypoint(struct MapHeader *mapHeader){
    
    struct Wfc wfc = init(30, 30);
    struct Brush b = {.superposition= tileset_walkable_superpos, .width= 2, .softness=2};

    dprintf("Starting rasterization\n");

    u8 startX = 50;
    u8 startY = 1;
    struct Coords8 ellipseCenters[ROOMS_NO];
    for(u8 i=0; i<ROOMS_NO; i++){
        u8 x = randUpTo(100);
        u8 y = randUpTo(100);
        //dprintf("generated random coords %d %d\n", x, y);
        struct Ellipse e = {.centerX=x, .centerY=y, .hRad=30, .vRad=30};
        rasterizeEllipse(e, wfc, b);
        struct Coords8 c = {.x=x, .y=y};
        ellipseCenters[i] = c;
    }

    struct Vector v = {.fromX=startX, .fromY=startY, .toX=ellipseCenters[0].x, .toY=ellipseCenters[0].y};
    rasterizeVector(v, wfc, b);

    for(u8 i=0; i<ROOMS_NO-1; i++){
        struct Vector v = {.fromX=ellipseCenters[i].x, .fromY=ellipseCenters[i].y, .toX=ellipseCenters[i+1].x, .toY=ellipseCenters[i+1].y};
        rasterizeVector(v, wfc, b);
    }
    dprintf("Ending rasterization\n");
    print(wfc);

    u8 mx = v.fromX*wfc.width/100;
    u8 my = v.fromY*wfc.height/100;
    dprintf("calcing prob %x %x\n", mx, my);
    recalc_prob_iterative(wfc, mx, my);

    u8 res = start(wfc);
    dprintf("wfcs res: %x\n", res);

    //Copy mapdata and mapTile into RAM to edit them
    struct MapData *mapData = mapHeader->data;
    memcpy((void *)RAM_FREESPACE, mapData, sizeof(struct MapData));
    mapData = (struct MapData *)RAM_FREESPACE; 
    mapData->width = wfc.width;
    mapData->height = wfc.height;

    mapData->data = (struct MapTile *) wfc.output;
    mapHeader->data = mapData;
    //free(wfc.addr);
}

#endif