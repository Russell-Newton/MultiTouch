#include "multistroke.h"

#include "math.h"
#include "stroke.h"
#include "utils.h"

multistroke_t multistroke_d[MAX_TOUCHES];

static void update_multistroke(multistroke_t* ms, int group);
static void zero_multistroke(multistroke_t* ms);
static void calculate_center(multistroke_t* ms);
static void calculate_transform(multistroke_t* ms);

gesture_event_t* recognize_multistroke(touch_event_t* event) {
    int free_index    = -1;
    stroke_t* strokes = get_stroke();
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (multistroke_d[i].uid == strokes[event->group].uid &&
            (multistroke_d[i].state == RECOGNIZER_STATE_POSSIBLE ||
             multistroke_d[i].state == RECOGNIZER_STATE_IN_PROGRESS)) {
            update_multistroke(multistroke_d + i, event->group);
            return 0;
        } else if (free_index < 0 && (multistroke_d[i].state == RECOGNIZER_STATE_NULL ||
                                      multistroke_d[i].state == RECOGNIZER_STATE_COMPLETED ||
                                      multistroke_d[i].state == RECOGNIZER_STATE_FAILED)) {
            free_index = i;
        }
    }
    update_multistroke(multistroke_d + free_index, event->group);
    return 0;
}

multistroke_t* get_multistroke() {
    return multistroke_d;
}

static void update_multistroke(multistroke_t* ms, int group) {
    stroke_t* strokes = get_stroke();
    ms->uid           = strokes[group].uid;
    if (!ms->strokes[group]) {
        ms->strokes[group] = 1;
        switch (ms->state) {
        case RECOGNIZER_STATE_NULL:
        case RECOGNIZER_STATE_FAILED:
        case RECOGNIZER_STATE_COMPLETED:
            ms->state = RECOGNIZER_STATE_POSSIBLE;
            break;
        case RECOGNIZER_STATE_POSSIBLE:
            ms->state = RECOGNIZER_STATE_IN_PROGRESS;
            zero_multistroke(ms);
            calculate_center(ms);
            break;
        case RECOGNIZER_STATE_IN_PROGRESS:
            calculate_center(ms);
            break;
        }
    } else {
        if (ms->state == RECOGNIZER_STATE_POSSIBLE) {
            if (strokes[group].state == RECOGNIZER_STATE_COMPLETED) {
                ms->state = RECOGNIZER_STATE_FAILED;
                for (int i = 0; i < MAX_TOUCHES; i++) {
                    ms->strokes[i] = 0;
                }
            }
        } else if (ms->state == RECOGNIZER_STATE_IN_PROGRESS) {
            calculate_transform(ms);
            if (strokes[group].state == RECOGNIZER_STATE_COMPLETED) {
                ms->strokes[group] = 0;
                calculate_center(ms);
                if (ms->size < 2) {
                    ms->state = RECOGNIZER_STATE_COMPLETED;
                }
            }
        }
    }
}

static void zero_multistroke(multistroke_t* ms) {
    ms->dx       = 0;
    ms->dy       = 0;
    ms->rotation = 0;
    ms->scale    = 1;
}

static void calculate_center(multistroke_t* ms) {
    // copy last translation and rotation and scaling
    ms->dx0       = ms->dx;
    ms->dy0       = ms->dy;
    ms->rotation0 = ms->rotation;
    ms->scale0    = ms->scale;

    // calculate new center
    stroke_t* strokes = get_stroke();
    ms->size          = 0;
    float x           = 0;
    float y           = 0;
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (ms->strokes[i]) {
            x += strokes[i].x;
            y += strokes[i].y;
            ms->size++;
        }
    }
    ms->cx = x / ms->size;
    ms->cy = y / ms->size;

    // calculate new scaled offsets
    float dx[MAX_TOUCHES];
    float dy[MAX_TOUCHES];
    float a = 0;
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (ms->strokes[i]) {
            dx[i] = strokes[i].x - ms->cx;
            dy[i] = strokes[i].y - ms->cy;
            a += SQUARE(dx[i]) + SQUARE(dy[i]);
        }
    }
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (ms->strokes[i]) {
            ms->sx[i] = dx[i] / a;
            ms->sy[i] = dy[i] / a;
        }
    }
}

static void calculate_transform(multistroke_t* ms) {
    // calculate current center
    stroke_t* strokes = get_stroke();
    float cx          = 0;
    float cy          = 0;
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (ms->strokes[i]) {
            cx += strokes[i].x;
            cy += strokes[i].y;
        }
    }
    cx /= ms->size;
    cy /= ms->size;

    // calculate translation
    ms->dx = ms->dx0 + (cx - ms->cx);
    ms->dy = ms->dy0 + (cy - ms->cy);

    // calculate rotation and scaling
    float a = 0;
    float b = 0;
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (ms->strokes[i]) {
            float dx = strokes[i].x - cx;
            float dy = strokes[i].y - cy;
            a += ms->sx[i] * dx + ms->sy[i] * dy;
            b += ms->sx[i] * dy - ms->sy[i] * dx;
        }
    }
    ms->rotation = ms->rotation0 + atan2f(b, a);
    ms->scale    = ms->scale0 * sqrtf(SQUARE_SUM(a, b));
}
