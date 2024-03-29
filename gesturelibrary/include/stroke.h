#pragma once

#include "gesturelib.h"
#include "recognizer.h"

#ifndef STROKE_CACHE_SIZE
// needs to be at least 2 in order to calculate velocity
#define STROKE_CACHE_SIZE 30
#endif

/// @brief Data structure for stroke gesture data
typedef struct stroke {
    // recognizer state
    state_t state;

    // touch event group
    unsigned int group;
    // user id
    int uid;

    // initial position and time
    float x0;
    float y0;
    float t0;

    // current position and time
    float x;
    float y;
    float t;

    // moving average velocity
    float vx;
    float vy;

    // instantaneous velocity cache
    int cache_last;
    int cache_size;
    float cache_vy[STROKE_CACHE_SIZE];
    float cache_vx[STROKE_CACHE_SIZE];
} stroke_t;

/// @brief Initialize stroke data structures
void init_stroke();

/// @brief Recognize stroke gesture
/// @param event touch event to recognize
void recognize_stroke(const touch_event_t* event);

/// @brief Access array of stroke_t of size MAX_TOUCHES
/// @return Pointer to first element of array of stroke_t of size MAX_TOUCHES
const stroke_t* get_stroke();
