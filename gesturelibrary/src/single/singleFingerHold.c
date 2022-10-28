#include "singleFingerHold.h"

#include "stdio.h"

// this class takes care of the single finger cases

sFingerHold_t sFingerHold_d[MAX_TOUCHES];
static touch_event_t* start;
static touch_event_t* prev = 0;
state_t updated_state;
static gesture_event_t* hold_gesture;

int size;
int d_index;

gesture_event_t* recognize_single_hold(touch_event_t* event) {
    switch (event->type) {
    case TOUCH_EVENT_DOWN:
        process_hold_down(event);
        // clear_data();
        break;
    case TOUCH_EVENT_MOVE:
        process_hold_move(event);
        break;
    case TOUCH_EVENT_UP:
        process_hold_up(event);
        break;
    }

    // switch (updated_state) {
    // case RECOGNIZER_STATE_NULL:
    //     printf("NULL");
    //     break;
    // case RECOGNIZER_STATE_COMPLETED:
    //     printf("COMPLETED");
    //     break;
    // case RECOGNIZER_STATE_FAILED:
    //     printf("FAILED");
    //     break;
    // case RECOGNIZER_STATE_IN_PROGRESS:
    //     printf("IN_PROGRESS");
    //     break;
    // case RECOGNIZER_STATE_POSSIBLE:
    //     printf("POSSIBLE");
    //     break;
    // default:
    //     printf("NOT A VALID STATE");
    // }

    add_to_data(create_touch_data(updated_state, event));
    // sFingerHold_d[0] = create_touch_data(updated_state, event);

    gesture_event_t hold;
    hold.type        = GESTURE_TYPE_HOLD;
    hold.num_touches = 1;
    hold.get_data    = (void* (*)(void))get_sFingerHold;
    hold_gesture     = &hold;
    return hold_gesture;
}

void add_to_data(sFingerHold_t* data) {
    // add data circularly

    if (d_index == MAX_TOUCHES) {
        d_index = 0;
    }

    sFingerHold_d[size] = *data;
    if (size != MAX_TOUCHES)
        size++;
    d_index++;
}

// void clear_data() {
//     for (int i = 0; i < )
// }

sFingerHold_t* create_touch_data(state_t state, touch_event_t* event) {
    sFingerHold_t sFingerHold_data;
    sFingerHold_data.state     = state;
    sFingerHold_data.last_x    = event->x;
    sFingerHold_data.last_y    = event->y;
    sFingerHold_data.last_time = event->t;
    sFingerHold_t* data        = &sFingerHold_data;
    return data;
}

void process_hold_down(touch_event_t* event) {
    start         = event;
    updated_state = RECOGNIZER_STATE_POSSIBLE;
}

void process_hold_move(touch_event_t* event) {
    if (prev) {  // if prev's been initiated
        if (event->x - prev->t > HOLD_DIST_MAX || event->y - prev->y > HOLD_DIST_MAX) {
            updated_state = RECOGNIZER_STATE_FAILED;
        }
    } else if (event->x - start->x > HOLD_DIST_MAX || event->y - start->y > HOLD_DIST_MAX) {
        updated_state = RECOGNIZER_STATE_FAILED;
    } else {
        updated_state = RECOGNIZER_STATE_POSSIBLE;
    }
}

void process_hold_up(touch_event_t* event) {
    if (event->x - start->x > HOLD_DIST_MAX || event->y - start->y > HOLD_DIST_MAX) {
        updated_state = RECOGNIZER_STATE_FAILED;
    } else if (event->t - start->t < HOLD_TIME_MIN) {
        updated_state = RECOGNIZER_STATE_FAILED;
    } else {
        updated_state = RECOGNIZER_STATE_COMPLETED;
    }
}

sFingerHold_t* get_sFingerHold() {
    return sFingerHold_d;
}
