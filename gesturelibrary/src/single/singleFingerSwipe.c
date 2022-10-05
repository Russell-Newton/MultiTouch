#include "singleFingerSwipe.h"

//this class takes care of the single finger cases

void recognize_swipe(touch_event_t* event) {
    //TODO: the first input is gonna be a possible tap, if tap's state is possible and we see the very next input is in a different location, swipe is possible
    //if swipe is possible and see a new location, status stays possible
    //if we get a null, stop
    //distance recognition?? if the swipe went x millimeters far, we perform the action or its an incomplete swipe
}

tap_data_t* get_tap_data() {
    return data;
}