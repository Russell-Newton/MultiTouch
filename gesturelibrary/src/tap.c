#include "tap.h"

tap_data_t data[MAX_TOUCHES];

void recognize_tap(touch_event_t* event) {
    // TODO: update state based on event
}

tap_data_t* get_tap_data() {
    return data;
}