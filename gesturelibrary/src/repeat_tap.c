#include "repeat_tap.h"

#include "gestureparams.h"
#include "stroke.h"
#include "utils.h"

// implemented as a circular queue of limited size
repeat_tap_t ktap_d[MAX_TOUCHES];
void (*on_ktap)(const repeat_tap_t*) = 0;

void init_repeat_tap(void) {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        ktap_d[i].state = RECOGNIZER_STATE_NULL;
        ktap_d[i].t     = 0;
        ktap_d[i].x     = 0;
        ktap_d[i].y     = 0;
        ktap_d[i].count = 0;
        ktap_d[i].group = TOUCH_GROUP_UNDEFINED;
    }
    on_ktap = 0;
}

void update_repeat_tap(event_type_t event_type, const stroke_t* stroke) {
    float stroke_squared_disp = SQUARE_SUM(stroke->x - stroke->x0, stroke->y - stroke->y0) > SQUARE(TAP_DIST_MAX);
    float stroke_dtime        = stroke->t - stroke->t0;
    for (int i = 0; i < MAX_TOUCHES; i++) {
        repeat_tap_t* check = ktap_d + i;

        // only update the slot this tap is modifying
        if (check->state != RECOGNIZER_STATE_POSSIBLE || check->group != stroke->group) {
            continue;
        }

        check->x = stroke->x;
        check->y = stroke->y;
        check->t = stroke->t;
        check->group = stroke->group;

        if (stroke_squared_disp > SQUARE(TAP_DIST_MAX) || stroke_dtime > TAP_TIME_MAX) {
            check->state = RECOGNIZER_STATE_FAILED;

            if (on_ktap) {
                on_ktap(check);
            }
        } else if (event_type == TOUCH_EVENT_UP) {
            check->state = RECOGNIZER_STATE_COMPLETED;

            if (on_ktap) {
                on_ktap(check);
            }
        } else {
            check->state = RECOGNIZER_STATE_POSSIBLE;
        }
    }
}

void new_repeat_tap(const stroke_t* stroke) {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        repeat_tap_t* check = ktap_d + i;

        // can't associate a new tap to an in-progress slot
        if (check->state == RECOGNIZER_STATE_POSSIBLE || check->state == RECOGNIZER_STATE_FAILED) {
            continue;
        }

        float squared_dist = SQUARED_DIST(check, stroke);
        float dtime        = stroke->t - check->t;

        // if the tap can't be associated to this slot, go to next one
        if (squared_dist > SQUARE(KTAP_DIST_MAX) || dtime < KTAP_DTIME_MIN || dtime > KTAP_DTIME_MAX) {
            continue;
        }

        // if a valid slot has been found, associate with this one
        check->x = stroke->x;
        check->y = stroke->y;
        check->t = stroke->t;
        check->count++;
        check->state = RECOGNIZER_STATE_POSSIBLE;
        check->group = stroke->group;

        if (on_ktap) {
            on_ktap(check);
        }
        return;
    }

    // if no valid slot was found, replace the oldest not in-progress slot
    int grab_slot = -1;
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (ktap_d[i].state == RECOGNIZER_STATE_POSSIBLE) {
            continue;
        }
        if (grab_slot == -1 || ktap_d[i].t < ktap_d[grab_slot].t) {
            grab_slot = i;
        }
    }
    if (grab_slot == -1) {
        return;
    }
    ktap_d[grab_slot].x     = stroke->x;
    ktap_d[grab_slot].y     = stroke->y;
    ktap_d[grab_slot].t     = stroke->t;
    ktap_d[grab_slot].count = 1;
    ktap_d[grab_slot].state = RECOGNIZER_STATE_POSSIBLE;
    ktap_d[grab_slot].group = stroke->group;
    if (on_ktap) {
        on_ktap(ktap_d + grab_slot);
    }
}

void recognize_repeat_tap(const touch_event_t* event) {
    // grab the now completed stroke associated with this event
    const stroke_t* stroke = get_stroke() + event->group;
    if (event->type == TOUCH_EVENT_DOWN) {
        new_repeat_tap(stroke);
        return;
    }
    update_repeat_tap(event->type, stroke);
}

const repeat_tap_t* get_ktap(void) {
    return ktap_d;
}

int set_on_ktap(void (*listener)(const repeat_tap_t*)) {
    if (on_ktap) {
        on_ktap = listener;
        return 0;
    } else {
        on_ktap = listener;
        return 1;
    }
}
