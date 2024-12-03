
#ifndef ELLIPSE_C
#define ELLIPSE_C

#include "ellipse.h"

u32 pow(u32 base, u32 exponent){
    if(base == 0){ return 0; }
    u32 res = 1;
    for(u32 i=0; i<exponent; i++){
        res *= base;
    }
    return res;
}

#define ABS_DIFF(a, b) (a > b) ? (a - b) : (b - a)
POKEAGB_EXTERN u16 sqrt(u16 in);


void rasterizeEllipse(struct Ellipse e, struct Wfc wfc, struct Brush brush){
    u8 matrixW = wfc.width;
    u8 matrixH = wfc.height;

    for(s32 x = -1*e.hRad; x<=e.hRad; x++){
        /*typical ellipse equation for y is y = +-vRad * sqrt(1 - (x^2)/(hRad^2))
        in other words, we multiply the vRad by the absolute, inverse proportion of x over hRad so that the y range is larger (up to vRad) in the center and then tapers of to 0. 
        typical approach:
        s32 y1 = e.vRad * sqrt(1 - pow(x,2) / pow(e.hRad,2));
        s32 y2 = -1 * y1;
        this doesn't work because GBA can't handle floats and the coefficient will always be either 0 or 1.
        To get around this we introduce a constant C that x is mutliplied by before the division with hRad, we then get yC, which divied by C gets us back y.
        */
        s32 C = 256;
        s32 ySquared = pow(e.vRad,2) * (C - C*pow(x,2) / pow(e.hRad, 2)) / C;
        s32 y1 = sqrt(ySquared);
        s32 y2 = -1 * y1;
        for(s32 y = MIN(y1,y2);  y <= MAX(y1,y2); y++){
            s8 mx = (x+e.centerX)*matrixW/100;
            s8 my = (y+e.centerY)*matrixH/100;
            paint(brush, wfc, mx, my);
        }
    }

    return;

}

#endif