#include "double.h"

tap_data_t data[MAX_TOUCHES];

static touch_event_t* group_1_touch_event;
static touch_event_t* group_2_touch_event;

/**
 * @brief Update group_1 and group_2 touch events
 * 
 */
void update_touches(touch_event_t* event1, touch_event_t* event2) {
    
}

/**
 * @brief 
 * 
 */
void recognize_two_double_tap(touch_event_t* event) {
    //if tap_2_finger returns "possible", look at the time they happened and if this is another two_tap event, 
    //make sure the time difference is valid for this to return "possible"
}

