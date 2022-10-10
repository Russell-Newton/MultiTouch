#include "singleFingerTap.h"

// data[group1, group2, group3, group4, group5]

sFingerTap_t sFingerTap_d[MAX_TOUCHES];

static touch_event_t* prev_event;

gesture_event_t* recognize_single_tap(touch_event_t* event) {
    // TODO: update state based on event (complete in sprint 3)
    // NOTE: if the timestamp of the new event is too far from the timestamp of the last one, this can no longer be
    // possible

    if (!(event->event_type == TOUCH_EVENT_DOWN)) {
        sFingerTap_d[0].state =
            (sFingerTap_d[0].state == RECOGNIZER_STATE_POSSIBLE) ? RECOGNIZER_STATE_COMPLETED : RECOGNIZER_STATE_FAILED;
    } else {
        if (event->timestamp - prev_event->timestamp > TAP_LENGTH) {
            sFingerTap_d[0].state = RECOGNIZER_STATE_FAILED;
        }
    }

    return 0;
}

sFingerTap_t* get_sFingerTap() {
    return sFingerTap_d;
}
