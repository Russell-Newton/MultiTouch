#include "zoom.h"

#include "math.h"
#include "multidrag.h"

zoom_t zoom_d[MAX_TOUCHES];

void init_zoom() {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        zoom_d[i].state    = RECOGNIZER_STATE_NULL;
        zoom_d[i].uid      = 0;
        zoom_d[i].size     = 0;
        zoom_d[i].rotation = 0;
    }
}

gesture_event_t zoom = {.type = GESTURE_TYPE_ZOOM, .get_data = (void* (*)(void))get_zoom};

gesture_event_t* recognize_zoom(touch_event_t* event) {
    // let multidrag process the event
    (void)event;

    multidrag_t* multidrags = get_multidrag();
    for (int index = 0; index < MAX_TOUCHES; index++) {
        zoom_d[index].state    = multidrags[index].state;
        zoom_d[index].uid      = multidrags[index].uid;
        zoom_d[index].size     = multidrags[index].size;
        zoom_d[index].rotation = multidrags[index].rotation;
    }

    return &zoom;
}

zoom_t* get_zoom() {
    return zoom_d;
}
