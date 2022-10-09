#pragma once

#include "gesturelib.h"
#include "recognizer.h"

/// @brief
typedef struct sFingerSwipe {
    state_t state;
    uint32_t last_x;
    uint32_t last_y;
    uint32_t last_time;
} sFingerSwipe_t;  // single finger double tap data

/// @brief
/// @param event
void recognize_swipe(touch_event_t* event);

/// @brief
/// @return
sFingerSwipe_t* get_sFingerSwipe();
