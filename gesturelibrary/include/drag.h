#pragma once

#include "gesturelib.h"
#include "recognizer.h"

#ifndef DRAG_CACHED_TOUCH_EVENTS
// needs to be at least 2 in order to calculate velocity
#define DRAG_CACHED_TOUCH_EVENTS 30
#endif

/// @brief
typedef struct drag {
    state_t state;
    float x0;
    float y0;
    float vx;
    float vy;
    unsigned int cache_start;
    unsigned int cache_size;
    touch_event_t cache[DRAG_CACHED_TOUCH_EVENTS];
} drag_t;

/// @brief
/// @param event
gesture_event_t* recognize_drag(touch_event_t* event);

/// @brief
/// @return
drag_t* get_drag();
