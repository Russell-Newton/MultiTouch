#include "singleFingerDouble.h"

// this class takes care of the single finger cases

sFingerDTap_t sFingerDTap_d[MAX_TOUCHES];

void recognize_double_tap(touch_event_t* event) {
    // TODO: take the last touch event in the data array and compare the time stamp of that one and this one and update
    // the state if the two could be a double tap
}

sFingerDTap_t* get_sFingerDTap() {
    return sFingerDTap_d;
}
