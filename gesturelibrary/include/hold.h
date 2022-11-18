#pragma once

#include "gesturelib.h"
#include "gestureparams.h"
#include "recognizer.h"

/// @brief Data structure for hold data
typedef struct hold {
    state_t state;

    // intial position and time
    float x0;
    float y0;
    float t0;

    // current position
    float x;
    float y;
    float t;
} hold_t;

/// @brief Initialize hold data structures
void init_hold();

/// @brief Recognize hold gesture
/// @param event touch event to recognize
void recognize_hold(touch_event_t* event);

/// @brief Access hold data array of size MAX_TOUCHES
/// @return Address of first element of hold data array of size MAX_TOUCHES
hold_t* get_hold();

/// @brief Listen to hold events
/// @param listener accepts a const hold_t*
/// @return 0 if first listener, 1 if replacing existing listener
int set_on_hold(void (*listener)(const hold_t*));
