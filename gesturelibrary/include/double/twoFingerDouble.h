#pragma once

//#include "gesturelib.h"
//#include "recognizer.h"

#include "twoRecog.h"

typedef struct tap_data {
    state_t state;
    uint32_t x1;
    uint32_t y1;
    uint32_t x2;
    uint32_t y2;
    uint32_t last_time;
} tap_data_t;

touch_event_t* group_1_touch_event;
touch_event_t* group_2_touch_event;

/**
 * @brief Update group_1 and group_2 touch events
 * 
 */
void update_touches(touch_event_t* event1, touch_event_t* event2);