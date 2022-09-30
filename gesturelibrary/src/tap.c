#include "tap.h"

tap_data_t data[MAX_TOUCHES];

void recognize_tap(touch_event_t* event) {
    // this is gonna update the state as possible as long as the location are within a certain close knit range
    
}

tap_data_t* get_tap_data() {
    return data;
}