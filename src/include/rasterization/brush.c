
#ifndef BRUSH_C
#define BRUSH_C

#include "brush.h"

void paint(struct Brush brush, struct Wfc wfc, u8 x, u8 y){
    //dprintf("painting %x %x\n", x, y);
    u8 matrixW = wfc.width;
    u8 matrixH = wfc.height;

    u32 *matrix = wfc.probs;
    //draw a square
    u8 extraWidth = brush.width-1;
    u8 startX = x-extraWidth;
    u8 startY = y-extraWidth;
    u8 endX = x+extraWidth;
    u8 endY = y+extraWidth;

    if(startX<0){startX = 0;}
    if(startY<0){startY = 0;}
    if(endX>=matrixW){endX = matrixW-1;}
    if(endY>=matrixH){endY = matrixH-1;}

    //Softness
    u8 softStartX = startX - brush.softness;
    u8 softStartY = startY - brush.softness;
    u8 softEndX   = endX   + brush.softness;
    u8 softEndY   = endY   + brush.softness;
    if(softStartX<0       ){softStartX = 0;}
    if(softStartY<0       ){softStartY = 0;}
    if(softEndX  >=matrixW){softEndX   = matrixW-1;}
    if(softEndY  >=matrixH){softEndY   = matrixH-1;}
    for(u8 i=softStartX; i<=softEndX; i++){
        for(u8 j=softStartY; j<=softEndY; j++){
            //dprintf("painting soft %x %x\n", j, i);
            matrix[j + i  *matrixW] |= brush.superposition;
            //setSemiPaintedCoords(new Coord(i, j))
        }
    }

    for(u8 i=startX; i<=endX; i++){
        for(u8 j=startY; j<=endY; j++){
            //dprintf("painting %x %x\n", j, i);
            matrix[j + i * matrixW] = brush.superposition;
            //((u32 *)matrix[i])[j] = brush.superposition;
            //setPaintedCoords(new Coord(i, j))
        }
    }
}

#endif