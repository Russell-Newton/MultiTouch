#include "single.h"

//this class takes care of the single finger cases

tap_data_t data[MAX_TOUCHES];

void recognize_single_drag(touch_event_t* event) {
    //TODO: if we had a hold over the last few touches (or started a drag because hold will now be updated to false meaning this recognizer needs to run first) and we see a new location within a certain time difference, we can say it is a drag now 
    //the last x and y will tell you where the drag has ended so far
    //to stop the drag (we can't keep stopping it after every touch_event recieved), it will probably have to be after we stop recieving touch data
        //The driver will need to send a NULL or something. I think what we do is have the driver send a null touch_data if it hasn't recieved an input at the next timestamp after a valid input.
    //this needs to output a continuous stream of locations (think of dragging an app around your home screen, it moves continuously, it doesn't teleport)
}

tap_data_t* get_tap_data() {
    return data;
}