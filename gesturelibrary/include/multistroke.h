#pragma once

#include "gesturelib.h"
#include "recognizer.h"
#include "stroke.h"

/// @brief Data structure for stroke gesture data
typedef struct multistroke {
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
    // center position of time of last touch change
    float cx;
    float cy;
    // scaled offsets since last center change
    float sx[MAX_TOUCHES];
    float sy[MAX_TOUCHES];
} multistroke_t;

/// @brief Initialize multistroke data structures
void init_multistroke();

/// @brief Recognize multistroke gesture. This gesture allows the number of fingers to change.
/// @param event touch event to recognize
void recognize_multistroke(const touch_event_t* event);

/// @brief Access array of multistroke_t of size MAX_TOUCHES
/// @return Pointer to first element of array of multistroke_t of size MAX_TOUCHES
const multistroke_t* get_multistroke();
