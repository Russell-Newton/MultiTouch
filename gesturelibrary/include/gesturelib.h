#pragma once

#include "stdint.h"

#ifndef SAVED_EVENTS
#define SAVED_EVENTS 100
#endif
#ifndef MAX_TOUCHES
#define MAX_TOUCHES 5
#endif
#ifndef MAX_GESTURES
#define MAX_GESTURES 5
#endif

/// @brief
typedef struct touch_event {
    uint32_t position_x;
    uint32_t position_y;
    uint32_t timestamp;
    // TODO: touch ID (which finger)
    // TODO: event type (down, hold, up, etc)
} touch_event_t;

/// @brief
typedef struct gesture_event {
    // TODO: stuff
} gesture_event_t;

/// @brief
/// @param touch_event
/// @param gestures
/// @param max_gestures
/// @return
int process_touch_event(touch_event_t* touch_event, gesture_event_t* gestures, int max_gestures);
