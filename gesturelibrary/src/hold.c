#include "hold.h"

#include "math.h"
#include "stroke.h"
#include "utils.h"

// data[group1, group2, group3, group4, group5]

hold_t hold_d[MAX_TOUCHES];
void (*on_hold)(const hold_t*) = 0;

void init_hold() {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        hold_d[i].state = RECOGNIZER_STATE_NULL;
        hold_d[i].t0    = 0;
        hold_d[i].x0    = 0;
        hold_d[i].y0    = 0;
        hold_d[i].t     = 0;
        hold_d[i].x     = 0;
        hold_d[i].y     = 0;
    }
    on_hold = 0;
}

gesture_event_t hold = {.type = GESTURE_TYPE_HOLD, .get_data = (void* (*)(void))get_hold};

static void update_hold(hold_t* hold, stroke_t* stroke, char down);

gesture_event_t* recognize_hold(touch_event_t* event) {
    stroke_t* strokes = get_stroke();
    for (int index = 0; index < MAX_TOUCHES; index++) {
        update_hold(hold_d + index, strokes + index, event->type == TOUCH_EVENT_DOWN);
    }

    return &hold;
}

int set_on_hold(void (*listener)(const hold_t*)) {
    if (on_hold) {
        on_hold = listener;
        return 0;
    } else {
        on_hold = listener;
        return 1;
    }
}

hold_t* get_hold() {
    return hold_d;
}

static void update_hold(hold_t* hold, stroke_t* stroke, char down) {
    switch (hold->state) {
    case RECOGNIZER_STATE_NULL:
        if (stroke->state == RECOGNIZER_STATE_IN_PROGRESS) {
            hold->state = RECOGNIZER_STATE_IN_PROGRESS;
            if (on_hold) {
                on_hold(hold);
            }
        }
        break;
    case RECOGNIZER_STATE_IN_PROGRESS:
        if (stroke->state == RECOGNIZER_STATE_IN_PROGRESS) {
            float dx = stroke->x - stroke->x0;
            float dy = stroke->y - stroke->y0;
            if (SQUARE_SUM(dx, dy) > SQUARE(HOLD_DIST_MAX)) {
                hold->state = RECOGNIZER_STATE_FAILED;
            }
            if (on_hold) {
                on_hold(hold);
            }
        } else if (stroke->state == RECOGNIZER_STATE_COMPLETED) {
            float dt = stroke->t - stroke->t0;
            float dx = stroke->x - stroke->x0;
            float dy = stroke->y - stroke->y0;
            if (dt <= HOLD_TIME_MIN || SQUARE_SUM(dx, dy) > SQUARE(HOLD_DIST_MAX)) {
                hold->state = RECOGNIZER_STATE_FAILED;
            } else {
                hold->state = RECOGNIZER_STATE_COMPLETED;
            }
            if (on_hold) {
                on_hold(hold);
            }
        }
        break;
    case RECOGNIZER_STATE_COMPLETED:
    case RECOGNIZER_STATE_FAILED:
        if (down && stroke->state == RECOGNIZER_STATE_IN_PROGRESS) {
            hold->state = RECOGNIZER_STATE_IN_PROGRESS;
            if (on_hold) {
                on_hold(hold);
            }
        }
        break;
    default:
        return;
    }

    hold->x0 = stroke->x0;
    hold->y0 = stroke->y0;
    hold->t0 = stroke->t0;
    hold->x  = stroke->x;
    hold->y  = stroke->y;
    hold->t  = stroke->t;
}
