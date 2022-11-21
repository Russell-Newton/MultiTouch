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
    // has zoomed at some point during the gesture
    char zoomed;
} zoom_t;

/// @brief Initialize zoom data structures
void init_zoom();

/// @brief Recognize zoom gesture. This gesture locks the number of fingers once any finger starts moving.
/// @param event touch event to recognize
void recognize_zoom(const touch_event_t* event);

/// @brief Access array of zoom_t of size MAX_TOUCHES
/// @return Pointer to first element of array of zoom_t of size MAX_TOUCHES
const zoom_t* get_zoom();

/// @brief Subscribe listener to zoom gesture updates
/// @param listener accepts a const zoom_t*
/// @return 0 if first listener, 1 if replacing existing listener
int set_on_zoom(void (*listener)(const zoom_t*));
