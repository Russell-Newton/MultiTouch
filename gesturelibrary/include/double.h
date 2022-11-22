#pragma once

#include "gesturelib.h"
#include "gestureparams.h"
#include "math.h"
#include "multistroke.h"
#include "recognizer.h"
#include "stroke.h"
#include "tap.h"

// test defines to fix the distance problem
#define DIST_X 20
#define DIST_Y 20

/// @brief holds the state, last location, and timestamp of a double tap instance
typedef struct double_tap {
    state_t state;

    int group;

    // int num_touches;

    float x0;
    float y0;
    float t0;
    float x;
    float y;
    float t;
} double_tap_t;  // single finger double tap data

// /// @brief tuple containing the x and y coordinates of the double_tap event
// typedef struct coords {
//     int x;
//     int y;
// } coords_t;

/// @brief initializes the data array by setting all field to 0/NULL
void init_double_tap();

/// @brief reads the last tap and stroke data and determines whethers this new stroke is part of a double_tap event
/// @param event touch event
void recognize_double_tap(const touch_event_t* event);

/// @brief returns an array of the ongoing double_touch data for each finger
/// @return
const double_tap_t* get_double_tap();

/// @brief Listen to double_tap events
/// @param listener accepts a const double_tap_t*
/// @return 0 if first listener, 1 if replacing existing listener
int set_on_double_tap(void (*listener)(const double_tap_t*));
