#include "single.h"

//this class takes care of the single finger cases

tap_data_t data[MAX_TOUCHES];

void recognize_swipe(touch_event_t* event) {
    //TODO: the first input is gonna be a possible tap, if tap's state is possible and we see the very next input is in a different location, swipe is possible
    //if swipe is possible and see a new location, status stays possible
    //if we get a null, stop
    //distance recognition?? if the swipe went x millimeters far, we perform the action or its an incomplete swipe
}

void recognize_single_tap(touch_event_t* event) {
    // TODO: update state based on event (complete in sprint 3)
    // NOTE: if the timestamp of the new event is too far from the timestamp of the last one, this can no longer be possible
}

void recognize_double_tap(touch_event_t* event) {
    //TODO: take the last touch event in the data array and compare the time stamp of that one and this one and update the state if the two could be a double tap
}

void recognize_triple_tap(touch_event_t* event) {
    //TODO: look at the last two taps and this tap and check if the time differences are plausible
}

void recognize_single_drag(touch_event_t* event) {
    //TODO: if we had a hold over the last few touches (or started a drag because hold will now be updated to false meaning this recognizer needs to run first) and we see a new location within a certain time difference, we can say it is a drag now 
    //the last x and y will tell you where the drag has ended so far
    //to stop the drag (we can't keep stopping it after every touch_event recieved), it will probably have to be after we stop recieving touch data
        //The driver will need to send a NULL or something. I think what we do is have the driver send a null touch_data if it hasn't recieved an input at the next timestamp after a valid input.
    //this needs to output a continuous stream of locations (think of dragging an app around your home screen, it moves continuously, it doesn't teleport)
}

void recognize_single_hold(touch_event_t* event) {
    //TODO: look at the last x timestamps/milliseconds of touch data, verifying they are in the same location there are n continuos touch_events that streamed in during that time (say we recieve data at 100 ms intervals and our hold needs to be a second long)
    //if last was a drag (drag's state is possible) this is default false if we didn't see a null at the end of it
}

tap_data_t* get_tap_data() {
    return data;
}