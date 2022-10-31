#pragma once

#include "gesturelib.h"
#include "recognizer.h"
#include "stroke.h"

/// @brief Data structure for swipe gesture data
typedef struct swipe {
    // recognizer state
    state_t state;

    // initial position
    float x0;
    float y0;

    // current (moving average) velocity
    float vx;
    float vy;
} swipe_t;

/// @brief Initialize swipe data structures
void init_swipe();

/// @brief Recognize swipe gesture
/// @param event touch event to recognize
gesture_event_t* recognize_swipe(touch_event_t* event);

/// @brief Access swipe data array of size MAX_TOUCHES
/// @return Address of first element of swipe data array of size MAX_TOUCHES
swipe_t* get_swipe();
