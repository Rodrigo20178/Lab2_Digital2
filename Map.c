#include "Map.h"

float map(float x,float x0,float x1,float y0,float y1)
{
    return y0+((y1-y0)/(x1-x0))*(x-x0);
}
