#include "hold_and_drag.h"

#include "drag.h"
#include "hold.h"
#include "math.h"
#include "utils.h"

// data[group1, group2, group3, group4, group5]

hold_and_drag_t hold_and_drag_d[MAX_TOUCHES];
void (*on_hold_and_drag)(const hold_and_drag_t*) = 0;

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
    on_hold_and_drag = 0;
}

static void update_hold_and_drag(hold_and_drag_t* hold_and_drag, const hold_t* hold, const drag_t* drag);

void recognize_hold_and_drag(const touch_event_t* event) {
    (void)event;
    const hold_t* hold_d = get_hold();
    const drag_t* drag_d = get_drag();
    for (int index = 0; index < MAX_TOUCHES; index++) {
        update_hold_and_drag(hold_and_drag_d + index, hold_d + index, drag_d + index);
    }
}

const hold_and_drag_t* get_hold_and_drag() {
    return hold_and_drag_d;
}

int set_on_hold_and_drag(void (*listener)(const hold_and_drag_t*)) {
    if (on_hold_and_drag) {
        on_hold_and_drag = listener;
        return 0;
    } else {
        on_hold_and_drag = listener;
        return 1;
    }
}

static void update_hold_and_drag(hold_and_drag_t* hold_and_drag, const hold_t* hold, const drag_t* drag) {
    if (hold_and_drag->state == RECOGNIZER_STATE_IN_PROGRESS) {
        hold_and_drag->state = drag->state;
        hold_and_drag->x     = drag->x;
        hold_and_drag->y     = drag->y;
        hold_and_drag->vx    = drag->vx;
        hold_and_drag->vy    = drag->vy;
        if (on_hold_and_drag) {
            on_hold_and_drag(hold_and_drag);
        }
    } else {
        if (hold->state == RECOGNIZER_STATE_IN_PROGRESS && (hold->t - hold->t0) > HOLD_TIME_MIN) {
            hold_and_drag->state = RECOGNIZER_STATE_IN_PROGRESS;
            hold_and_drag->x0    = hold->x0;
            hold_and_drag->y0    = hold->y0;
            hold_and_drag->x     = hold->x;
            hold_and_drag->y     = hold->y;
            if (on_hold_and_drag) {
                on_hold_and_drag(hold_and_drag);
            }
        }
    }
}
