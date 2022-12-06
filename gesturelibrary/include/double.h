#pragma once

#include "gesturelib.h"
#include "recognizer.h"

/// @brief holds the state, last location, and timestamp of a double tap instance
typedef struct double_tap {
    state_t state;

    float x;
    float y;
    float t;
} double_tap_t;  // single finger double tap data

/// @brief initializes the data array by setting all field to 0/NULL
void init_double_tap(void);

/// @brief reads the last tap and stroke data and determines whethers this new stroke is part of a double_tap event
/// @param event touch event
void recognize_double_tap(const touch_event_t* event);

/// @brief returns an array of the ongoing double_touch data for each finger
/// @return array of size MAX_TOUCHES of double_touch data
const double_tap_t* get_double_tap();

/// @brief Listen to double_tap events
/// @param listener accepts a const double_tap_t*
/// @return 0 if first listener, 1 if replacing existing listener
int set_on_double_tap(void (*listener)(const double_tap_t*));
