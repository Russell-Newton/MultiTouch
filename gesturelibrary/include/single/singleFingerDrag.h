#pragma once

#include "gesturelib.h"
#include "recognizer.h"

#ifndef DRAG_CACHED_TOUCH_EVENTS
// needs to be at least 2 in order to calculate velocity
#define DRAG_CACHED_TOUCH_EVENTS 30
#endif

#ifndef DRAG_MIN_DIST
// minimum euclidean distance of a gesture to be a drag
#define DRAG_MIN_DIST 5
#endif

/// @brief
typedef struct sFingerDrag {
    state_t state;
    float x0;
    float y0;
    float vx;
    float vy;
    unsigned int cache_start;
    unsigned int cache_size;
    touch_event_t cache[DRAG_CACHED_TOUCH_EVENTS];
} sFingerDrag_t;

/// @brief
/// @param event
gesture_event_t* recognize_single_drag(touch_event_t* event);

/// @brief
/// @return
sFingerDrag_t* get_sFingerDrag();
