#include "gesturelib.h"

#include "singleFingerDouble.h"
#include "singleFingerDrag.h"
#include "singleFingerHold.h"
#include "singleFingerSwipe.h"
#include "singleFingerTap.h"

gesture_recognizer_t recognizers[MAX_RECOGNIZERS];
int num_recognizers = 0;

/** This is a documentation test. **/
void init_gesturelib() {
    for (uint32_t index = 0; index < MAX_RECOGNIZERS; index++) {
        recognizers[index].enabled = 0;
    }

    add_recognizer(recognize_single_tap);
    add_recognizer(recognize_double_tap);
    add_recognizer(recognize_single_hold);
    add_recognizer(recognize_swipe);
    add_recognizer(recognize_single_drag);
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

int add_recognizer(gesture_event_t* (*recognize)(touch_event_t*)) {
    gesture_recognizer_t recognizer = { .recognize = recognize, .enabled = 1 };
    recognizers[num_recognizers++] = recognizer;
    return num_recognizers - 1;
}

gesture_recognizer_t remove_recognizer(int recognizer) {
    gesture_recognizer_t out = recognizers[recognizer];
    for (unsigned int i = recognizer; i < num_recognizers - 1; i++) {
        recognizers[i] = recognizers[i + 1];
    }
    num_recognizers--;

    return out;
}

void enable_recognizer(int recognizer) {
    recognizers[recognizer].enabled = 1;
}

void disable_recognizer(int recognizer) {
    recognizers[recognizer].enabled = 0;
}
