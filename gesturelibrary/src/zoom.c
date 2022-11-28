#include "zoom.h"

#include "math.h"
#include "multidrag.h"

zoom_t zoom_d[MAX_TOUCHES];
void (*on_zoom)(const zoom_t*) = 0;

void init_zoom() {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        zoom_d[i].state  = RECOGNIZER_STATE_NULL;
        zoom_d[i].uid    = 0;
        zoom_d[i].size   = 0;
        zoom_d[i].scale  = 1;
        zoom_d[i].zoomed = 0;
    }
}

void recognize_zoom(const touch_event_t* event) {
    // let multidrag process the event
    (void)event;

    const multidrag_t* multidrags = get_multidrag();
    for (int index = 0; index < MAX_TOUCHES; index++) {
        zoom_t previous     = zoom_d[index];
        zoom_d[index].uid   = multidrags[index].uid;
        zoom_d[index].size  = multidrags[index].size;
        zoom_d[index].scale = multidrags[index].scale;
        if (!zoom_d[index].zoomed && fabsf(1 - multidrags[index].scale) > ZOOM_SCALE_MIN) {
            zoom_d[index].zoomed = 1;
        }
        if (multidrags[index].state == RECOGNIZER_STATE_COMPLETED) {
            if (zoom_d[index].state == RECOGNIZER_STATE_IN_PROGRESS) {
                if (zoom_d[index].zoomed) {
                    zoom_d[index].state = RECOGNIZER_STATE_COMPLETED;
                } else {
                    zoom_d[index].state = RECOGNIZER_STATE_FAILED;
                }
            }
        } else {
            if (zoom_d[index].state == RECOGNIZER_STATE_POSSIBLE &&
                multidrags[index].state == RECOGNIZER_STATE_IN_PROGRESS) {
                zoom_d[index].zoomed = 0;
            }
            zoom_d[index].state = multidrags[index].state;
        }
        if (on_zoom && (previous.uid != zoom_d[index].uid || previous.state != zoom_d[index].state ||
                        previous.size != zoom_d[index].size || previous.scale != zoom_d[index].scale)) {
            on_zoom(zoom_d + index);
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
