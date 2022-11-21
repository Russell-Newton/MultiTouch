#include "tap.h"

#include "math.h"
#include "stroke.h"
#include "utils.h"

// data[group1, group2, group3, group4, group5]

tap_t tap_d[MAX_TOUCHES];
void (*on_tap)(const tap_t*) = 0;

void init_tap() {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        tap_d[i].state = RECOGNIZER_STATE_NULL;
        tap_d[i].t0    = 0;
        tap_d[i].x0    = 0;
        tap_d[i].y0    = 0;
        tap_d[i].t     = 0;
        tap_d[i].x     = 0;
        tap_d[i].y     = 0;
    }
    on_tap = 0;
}

static void update_tap(tap_t* tap, const stroke_t* stroke, char down);

void recognize_tap(const touch_event_t* event) {
    const stroke_t* strokes = get_stroke();
    for (int index = 0; index < MAX_TOUCHES; index++) {
        update_tap(tap_d + index, strokes + index, event->type == TOUCH_EVENT_DOWN);
    }
}

const tap_t* get_tap() {
    return tap_d;
}

int set_on_tap(void (*listener)(const tap_t*)) {
    if (on_tap) {
        on_tap = listener;
        return 0;
    } else {
        on_tap = listener;
        return 1;
    }
}

static void update_tap(tap_t* tap, const stroke_t* stroke, char down) {
    switch (tap->state) {
    case RECOGNIZER_STATE_NULL:
        if (stroke->state == RECOGNIZER_STATE_IN_PROGRESS) {
            tap->state = RECOGNIZER_STATE_IN_PROGRESS;
            if (on_tap) {
                on_tap(tap);
            }
        }
        break;
    case RECOGNIZER_STATE_IN_PROGRESS:
        if (stroke->state == RECOGNIZER_STATE_IN_PROGRESS) {
            float dt = stroke->t - stroke->t0;
            float dx = stroke->x - stroke->x0;
            float dy = stroke->y - stroke->y0;
            if (dt > TAP_TIME_MAX || SQUARE_SUM(dx, dy) > SQUARE(TAP_DIST_MAX)) {
                tap->state = RECOGNIZER_STATE_FAILED;
            }
        } else if (stroke->state == RECOGNIZER_STATE_COMPLETED) {
            float dt = stroke->t - stroke->t0;
            float dx = stroke->x - stroke->x0;
            float dy = stroke->y - stroke->y0;
            if (dt > TAP_TIME_MAX || SQUARE_SUM(dx, dy) > SQUARE(TAP_DIST_MAX)) {
                tap->state = RECOGNIZER_STATE_FAILED;
            } else {
                tap->state = RECOGNIZER_STATE_COMPLETED;
            }
        }
        if (on_tap) {
            on_tap(tap);
        }
        break;
    case RECOGNIZER_STATE_COMPLETED:
    case RECOGNIZER_STATE_FAILED:
        if (down && stroke->state == RECOGNIZER_STATE_IN_PROGRESS) {
            tap->state = RECOGNIZER_STATE_IN_PROGRESS;
            if (on_tap) {
                on_tap(tap);
            }
        }
        break;
    default:
        return;
    }

    tap->x0 = stroke->x0;
    tap->y0 = stroke->y0;
    tap->t0 = stroke->t0;
    tap->x  = stroke->x;
    tap->y  = stroke->y;
    tap->t  = stroke->t;
}
