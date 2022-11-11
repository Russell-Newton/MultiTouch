#pragma once

#include "gesturelib.h"
#include "recognizer.h"

/// @brief Data structure for rotate gesture data
typedef struct rotate {
    // recognizer state
    state_t state;
    // user id of tracked group
    int uid;
    // number of tracked strokes
    int size;
    // rotation counterclockwise in radians
    float rotation;
} rotate_t;

/// @brief Initialize rotate data structures
void init_rotate();

/// @brief Recognize rotate gesture. This gesture locks the number of fingers once any finger starts moving.
/// @param event touch event to recognize
gesture_event_t* recognize_rotate(touch_event_t* event);

/// @brief Access array of rotate_t of size MAX_TOUCHES
/// @return Pointer to first element of array of rotate_t of size MAX_TOUCHES
rotate_t* get_rotate();

/// @brief Subscribe listener to rotate gesture updates
/// @param listener accepts a const rotate_t*
/// @return 0 if first listener, 1 if replacing existing listener
int set_on_rotate(void (*listener)(const rotate_t*));
