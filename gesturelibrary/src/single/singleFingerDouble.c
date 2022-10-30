#include "singleFingerDouble.h"

// this class takes care of the single finger cases

sFingerDTap_t sFingerDTap_d[MAX_TOUCHES];

// static touch_event_t* prev_event;

gesture_event_t* recognize_double_tap(touch_event_t* event) {
    (void)event;
    // TODO: take the last touch event in the data array and compare the time stamp of that one and this one and update
    // the state if the two could be a double tap

    // TODO: The following section has been commented since it causes a segfault.
    // if (event->t - prev_event->t > DIFF) {
    //     sFingerDTap_d[0].state = RECOGNIZER_STATE_POSSIBLE;
    // }

    return 0;
}

sFingerDTap_t* get_sFingerDTap() {
    return sFingerDTap_d;
}
