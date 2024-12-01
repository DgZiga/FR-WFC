
#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "rasterization.h"

struct Ellipse{
    u8 centerX;
    u8 centerY;
    u8 hRad;
    u8 vRad;
};

void rasterizeEllipse(struct Ellipse v, struct Wfc wfc, struct Brush brush);

#endif