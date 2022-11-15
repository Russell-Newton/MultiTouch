#include "double.h"

#include "gestureparams.h"
#include "utils.h"

double_tap_t double_tap_d[MAX_TOUCHES];
void (*on_double_tap)(const double_tap_t*) = 0;

void init_double_tap() {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        double_tap_d[i].state = RECOGNIZER_STATE_NULL;
        double_tap_d[i].t     = 0;
        double_tap_d[i].x     = 0;
        double_tap_d[i].y     = 0;
    }
    on_double_tap = 0;
}

gesture_event_t double_tap = {.type = GESTURE_TYPE_DOUBLE_TAP, .get_data = (void* (*)(void))get_double_tap};

// static touch_event_t* prev_event;

static void update_double_taps(double_tap_t* double_tap, stroke_t* stroke, tap_t* tap);

gesture_event_t* recognize_double_tap(touch_event_t* event) {
    (void)event;  // do we need the touch event

    stroke_t* strokes = get_stroke();
    tap_t* taps       = get_tap();

    for (int i = 0; i < MAX_TOUCHES; i++) {
        update_double_taps(double_tap_d + i, strokes + i, taps + i);
    }

    return &double_tap;
}

int set_on_double_tap(void (*listener)(const double_tap_t*)) {
    if (on_double_tap) {
        on_double_tap = listener;
        return 0;
    } else {
        on_double_tap = listener;
        return 1;
    }
}

static void update_double_taps(double_tap_t* double_tap, stroke_t* stroke, tap_t* tap) {
    double_tap->x = stroke->x;
    double_tap->y = stroke->y;
    double_tap->t = stroke->t;

    switch (double_tap->state) {
    case RECOGNIZER_STATE_IN_PROGRESS:
        // look at the timestamp of the tap and the location
        // if too far, move on to the next one
        // if timestamp is too far, set this double to failed

        if (tap->state == RECOGNIZER_STATE_COMPLETED) {
            int dist = SQUARED_DIST(double_tap, tap);

            if (double_tap->t - stroke->t > DOUBLE_DIFF) {  // if t_diff too big, this dTap has failed
                double_tap->state = RECOGNIZER_STATE_FAILED;
                break;
            } else if (dist > DOUBLE_DISTANCE) {
                break;
            } else {
                double_tap->state = RECOGNIZER_STATE_COMPLETED;
                break;
            }
        }
        if (on_double_tap) {
            on_double_tap(double_tap);
        }
        break;
    case RECOGNIZER_STATE_FAILED:
    case RECOGNIZER_STATE_NULL:
    case RECOGNIZER_STATE_COMPLETED:
        if (tap->state == RECOGNIZER_STATE_COMPLETED) {
            double_tap->state = RECOGNIZER_STATE_IN_PROGRESS;
        }
        if (on_double_tap) {
            on_double_tap(double_tap);
        }
        break;
    default:
        return;
    }
}

double_tap_t* get_double_tap() {
    return double_tap_d;
}
