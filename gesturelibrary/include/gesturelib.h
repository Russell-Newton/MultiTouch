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

extern int num_recognizers;

/// @brief
typedef enum event_type {
    TOUCH_EVENT_DOWN,
    TOUCH_EVENT_MOVE,
    TOUCH_EVENT_UP
} event_type_t;

/// @brief
typedef struct touch_event {
    event_type_t event_type;
    float position_x;
    float position_y;
    float timestamp;
    unsigned int id;
} touch_event_t;

typedef enum gesture_type {
    GESTURE_TYPE_TAP,
    GESTURE_TYPE_SWIPE,
    GESTURE_TYPE_HOLD,
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
} gesture_recognizer_t;

/// @brief
void init_gesturelib();

/// @brief
/// @param touch_event
/// @param gestures
/// @param max_gestures
/// @return
int process_touch_event(touch_event_t* touch_event, gesture_event_t* gestures, int max_gestures);

/// @brief assign a group ID to this touch_event. Update group_heads to reflect this. If this is not a DOWN event
///        and there are no groups being tracked, do not assign an ID. If this is a DOWN event but MAX_TOUCHES groups
///        are already being tracked, do not assign an ID.
/// @param touch_event
/// @return
unsigned int assign_group(touch_event_t* touch_event);

/// @brief register a recognizer via a recognition function
/// @param recognize the recognition function to add
/// @return which index the recognizer is saved to. -1 if the recognizer couldn't be saved
int add_recognizer(gesture_event_t* (*recognizer)(touch_event_t*));

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
