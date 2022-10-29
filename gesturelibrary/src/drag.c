#include "drag.h"

#include "math.h"
#include "stroke.h"
#include "swipe.h"
#include "utils.h"

drag_t drag_d[MAX_TOUCHES];

gesture_event_t drag = {.type = GESTURE_TYPE_DRAG, .num_touches = 1, .get_data = (void* (*)(void))get_drag};

static void update_drag(drag_t* drag, stroke_t* stroke, swipe_t* swipe);

gesture_event_t* recognize_drag(touch_event_t* event) {
    // let stroke and swipe process the event
    (void)event;

    stroke_t* strokes = get_stroke();
    swipe_t* swipes   = get_swipe();
    for (int index = 0; index < MAX_TOUCHES; index++) {
        update_drag(drag_d + index, strokes + index, swipes + index);
    }

    return &drag;
}

drag_t* get_drag() {
    return drag_d;
}

static void update_drag(drag_t* drag, stroke_t* stroke, swipe_t* swipe) {
    switch (drag->state) {
    case RECOGNIZER_STATE_NULL:
    case RECOGNIZER_STATE_COMPLETED:
    case RECOGNIZER_STATE_FAILED:
        if (swipe->state == RECOGNIZER_STATE_IN_PROGRESS) {
            drag->state = RECOGNIZER_STATE_IN_PROGRESS;
        }
        break;
    case RECOGNIZER_STATE_IN_PROGRESS:
        if (stroke->state == RECOGNIZER_STATE_COMPLETED) {
            float dx = stroke->x - stroke->x0;
            float dy = stroke->y - stroke->y0;
            if (swipe->state == RECOGNIZER_STATE_FAILED && SQUARE_SUM(dx, dy) > SQUARE(DRAG_DIST_MIN)) {
                drag->state = RECOGNIZER_STATE_COMPLETED;
            } else {
                drag->state = RECOGNIZER_STATE_FAILED;
            }
        }
        break;
    default:
        return;
    }

    drag->x0 = stroke->x0;
    drag->y0 = stroke->y0;
    drag->x  = stroke->x;
    drag->y  = stroke->y;
}
