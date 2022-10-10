#include "singleFingerSwipe.h"

// this class takes care of the single finger cases

sFingerSwipe_t sFingerSwipe_d[MAX_TOUCHES];

gesture_event_t* recognize_swipe(touch_event_t* event) {
    // TODO: the first input is gonna be a possible tap, if tap's state is possible and we see the very next input is in
    // a different location, swipe is possible if swipe is possible and see a new location, status stays possible if we
    // get a null, stop distance recognition?? if the swipe went x millimeters far, we perform the action or its an
    // incomplete swipe

    return 0;
}

sFingerSwipe_t* get_sFingerSwipe() {
    return sFingerSwipe_d;
}
