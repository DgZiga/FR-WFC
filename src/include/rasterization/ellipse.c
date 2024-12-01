
#ifndef ELLIPSE_C
#define ELLIPSE_C

#include "ellipse.h"

u32 pow(u8 base, u8 exponent){
    u32 res = 1;
    for(u8 i=0; i<exponent; i++){
        res *= base;
    }
    return res;
}

#define ABS_DIFF(a, b) (a > b) ? (a - b) : (b - a)

u16 sqrt(u16 in){
    __asm__("swi 0x8":
    "=r" (in));
    return in;
}

void rasterizeEllipse(struct Ellipse e, struct Wfc wfc, struct Brush brush){

        u8 matrixW = wfc.width;
        u8 matrixH = wfc.height;

        // ChatGPT approximation for ellipse fill
        u8 h = e.centerX * matrixW / 100;
        u8 k = e.centerY * matrixH / 100;
        u8 a = e.hRad    * matrixW / 100;
        u8 b = e.vRad    * matrixH / 100;

        // Iterate over all rows (y-values) in the bounding box of the ellipse
        for (u8 y = k - b; y <= k + b; y++) {
            if (y < 0 || y >= matrixH) continue; // Skip rows outside the grid
            // Compute the x-range for this row
            u32 yOffset = pow(ABS_DIFF(y,k), 2) / pow(b, 2);
            if (yOffset > 1) continue; // Outside the ellipse
            u32 xRange = sqrt(pow(ABS_DIFF(1, yOffset) * a, 2));
            u32 xMin = MAX(0, ABS_DIFF(h, xRange));
            u32 xMax = MIN(ABS_DIFF(matrixW, 1), h + xRange);

            // Fill all cells between xMin and xMax
            for (u32 x = xMin; x <= xMax; x++) {
                paint(brush, wfc, x, y);
            }
        }

}

#endif