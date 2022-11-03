#pragma once

#include "gesturelib.h"
#include "recognizer.h"

/// @brief Data structure for zoom gesture data
typedef struct zoom {
    // recognizer state
    state_t state;
    // user id of tracked group
    int uid;
    // number of tracked strokes
    int size;
    // scale factor
    float scale;
} zoom_t;

/// @brief Initialize zoom data structures
void init_zoom();

/// @brief Recognize zoom gesture. This gesture locks the number of fingers once any finger starts moving.
/// @param event touch event to recognize
gesture_event_t* recognize_zoom(touch_event_t* event);

/// @brief Access array of zoom_t of size MAX_TOUCHES
/// @return Pointer to first element of array of zoom_t of size MAX_TOUCHES
zoom_t* get_zoom();
