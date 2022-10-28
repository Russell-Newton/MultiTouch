#include "singleFingerHold.h"

// this class takes care of the single finger cases

sFingerHold_t* sFingerHold_d[MAX_TOUCHES];
static touch_event_t* start;
static touch_event_t* prev = 0;
static state_t updated_state;
static gesture_event_t* hold_gesture;

int size;
int d_index;

gesture_event_t* recognize_single_hold(touch_event_t* event) {
    switch (event->event_type) {
    case TOUCH_EVENT_DOWN:
        process_hold_down(event);
        clear_data();
        break;
    case TOUCH_EVENT_MOVE:
        process_hold_move(event);
        break;
    case TOUCH_EVENT_UP:
        process_hold_up(event);
        break;
    }

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

    sFingerHold_d[size] = data;
    if (size != MAX_TOUCHES)
        size++;
    d_index++;
}

void clear_data() {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        sFingerHold_d[i] = 0;
    }
}

sFingerHold_t* create_touch_data(state_t state, touch_event_t* event) {
    sFingerHold_t sFingerHold_data;
    sFingerHold_data.state     = state;
    sFingerHold_data.last_x    = event->position_x;
    sFingerHold_data.last_y    = event->position_y;
    sFingerHold_data.last_time = event->timestamp;
    sFingerHold_t* data        = &sFingerHold_data;
    return data;
}

void process_hold_down(touch_event_t* event) {
    start         = event;
    updated_state = RECOGNIZER_STATE_START;
}

void process_hold_move(touch_event_t* event) {
    if (prev) {  // if prev's been initiated
        if (event->position_x - prev->timestamp > MOVE_RADIUS || event->position_y - prev->position_y > MOVE_RADIUS) {
            updated_state = RECOGNIZER_STATE_FAILED;
        }
    } else if (event->position_x - start->position_x > MOVE_RADIUS ||
               event->position_y - start->position_y > MOVE_RADIUS) {
        updated_state = RECOGNIZER_STATE_FAILED;
    } else {
        updated_state = RECOGNIZER_STATE_POSSIBLE;
    }
}

void process_hold_up(touch_event_t* event) {
    if (event->position_x - start->position_x > MOVE_RADIUS || event->position_y - start->position_y > MOVE_RADIUS) {
        updated_state = RECOGNIZER_STATE_FAILED;
    } else if (event->timestamp - start->timestamp < TIME_DIFF) {
        updated_state = RECOGNIZER_STATE_FAILED;
    } else {
        updated_state = RECOGNIZER_STATE_COMPLETED;
    }
}

sFingerHold_t* get_sFingerHold() {
    return sFingerHold_d;
}
