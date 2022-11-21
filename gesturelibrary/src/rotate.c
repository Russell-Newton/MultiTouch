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
        rotate_d[i].rotated  = 0;
    }
}

void recognize_rotate(const touch_event_t* event) {
    // let multidrag process the event
    (void)event;

    const multidrag_t* multidrags = get_multidrag();
    for (int index = 0; index < MAX_TOUCHES; index++) {
        rotate_t previous        = rotate_d[index];
        rotate_d[index].uid      = multidrags[index].uid;
        rotate_d[index].size     = multidrags[index].size;
        rotate_d[index].rotation = multidrags[index].rotation;
        if (!rotate_d[index].rotated && fabsf(multidrags[index].rotation) > ROTATE_ANGLE_MIN) {
            rotate_d[index].rotated = 1;
        }
        if (multidrags[index].state == RECOGNIZER_STATE_COMPLETED) {
            if (rotate_d[index].state == RECOGNIZER_STATE_IN_PROGRESS) {
                if (rotate_d[index].rotated) {
                    rotate_d[index].state = RECOGNIZER_STATE_COMPLETED;
                } else {
                    rotate_d[index].state = RECOGNIZER_STATE_FAILED;
                }
            }
        } else {
            if (rotate_d[index].state == RECOGNIZER_STATE_POSSIBLE &&
                multidrags[index].state == RECOGNIZER_STATE_IN_PROGRESS) {
                rotate_d[index].rotated = 0;
            }
            rotate_d[index].state = multidrags[index].state;
        }
        if (on_rotate && (previous.uid != rotate_d[index].uid || previous.state != rotate_d[index].state ||
                          previous.size != rotate_d[index].size || previous.rotation != rotate_d[index].rotation)) {
            on_rotate(rotate_d + index);
        }
    }
}

const rotate_t* get_rotate() {
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
