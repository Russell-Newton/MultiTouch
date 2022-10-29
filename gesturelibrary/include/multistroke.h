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
    // standard matrix A of transformation T(x)=Ax
    float a11;
    float a12;
    float a21;
    float a22;

    // 1 if stroke is tracked, else 0
    char strokes[MAX_TOUCHES];
    // last complete translation
    float dx0;
    float dy0;
    // last transformation
    float a110;
    float a120;
    float a210;
    float a220;
    // center position of time of last touch change
    float x0;
    float y0;
    // transformation helper matrix
    float B[2][MAX_TOUCHES];
} multistroke_t;

/// @brief Recognize multistroke gesture
/// @param event touch event to recognize
gesture_event_t* recognize_multistroke(touch_event_t* event);

/// @brief Access array of multistroke_t of size MAX_TOUCHES
/// @return Pointer to first element of array of multistroke_t of size MAX_TOUCHES
multistroke_t* get_multistroke();
