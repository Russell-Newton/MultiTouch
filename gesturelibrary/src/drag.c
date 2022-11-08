#include "drag.h"

#include "math.h"
#include "stroke.h"
#include "utils.h"

drag_t drag_d[MAX_TOUCHES];
void (*on_drag)(const drag_t*) = 0;

void init_drag() {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        drag_d[i].state = RECOGNIZER_STATE_NULL;
        drag_d[i].x0    = 0;
        drag_d[i].y0    = 0;
        drag_d[i].x     = 0;
        drag_d[i].y     = 0;
    }
    on_drag = 0;
}

gesture_event_t drag = {.type = GESTURE_TYPE_DRAG, .num_touches = 1, .get_data = (void* (*)(void))get_drag};

static void update_drag(drag_t* drag, stroke_t* stroke, char down);

gesture_event_t* recognize_drag(touch_event_t* event) {
    stroke_t* strokes = get_stroke();
    for (int index = 0; index < MAX_TOUCHES; index++) {
        update_drag(drag_d + index, strokes + index, event->type == TOUCH_EVENT_DOWN);
    }
    return &drag;
}

drag_t* get_drag() {
    return drag_d;
}

int set_on_drag(void (*event)(const drag_t*)) {
    if (on_drag) {
        on_drag = event;
        return 0;
    } else {
        on_drag = event;
        return 1;
    }
}

static void update_drag(drag_t* drag, stroke_t* stroke, char down) {
    drag->group = stroke->group;
    drag->x0    = stroke->x0;
    drag->y0    = stroke->y0;
    drag->x     = stroke->x;
    drag->y     = stroke->y;
    drag->vx    = stroke->vx;
    drag->vy    = stroke->vy;

    switch (drag->state) {
    case RECOGNIZER_STATE_NULL:
        if (stroke->state == RECOGNIZER_STATE_IN_PROGRESS) {
            drag->state = RECOGNIZER_STATE_IN_PROGRESS;
            if (on_drag) {
                on_drag(drag);
            }
        }
        break;
    case RECOGNIZER_STATE_IN_PROGRESS:
        if (stroke->state == RECOGNIZER_STATE_COMPLETED) {
            float dx = stroke->x - stroke->x0;
            float dy = stroke->y - stroke->y0;
            if (SQUARE_SUM(dx, dy) > SQUARE(DRAG_DIST_MIN)) {
                drag->state = RECOGNIZER_STATE_COMPLETED;
            } else {
                drag->state = RECOGNIZER_STATE_FAILED;
            }
        }
        if (on_drag) {
            on_drag(drag);
        }
        break;
    case RECOGNIZER_STATE_COMPLETED:
    case RECOGNIZER_STATE_FAILED:
        if (down && stroke->state == RECOGNIZER_STATE_IN_PROGRESS) {
            drag->state = RECOGNIZER_STATE_IN_PROGRESS;
            if (on_drag) {
                on_drag(drag);
            }
        }
        break;
    default:
        return;
    }
}
