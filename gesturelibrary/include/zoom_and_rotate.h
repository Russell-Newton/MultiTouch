#pragma once

#include "drag.h"
#include "gesturelib.h"
#include "recognizer.h"

/// @brief Data structure for holding room_and_rotate data
typedef struct zoom_and_rotate {
    state_t state;
    // composite drag gestures
    drag_t* drag1;
    drag_t* drag2;
    // zooming scale factor
    float zoom;
    // counterclockwise rotation in radians
    float rotate;
} zoom_and_rotate_t;

/// @brief Recognizer for zoom_and_rotate gesture
/// @param event touch event to recognize
gesture_event_t* recognize_zoom_and_rotate(touch_event_t* event);

/// @brief Accessor for zoom_and_rotate data
/// @return First element of zoom_and_rotate data array of size MAX_TOUCHES
zoom_and_rotate_t* get_zoom_and_rotate();
