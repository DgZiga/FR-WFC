
#ifndef VECTOR_C
#define VECTOR_C

#include "vector.h"

/*
if a point stisfies y=mx+b it's on the line
    y=mx+b doesnt detect vertical and horizontal lines, so:
    m=dy/dx
    y=(dy/dx)x+b
    (dx)y = (dy)x + (dx)b
    0 = (dy)x + (dx)b - (dx)y
if a point stisfies f(x, y) = (dy)x + (dx)b - (dx)y = 0 it's on the line

given starting point x0, y0 and destination point x1, y1 we must:
- starting from x0, y0, decide wether to go to:
    - x0 +1, y0
    - x0   , y0 +1
    - x0 +1, y0 +1
- we check which point is closest to the line. To do that, we evaluate the point x0+1, y0+1/2
    f(x0+1, y0+1/2) = A(x0+1) + B(y0+1/2) + C

 */

void rasterizeVector(struct Vector v, struct Wfc wfc, struct Brush brush){
    u8 matrixW = wfc.width;
    u8 matrixH = wfc.height;

    u8 x0 = v.fromX;
    u8 y0 = v.fromY;
    u8 x1 = v.toX;
    u8 y1 = v.toY;
    dprintf("rasterizing line from %d, %d to %d, %d (%x, %x to %x, %x)\n",x0, y0, x1, y1, x0 * matrixW / 100, y0 * matrixH / 100, x1 * matrixW / 100, y1 * matrixH / 100);

    s8 dx = (x1 > x0) ? (x1 - x0) : (x0 - x1); // Absolute difference in x
    s8 dy = (y1 > y0) ? (y1 - y0) : (y0 - y1); // Absolute difference in y

    s8 A = dy;
    s8 B = -1* dx;
    s8 C = -A*x0 - B*y0;

    do {
        //scale x0 y0 to matrix coords
        s8 mx = x0 * matrixW / 100;
        s8 my = y0 * matrixH / 100;
        //dprintf("%x * %x / 100 = %x\n", y0, matrixH, my);
        paint(brush, wfc, mx, my);

        //evaluate at (x0+1, y0+1/2) = A(x0+1) + B(y0+1/2) + C
        s8 out = A*(x0+1) + B*(y0+1/2) + C;
        if(out > 0){
            y0++;
        } else {
            x0++;
        }
    } while(x0 != x1 && y0 != y1);
}

#endif