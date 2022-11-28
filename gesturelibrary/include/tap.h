#pragma once

#include "gesturelib.h"
#include "recognizer.h"

/// @brief Data structure for tap data
typedef struct tap {
    state_t state;

    float x;
    float y;
    float t;
} tap_t;

/// @brief Initialize tap data structures
void init_tap(void);

/// @brief Recognize tap gesture
/// @param event touch event to recognize
void recognize_tap(const touch_event_t* event);

/// @brief Access tap data array of size MAX_TOUCHES
/// @return Address of first element of tap data array of size MAX_TOUCHES
const tap_t* get_tap(void);

/// @brief Subscribe listener to tap gesture updates
/// @param Listener accepts a const tap_t*
/// @return 0 if first listener, 1 if replacing existing listener
int set_on_tap(void (*listener)(const tap_t*));
