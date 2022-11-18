#include "multistroke.h"

#include "math.h"
#include "stroke.h"
#include "utils.h"

multistroke_t multistroke_d[MAX_TOUCHES];

void init_multistroke() {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        multistroke_d[i].state     = RECOGNIZER_STATE_NULL;
        multistroke_d[i].uid       = 0;
        multistroke_d[i].size      = 0;
        multistroke_d[i].dx        = 0;
        multistroke_d[i].dy        = 0;
        multistroke_d[i].rotation  = 0;
        multistroke_d[i].scale     = 1;
        multistroke_d[i].dx0       = 0;
        multistroke_d[i].dy0       = 0;
        multistroke_d[i].rotation0 = 0;
        multistroke_d[i].scale0    = 1;
        multistroke_d[i].cx        = 0;
        multistroke_d[i].cy        = 0;
        for (int j = 0; j < MAX_TOUCHES; j++) {
            multistroke_d[i].strokes[j] = 0;
            multistroke_d[i].sx[j]      = 0;
            multistroke_d[i].sy[j]      = 0;
        }
    }
}

static void update_multistroke(multistroke_t* ms, int group);
static void zero_multistroke(multistroke_t* ms);
static void calculate_center(multistroke_t* ms);
static void calculate_transform(multistroke_t* ms);

void recognize_multistroke(touch_event_t* event) {
    int free_index    = -1;
    stroke_t* strokes = get_stroke();
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (multistroke_d[i].uid == strokes[event->group].uid && multistroke_d[i].size) {
            update_multistroke(multistroke_d + i, event->group);
            return;
        } else if (free_index < 0 && !multistroke_d[i].size) {
            free_index = i;
        }
    }
    if (free_index >= 0 && strokes[event->group].state == RECOGNIZER_STATE_IN_PROGRESS) {
        update_multistroke(multistroke_d + free_index, event->group);
    }
}

multistroke_t* get_multistroke() {
    return multistroke_d;
}

static void update_multistroke(multistroke_t* ms, int group) {
    stroke_t* strokes = get_stroke();
    ms->uid           = strokes[group].uid;
    if (!ms->strokes[group]) {
        switch (ms->state) {
        case RECOGNIZER_STATE_NULL:
        case RECOGNIZER_STATE_COMPLETED:
            ms->strokes[group] = 1;
            zero_multistroke(ms);
            calculate_center(ms);
            if (ms->size > 1) {
                ms->state = RECOGNIZER_STATE_IN_PROGRESS;
            }
            break;
        case RECOGNIZER_STATE_IN_PROGRESS:
            ms->strokes[group] = 1;
            calculate_center(ms);
            break;
        default:
            break;
        }
    } else {
        if (ms->state == RECOGNIZER_STATE_IN_PROGRESS) {
            calculate_transform(ms);
            if (strokes[group].state == RECOGNIZER_STATE_COMPLETED) {
                ms->strokes[group] = 0;
                calculate_center(ms);
                if (ms->size < 2) {
                    ms->state = RECOGNIZER_STATE_COMPLETED;
                }
            }
        } else if (strokes[group].state == RECOGNIZER_STATE_COMPLETED) {
            ms->strokes[group] = 0;
            calculate_center(ms);
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
