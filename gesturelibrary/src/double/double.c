#include "double.h"

/// @brief
/// @param
void recognize_two_tap(touch_event_t* event) {

}

/// @brief
/// @param
void recognize_two_rotate(touch_event_t*) {
    //two swipes moving laterally opposite?
} //clockwise vs counterclockwise?

/// @brief
/// @param
void recognize_two_zoom(touch_event_t*) {
    //this will be two swipes moving away from each other
}

/// @brief
/// @param
void recognize_two_pinch(touch_event_t*) {
    //this will be two swipes moving towards each other
}

/**
 * @brief Get the tap data object
 * 
 * @return tap_data_t* 
 */
tap_data_t* get_tap_data() {
    return data;
}