#pragma once

#include "twoRecog.h"

typedef struct tap_data {
    state_t state;
    uint32_t x1;
    uint32_t y1;
    uint32_t x2;
    uint32_t y2;
    uint32_t last_time;
} tap_data_t;