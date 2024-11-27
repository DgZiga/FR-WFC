
#ifndef VECTOR_C
#define VECTOR_C

#include "vector.h"

void rasterizeVector(struct Vector v, struct Wfc wfc, struct Brush brush){
    u8 matrixW = wfc.width;
    u8 matrixH = wfc.height;

    u8 x0 = v.fromX;
    u8 y0 = v.fromY;
    u8 x1 = v.toX;
    u8 y1 = v.toY;

    //Bresenham's line algorithm (thx ChatGPT)
    u8 dx = (x1 > x0) ? (x1 - x0) : (x0 - x1); // Absolute difference in x
    u8 dy = (y1 > y0) ? (y1 - y0) : (y0 - y1); // Absolute difference in y
    u8 sx = (x0 < x1) ? 1 : -1u; // Step direction for x
    u8 sy = (y0 < y1) ? 1 : -1u; // Step direction for y
    u8 err = dx > dy ? dx : -dy; // Error term

    err /= 2; // Start with half of the error range

    while (1) {
        //scale x0 and y0 to matrix
        u8 mx = x0 * matrixW / 100;
        u8 my = x0 * matrixH / 100;
        //dprintf("%x * %x / 100 = %x\n", x0, matrixW, mx);
        //dprintf("%x * %x / 100 = %x\n", y0, matrixH, my);
        paint(brush, wfc, mx, my);

        if (x0 == x1 && y0 == y1) break; // Stop when endpoint is reached

        u8 e2 = err; // Save error term for comparison
        if (e2 > dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }

}

#endif