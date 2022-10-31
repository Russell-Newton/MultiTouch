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
/// @param data
void add_to_data(sFingerHold_t* data);

// /// @brief
// void clear_data();

/// @brief
/// @param state
/// @param event
/// @return
sFingerHold_t* create_touch_data(state_t state, touch_event_t* event);

/// @brief
/// @param event
void process_hold_down(touch_event_t* event);

/// @brief
/// @param event
void process_hold_move(touch_event_t* event);

/// @brief
/// @param event
void process_hold_up(touch_event_t* event);

/// @brief
/// @return
sFingerHold_t* get_sFingerHold();
