#include "ktap.h"

#include "gestureparams.h"
#include "stroke.h"
#include "utils.h"

// implemented as a circular queue of limited size
ktap_t ktap_d[MAX_TOUCHES];
int data_head = 0;
void (*on_ktap)(const ktap_t*) = 0;

void init_ktap(void) {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        ktap_d[i].state = RECOGNIZER_STATE_NULL;
        ktap_d[i].t     = 0;
        ktap_d[i].x     = 0;
        ktap_d[i].y     = 0;
        ktap_d[i].count = 0;
    }
    on_ktap = 0;
}

void recognize_ktap(const touch_event_t* event) {
    // break if this event isn't an up event (we really don't care about anything else with taps)
    if (event->type != TOUCH_EVENT_UP) {
        return;
    }

    // grab the now completed stroke associated with this event
    const stroke_t* stroke = get_stroke() + event->group;

    // break if this stroke wasn't a tap
    if (SQUARE_SUM(stroke->x - stroke->x0, stroke->y - stroke->y0) > SQUARE(TAP_DIST_MAX)) {
        return;
    }

    for (int i = 0; i < MAX_TOUCHES; i++) {
        ktap_t* check = ktap_d + i;
        float squared_dist = SQUARED_DIST(check, stroke);
        float dtime = stroke->t - check->t;

        // if the tap can't be associated to this slot, go to next one
        if (squared_dist > SQUARE(KTAP_DIST_MAX) || dtime < KTAP_DTIME_MIN || dtime > KTAP_DTIME_MAX) {
            continue;
        }

        // if a valid slot has been found, associate with this one
        check->x = event->x;
        check->y = event->y;
        check->t = event->t;
        check->count++;
        check->state = RECOGNIZER_STATE_COMPLETED;
        if (on_ktap) {
            on_ktap(check);
        }
        return;
    }

    // if no valid slot was found, replace the oldest slot
    ktap_d[data_head].x = event->x;
    ktap_d[data_head].y = event->y;
    ktap_d[data_head].t = event->t;
    ktap_d[data_head].count = 1;
    ktap_d[data_head].state = RECOGNIZER_STATE_POSSIBLE;
    if (on_ktap) {
        on_ktap(ktap_d + data_head);
    }
    data_head = (data_head + 1) % MAX_TOUCHES;
    return;
}

const ktap_t* get_ktap(void) {
    return ktap_d;
}

int set_on_ktap(void (*listener)(const ktap_t*)) {
    if (on_ktap) {
        on_ktap = listener;
        return 0;
    } else {
        on_ktap = listener;
        return 1;
    }
}
