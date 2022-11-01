#pragma once

#include "gesturelib.h"
#include "math.h"
#include "recognizer.h"
#include "stroke.h"
#include "tap.h"

/// @brief
typedef struct double_tap {
    state_t state;
    int x;
    int y;
    int t;
} double_tap_t;  // single finger double tap data

typedef struct coords {
    int x;
    int y;
} coords_t;

/// @brief
/// @param event
gesture_event_t* recognize_double_tap(touch_event_t* event);

/// @brief
/// @param _double
/// @param strokes
/// @param taps
void update_double_taps(double_tap_t* _double, stroke_t* strokes, tap_t* taps);

/// @brief
/// @param c1
/// @param c2
/// @return
int euclidean_distance(coords_t c1, coords_t c2);

/// @brief
/// @return
double_tap_t* get_double_tap();
