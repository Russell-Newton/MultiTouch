#include "gesturelib.h"

#include "drag.h"
#include "singleFingerDouble.h"
#include "singleFingerHold.h"
#include "singleFingerSwipe.h"
#include "singleFingerTap.h"
#include "zoom_and_rotate.h"

gesture_recognizer_t recognizers[MAX_RECOGNIZERS];
int num_recognizers = 0;

/// @brief set containing most recent touch within finger group
touch_event_t* groups_heads[MAX_TOUCHES];
touch_event_t empty_touch_event = {
    .event_type = TOUCH_EVENT_UP, .position_x = 0, .position_y = 0, .timestamp = 0, .id = TOUCH_ID_UNDEFINED};

static float squared_distance(touch_event_t* a, touch_event_t* b);

/** This is a documentation test. **/
void init_gesturelib() {
    for (uint32_t index = 0; index < MAX_RECOGNIZERS; index++) {
        recognizers[index].enabled = 0;
    }
    for (unsigned int i = 0; i < MAX_TOUCHES; i++) {
        groups_heads[i] = &empty_touch_event;
    }

    add_recognizer(recognize_single_tap);
    add_recognizer(recognize_double_tap);
    add_recognizer(recognize_single_hold);
    add_recognizer(recognize_swipe);
    add_recognizer(recognize_drag);
    add_recognizer(recognize_zoom_and_rotate);
}

int process_touch_event(touch_event_t* touch_event, gesture_event_t* gestures, int max_gestures) {
    uint32_t size = 0;
    if (touch_event->id == TOUCH_ID_UNDEFINED) {
        assign_group(touch_event);
    }
    for (uint32_t index = 0; index < MAX_RECOGNIZERS; index++) {
        if (recognizers[index].enabled) {
            gesture_event_t* gesture = recognizers[index].recognize(touch_event);
            if (gesture && size < max_gestures) {
                gestures[size++] = *gesture;
            }
        }
    }
    return size;
}

unsigned int assign_group(touch_event_t* touch_event) {
    if (touch_event->event_type == TOUCH_EVENT_DOWN) {
        unsigned int free_group = MAX_TOUCHES;
        // Find the first untracked group to assign this touch to
        for (unsigned int i = 0; i < MAX_TOUCHES; i++) {
            if (groups_heads[i]->event_type == TOUCH_EVENT_UP) {
                free_group = i;
                break;
            }
        }

        // return early if no free groups
        if (free_group == MAX_TOUCHES) {
            return TOUCH_ID_UNDEFINED;
        }

        touch_event->id          = free_group;
        groups_heads[free_group] = touch_event;
        return free_group;
    }

    // Find the closest tracked group
    unsigned int closest_group = TOUCH_ID_UNDEFINED;
    float closest_dist         = -1;
    for (unsigned int i = 0; i < MAX_TOUCHES; i++) {
        // skip untracked groups
        if (groups_heads[i]->event_type == TOUCH_EVENT_UP) {
            continue;
        }

        float dist = squared_distance(groups_heads[i], touch_event);
        if (dist <= EVENT_GROUPING_DIST_MAX * EVENT_GROUPING_DIST_MAX &&
            (closest_group == TOUCH_ID_UNDEFINED || dist < closest_dist)) {
            closest_group = i;
            closest_dist  = dist;
        }
    }

    // return if all groups are untracked
    if (closest_group == TOUCH_ID_UNDEFINED) {
        return TOUCH_ID_UNDEFINED;
    }

    touch_event->id             = closest_group;
    groups_heads[closest_group] = touch_event;

    return closest_group;
}

static float squared_distance(touch_event_t* a, touch_event_t* b) {
    float dx = a->position_x - b->position_x;
    float dy = a->position_y - b->position_y;
    return dx * dx + dy * dy;
}

int add_recognizer(gesture_event_t* (*recognize)(touch_event_t*)) {
    if (num_recognizers == MAX_RECOGNIZERS) {
        return -1;
    }
    gesture_recognizer_t recognizer = {.recognize = recognize, .enabled = 1};
    recognizers[num_recognizers++]  = recognizer;
    return num_recognizers - 1;
}

gesture_recognizer_t remove_recognizer(int recognizer) {
    if (recognizer < 0 || recognizer >= num_recognizers) {
        gesture_recognizer_t null_recognizer = {.recognize = 0, .enabled = 0};
        return null_recognizer;
    }
    gesture_recognizer_t out = recognizers[recognizer];
    for (unsigned int i = recognizer; i < num_recognizers - 1; i++) {
        recognizers[i] = recognizers[i + 1];
    }
    num_recognizers--;

    return out;
}

int enable_recognizer(int recognizer) {
    if (recognizer < 0 || recognizer >= num_recognizers) {
        return 0;
    }
    recognizers[recognizer].enabled = 1;
    return 1;
}

int disable_recognizer(int recognizer) {
    if (recognizer < 0 || recognizer >= num_recognizers) {
        return 0;
    }
    recognizers[recognizer].enabled = 0;
    return 1;
}
