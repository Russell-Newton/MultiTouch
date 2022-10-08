#include "singleFingerHold.h"

//this class takes care of the single finger cases

sFingerHold_t sFingerHold_d[MAX_TOUCHES];

void recognize_single_hold(touch_event_t* event) {
    //TODO: look at the last x timestamps/milliseconds of touch data, verifying they are in the same location there are n continuos touch_events that streamed in during that time (say we recieve data at 100 ms intervals and our hold needs to be a second long)
    //if last was a drag (drag's state is possible) this is default false if we didn't see a null at the end of it
}

sFingerHold_t* get_sFingerHold() {
    return sFingerHold_d;
}
