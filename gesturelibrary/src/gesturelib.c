#include "gesturelib.h"

#include "singleFingerDouble.h"
#include "singleFingerDrag.h"
#include "singleFingerHold.h"
#include "singleFingerSwipe.h"
#include "singleFingerTap.h"

gesture_recognizer_t recognizers[MAX_RECOGNIZERS];

/** This is a documentation test. **/
void init_gesturelib() {
    for (uint32_t index = 0; index < MAX_RECOGNIZERS; index++) {
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

    recognizers[4].enabled   = 1;
    recognizers[4].recognize = recognize_single_drag;
}

int process_touch_event(touch_event_t* touch_event, gesture_event_t* gestures, int max_gestures) {
    uint32_t size = 0;
    for (uint32_t index = 0; index < MAX_RECOGNIZERS; index++) {
        if (recognizers[index].enabled) {
            gesture_event_t* gesture = recognizers[index].recognize(touch_event);
            if (gesture && size < max_gestures) {
                gestures[size++] = *gesture;
            }
        }
    }
    return size;
}
