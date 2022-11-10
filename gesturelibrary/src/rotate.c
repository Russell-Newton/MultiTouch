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
        if (rotate_d[index].state != multidrags[index].state || rotate_d[index].uid != multidrags[index].uid ||
            rotate_d[index].size != multidrags[index].size || rotate_d[index].rotation != multidrags[index].rotation) {
            rotate_d[index].state    = multidrags[index].state;
            rotate_d[index].uid      = multidrags[index].uid;
            rotate_d[index].size     = multidrags[index].size;
            rotate_d[index].rotation = multidrags[index].rotation;
            if (on_rotate) {
                on_rotate(rotate_d + index);
            }
        }
    }

    return &rotate;
}

rotate_t* get_rotate() {
    return rotate_d;
}

int set_on_rotate(void (*event)(const rotate_t*)) {
    if (on_rotate) {
        on_rotate = event;
        return 0;
    } else {
        on_rotate = event;
        return 1;
    }
}
