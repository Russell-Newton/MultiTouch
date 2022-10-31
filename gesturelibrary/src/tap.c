#include "tap.h"

#include "math.h"
#include "stroke.h"
#include "utils.h"

// data[group1, group2, group3, group4, group5]

tap_t tap_d[MAX_TOUCHES];

gesture_event_t tap = {.type = GESTURE_TYPE_TAP, .num_touches = 1, .get_data = (void* (*)(void))get_tap};

static void update_tap(tap_t* tap, stroke_t* stroke, char down);

gesture_event_t* recognize_swipe(touch_event_t* event) {
    stroke_t* strokes = get_stroke();
    for (int index = 0; index < MAX_TOUCHES; index++) {
        update_tap(tap_d + index, strokes + index, event->type == TOUCH_EVENT_DOWN);
    }

    return &tap;
}

tap_t* get_tap() {
    return tap_d;
}

static void update_tap(tap_t* tap, stroke_t* stroke, char down) {
    switch (tap->state) {
    case RECOGNIZER_STATE_NULL:
        if (stroke->state == RECOGNIZER_STATE_IN_PROGRESS) {
            tap->state = RECOGNIZER_STATE_IN_PROGRESS;
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
        break;
    case RECOGNIZER_STATE_COMPLETED:
    case RECOGNIZER_STATE_FAILED:
        if (down && stroke->state == RECOGNIZER_STATE_IN_PROGRESS) {
            tap->state = RECOGNIZER_STATE_IN_PROGRESS;
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