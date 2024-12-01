
#ifndef VECTOR_C
#define VECTOR_C

#include "vector.h"
#define ABS_DIFF(x, y) (x > y) ? (x - y) : (y - x)

void rasterizeVector(struct Vector v, struct Wfc wfc, struct Brush brush){

    u8 matrixW = wfc.width;
    u8 matrixH = wfc.height;

    s8 x0 = v.fromX;
    s8 y0 = v.fromY;
    s8 x1 = v.toX;
    s8 y1 = v.toY;
    x0 = v.fromX * matrixW / 100;
    x1 = v.toX   * matrixW / 100;
    y0 = v.fromY * matrixH / 100;
    y1 = v.toY   * matrixH / 100;

    s8 sx = x0 < x1 ? 1 : -1;
    s8 sy = y0 < y1 ? 1 : -1;
    s8 dx = ABS_DIFF(x1, x0);
    s8 dy = ABS_DIFF(y1, y0);
    dy = dy*-1;
    s8 error = dx + dy;
    //dprintf("rasterizing line from %x, %x to %x, %x (sx, sy: %d, %d, dx, dy: %d %d, err: %d)\n",x0, y0, x1, y1, sx, sy, dx, dy, error);

    while (true){
        //dprintf("calling paint on %x, %x. error %d\n", x0, y0, error);
        paint(brush, wfc, x0, y0);
        if (x0 == x1 && y0 == y1) { break; }
        s8 e2 = 2 * error;
        if (e2 >= dy) {
            error = error + dy;
            x0 = x0 + sx;
        }
        if (e2 <= dx) {
            error = error + dx;
            y0 = y0 + sy;
        }
    }
}

#endif