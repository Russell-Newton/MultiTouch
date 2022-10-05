#include "singleFingerTap.h"

//data[group1, group2, group3, group4, group5]

/// @brief 
/// @param event 
void recognize_single_tap(touch_event_t* event) {
    // TODO: update state based on event (complete in sprint 3)
    // NOTE: if the timestamp of the new event is too far from the timestamp of the last one, this can no longer be possible
}

/// @brief 
/// @return 
tap_data_t* get_tap_data() {
    return data;
}