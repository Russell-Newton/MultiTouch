#pragma once

#define SQUARE(x)              ((x) * (x))
#define SQUARE_SUM(x, y)       (SQUARE(x) + SQUARE(y))
#define SQUARED_DIST(a, b)     (SQUARE_SUM((a)->x - (b)->x, (a)->y - (b)->y))
#define SQUARED_DIST_0(a, b)   (SQUARE_SUM((a)->x - (b).x0, (a)->y - (b).y0))
#define SQU_DIST(x, y, x0, y0) (SQUARE_SUM(x - x0, y - y0))
