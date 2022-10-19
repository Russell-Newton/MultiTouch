#include "singleFingerDrag.h"

sFingerDrag_t sFingerDrag_d[MAX_TOUCHES];

gesture_event_t drag_gesture;

static void process_drag_down(touch_event_t* event);
static void process_drag_move(touch_event_t* event);
static void process_drag_up(touch_event_t* event);

static void cache(sFingerDrag_t* data, touch_event_t* event);
static void calculate_velocity(sFingerDrag_t* data);

/// @brief
/// @param event
gesture_event_t* recognize_single_drag(touch_event_t* event) {
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

    drag_gesture.type        = GESTURE_TYPE_DRAG;
    drag_gesture.num_touches = 1;
    drag_gesture.get_data    = (void* (*)(void))get_sFingerDrag;
    return &drag_gesture;
}

/// @brief
/// @return
sFingerDrag_t* get_sFingerDrag() {
    return sFingerDrag_d;
}

static void process_drag_down(touch_event_t* event) {
    for (int index = 0; index < MAX_TOUCHES; index++) {
        if (sFingerDrag_d[index].state == RECOGNIZER_STATE_START ||
            sFingerDrag_d[index].state == RECOGNIZER_STATE_FAILED ||
            sFingerDrag_d[index].state == RECOGNIZER_STATE_COMPLETED) {
            sFingerDrag_d[index].x0          = event->position_x;
            sFingerDrag_d[index].y0          = event->position_y;
            sFingerDrag_d[index].state       = RECOGNIZER_STATE_POSSIBLE;
            sFingerDrag_d[index].cache_start = 0;
            sFingerDrag_d[index].cache[0]    = *event;
            sFingerDrag_d[index].cache_size  = 1;
            return;
        }
    }
}

static void process_drag_move(touch_event_t* event) {
    for (int index = 0; index < MAX_TOUCHES; index++) {
        int size              = sFingerDrag_d[index].cache_size;
        int start             = sFingerDrag_d[index].cache_start;
        int cache_index       = (start + size - 1) % DRAG_CACHED_TOUCH_EVENTS;
        touch_event_t* last_e = &sFingerDrag_d[index].cache[cache_index];
        if (last_e->id == event->id) {
            cache(&sFingerDrag_d[index], event);
            calculate_velocity(&sFingerDrag_d[index]);
            return;
        }
    }
}

static void process_drag_up(touch_event_t* event) {
    for (int index = 0; index < MAX_TOUCHES; index++) {
        int size              = sFingerDrag_d[index].cache_size;
        int start             = sFingerDrag_d[index].cache_start;
        int cache_index       = (start + size - 1) % DRAG_CACHED_TOUCH_EVENTS;
        touch_event_t* last_e = &sFingerDrag_d[index].cache[cache_index];
        if (last_e->id == event->id) {
            if (sFingerDrag_d[index].state == RECOGNIZER_STATE_IN_PROGRESS) {
                sFingerDrag_d[index].state = RECOGNIZER_STATE_COMPLETED;
            } else {
                sFingerDrag_d[index].state = RECOGNIZER_STATE_FAILED;
            }
            return;
        }
    }
}

static void cache(sFingerDrag_t* data, touch_event_t* event) {
    int index          = (data->cache_start + data->cache_size) % DRAG_CACHED_TOUCH_EVENTS;
    data->cache[index] = *event;
    if (data->cache_size < DRAG_CACHED_TOUCH_EVENTS) {
        data->cache_size++;
    } else {
        data->cache_start = (data->cache_start + 1) % DRAG_CACHED_TOUCH_EVENTS;
    }
}

static void calculate_velocity(sFingerDrag_t* data) {
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
            if (distance > DRAG_MIN_DIST * DRAG_MIN_DIST) {
                data->state = RECOGNIZER_STATE_IN_PROGRESS;
            }
        }
        for (int index = data->cache_start; index != end_index; index = (index + 1) % DRAG_CACHED_TOUCH_EVENTS) {
            vx += data->cache[(index + 1) % DRAG_CACHED_TOUCH_EVENTS].position_x - data->cache[index].position_x;
            vy += data->cache[(index + 1) % DRAG_CACHED_TOUCH_EVENTS].position_y - data->cache[index].position_y;
        }
        data->vx = vx / data->cache_size;
        data->vy = vy / data->cache_size;
    }
}
