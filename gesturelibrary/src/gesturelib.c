#include "gesturelib.h"

#include "double.h"
#include "drag.h"
#include "hold.h"
#include "hold_and_drag.h"
#include "multidrag.h"
#include "multistroke.h"
#include "rotate.h"
#include "stroke.h"
#include "tap.h"
#include "utils.h"
#include "zoom.h"

gesture_recognizer_t recognizers[MAX_RECOGNIZERS];
int num_recognizers = 0;

/// @brief set containing most recent touch within finger group
touch_event_t latest_touch_events[MAX_TOUCHES];
touch_event_t empty_touch_event = {.type = TOUCH_EVENT_UP, .x = 0, .y = 0, .t = 0, .group = TOUCH_GROUP_UNDEFINED};

/// @brief assign a group ID to this touch_event. Update group_heads to reflect this. If this is not a DOWN event
///        and there are no groups being tracked, do not assign an ID. If this is a DOWN event but MAX_TOUCHES groups
///        are already being tracked, do not assign an ID.
/// @param touch_event
/// @return
static unsigned int assign_group(touch_event_t* touch_event);

int init_gesturelib() {
    for (int i = 0; i < MAX_RECOGNIZERS; i++) {
        recognizers[i].enabled = 0;
    }
    for (int i = 0; i < MAX_TOUCHES; i++) {
        latest_touch_events[i] = empty_touch_event;
    }

    add_recognizer(recognize_stroke, init_stroke);
    add_recognizer(recognize_multistroke, init_multistroke);
    add_recognizer(recognize_tap, init_tap);
    add_recognizer(recognize_double_tap, init_double_tap);
    add_recognizer(recognize_hold, init_hold);
    add_recognizer(recognize_drag, init_drag);
    add_recognizer(recognize_hold_and_drag, init_hold_and_drag);
    add_recognizer(recognize_multidrag, init_multidrag);
    add_recognizer(recognize_zoom, init_zoom);
    add_recognizer(recognize_rotate, init_rotate);

    return 0;
}

void process_touch_event(touch_event_t* touch_event) {
    if (touch_event->group == TOUCH_GROUP_UNDEFINED) {
        assign_group(touch_event);
    }
    for (int index = 0; index < num_recognizers; index++) {
        if (recognizers[index].enabled) {
            recognizers[index].recognize(touch_event);
        }
    }
    if (touch_event->group < MAX_TOUCHES) {
        latest_touch_events[touch_event->group] = *touch_event;
    }
}

static unsigned int assign_group(touch_event_t* touch_event) {
    if (touch_event->type == TOUCH_EVENT_DOWN) {
        unsigned int free_group = MAX_TOUCHES;
        // Find the first untracked group to assign this touch to
        for (unsigned int i = 0; i < MAX_TOUCHES; i++) {
            if (latest_touch_events[i].type == TOUCH_EVENT_UP) {
                free_group = i;
                break;
            }
        }

        // return early if no free groups
        if (free_group == MAX_TOUCHES) {
            return TOUCH_GROUP_UNDEFINED;
        }

        touch_event->group = free_group;
        return free_group;
    }

    // Find the closest tracked group
    unsigned int closest_group = TOUCH_GROUP_UNDEFINED;
    float closest_dist         = -1;
    for (unsigned int i = 0; i < MAX_TOUCHES; i++) {
        // skip untracked groups
        if (latest_touch_events[i].type == TOUCH_EVENT_UP) {
            continue;
        }

        float dist = SQUARED_DIST(&latest_touch_events[i], touch_event);
        if (dist <= EVENT_GROUPING_DIST_MAX * EVENT_GROUPING_DIST_MAX &&
            (closest_group == TOUCH_GROUP_UNDEFINED || dist < closest_dist)) {
            closest_group = i;
            closest_dist  = dist;
        }
    }

    // return if all groups are untracked
    if (closest_group == TOUCH_GROUP_UNDEFINED) {
        return TOUCH_GROUP_UNDEFINED;
    }

    touch_event->group = closest_group;
    return closest_group;
}

int add_recognizer(void (*recognize)(const touch_event_t*), void (*init)(void)) {
    if (num_recognizers == MAX_RECOGNIZERS) {
        return -1;
    }
    gesture_recognizer_t recognizer = {.recognize = recognize, .enabled = 1, .init = init};
    recognizers[num_recognizers]    = recognizer;
    if (recognizers[num_recognizers].init) {
        recognizers[num_recognizers].init();
    }
    return num_recognizers++ - 1;
}

gesture_recognizer_t remove_recognizer(int recognizer) {
    if (recognizer < 0 || recognizer >= num_recognizers) {
        gesture_recognizer_t null_recognizer = {.recognize = 0, .enabled = 0, .init = 0};
        return null_recognizer;
    }
    gesture_recognizer_t out = recognizers[recognizer];
    for (int i = recognizer; i < num_recognizers - 1; i++) {
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
    if (recognizers[recognizer].init) {
        recognizers[recognizer].init();
    }
    return 1;
}

int disable_recognizer(int recognizer) {
    if (recognizer < 0 || recognizer >= num_recognizers) {
        return 0;
    }
    recognizers[recognizer].enabled = 0;
    return 1;
}
