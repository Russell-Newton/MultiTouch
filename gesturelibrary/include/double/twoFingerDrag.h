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

int state = 0; //update to 1 if hold or drag
static touch_event_t* group_1_touch_event;
static touch_event_t* group_2_touch_event;

touch_event_t* previous_event;

/**
 * @brief if hold or drag, update this to 1
 * 
 * @param prev_event old touch_event (null if i == 0)
 */
void update_state(touch_event_t* prev_event, int i);