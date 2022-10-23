#include "drag.h"

drag_t drag_d[MAX_TOUCHES];

gesture_event_t drag = {.type = GESTURE_TYPE_DRAG, .num_touches = 1, .get_data = (void* (*)(void))get_drag};

static void process_drag_down(touch_event_t* event);
static void process_drag_move(touch_event_t* event);
static void process_drag_up(touch_event_t* event);

static void cache(drag_t* data, touch_event_t* event);
static void calculate_velocity(drag_t* data);

/// @brief
/// @param event
gesture_event_t* recognize_drag(touch_event_t* event) {
    switch (event->event_type) {
    case TOUCH_EVENT_DOWN:
        process_drag_down(event);
        break;
    case TOUCH_EVENT_MOVE:
        process_drag_move(event);
        break;
    case TOUCH_EVENT_UP:
        process_drag_up(event);
        break;
    default:
        break;
    }
    return &drag;
}

/// @brief
/// @return
drag_t* get_drag() {
    return drag_d;
}

static void process_drag_down(touch_event_t* event) {
    for (int index = 0; index < MAX_TOUCHES; index++) {
        if (drag_d[index].state == RECOGNIZER_STATE_START || drag_d[index].state == RECOGNIZER_STATE_FAILED ||
            drag_d[index].state == RECOGNIZER_STATE_COMPLETED) {
            drag_d[index].x0          = event->position_x;
            drag_d[index].y0          = event->position_y;
            drag_d[index].state       = RECOGNIZER_STATE_POSSIBLE;
            drag_d[index].cache_start = 0;
            drag_d[index].cache[0]    = *event;
            drag_d[index].cache_size  = 1;
            return;
        }
    }
}

static void process_drag_move(touch_event_t* event) {
    for (int index = 0; index < MAX_TOUCHES; index++) {
        int size              = drag_d[index].cache_size;
        int start             = drag_d[index].cache_start;
        int cache_index       = (start + size - 1) % DRAG_CACHED_TOUCH_EVENTS;
        touch_event_t* last_e = &drag_d[index].cache[cache_index];
        if (last_e->id == event->id) {
            cache(&drag_d[index], event);
            calculate_velocity(&drag_d[index]);
            return;
        }
    }
}

static void process_drag_up(touch_event_t* event) {
    for (int index = 0; index < MAX_TOUCHES; index++) {
        int size              = drag_d[index].cache_size;
        int start             = drag_d[index].cache_start;
        int cache_index       = (start + size - 1) % DRAG_CACHED_TOUCH_EVENTS;
        touch_event_t* last_e = &drag_d[index].cache[cache_index];
        if (last_e->id == event->id) {
            if (drag_d[index].state == RECOGNIZER_STATE_IN_PROGRESS) {
                drag_d[index].state = RECOGNIZER_STATE_COMPLETED;
            } else {
                drag_d[index].state = RECOGNIZER_STATE_FAILED;
            }
            return;
        }
    }
}

static void cache(drag_t* data, touch_event_t* event) {
    int index          = (data->cache_start + data->cache_size) % DRAG_CACHED_TOUCH_EVENTS;
    data->cache[index] = *event;
    if (data->cache_size < DRAG_CACHED_TOUCH_EVENTS) {
        data->cache_size++;
    } else {
        data->cache_start = (data->cache_start + 1) % DRAG_CACHED_TOUCH_EVENTS;
    }
}

static void calculate_velocity(drag_t* data) {
    if (data->cache_size < DRAG_CACHED_TOUCH_EVENTS) {
        return;
    } else {
        int end_index = (data->cache_start + data->cache_size - 1) % DRAG_CACHED_TOUCH_EVENTS;
        float vx      = 0;
        float vy      = 0;
        if (data->state == RECOGNIZER_STATE_POSSIBLE) {
            float dx       = data->cache[end_index].position_x - data->x0;
            float dy       = data->cache[end_index].position_y - data->y0;
            float distance = dx * dx + dy * dy;
            if (distance > DRAG_DIST_MIN * DRAG_DIST_MIN) {
                data->state = RECOGNIZER_STATE_IN_PROGRESS;
            }
        }
        for (int index = data->cache_start; index != end_index; index = (index + 1) % DRAG_CACHED_TOUCH_EVENTS) {
            touch_event_t* e1 = &data->cache[index];
            touch_event_t* e2 = &data->cache[(index + 1) % DRAG_CACHED_TOUCH_EVENTS];
            vx += (e2->position_x - e1->position_x) / (e2->timestamp - e1->timestamp);
            vy += (e2->position_y - e1->position_y) / (e2->timestamp - e1->timestamp);
        }
        data->vx = vx / data->cache_size;
        data->vy = vy / data->cache_size;
    }
}
