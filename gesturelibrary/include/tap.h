#pragma once

#include "gesturelib.h"
#include "gestureparams.h"
#include "recognizer.h"

/// @brief Data structure for tap data
typedef struct tap {
    state_t state;

    // intial position and time
    float x0;
    float y0;
    float t0;

    // current position
    float x;
    float y;
    float t;
} tap_t;

/// @brief Initialize tap data structures
void init_tap();

/// @brief Recognize tap gesture
/// @param event touch event to recognize
gesture_event_t* recognize_tap(touch_event_t* event);

/// @brief Access tap data array of size MAX_TOUCHES
/// @return Address of first element of tap data array of size MAX_TOUCHES
tap_t* get_tap();

/// @brief Subscribe listener to tap gesture updates
/// @param Listener accepts a const tap_t*
/// @return 0 if first listener, 1 if replacing existing listener
int set_on_tap(void (*listener)(const tap_t*));
