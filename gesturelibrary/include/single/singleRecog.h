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

tap_data_t* data[MAX_TOUCHES];

/// @brief
/// @param event
void recognize_single_tap(touch_event_t* event);

/// @brief
/// @param event
void recognize_double_tap(touch_event_t* event);

/// @brief
/// @param event
void recognize_swipe(touch_event_t* event);

/// @brief
/// @param event
void recognize_single_tap(touch_event_t* event);

/// @brief
/// @param event
void recognize_double_tap(touch_event_t* event);

/// @brief
/// @param event
void recognize_triple_tap(touch_event_t* event);

/// @brief
/// @param event
void recognize_single_drag(touch_event_t* event);

/// @brief
/// @param event
void recognize_single_hold(touch_event_t* event);

/// @brief
/// @return array of tap_data_t with size MAX_TOUCHES as defined in gesturelib.h
tap_data_t* get_tap_data();
