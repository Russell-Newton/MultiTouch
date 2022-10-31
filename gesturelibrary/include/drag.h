#pragma once

#include "gesturelib.h"
#include "recognizer.h"

/// @brief Data structure for drag gesture data
typedef struct drag {
    // recognizer state
    state_t state;

    // initial position
    float x0;
    float y0;

    // current position
    float x;
    float y;
} drag_t;

/// @brief Initialize drag data structures
void init_drag();

/// @brief Recognize drag gesture
/// @param event touch event to recognize
gesture_event_t* recognize_drag(touch_event_t* event);

/// @brief Access drag data array of size MAX_TOUCHES
/// @return Address of first element of drag data array of size MAX_TOUCHES
drag_t* get_drag();
