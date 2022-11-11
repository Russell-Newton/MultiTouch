#pragma once

#include "drag.h"
#include "gesturelib.h"
#include "recognizer.h"

/// @brief Data structure for hold and drag data
typedef struct hold_and_drag {
    state_t state;

    // intial position
    float x0;
    float y0;

    // current position and velocity
    float x;
    float y;
    float vx;
    float vy;
} hold_and_drag_t;

/// @brief Initialize hold and drag data structures
void init_hold_and_drag();

/// @brief Recognize hold and drag gesture
/// @param event touch event to recognize
gesture_event_t* recognize_hold_and_drag(touch_event_t* event);

/// @brief Access hold and drag data array of size MAX_TOUCHES
/// @return Address of first element of hold data array of size MAX_TOUCHES
hold_and_drag_t* get_hold_and_drag();

/// @brief Subscribe listener to hold and drag gesture updates
/// @param listener accepts a const hold_and_drag_t*
/// @return 0 if first listener, 1 if replacing existing listener
int set_on_hold_and_drag(void (*listener)(const hold_and_drag_t*));
