#pragma once

#include "gesturelib.h"
#include "recognizer.h"

/// @brief
typedef struct sFingerHold {
    state_t state;
    uint32_t last_x;
    uint32_t last_y;
    uint32_t last_time;
} sFingerHold_t;

#ifndef HOLD_LENGTH
#define HOLD_LENGTH 10
#endif

/// @brief
/// @param event
gesture_event_t* recognize_single_hold(touch_event_t* event);

/// @brief
/// @return
sFingerHold_t* get_sFingerHold();
