#include "stroke.h"

#include "stdio.h"

stroke_t stroke_d[MAX_TOUCHES];

void init_stroke() {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        stroke_d[i].state      = RECOGNIZER_STATE_NULL;
        stroke_d[i].t0         = 0;
        stroke_d[i].x0         = 0;
        stroke_d[i].y0         = 0;
        stroke_d[i].t          = 0;
        stroke_d[i].x          = 0;
        stroke_d[i].y          = 0;
        stroke_d[i].vx         = 0;
        stroke_d[i].vy         = 0;
        stroke_d[i].uid        = 0;
        stroke_d[i].cache_last = 0;
        stroke_d[i].cache_size = 0;
        for (int j = 0; j < STROKE_CACHE_SIZE; j++) {
            stroke_d[i].cache_vx[j] = 0;
            stroke_d[i].cache_vy[j] = 0;
        }
    }
}

static void begin_stroke(touch_event_t* event);
static void update_stroke(touch_event_t* event, char up);

static void update_velocity(stroke_t* stroke, float vx, float vy);

gesture_event_t* recognize_stroke(touch_event_t* event) {
    switch (event->type) {
    case TOUCH_EVENT_DOWN:
        begin_stroke(event);
        break;
    case TOUCH_EVENT_MOVE:
        update_stroke(event, 0);
        break;
    case TOUCH_EVENT_UP:
        update_stroke(event, 1);
        break;
    }
    return 0;
}

stroke_t* get_stroke() {
    return stroke_d;
}

static void begin_stroke(touch_event_t* event) {
    if (event->group < MAX_TOUCHES && (stroke_d[event->group].state == RECOGNIZER_STATE_NULL ||
                                       stroke_d[event->group].state == RECOGNIZER_STATE_COMPLETED)) {

        stroke_d[event->group].state      = RECOGNIZER_STATE_IN_PROGRESS;
        stroke_d[event->group].uid        = event->uid;
        stroke_d[event->group].x0         = event->x;
        stroke_d[event->group].y0         = event->y;
        stroke_d[event->group].t0         = event->t;
        stroke_d[event->group].x          = event->x;
        stroke_d[event->group].y          = event->y;
        stroke_d[event->group].t          = event->t;
        stroke_d[event->group].vx         = 0;
        stroke_d[event->group].vy         = 0;
        stroke_d[event->group].cache_last = 0;
        stroke_d[event->group].cache_size = 0;
    }
}

static void update_stroke(touch_event_t* event, char up) {
    if (event->group < MAX_TOUCHES && stroke_d[event->group].state == RECOGNIZER_STATE_IN_PROGRESS) {
        touch_event_t* last = &latest_touch_events[event->group];
        if (event->t > last->t) {
            float vx = (event->x - last->x) / (event->t - last->t);
            float vy = (event->y - last->y) / (event->t - last->t);
            update_velocity(stroke_d + event->group, vx, vy);
            stroke_d[event->group].x = event->x;
            stroke_d[event->group].y = event->y;
            stroke_d[event->group].t = event->t;
        }
        if (up) {
            stroke_d[event->group].state = RECOGNIZER_STATE_COMPLETED;
        }
    }
}

static void update_velocity(stroke_t* stroke, float vx, float vy) {
    float nvx = stroke->vx * stroke->cache_size + vx;
    float nvy = stroke->vy * stroke->cache_size + vy;
    if (stroke->cache_size < STROKE_CACHE_SIZE) {
        stroke->cache_last = stroke->cache_size++;
    } else {
        stroke->cache_last = (stroke->cache_last + 1) % STROKE_CACHE_SIZE;
        nvx -= stroke->cache_vx[stroke->cache_last];
        nvy -= stroke->cache_vy[stroke->cache_last];
    }
    stroke->cache_vx[stroke->cache_last] = vx;
    stroke->cache_vy[stroke->cache_last] = vy;
    stroke->vx                           = nvx / stroke->cache_size;
    stroke->vy                           = nvy / stroke->cache_size;
}
