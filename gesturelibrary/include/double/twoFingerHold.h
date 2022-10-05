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

#ifndef HOLD_LENGTH
#define HOLD_LENGTH 5
#endif

int backlog[HOLD_LENGTH];

static int index = 0;

static touch_event_t* group_1_touch_event;
static touch_event_t* group_2_touch_event; //make into struct?

/**
 * @brief Adds a 1 to the backlog signalling that a touch input was just recieved (touch_2 does this with every possible tap)
 * 
 * @param event_1 
 * @param event_2 if the location of this event is far from the static variables', this is another gesture happening in a new spot
 */
void add_to_backlog(touch_event);

/**
 * @brief wipe the backlog in the presence of a null event
 * 
 */
void wipe_backlog();