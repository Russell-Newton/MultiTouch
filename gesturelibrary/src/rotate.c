#include "rotate.h"

#include "math.h"
#include "multidrag.h"

rotate_t rotate_d[MAX_TOUCHES];
void (*on_rotate)(const rotate_t*) = 0;

void init_rotate() {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        rotate_d[i].state    = RECOGNIZER_STATE_NULL;
        rotate_d[i].uid      = 0;
        rotate_d[i].size     = 0;
        rotate_d[i].rotation = 0;
    }
}

gesture_event_t rotate = {.type = GESTURE_TYPE_ROTATE, .get_data = (void* (*)(void))get_rotate};

gesture_event_t* recognize_rotate(touch_event_t* event) {
    // let multidrag process the event
    (void)event;

    multidrag_t* multidrags = get_multidrag();
    for (int index = 0; index < MAX_TOUCHES; index++) {
        rotate_t previous        = rotate_d[index];
        rotate_d[index].uid      = multidrags[index].uid;
        rotate_d[index].size     = multidrags[index].size;
        rotate_d[index].rotation = multidrags[index].rotation;
        if (multidrags[index].state == RECOGNIZER_STATE_COMPLETED) {
            if (rotate_d[index].state == RECOGNIZER_STATE_IN_PROGRESS) {
                if (fabsf(multidrags[index].rotation) > ROTATE_ANGLE_MIN) {
                    rotate_d[index].state = RECOGNIZER_STATE_COMPLETED;
                } else {
                    rotate_d[index].state = RECOGNIZER_STATE_FAILED;
                }
            }
        } else {
            rotate_d[index].state = multidrags[index].state;
        }
        if (on_rotate && (previous.uid != rotate_d[index].uid || previous.state != rotate_d[index].state ||
                          previous.size != rotate_d[index].size || previous.rotation != rotate_d[index].rotation)) {
            on_rotate(rotate_d + index);
        }
    }

    return &rotate;
}

rotate_t* get_rotate() {
    return rotate_d;
}

int set_on_rotate(void (*listener)(const rotate_t*)) {
    if (on_rotate) {
        on_rotate = listener;
        return 0;
    } else {
        on_rotate = listener;
        return 1;
    }
}
