#include "singleFingerDouble.h"

// this class takes care of the single finger cases

sFingerDTap_t sFingerDTap_d[MAX_TOUCHES];

static touch_event_t* prev_event;

void recognize_double_tap(touch_event_t* event) {
    // TODO: take the last touch event in the data array and compare the time stamp of that one and this one and update
    // the state if the two could be a double tap

    if (event->timestamp - prev_event->timestamp > DIFF) {
        sFingerDTap_d[0].state = possible;
    }
}

sFingerDTap_t* get_sFingerDTap() {
    return sFingerDTap_d;
}
