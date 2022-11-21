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

gesture_event_t double_tap = {.type = GESTURE_TYPE_DOUBLE_TAP, .get_data = (void* (*)(void))get_double_tap};

// static touch_event_t* prev_event;

static void update_double_taps(tap_t* tap);

gesture_event_t* recognize_double_tap(touch_event_t* event) {
    // pass in the event and the taps incrementally
    // if a tap is completed, compare it to all of the double taps in the double data array
    // and check if for any in_progress ones (if it's null or complete, save that index cuz
    // we'll need it later) and check if it's close enough; if so, change that double_tap to
    // complete and update the data fields for it; along the way also compare the timestamps
    // save the index of any within GROUP_TIME_DIFF (we'll group touches this way); if
    // nothing is found, update the group and replace the existing complete/null with this
    // new in_progress double tap
    // change all in_progress to possible

    // printf("touch event information: %f, %f, %f\n", event->x, event->y, event->t);

    // stroke_t* strokes       = get_stroke();
    // printf("printing strokes: ");
    // for (size_t i = 0; i < MAX_TOUCHES; i++) {
    //     printf("%f, ", strokes[i].x);
    // }
    // printf("\n");
    tap_t* taps = get_tap();

    for (int i = 0; i < MAX_TOUCHES; i++) {
        if ((taps + i)->t == event->t && (taps + i)->x == event->x &&
            (taps + i)->y == event->y) {  // make sure we're not looking at an old completed tap
            update_double_taps(taps + i);  // we don't even need event i think
        }
    }

    return &double_tap;
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

static void update_double_taps(tap_t* tap) {
    int null_index  = -1;  // first null or complete double_d spot
    int group_index = -1;  // double tap with the closest time the completed tap, will group with this tap
    // int group_number;

    float x_diff;
    float y_diff;
    float time_diff;

    if (tap->state == RECOGNIZER_STATE_COMPLETED) {
        // if completed, compare it to all double_taps in double_d
        // look for any possible ones and compare time and position

        for (int i = 0; i < MAX_TOUCHES; i++) {
            double_tap_t d_tap = double_tap_d[i];

            switch (d_tap.state) {
            case RECOGNIZER_STATE_POSSIBLE:
                // check position and time

                // checking tap position to double tap position

                x_diff    = (tap->x - d_tap.x0) < 0 ? (tap->x - d_tap.x0) * -1 : (tap->x - d_tap.x0);
                y_diff    = (tap->y - d_tap.y0) < 0 ? (tap->y - d_tap.y0) * -1 : (tap->y - d_tap.y0);
                time_diff = (tap->t - d_tap.t0) < 0 ? (tap->t - d_tap.t0) * -1 : (tap->t - d_tap.t0);

                if (x_diff < DOUBLE_MAX_X && y_diff < DOUBLE_MAX_Y) {
                    if (time_diff < DOUBLE_TIME_DIFF) {
                        // found the corresponding d_tap
                        double_tap_d[i].state = RECOGNIZER_STATE_COMPLETED;

                        // update data fields
                        double_tap_d[i].x = tap->x;
                        double_tap_d[i].y = tap->y;
                        double_tap_d[i].t = tap->t;
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
            case RECOGNIZER_STATE_COMPLETED:
            case RECOGNIZER_STATE_NULL:
                // save the index (need to replace)
                null_index = i;
            default:
                break;
            }
        }

        if (null_index != -1) {
            double_tap_d[null_index].state = RECOGNIZER_STATE_POSSIBLE;
            double_tap_d[null_index].x0    = tap->x;
            double_tap_d[null_index].y0    = tap->y;
            double_tap_d[null_index].t0    = tap->t;

            if (group_index != -1) {
                double_tap_d[null_index].group = double_tap_d[group_index].group;
            }

            if (on_double_tap) {  // listener
                on_double_tap(&double_tap_d[null_index]);
            }
        }
    }
}

// static void update_double_taps(double_tap_t* double_tap, stroke_t* stroke, tap_t* tap, touch_event_t* current_event)
// {
//     double_tap->x = stroke->x;
//     double_tap->y = stroke->y;
//     double_tap->t = stroke->t;

//     // pass in touch_event and compare the event's x and y to the current stroke we're looking at; if not equal,
//     we're not currently looking at this stroke

//     printf("printing double_tap: %f, %f, %f\n", double_tap->x, double_tap->y, double_tap->t);
//     printf("printing stroke: %f, %f, %f, %d\n", stroke->x, stroke->y, stroke->t, stroke->uid);
//     printf("printing tap: %f, %f, %f\n", tap->x, tap->y, tap->t);

//     // for (int i = 0; i < 0; i++) { //dummy code to not give "variable not used" compilation error
//     //     int num_taps = m_stroke->size;
//     //     if (num_taps == 3) {
//     //         break;
//     //     }
//     // }

//     switch (double_tap->state) {
//     case RECOGNIZER_STATE_IN_PROGRESS:
//         // look at the timestamp of the tap and the location
//         // if too far, move on to the next one
//         // if timestamp is too far, set this double to failed

//         if (tap->state == RECOGNIZER_STATE_COMPLETED &&
//             stroke->x == current_event->x &&
//             stroke->y == current_event->y) {
//             // after completing one double_tap, sift through the double_tap array and look for any other completed
//             ones of an acceptable distance away and with the same timestamp or a fraction of a unit off (~0.0001)
//             // upon finding a valid one, group the two together by incrementing all of their num_touches and grouping
//             them (new "group" field will be needed) together (we need a num_touches and a group, or we just do group
//             and when interpreting the data we look for how many in the group)

//             float x_dist = tap->x - double_tap->x0;
//             printf("printing tap_x and double_tap_x: %f %f %f %f %f %f\n", tap->x, double_tap->x0, double_tap->x,
//             stroke->x, tap->x0, stroke->x0); //mstroke gives null float y_dist = tap->y - double_tap->y0;
//             printf("printing tap_y and double_tap_y: %f, %f %f %f %f %f\n", tap->y, double_tap->y0, double_tap->y,
//             stroke->y, tap->y0, stroke->y0);

//             printf("printing x_dist and y_dist: %f, %f\n", x_dist, y_dist);
//             //Saved the first tap's info in x0, y0, and t0. Actually, I don't think we need this as we can just use
//             tap's information. UPDATE if (tap->t - double_tap->t0 > DOUBLE_DIFF) {  // if t_diff too big, this dTap
//             has failed
//                 double_tap->state = RECOGNIZER_STATE_FAILED;
//             } else if (x_dist <= DIST_X && y_dist <= DIST_Y) {
//                 double_tap->state = RECOGNIZER_STATE_COMPLETED;
//                 //look for other completed
//                 for (size_t i = 0; i < MAX_TOUCHES; i++) {
//                     if (double_tap_d[i].state == RECOGNIZER_STATE_COMPLETED) {
//                         //TODO: grouping logic
//                     }
//                 }
//             }
//         }
//         if (on_double_tap) {
//             on_double_tap(double_tap);
//         }
//         break;
//     case RECOGNIZER_STATE_FAILED:
//     case RECOGNIZER_STATE_NULL:
//     case RECOGNIZER_STATE_COMPLETED:
//         if (tap->state == RECOGNIZER_STATE_COMPLETED &&
//             stroke->x == current_event->x &&
//             stroke->y == current_event->y) {
//             double_tap->state = RECOGNIZER_STATE_IN_PROGRESS;
//             //beginning of double tap, initialize initial position and time
//             double_tap->x0 = stroke->x;
//             double_tap->y0 = stroke->y;
//             double_tap->t0 = stroke->t;
//         }
//         if (on_double_tap) {
//             on_double_tap(double_tap);
//         }
//         break;
//     default:
//         return;
//     }
// }

double_tap_t* get_double_tap() {
    return double_tap_d;
}
