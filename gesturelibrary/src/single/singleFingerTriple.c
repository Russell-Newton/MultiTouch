#include "singleFingerTriple.h"

sFingerTriple_t sFingerTriple_d[MAX_TOUCHES];

/// @brief
/// @param event
gesture_event_t* recognize_triple_tap(touch_event_t* event) {
    // TODO: look at the last two taps and this tap and check if the time differences are plausible
}

/// @brief
/// @return
sFingerTriple_t* get_tap_data() {
    return sFingerTriple_d;
}
