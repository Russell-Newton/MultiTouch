#include "singleFingerTap.h"

// data[group1, group2, group3, group4, group5]

sFingerTap_t sFingerTap_d[MAX_TOUCHES];

gesture_event_t tap_gesture;

static state_t updated_state;
static touch_event_t* start;

static void process_tap_down(touch_event_t* event);
static void process_tap_move(touch_event_t* event);
static void process_tap_up(touch_event_t* event);

gesture_event_t* recognize_single_tap(touch_event_t* event) {
    // TODO: update state based on event (complete in sprint 3)
    // NOTE: if the timestamp of the new event is too far from the timestamp of the last one, this can no longer be
    // possible

    // TODO: The following section has been commented since it causes a segfault.
    // if (!(event->event_type == TOUCH_EVENT_DOWN)) {
    //     sFingerTap_d[0].state =
    //         (sFingerTap_d[0].state == RECOGNIZER_STATE_POSSIBLE) ? RECOGNIZER_STATE_COMPLETED :
    //         RECOGNIZER_STATE_FAILED;
    // } else {
    //     if (event->timestamp - prev_event->timestamp > TAP_LENGTH) {
    //         sFingerTap_d[0].state = RECOGNIZER_STATE_FAILED;
    //     }
    // }

    return 0;
}

void clear_data() {
    for (int index = 0; index < MAX_TOUCHES; index++) {
        sFingerTap_d[index] = 0;
    }
}

static void process_tap_move(touch_event_t* event) {
    if (updated_state == RECOGNIZER_STATE_FAILED) {
        return;
    }
    float deltaX = event->position_x - start->position_x;
    float deltaY = event->position_y - start->position_y;
    if (deltaX * deltaX + deltaY * deltaY > TAP_DISTANCE || event->timestamp - start->timestamp > TAP_TIME) {
        updated_state = RECOGNIZER_STATE_FAILED;
    } else {
        updated_state = RECOGNIZER_STATE_POSSIBLE;
    }
}

sFingerTap_t* get_sFingerTap() {
    return sFingerTap_d;
}
