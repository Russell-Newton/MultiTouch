
#include "double.h"

tap_data_t data[MAX_TOUCHES];

//INITIAL IDEA: this looks at n previous events and, if they're all continuous touch events within a time range of each other, this is currently a hold
//if it was a hold before, this stays a hold

//We can do this by updating a state_t field with the previous state and a gesture_event_t 1 and 2 for the times and locations, but the issue i'm running into is concerning 
//actually realizing it's a drag at first. We would need to keep track of n of the previous touch_events, but we can't really set a number of touch_events for this because
//different devices are gonna input data at possibly different rates. We also can't add random arrays everywhere because of the space problem. Or we say fuck it and add an 
//array of integers. Whenever we get a tap, we populate this array (I think this will have to be circular) with 1 in the next spot until we fully populate the array. If we 
//get a break, tap wipes the array singalling to this class that this can't be a hold anymore, at least not the current sequence of touches. Will this work with multiple 
//gestures happening at the same time?

int backlog[HOLD_LENGTH];

static int index = 0;

static touch_event_t* group_1_touch_event;
static touch_event_t* group_2_touch_event; //make into struct?

/**
 * @brief Adds a 1 to the backlog signalling that a touch input was just recieved (touch_2 does this with every possible tap)
 * 
 * @param event_1 
 * @param event_2 if the location of this event is far from the static variables', this is another gesture happening in a new spot
 */
void add_to_backlog(touch_event) {
    if (false) {
        //if location is too far away, ignore this call (we don't need to wipe it because we could have multiple gestures on the screen)
    } else {
        backlog[index] = 1;
        index++;
    }
}

/**
 * @brief wipe the backlog in the presence of a null event
 * 
 */
void wipe_backlog() {

}

/**
 * @brief 
 * 
 */
void recognize_two_hold(touch_event_t*) {
    
}