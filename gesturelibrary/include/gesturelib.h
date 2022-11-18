#pragma once

#include "gestureparams.h"
#include "stdint.h"

/// @brief This is the type of touch event. There are only 3 supported types.
typedef enum event_type {
    // This type of touch event indicates a pointer starting to contact the surface.
    TOUCH_EVENT_DOWN,
    // This type of touch event indicates an update to a pointer already contacting the surface.
    TOUCH_EVENT_MOVE,
    // This type of touch event indicates a pointer already contacting the surface stops contact.
    TOUCH_EVENT_UP
} event_type_t;

/// @brief To use the gesture library, users create touch events and fill in the appropriate fields.
typedef struct touch_event {
    // This is the type of the touch event; see the enum event_type_t.
    event_type_t type;
    // This is the horizontal position of the touch event; adjust distance values in gestureparams.h to reflect
    // appropriate units used for this.
    float x;
    // This is the vertical position of the touch event; adjust distance values in gestureparams.h to reflect
    // appropriate units used for this.
    float y;
    // This is the time of the touch event in seconds. Timestamps can be relative to any fixed time point, but should be
    // consistent for a single gesture.
    float t;
    // Touch event groups indicate which finger a touch event originates from, ranging from 0 to MAX_TOUCHES. Set this
    // to TOUCH_GROUP_UNDEFINED to let the library assign it automatically.
    unsigned int group;
    // Touch event uids indicate which touch events should be processed as part of the same multi-finger gesture. Touch
    // events that share a uid will be processed into one multi-finger gesture while touch events that do not share a
    // uid will always be processed separately.
    int uid;
} touch_event_t;

/// @brief The user may create their own gesture recognizer to add to the library.
typedef struct gesture_recognizer {
    // The library uses this flag to track whether a recognizer is enabled.
    char enabled;
    // This is the recognizer function itself.
    void (*recognize)(touch_event_t*);
    // This is the recognizer initialization function. This function is invoked when a recognizer is added and when the
    // library is initialized.
    void (*init)(void);
} gesture_recognizer_t;

// This is the current number of recognizers the library holds. Declared in gesturelib.c
extern int num_recognizers;
// This is the last processed touch event of each group. Indices correspond to groups. Declared in gesturelib.c
extern touch_event_t latest_touch_events[];

/// @brief This function should be called by the user before passing touch events to the library. Calling
/// process_touch_event before library initialization results in undefined behavior.
int init_gesturelib();

/// @brief Users call this function with a generated touch event to let the library process it. To obtain recognized
/// gestures, users call the accessor function of their desired recognizer or register a listener to their desired
/// recognizer before processing touch events.
/// @param touch_event The user generated touch event to process.
void process_touch_event(touch_event_t* touch_event);

/// @brief Register a recognizer via a recognition function.
/// @param recognize The recognition function to add.
/// @param init Recognizer initialization function, pass 0 or NULL for none.
/// @return Which index the recognizer is saved to. -1 if the recognizer could not be saved.
int add_recognizer(void (*recognizer)(touch_event_t*), void (*init)(void));

/// @brief Remove a recognizer in the library.
/// @param recognizer Index of the recognizer to remove.
/// @return The removed recognizer. If an invalid index is provided, will return an empty recognizer with null recognize
/// and init functions.
gesture_recognizer_t remove_recognizer(int recognizer);

/// @brief Enable a recognizer already in the library.
/// @param recognizer Index of the recognizer to enable.
/// @return 0 if an invalid index is provided, 1 otherwise.
int enable_recognizer(int recognizer);

/// @brief Disable a recognizer already in the library.
/// @param recognizer Index of the recognizer to disable.
/// @return 0 if an invalid index is provided, 1 otherwise.
int disable_recognizer(int recognizer);
