#pragma once

#include "gesturelib.h"
#include "recognizer.h"

/// @brief Data structure for stroke gesture data
typedef struct multistroke {
    // recognizer state
    state_t state;

    // center, initial and current
    float x0;
    float y0;
    float x;
    float y;

    // rotation angle clockwise in radians
    float rotation;
    // scale factor
    float scale;

    // tracked stroke groups, for each index: 0 if untracked, else 1
    int strokes[MAX_TOUCHES];
    // original stroke offsets from last changed center
    float sx[MAX_TOUCHES];
    float sy[MAX_TOUCHEs];
} multistroke_t;

/// @brief Recognize multistroke gesture
/// @param event touch event to recognize
gesture_event_t* recognize_multistroke(touch_event_t* event);

/// @brief Access array of multistroke_t of size MAX_TOUCHES
/// @return Pointer to first element of array of multistroke_t of size MAX_TOUCHES
multistroke_t* get_multistroke();
