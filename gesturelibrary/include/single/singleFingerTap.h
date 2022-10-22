#pragma once

#include "gesturelib.h"
#include "gestureparams.h"
#include "recognizer.h"

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
