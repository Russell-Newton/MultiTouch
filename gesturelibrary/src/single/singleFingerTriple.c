#include "single.h"

//this class takes care of the single finger cases

tap_data_t data[MAX_TOUCHES];

void recognize_triple_tap(touch_event_t* event) {
    //TODO: look at the last two taps and this tap and check if the time differences are plausible
}

tap_data_t* get_tap_data() {
    return data;
}