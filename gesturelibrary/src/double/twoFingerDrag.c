#include "twoFingerDrag.h"

//if the previous event resulted in a hold and this one is within a reasonable distance, this is a drag
//if the previous event was a drag, this is a drag
//so, if hold is true or drag is true, update this variable to 1
//the output of this just need to spit out the new location

/**
 * @brief if hold or drag, update this to 1
 * 
 * @param prev_event old touch_event (null if i == 0)
 */
void update_state(touch_event_t* prev_event, int i) {
    state = i;
    previous_event = prev_event;
}

/**
 * @brief 
 * 
 */
void recognize_two_double_drag(touch_event_t* event) {
    //if state is 1, take prev_event from update state and compare to this input and process accordingly
}

