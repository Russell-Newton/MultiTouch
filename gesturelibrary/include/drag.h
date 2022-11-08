#pragma once

#include "gesturelib.h"
#include "recognizer.h"

/// @brief Data structure for drag gesture data
typedef struct drag {
    // recognizer state
    state_t state;

    // tracked touch event group
    int group;

    // initial position
    float x0;
    float y0;

    // current position
    float x;
    float y;

    // current velocity
    float vx;
    float vy;
} drag_t;

/// @brief Initialize drag data structures
void init_drag();

/// @brief Recognize drag gesture
/// @param event touch event to recognize
gesture_event_t* recognize_drag(touch_event_t* event);

/// @brief Access drag data array of size MAX_TOUCHES
/// @return Address of first element of drag data array of size MAX_TOUCHES
drag_t* get_drag();

/// @brief Subscribe listener to drag gesture updates
/// @param event listener that accepts a const drag_t*
/// @return 0 if first listener, 1 if replacing existing listener
int set_on_drag(void (*event)(const drag_t*));
