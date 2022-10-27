#include "zoom_and_rotate.h"

#include "math.h"
#include "utils.h"

zoom_and_rotate_t zoom_and_rotate_d[MAX_TOUCHES * (MAX_TOUCHES - 1) / 2];

gesture_event_t zoom_and_rotate = {
    .type = GESTURE_TYPE_ZOOM_AND_ROTATE, .num_touches = 2, .get_data = (void* (*)(void))get_zoom_and_rotate};

static void update_znr(zoom_and_rotate_t* znr, drag_t* drag1, drag_t* drag2);

static void update_znr_data(zoom_and_rotate_t* znr, drag_t* drag1, drag_t* drag2);

gesture_event_t* recognize_zoom_and_rotate(touch_event_t* event) {
    // Let drag process the event
    (void)event;

    drag_t* drag_d = get_drag();
    int index      = 0;
    for (int i = 0; i < MAX_TOUCHES - 1; i++) {
        for (int j = i + 1; j < MAX_TOUCHES; j++) {
            update_znr(zoom_and_rotate_d + index++, drag_d + i, drag_d + j);
        }
    }

    return &zoom_and_rotate;
}

zoom_and_rotate_t* get_zoom_and_rotate() {
    return zoom_and_rotate_d;
}

static void update_znr(zoom_and_rotate_t* znr, drag_t* drag1, drag_t* drag2) {
    switch (znr->state) {
    case RECOGNIZER_STATE_NULL:
        if (znr->drag1_state != RECOGNIZER_STATE_IN_PROGRESS || znr->drag2_state != RECOGNIZER_STATE_IN_PROGRESS) {
            if (drag1->state == RECOGNIZER_STATE_IN_PROGRESS && drag2->state == RECOGNIZER_STATE_IN_PROGRESS) {
                if (SQUARE_SUM(drag2->x - drag1->x, drag2->y - drag1->y) < SQUARE(ZNR_SEP_MAX)) {
                    znr->state = RECOGNIZER_STATE_IN_PROGRESS;
                    znr->x     = (drag1->x + drag2->x) / 2;
                    znr->y     = (drag1->y + drag2->y) / 2;
                    update_znr_data(znr, drag1, drag2);
                }
            }
        }
        break;
    case RECOGNIZER_STATE_IN_PROGRESS:
        if (drag1->state == RECOGNIZER_STATE_IN_PROGRESS && drag2->state == RECOGNIZER_STATE_IN_PROGRESS) {
            float x = (drag1->x + drag2->x) / 2;
            float y = (drag1->y + drag2->y) / 2;
            if update_znr_data (znr, drag1, drag2)
                ;
        }
        break;
    case RECOGNIZER_STATE_COMPLETED:
        break;
    case RECOGNIZER_STATE_FAILED:
        break;
    }
    znr->drag1_state = drag1->state;
    znr->drag2_state = drag2->state;
    if (gesture->state == RECOGNIZER_STATE_IN_PROGRESS) {
        if (gesture->drag1 == drag || gesture->drag2 == drag) {
            if (drag->state == RECOGNIZER_STATE_IN_PROGRESS) {
                float x, y, m0, mt, a0, at;

                x  = gesture->drag2->x0 - gesture->drag1->x0;
                y  = gesture->drag2->y0 - gesture->drag1->y0;
                m0 = SQUARE_SUM(x, y);
                a0 = atan2f(y, x);

                x  = gesture->drag2->x - gesture->drag1->x;
                y  = gesture->drag2->y - gesture->drag1->y;
                mt = SQUARE_SUM(x, y);
                at = atan2f(y, x);

                gesture->zoom   = sqrtf(mt / m0);
                gesture->rotate = at - a0;
            } else {
                gesture->state = RECOGNIZER_STATE_COMPLETED;
                gesture->drag1 = 0;
                gesture->drag2 = 0;
            }
        }
    }
}

static void update_znr_data(zoom_and_rotate_t* znr, drag_t* drag1, drag_t* drag2) {
    float x, y, m0, mt, a0, at;
    x           = drag2->x0 - drag1->x0;
    y           = drag2->y0 - drag1->y0;
    m0          = SQUARE_SUM(x, y);
    a0          = atan2f(y, x);
    x           = drag2->x - drag1->x;
    y           = drag2->y - drag1->y;
    mt          = SQUARE_SUM(x, y);
    at          = atan2f(y, x);
    znr->zoom   = sqrtf(mt / m0);
    znr->rotate = at - a0;
}
