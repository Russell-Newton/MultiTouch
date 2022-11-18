#pragma once

#include "gesturelib.h"
#include "recognizer.h"
#include "stroke.h"

/// @brief Data structure for multidrag gesture data
typedef struct multidrag {
    // recognizer state
    state_t state;
    // user id of tracked group
    int uid;
    // number of tracked strokes
    int size;
    // translation
    float dx;
    float dy;
    // rotation counterclockwise in radians
    float rotation;
    // scale factor
    float scale;

    // 1 if stroke is tracked, else 0
    char strokes[MAX_TOUCHES];
    // last complete translation
    float dx0;
    float dy0;
    // last rotation
    float rotation0;
    // last scaling
    float scale0;
    // last positions
    float px[MAX_TOUCHES];
    float py[MAX_TOUCHES];
    // center position of time of last touch change
    float cx;
    float cy;
    // scaled offsets since last center change
    float sx[MAX_TOUCHES];
    float sy[MAX_TOUCHES];
} multidrag_t;

/// @brief Initialize multidrag data structures
void init_multidrag();

/// @brief Recognize multidrag gesture. This gesture locks the number of fingers once any finger starts moving.
/// @param event touch event to recognize
gesture_event_t* recognize_multidrag(touch_event_t* event);

/// @brief Access array of multidrag_t of size MAX_TOUCHES
/// @return Pointer to first element of array of multidrag_t of size MAX_TOUCHES
multidrag_t* get_multidrag();
