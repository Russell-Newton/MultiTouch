#include "gesturelib.h"

#include "singleFingerDouble.h"
#include "singleFingerHold.h"
#include "singleFingerSwipe.h"
#include "singleFingerTap.h"

gesture_recognizer_t recognizers[MAX_GESTURES];

/** This is a documentation test. **/
void init_gesturelib() {
    for (uint32_t index = 0; index < MAX_GESTURES; index++) {
        recognizers[index].enabled = 0;
    }

    recognizers[0].enabled   = 1;
    recognizers[0].recognize = recognize_single_tap;

    recognizers[1].enabled   = 1;
    recognizers[1].recognize = recognize_double_tap;

    recognizers[2].enabled   = 1;
    recognizers[2].recognize = recognize_single_hold;

    recognizers[3].enabled   = 1;
    recognizers[3].recognize = recognize_swipe;
}

int process_touch_event(touch_event_t* touch_event, gesture_event_t* gestures, int max_gestures) {
    for (uint32_t index = 0; index < MAX_GESTURES; index++) {
        if (recognizers[index].enabled) {
            recognizers[index].recognize(touch_event);
        }
    }
    return 0;
}
