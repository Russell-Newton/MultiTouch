#pragma once

#include "stdint.h"

#ifndef SAVED_EVENTS
#define SAVED_EVENTS 100
#endif
#ifndef MAX_TOUCHES
#define MAX_TOUCHES 5
#endif
#ifndef MAX_GESTURES
#define MAX_GESTURES 100
#endif

/// @brief
typedef enum event_type {
    down,
    move,
    up
} event_type_t;

/// @brief
typedef struct touch_event {
    event_type_t event_type;
    bool down;
    float position_x;
    float position_y;
    float timestamp;
    // TODO: touch ID (which finger)
} touch_event_t;

/// @brief
typedef struct gesture_event {
    // TODO: stuff
} gesture_event_t;

typedef struct gesture_recognizer {
    char enabled;
    void (*recognize)(touch_event_t*);
} gesture_recognizer_t;

/// @brief
void init_gesturelib();

/// @brief
/// @param touch_event
/// @param gestures
/// @param max_gestures
/// @return
int process_touch_event(touch_event_t* touch_event, gesture_event_t* gestures, int max_gestures);
