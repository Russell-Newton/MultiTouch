#include "multistroke.h"

#include "stroke.h"
#include "utils.h"

multistroke_t multistroke_d[MAX_TOUCHES];

static void update_multistroke(multistroke_t* ms, int group);
static void zero_multistroke(multistroke_t* ms);
static void calculate_center(multistroke_t* ms);
static void calculate_transform(multistroke_t* ms);

gesture_event_t* recognize_multistroke(touch_event_t* event) {
    char tracked      = 0;
    stroke_t* strokes = get_stroke();
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if ((multistroke_d[i].state == RECOGNIZER_STATE_POSSIBLE ||
             multistroke_d[i].state == RECOGNIZER_STATE_IN_PROGRESS) &&
            multistroke_d[i].uid == strokes[event->group].uid) {
            update_multistroke(multistroke_d + i, event->group);
            tracked = 1;
            break;
        }
    }
    if (!tracked) {
        for (int i = 0; i < MAX_TOUCHES; i++) {
            if (multistroke_d[i].state == RECOGNIZER_STATE_NULL ||
                multistroke_d[i].state == RECOGNIZER_STATE_COMPLETED ||
                multistroke_d[i].state == RECOGNIZER_STATE_FAILED) {
                update_multistroke(multistroke_d + i, event->group);
                break;
            }
        }
    }
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
    ms->dx  = 0;
    ms->dy  = 0;
    ms->a11 = 1;
    ms->a12 = 0;
    ms->a21 = 0;
    ms->a22 = 1;
}

static void calculate_center(multistroke_t* ms) {
    // copy last translation and transformation
    ms->dx0  = ms->dx;
    ms->dy0  = ms->dy;
    ms->a110 = ms->a11;
    ms->a120 = ms->a12;
    ms->a210 = ms->a21;
    ms->a220 = ms->a22;

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
    ms->x0 = x / ms->size;
    ms->y0 = y / ms->size;

    // calculate new transformation helper matrix
    float dx[MAX_TOUCHES];
    float dy[MAX_TOUCHES];
    float a11 = 0;
    float a12 = 0;
    float a22 = 0;
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (ms->strokes[i]) {
            dx[i] = strokes[i].x - ms->x0;
            dy[i] = strokes[i].y - ms->y0;
            a11 += SQUARE(dx[i]);
            a12 += dx[i] * dy[i];
            a22 += SQUARE(dy[i]);
        }
    }
    float det = 1 / (a11 * a22 - SQUARE(a12));
    for (int i = 0; i < MAX_TOUCHES; i++) {
        ms->B[0][i] = det * (a22 * dx[i] - a12 * dy[i]);
        ms->B[1][i] = det * (a11 * dy[i] - a12 * dx[i]);
    }
}

static void calculate_transform(multistroke_t* ms) {
    // calculate current center
    stroke_t* strokes = get_stroke();
    float x0          = 0;
    float y0          = 0;
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (ms->strokes[i]) {
            x0 += strokes[i].x;
            y0 += strokes[i].y;
        }
    }
    x0 /= ms->size;
    y0 /= ms->size;

    // calculate translation
    ms->dx = ms->dx0 + (x0 - ms->x0);
    ms->dy = ms->dy0 + (y0 - ms->y0);

    // calculate transformation
    float a11 = 0;
    float a12 = 0;
    float a21 = 0;
    float a22 = 0;
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (ms->strokes[i]) {
            a11 += ms->B[0][i] * (strokes[i].x - x0);
            a12 += ms->B[1][i] * (strokes[i].x - x0);
            a21 += ms->B[0][i] * (strokes[i].y - y0);
            a22 += ms->B[1][i] * (strokes[i].y - y0);
        }
    }
    ms->a11 = a11 * ms->a110 + a12 * ms->a210;
    ms->a12 = a11 * ms->a120 + a12 * ms->a220;
    ms->a21 = a21 * ms->a110 + a22 * ms->a210;
    ms->a22 = a21 * ms->a120 + a22 * ms->a220;
}
