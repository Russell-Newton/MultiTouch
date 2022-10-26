#include "singleFingerTap.h"

// data[group1, group2, group3, group4, group5]

sFingerTap_t sFingerTap_d[MAX_TOUCHES];

gesture_event_t tap_gesture;

static state_t updated_state;
static touch_event_t* start;

static void process_tap_down(touch_event_t* event);
static void process_tap_move(touch_event_t* event);
static void process_tap_up(touch_event_t* event);

sFingerTap_t create_tap_data(state_t state, touch_event_t* event) {
    sFingerTap_t tap_data;
    tap_data.state     = state;
    tap_data.last_x    = event->x;
    tap_data.last_y    = event->y;
    tap_data.last_time = event->t;
    return tap_data;
}

gesture_event_t* recognize_single_tap(touch_event_t* event) {
    switch (event->type) {
    case TOUCH_EVENT_DOWN:
        process_tap_down(event);
        break;
    case TOUCH_EVENT_MOVE:
        process_tap_move(event);
        break;
    case TOUCH_EVENT_UP:
        process_tap_up(event);
        break;
    default:
        break;
    }
    sFingerTap_d[0] = create_tap_data(updated_state, event);

    tap_gesture.type        = GESTURE_TYPE_TAP;
    tap_gesture.num_touches = 1;
    tap_gesture.get_data    = (void* (*)(void))get_sFingerTap;
    return &tap_gesture;
}

sFingerTap_t create_null_data() {
    sFingerTap_t tap_data;
    tap_data.state     = RECOGNIZER_STATE_FAILED;
    tap_data.last_x    = 0;
    tap_data.last_y    = 0;
    tap_data.last_time = 0;
    return tap_data;
}

void clear_data() {
    for (int index = 0; index < MAX_TOUCHES; index++) {
        sFingerTap_d[index] = create_null_data();
    }
}

static void process_tap_down(touch_event_t* event) {
    start         = event;
    updated_state = RECOGNIZER_STATE_NULL;
}

static void process_tap_move(touch_event_t* event) {
    if (updated_state == RECOGNIZER_STATE_FAILED) {
        return;
    }
    float deltaX   = event->x - start->x;
    float deltaY   = event->y - start->y;
    float distance = deltaX * deltaX + deltaY * deltaY;
    if (distance > TAP_DIST_MAX || event->t - start->t > TAP_TIME_MAX) {
        updated_state = RECOGNIZER_STATE_FAILED;
    } else {
        updated_state = RECOGNIZER_STATE_POSSIBLE;
    }
}

static void process_tap_up(touch_event_t* event) {
    process_tap_move(event);
    if (updated_state == RECOGNIZER_STATE_FAILED) {
        return;
    }
    updated_state = RECOGNIZER_STATE_COMPLETED;
}

sFingerTap_t* get_sFingerTap() {
    return sFingerTap_d;
}
