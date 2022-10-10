#pragma once

#include "gesturelib.h"
#include "recognizer.h"

/// @brief
typedef struct sFingerDTap {
    state_t state;
    uint32_t last_x;
    uint32_t last_y;
    uint32_t last_time;
} sFingerDTap_t;  // single finger double tap data

#ifndef DIFF
#define DIFF 5
#endif

/// @brief
/// @param event
gesture_event_t* recognize_double_tap(touch_event_t* event);

/// @brief
/// @return
sFingerDTap_t* get_sFingerDTap();
