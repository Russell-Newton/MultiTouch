#include "single.h"

//this class takes care of the single finger cases

tap_data_t data[MAX_TOUCHES];

void recognize_single_hold(touch_event_t* event) {
    //TODO: look at the last x timestamps/milliseconds of touch data, verifying they are in the same location there are n continuos touch_events that streamed in during that time (say we recieve data at 100 ms intervals and our hold needs to be a second long)
    //if last was a drag (drag's state is possible) this is default false if we didn't see a null at the end of it
}

tap_data_t* get_tap_data() {
    return data;
}