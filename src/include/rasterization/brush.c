
#ifndef BRUSH_C
#define BRUSH_C

#include "brush.h"

void paint(struct Brush brush, struct Wfc wfc, s8 x, s8 y){
    //dprintf("painting %x %x\n", x, y);
    s8 matrixW = wfc.width;
    s8 matrixH = wfc.height;

    u32 *matrix = wfc.probs;
    //draw a square
    s8 extraWidth = brush.width-1;
    s8 startX = x-extraWidth;
    s8 startY = y-extraWidth;
    s8 endX = x+extraWidth;
    s8 endY = y+extraWidth;

    if(startX<0){startX = 0;}
    if(startY<0){startY = 0;}
    if(endX>=matrixW){endX = matrixW-1;}
    if(endY>=matrixH){endY = matrixH-1;}

    //Softness
    s8 softStartX = startX - brush.softness;
    s8 softStartY = startY - brush.softness;
    s8 softEndX   = endX   + brush.softness;
    s8 softEndY   = endY   + brush.softness;
    if(softStartX<0       ){softStartX = 0;}
    if(softStartY<0       ){softStartY = 0;}
    if(softEndX  >=matrixW){softEndX   = matrixW-1;}
    if(softEndY  >=matrixH){softEndY   = matrixH-1;}
    for(u8 i=softStartX; i<=softEndX; i++){
        for(u8 j=softStartY; j<=softEndY; j++){
            //dprintf("painting soft %x %x\n", j, i);
            matrix[i + j  *matrixW] |= brush.superposition;
            //setSemiPaintedCoords(new Coord(i, j))
        }
    }

    for(u8 i=startX; i<=endX; i++){
        for(u8 j=startY; j<=endY; j++){
            //dprintf("painting %x %x\n", i, j);
            matrix[i + j * matrixW] = brush.superposition;
            //((u32 *)matrix[i])[j] = brush.superposition;
            //setPaintedCoords(new Coord(i, j))
        }
    }
}

#endif