#pragma once

#include "gesturelib.h"
#include "recognizer.h"

typedef struct touch_data {
    state_t state;
    uint32_t x1;
    uint32_t y1;
    uint32_t x2;
    uint32_t y2;
    uint32_t last_time;
} touch_data_t;

#ifndef HOLD_LENGTH
#define HOLD_LENGTH 5
#endif

/// @brief
/// @param
void recognize_two_tap(touch_event_t* event);

/// @brief
/// @param
void recognize_two_double_tap(touch_event_t*);

/// @brief
/// @param
void recognize_two_hold(touch_event_t*);

/// @brief
/// @param
void recognize_two_drag(touch_event_t*);

/// @brief
/// @param
void recognize_two_swipe(touch_event_t*);

/// @brief
/// @param
void recognize_two_rotate(touch_event_t*); //clockwise vs counterclockwise?

/// @brief
/// @param
void recognize_two_zoom(touch_event_t*);

/// @brief
/// @param
void recognize_two_pinch(touch_event_t*);