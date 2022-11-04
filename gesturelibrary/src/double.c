#include "double.h"

double_tap_t double_tap_d[MAX_TOUCHES];

gesture_event_t double_tap = {
    .type = GESTURE_TYPE_DOUBLE_TAP, .num_touches = 1, .get_data = (void* (*)(void))get_double_tap};

// static touch_event_t* prev_event;

static void update_double_taps(double_tap_t* _double, stroke_t* strokes, tap_t* taps);
static int euclidean_distance(coords_t c1, coords_t c2);

gesture_event_t* recognize_double_tap(touch_event_t* event) {
    (void)event;  // do we need the touch event

    stroke_t* strokes = get_stroke();
    tap_t* taps       = get_tap();

    for (int i = 0; i < MAX_TOUCHES; i++) {
        update_double_taps(double_tap_d + i, strokes + i, taps + i);
    }

    return &double_tap;
}

static void update_double_taps(double_tap_t* _double, stroke_t* strokes, tap_t* taps) {
    switch (_double->state) {
    case RECOGNIZER_STATE_POSSIBLE:
        // look at the timestamp of the tap and the location
        // if too far, move on to the next one
        // if timestamp is too far, set this double to failed
        if (strokes->state == RECOGNIZER_STATE_COMPLETED) {
            coords_t c1 = {.x = _double->x, .y = _double->y};
            coords_t c2 = {.x = strokes->x, .y = strokes->y};
            int dist    = euclidean_distance(c1, c2);

            if (_double->t - strokes->t > DOUBLE_DIFF) {  // if t_diff too big, this dTap has failed
                _double->state = RECOGNIZER_STATE_FAILED;
                break;
            } else if (dist > DOUBLE_DISTANCE) {
                break;
            } else {
                _double->state = RECOGNIZER_STATE_COMPLETED;
                break;
            }
        }
        break;
    case RECOGNIZER_STATE_FAILED:
        if (taps->state == RECOGNIZER_STATE_COMPLETED) {
            _double->state = RECOGNIZER_STATE_POSSIBLE;
            break;
        }
        break;
    case RECOGNIZER_STATE_NULL:
    case RECOGNIZER_STATE_COMPLETED:
    default:
        return;
    }

    _double->x = strokes->x;
    _double->y = strokes->y;
    _double->t = strokes->t;
}

static int euclidean_distance(coords_t c1, coords_t c2) {
    return sqrt(pow(c2.x - c1.x, 2) + pow(c2.y - c1.y, 2) * 1.0);
}

double_tap_t* get_double_tap() {
    return double_tap_d;
}
