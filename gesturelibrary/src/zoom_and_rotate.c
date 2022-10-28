#include "zoom_and_rotate.h"

#include "math.h"
#include "utils.h"

zoom_and_rotate_t zoom_and_rotate_d[MAX_TOUCHES];

gesture_event_t zoom_and_rotate = {
    .type = GESTURE_TYPE_ZOOM_AND_ROTATE, .num_touches = 2, .get_data = (void* (*)(void))get_zoom_and_rotate};

static int update_zoom_and_rotate(drag_t* drag);
static void initialize_zoom_and_rotate(drag_t* drag);

gesture_event_t* recognize_zoom_and_rotate(touch_event_t* event) {
    // Let drag process the event
    (void)event;

    drag_t* drag_d = get_drag();
    for (drag_t* drag = drag_d; drag < drag_d + MAX_TOUCHES; drag++) {
        if (drag->state == RECOGNIZER_STATE_IN_PROGRESS || drag->state == RECOGNIZER_STATE_COMPLETED) {
            if (!update_zoom_and_rotate(drag)) {
                initialize_zoom_and_rotate(drag);
            }
        }
    }

    return &zoom_and_rotate;
}

zoom_and_rotate_t* get_zoom_and_rotate() {
    return zoom_and_rotate_d;
}

static int update_zoom_and_rotate(drag_t* drag) {
    for (zoom_and_rotate_t* gesture = zoom_and_rotate_d; gesture < zoom_and_rotate_d + MAX_TOUCHES; gesture++) {
        if (gesture->state == RECOGNIZER_STATE_IN_PROGRESS) {
            if (gesture->drag1 == drag || gesture->drag2 == drag) {
                if (drag->state == RECOGNIZER_STATE_IN_PROGRESS) {
                    float x, y, m0, mt, a0, at;

                    x  = gesture->drag2->x0 - gesture->drag1->x0;
                    y  = gesture->drag2->y0 - gesture->drag1->y0;
                    m0 = SQUARE_SUM(x, y);
                    a0 = atan2(y, x);

                    x  = gesture->drag2->x - gesture->drag1->x;
                    y  = gesture->drag2->y - gesture->drag1->y;
                    mt = SQUARE_SUM(x, y);
                    at = atan2(y, x);

                    gesture->zoom   = sqrt(mt / m0);
                    gesture->rotate = at - a0;
                } else {
                    gesture->state = RECOGNIZER_STATE_COMPLETED;
                    gesture->drag1 = 0;
                    gesture->drag2 = 0;
                }
                return 1;
            }
        }
    }
    return 0;
}

static void initialize_zoom_and_rotate(drag_t* drag) {
    if (drag->state == RECOGNIZER_STATE_IN_PROGRESS) {
        for (zoom_and_rotate_t* gesture = zoom_and_rotate_d; gesture < zoom_and_rotate_d + MAX_TOUCHES; gesture++) {
            state_t state = gesture->state;
            if (state == RECOGNIZER_STATE_NULL || state == RECOGNIZER_STATE_COMPLETED) {
                if (!gesture->drag1) {
                    gesture->drag1 = drag;
                } else if (drag != gesture->drag1) {
                    if (SQUARE_SUM(drag->x - gesture->drag1->x, drag->y - gesture->drag1->y) <
                        SQUARE(ZOOM_AND_ROTATE_SEPARATION_MAX)) {
                        gesture->drag2 = drag;
                        gesture->state = RECOGNIZER_STATE_IN_PROGRESS;
                    } else {
                        continue;
                    }
                }
                return;
            }
        }
    }
}
