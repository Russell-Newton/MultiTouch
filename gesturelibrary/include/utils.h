#pragma once

#define SQUARE(x)          ((x) * (x))
#define SQUARE_SUM(x, y)   (SQUARE(x) + SQUARE(y))
#define SQUARED_DIST(a, b) (SQUARE_SUM((a)->x - (b)->x, (a)->y - (b)->y))
