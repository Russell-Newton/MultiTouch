#include "double.h"

#include "gestureparams.h"
#include "utils.h"

#include <stdio.h>

double_tap_t double_tap_d[MAX_TOUCHES];
void (*on_double_tap)(const double_tap_t*) = 0;

void init_double_tap() {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        double_tap_d[i].state = RECOGNIZER_STATE_NULL;
        double_tap_d[i].group = i + 1;
        // double_tap_d[i].num_touches = 0;
        double_tap_d[i].t = 0;
        double_tap_d[i].x = 0;
        double_tap_d[i].y = 0;
    }
    on_double_tap = 0;
}

// gesture_event_t double_tap = {.type = GESTURE_TYPE_DOUBLE_TAP, .get_data = (void* (*)(void))get_double_tap};

// static touch_event_t* prev_event;

static void update_double_taps(const tap_t* tap, const touch_event_t* event);
static void update_data0_fields(int index, int group_index, const touch_event_t* event);

void recognize_double_tap(const touch_event_t* event) {
    // pass in the event and the taps incrementally
    // if a tap is completed, compare it to all of the double taps in the double data array
    // and check if for any in_progress ones (if it's null or complete, save that index cuz
    // we'll need it later) and check if it's close enough; if so, change that double_tap to
    // complete and update the data fields for it; along the way also compare the timestamps
    // save the index of any within GROUP_TIME_DIFF (we'll group touches this way); if
    // nothing is found, update the group and replace the existing complete/null with this
    // new in_progress double tap
    // change all in_progress to possible

    const tap_t* taps = get_tap();

    // for (int i = 0; i < MAX_TOUCHES; i++) {
    //     if ((taps + i)->t == event->t && (taps + i)->x == event->x &&
    //         (taps + i)->y == event->y) {  // make sure we're not looking at an old completed tap
    //         update_double_taps(taps + i, event);  // we don't need event, but it was for the sake of security
    //     }
    // }
    update_double_taps(taps + event->group, event);
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

static void update_double_taps(const tap_t* tap, const touch_event_t* event) {
    int null_index     = -1;  // last seen null or failed double_d spot
    int complete_index = -1;  // last seen complete spot
    int update         = 1;
    int group_index    = -1;  // double tap with the closest time the completed tap, will group with this tap
    // int group_number;

    int found = 0;

    // float x_diff;
    // float y_diff;
    float pos_diff;
    float time_diff;

    // printf("printing tap_state: %d\n", tap->state);

    if (tap->state == RECOGNIZER_STATE_COMPLETED) {
        // printf("entered with tap x = %f\n", tap->x);
        // if completed, compare it to all double_taps in double_d
        // look for any possible ones and compare time and position

        for (int i = 0; i < MAX_TOUCHES; i++) {
            // printf("entered for loop with tap x = %f\n", tap->x);
            double_tap_t d_tap = double_tap_d[i];

            switch (d_tap.state) {
            case RECOGNIZER_STATE_POSSIBLE:
                // check position and time

                // checking tap position to double tap position

                pos_diff = SQUARED_DIST_0(event, d_tap);
                // x_diff    = (event->x - d_tap.x0) < 0 ? (event->x - d_tap.x0) * -1 : (event->x - d_tap.x0);
                // y_diff    = (event->y - d_tap.y0) < 0 ? (event->y - d_tap.y0) * -1 : (event->y - d_tap.y0);
                time_diff = event->t - d_tap.t0;

                // printf("Printing possible case: x = %f\n", d_tap.x0);
                // printf("Printing tap data: x = %f\n", tap->x);
                // printf("Printing distances: x_diff = %f, y_diff = %f, time_diff = %f\n", x_diff, y_diff, time_diff);

                if (pos_diff < DOUBLE_DIST) {
                    if (time_diff < DOUBLE_TIME_DIFF) {
                        // found the corresponding d_tap
                        double_tap_d[i].state = RECOGNIZER_STATE_COMPLETED;
                        found                 = 1;
                        null_index            = -1;
                        update                = 0;

                        // update data fields
                        double_tap_d[i].x = event->x;
                        double_tap_d[i].y = event->y;
                        double_tap_d[i].t = event->t;
                    } else {
                        double_tap_d[i].state = RECOGNIZER_STATE_FAILED;
                    }

                    if (on_double_tap) {  // listener
                        on_double_tap(&double_tap_d[i]);
                    }
                } else {  // grouping method needs to change
                    float group_time_diff = (tap->t - d_tap.t0) < 0 ? (tap->t - d_tap.t0) * -1 : (tap->t - d_tap.t0);

                    if (group_time_diff <
                        DOUBLE_GROUP_TIME_DIFF) {  // if not it, we need to check if this is part of the same group
                        group_index = i;
                        // group_number = d_tap.group; //set this one's group to this if group_index dne -1
                    }
                }

                break;
            case RECOGNIZER_STATE_FAILED:
            case RECOGNIZER_STATE_NULL:
                // printf("entered null with tap x = %f\n", tap->x);
                // save the index (need to replace)
                null_index = i;  // we want to prioritize null spots and failed spots over completed ones
                break;
            case RECOGNIZER_STATE_COMPLETED:
                complete_index = i;
            default:
                break;
            }

            if (found)
                break;
        }

        if (update) {
            if (null_index != -1) {
                // printf("entered null_index if with tap x = %f and index = %d\n", tap->x, null_index);
                update_data0_fields(null_index, group_index, event);
            } else if (complete_index != -1) {
                update_data0_fields(complete_index, group_index, event);
            }
        }

        for (int j = 0; j < MAX_TOUCHES; j++) {
            printf("Printing state: %d, x: %f, x0: %f, group: %d\n",
                   double_tap_d[j].state,
                   double_tap_d[j].x,
                   double_tap_d[j].x0,
                   double_tap_d[j].group);
        }
    }
}

static void update_data0_fields(int index, int group_index, const touch_event_t* event) {
    double_tap_d[index].state = RECOGNIZER_STATE_POSSIBLE;
    double_tap_d[index].x0    = event->x;
    double_tap_d[index].y0    = event->y;
    double_tap_d[index].t0    = event->t;

    if (group_index != -1) {
        double_tap_d[index].group = double_tap_d[group_index].group;
    }

    if (on_double_tap) {  // listener
        on_double_tap(&double_tap_d[index]);
    }
}

const double_tap_t* get_double_tap() {
    return double_tap_d;
}
