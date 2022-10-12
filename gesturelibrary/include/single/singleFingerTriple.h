#pragma once

#include "gesturelib.h"
#include "recognizer.h"

/// @brief
typedef struct sFingerTriple {
    state_t state;
    uint32_t last_x;
    uint32_t last_y;
    uint32_t last_time;
} sFingerTriple_t;

/// @brief
/// @param event
gesture_event_t* recognize_single_Triple(touch_event_t* event);

/// @brief
/// @return
sFingerTriple_t* get_sFingerTriple();
