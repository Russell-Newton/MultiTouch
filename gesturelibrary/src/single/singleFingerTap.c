#include "singleFingerTap.h"

// data[group1, group2, group3, group4, group5]

sFingerTap_t sFingerTap_d[MAX_TOUCHES];

static touch_event_t* prev_event;

void recognize_single_tap(touch_event_t* event) {
    // TODO: update state based on event (complete in sprint 3)
    // NOTE: if the timestamp of the new event is too far from the timestamp of the last one, this can no longer be
    // possible

    if (!(event->down)) {
        sFingerTap_d[0].state = (sFingerTap_d[0].state == possible) ? complete : failed;
    }
}

sFingerTap_t* get_sFingerTap() {
    return sFingerTap_d;
}
