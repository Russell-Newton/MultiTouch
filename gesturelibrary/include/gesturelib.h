#pragma once

#include "gestureparams.h"
#include "stdint.h"

#ifndef SAVED_EVENTS
#define SAVED_EVENTS 100
#endif
#ifndef MAX_TOUCHES
#define MAX_TOUCHES 5
#endif
#ifndef MAX_RECOGNIZERS
#define MAX_RECOGNIZERS 100
#endif
#define TOUCH_ID_UNDEFINED (MAX_TOUCHES + 1)

/// @brief
typedef enum event_type {
    TOUCH_EVENT_DOWN,
    TOUCH_EVENT_MOVE,
    TOUCH_EVENT_UP
} event_type_t;

/// @brief
typedef struct touch_event {
    event_type_t type;
    float x;
    float y;
    float t;
    unsigned int group;
    int uid;
} touch_event_t;

typedef enum gesture_type {
    GESTURE_TYPE_TAP,
    GESTURE_TYPE_SWIPE,
    GESTURE_TYPE_HOLD,
    GESTURE_TYPE_HOLD_AND_DRAG,
    GESTURE_TYPE_DOUBLE_TAP,
    GESTURE_TYPE_DRAG,
    GESTURE_TYPE_ZOOM_AND_ROTATE
} gesture_type_t;

/// @brief
typedef struct gesture_event {
    gesture_type_t type;
    unsigned int num_touches;
    unsigned int index;  // do we need this?
    void* (*get_data)(void);
} gesture_event_t;

typedef struct gesture_recognizer {
    char enabled;
    gesture_event_t* (*recognize)(touch_event_t*);
    void (*init)(void);
} gesture_recognizer_t;

extern int num_recognizers;
extern touch_event_t latest_touch_events[];

/// @brief
int init_gesturelib();

/// @brief
/// @param touch_event
/// @param gestures
/// @param max_gestures
/// @return
int process_touch_event(touch_event_t* touch_event, gesture_event_t* gestures, int max_gestures);

/// @brief register a recognizer via a recognition function
/// @param recognize the recognition function to add
/// @param init recognizer initialization function, pass 0 for none
/// @return which index the recognizer is saved to. -1 if the recognizer couldn't be saved
int add_recognizer(gesture_event_t* (*recognizer)(touch_event_t*), void (*init)(void));

/// @brief
/// @param recognizer which recognizer to remove
/// @return the removed recognizer. A "null" recognizer if an invalid index is provided
gesture_recognizer_t remove_recognizer(int recognizer);

/// @brief
/// @param recognizer which recognizer to enable
/// @return 0 if an invalid index is provided, 1 otherwise
int enable_recognizer(int recognizer);

/// @brief
/// @param recognizer which recognizer to disable
/// @return 0 if an invalid index is provided, 1 otherwise
int disable_recognizer(int recognizer);
