#include "zoom.h"

#include "math.h"
#include "multidrag.h"

zoom_t zoom_d[MAX_TOUCHES];
void (*on_zoom)(const zoom_t*) = 0;

void init_zoom() {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        zoom_d[i].state = RECOGNIZER_STATE_NULL;
        zoom_d[i].uid   = 0;
        zoom_d[i].size  = 0;
        zoom_d[i].scale = 1;
    }
}

void recognize_zoom(const touch_event_t* event) {
    // let multidrag process the event
    (void)event;

    const multidrag_t* multidrags = get_multidrag();
    for (int index = 0; index < MAX_TOUCHES; index++) {
        if (zoom_d[index].state != multidrags[index].state || zoom_d[index].uid != multidrags[index].uid ||
            zoom_d[index].size != multidrags[index].size || zoom_d[index].scale != multidrags[index].scale) {
            zoom_d[index].state = multidrags[index].state;
            zoom_d[index].uid   = multidrags[index].uid;
            zoom_d[index].size  = multidrags[index].size;
            zoom_d[index].scale = multidrags[index].scale;
            if (on_zoom) {
                on_zoom(zoom_d + index);
            }
        }
    }
}

const zoom_t* get_zoom() {
    return zoom_d;
}

int set_on_zoom(void (*listener)(const zoom_t*)) {
    if (on_zoom) {
        on_zoom = listener;
        return 0;
    } else {
        on_zoom = listener;
        return 1;
    }
}
