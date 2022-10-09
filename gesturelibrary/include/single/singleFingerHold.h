#pragma once

#include "gesturelib.h"
#include "recognizer.h"

/// @brief
typedef struct sFingerHold {
    state_t state;
    uint32_t last_x;
    uint32_t last_y;
    uint32_t last_time;
} sFingerHold_t; //single finger double tap data

/// @brief
/// @param event
void recognize_single_hold(touch_event_t* event);

/// @brief 
/// @return 
sFingerHold_t* get_sFingerHold();
