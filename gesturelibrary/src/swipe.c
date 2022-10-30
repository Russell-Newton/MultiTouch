#include "swipe.h"

#include "math.h"
#include "stroke.h"
#include "utils.h"

swipe_t swipe_d[MAX_TOUCHES];

gesture_event_t swipe = {.type = GESTURE_TYPE_SWIPE, .num_touches = 1, .get_data = (void* (*)(void))get_swipe};

static void update_swipe(swipe_t* swipe, stroke_t* stroke, char down);

gesture_event_t* recognize_swipe(touch_event_t* event) {
    stroke_t* strokes = get_stroke();
    for (int index = 0; index < MAX_TOUCHES; index++) {
        update_swipe(swipe_d + index, strokes + index, event->type == TOUCH_EVENT_DOWN);
    }

    return &swipe;
}

swipe_t* get_swipe() {
    return swipe_d;
}

static void update_swipe(swipe_t* swipe, stroke_t* stroke, char down) {
    switch (swipe->state) {
    case RECOGNIZER_STATE_NULL:
        if (stroke->state == RECOGNIZER_STATE_IN_PROGRESS) {
            swipe->state = RECOGNIZER_STATE_IN_PROGRESS;
        }
        break;
    case RECOGNIZER_STATE_IN_PROGRESS:
        if (stroke->state == RECOGNIZER_STATE_IN_PROGRESS) {
            float dx    = stroke->x - stroke->x0;
            float dy    = stroke->y - stroke->y0;
            float angle = fabsf(atan2f(dy, dx) - atan2f(stroke->vy, stroke->vx));
            if (SQUARE_SUM(stroke->vx, stroke->vy) < SQUARE_SUM(swipe->vx, swipe->vy) ||
                fminf(angle, 2 * M_PI - angle) > SWIPE_ANGLE_MAX) {
                swipe->state = RECOGNIZER_STATE_FAILED;
            }
        } else if (stroke->state == RECOGNIZER_STATE_COMPLETED) {
            float dx = stroke->x - stroke->x0;
            float dy = stroke->y - stroke->y0;
            if (SQUARE_SUM(dx, dy) > SQUARE(SWIPE_DIST_MIN)) {
                swipe->state = RECOGNIZER_STATE_COMPLETED;
            } else {
                swipe->state = RECOGNIZER_STATE_FAILED;
            }
        }
        break;
    case RECOGNIZER_STATE_COMPLETED:
    case RECOGNIZER_STATE_FAILED:
        if (down && stroke->state == RECOGNIZER_STATE_IN_PROGRESS) {
            swipe->state = RECOGNIZER_STATE_IN_PROGRESS;
        }
        break;
    default:
        return;
    }

    swipe->x0 = stroke->x0;
    swipe->y0 = stroke->y0;
    swipe->vx = stroke->vx;
    swipe->vy = stroke->vy;
}
