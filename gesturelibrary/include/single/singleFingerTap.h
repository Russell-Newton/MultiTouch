#pragma once

#include "gesturelib.h"
#include "recognizer.h"

#ifndef TAP_LENGTH
#define TAP_LENGTH 10
#endif

#ifndef TAP_DISTANCE
#define TAP_DISTANCE 100
#endif

/// @brief
typedef struct sFingerTap {
    state_t state;
    uint32_t last_x;
    uint32_t last_y;
    uint32_t last_time;
} sFingerTap_t;  // single finger double tap data

/// @brief
/// @param event
gesture_event_t* recognize_single_tap(touch_event_t* event);

/// @brief
/// @return
sFingerTap_t* get_sFingerTap();
