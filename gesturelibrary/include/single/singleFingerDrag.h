#pragma once

#include "gesturelib.h"
#include "recognizer.h"

/// @brief
typedef struct sFingerDrag {
    state_t state;
    uint32_t last_x;
    uint32_t last_y;
    uint32_t last_time;
} sFingerDrag_t;

/// @brief
/// @param event
gesture_event_t* recognize_single_drag(touch_event_t* event);

/// @brief
/// @return
sFingerDrag_t* get_sFingerDrag();
