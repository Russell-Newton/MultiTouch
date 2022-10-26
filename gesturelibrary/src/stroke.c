#include "stroke.h"

stroke_t stroke_d[MAX_TOUCHES];

static void begin_stroke(touch_event_t* event);
static void update_stroke(touch_event_t* event, int finish);
static void process_stroke_up(touch_event_t* event);

static void update_velocity(stroke_t* stroke, float vx, float vy);
static void calculate_velocity(stroke_t* stroke);

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

static void update_stroke(touch_event_t* event, int finish) {
    if (event->group < MAX_TOUCHES && stroke_d[event->group].state == RECOGNIZER_STATE_IN_PROGRESS) {
        touch_event_t* last = &latest_touch_events[event->group];
        float vx            = (event->x - last->x) / (event->t - last->t);
        float vy            = (event->y - last->y) / (event->t - last->t);
        update_velocity(stroke_d + event->group, vx, vy);
        stroke_d[event->group].x = event->x;
        stroke_d[event->group].y = event->y;
        stroke_d[event->group].t = event->t;
        if (finish) {
            stroke_d[event->group].state = RECOGNIZER_STATE_COMPLETED;
        }
    }
}

static void update_velocity(stroke_t* stroke, float vx, float vy) {
    if (stroke->cache_size < STROKE_CACHE_SIZE) {
        stroke->cache_last                   = stroke->cache_size;
        stroke->cache_vx[stroke->cache_last] = vx;
        stroke->cache_vy[stroke->cache_last] = vy;
        stroke->cache_size++;
        if (stroke->cache_size == STROKE_CACHE_SIZE) {
            calculate_velocity(stroke);
        }
    } else {
        float nvx                            = stroke->vx * STROKE_CACHE_SIZE;
        float nvy                            = stroke->vy * STROKE_CACHE_SIZE;
        stroke->cache_last                   = (stroke->cache_last + 1) % STROKE_CACHE_SIZE;
        nvx                                  = nvx - stroke->cache_vx[stroke->cache_last] + vx;
        nvy                                  = nvy - stroke->cache_vy[stroke->cache_last] + vy;
        stroke->cache_vx[stroke->cache_last] = vx;
        stroke->cache_vy[stroke->cache_last] = vy;
        stroke->vx                           = nvx / STROKE_CACHE_SIZE;
        stroke->vy                           = nvx / STROKE_CACHE_SIZE;
    }
}

static void calculate_velocity(stroke_t* stroke) {
    float vx = 0;
    float vy = 0;
    for (int index = 0; index < stroke->cache_size; index++) {
        vx += stroke->cache_vx[index];
        vy += stroke->cache_vy[index];
    }
    stroke->vx = vx / stroke->cache_size;
    stroke->vy = vy / stroke->cache_size;
}
