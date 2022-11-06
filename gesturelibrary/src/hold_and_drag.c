#include "hold_and_drag.h"

#include "drag.h"
#include "hold.h"
#include "math.h"
#include "utils.h"

// data[group1, group2, group3, group4, group5]

hold_and_drag_t hold_and_drag_d[MAX_TOUCHES];

void init_hold_and_drag() {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        hold_and_drag_d[i].state = RECOGNIZER_STATE_NULL;
        hold_and_drag_d[i].x0    = 0;
        hold_and_drag_d[i].y0    = 0;
        hold_and_drag_d[i].x     = 0;
        hold_and_drag_d[i].y     = 0;
        hold_and_drag_d[i].vx    = 0;
        hold_and_drag_d[i].vy    = 0;
    }
}

gesture_event_t hold_and_drag = {
    .type = GESTURE_TYPE_HOLD_AND_DRAG, .num_touches = 1, .get_data = (void* (*)(void))get_hold_and_drag};

static void update_hold_and_drag(hold_and_drag_t* hold_and_drag, hold_t* hold, drag_t* drag);

gesture_event_t* recognize_hold_and_drag(touch_event_t* event) {
    (void)event;
    hold_t* hold_d = get_hold();
    drag_t* drag_d = get_drag();
    for (int index = 0; index < MAX_TOUCHES; index++) {
        update_hold_and_drag(hold_and_drag_d + index, hold_d + index, drag_d + index);
    }

    return &hold_and_drag;
}

hold_and_drag_t* get_hold_and_drag() {
    return hold_and_drag_d;
}

static void update_hold_and_drag(hold_and_drag_t* hold_and_drag, hold_t* hold, drag_t* drag) {
    if (hold_and_drag->state == RECOGNIZER_STATE_IN_PROGRESS) {
        hold_and_drag->state = drag->state;
        hold_and_drag->x     = drag->x;
        hold_and_drag->y     = drag->y;
        hold_and_drag->vx    = drag->vx;
        hold_and_drag->vy    = drag->vy;
    } else {
        if (hold->state == RECOGNIZER_STATE_IN_PROGRESS && (hold->t - hold->t0) > HOLD_TIME_MIN) {
            hold_and_drag->state = RECOGNIZER_STATE_IN_PROGRESS;
            hold_and_drag->x0    = hold->x0;
            hold_and_drag->y0    = hold->y0;
            hold_and_drag->x     = hold->x;
            hold_and_drag->y     = hold->y;
        }
    }
}
