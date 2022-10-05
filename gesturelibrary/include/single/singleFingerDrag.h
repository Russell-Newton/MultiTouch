#include "singleRecog.h"

/// @brief
typedef struct tap_data {
    state_t state;
    uint32_t last_x;
    uint32_t last_y;
    uint32_t last_time;
} tap_data_t;

