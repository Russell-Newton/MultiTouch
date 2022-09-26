#pragma once

#include "gesturelib.h"
#include "recognizer.h"

/// @brief
typedef struct tap_data {
    state_t state;
    uint32_t last_x;
    uint32_t last_y;
    uint32_t last_time;
} tap_data_t;

/// @brief
/// @param event
void recognize_tap(touch_event_t* event);

/// @brief
/// @return array of tap_data_t with size MAX_TOUCHES as defined in gesturelib.h
tap_data_t* get_tap_data();
