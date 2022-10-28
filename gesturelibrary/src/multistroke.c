#include "multistroke.h"

multistroke_t multistroke_d[MAX_TOUCHES];

gesture_event_t* recognize_multistroke(touch_event_t* event) {
    return 0;
}

multistroke_t* get_multistroke() {
    return multistroke_d;
}
