#include "multidrag.h"

#include "drag.h"
#include "math.h"
#include "utils.h"

multidrag_t multidrag_d[MAX_TOUCHES];

void init_multidrag() {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        multidrag_d[i].state     = RECOGNIZER_STATE_NULL;
        multidrag_d[i].uid       = 0;
        multidrag_d[i].size      = 0;
        multidrag_d[i].dx        = 0;
        multidrag_d[i].dy        = 0;
        multidrag_d[i].rotation  = 0;
        multidrag_d[i].scale     = 1;
        multidrag_d[i].dx0       = 0;
        multidrag_d[i].dy0       = 0;
        multidrag_d[i].rotation0 = 0;
        multidrag_d[i].scale0    = 1;
        multidrag_d[i].cx        = 0;
        multidrag_d[i].cy        = 0;
        for (int j = 0; j < MAX_TOUCHES; j++) {
            multidrag_d[i].strokes[j] = 0;
            multidrag_d[i].sx[j]      = 0;
            multidrag_d[i].sy[j]      = 0;
        }
    }
}

static void update_multidrag(multidrag_t* md, int group);
static void zero_multidrag(multidrag_t* md);
static void calculate_center(multidrag_t* md);
static void calculate_transform(multidrag_t* md);

void recognize_multidrag(const touch_event_t* event) {
    int free_index          = -1;
    const stroke_t* strokes = get_stroke();
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (multidrag_d[i].uid == strokes[event->group].uid && multidrag_d[i].size) {
            update_multidrag(multidrag_d + i, event->group);
            return;
        } else if (free_index < 0 && !multidrag_d[i].size) {
            free_index = i;
        }
    }
    if (free_index >= 0 && strokes[event->group].state == RECOGNIZER_STATE_IN_PROGRESS) {
        update_multidrag(multidrag_d + free_index, event->group);
    }
}

const multidrag_t* get_multidrag() {
    return multidrag_d;
}

static void update_multidrag(multidrag_t* md, int group) {
    const stroke_t* strokes = get_stroke();
    md->uid                 = strokes[group].uid;
    if (!md->strokes[group]) {
        switch (md->state) {
        case RECOGNIZER_STATE_NULL:
        case RECOGNIZER_STATE_POSSIBLE:
        case RECOGNIZER_STATE_COMPLETED:
        case RECOGNIZER_STATE_FAILED:
            md->strokes[group] = 1;
            calculate_center(md);
            if (md->size > 0) {
                md->state = RECOGNIZER_STATE_POSSIBLE;
            }
            break;
        default:
            break;
        }
    } else {
        switch (md->state) {
        case RECOGNIZER_STATE_IN_PROGRESS:
            calculate_transform(md);
            if (strokes[group].state == RECOGNIZER_STATE_COMPLETED) {
                md->strokes[group] = 0;
                calculate_center(md);
                if (md->size < 2) {
                    md->state = RECOGNIZER_STATE_COMPLETED;
                }
            }
            break;
        case RECOGNIZER_STATE_POSSIBLE:
            if (strokes[group].state == RECOGNIZER_STATE_IN_PROGRESS) {
                if (md->size > 1) {
                    float dist = SQUARE_SUM(strokes[group].x - md->px[group], strokes[group].y - md->py[group]);
                    if (dist > SQUARE(DRAG_DIST_MIN)) {
                        zero_multidrag(md);
                        calculate_center(md);
                        md->state = RECOGNIZER_STATE_IN_PROGRESS;
                    }
                }
            } else {
                md->strokes[group] = 0;
                calculate_center(md);
                if (md->size < 1) {
                    md->state = RECOGNIZER_STATE_FAILED;
                }
            }
            break;
        default:
            if (strokes[group].state == RECOGNIZER_STATE_COMPLETED) {
                md->strokes[group] = 0;
                calculate_center(md);
            }
            break;
        }
    }
}

static void zero_multidrag(multidrag_t* md) {
    md->dx       = 0;
    md->dy       = 0;
    md->rotation = 0;
    md->scale    = 1;
}

static void calculate_center(multidrag_t* md) {
    // copy last translation and rotation and scaling
    md->dx0       = md->dx;
    md->dy0       = md->dy;
    md->rotation0 = md->rotation;
    md->scale0    = md->scale;

    // calculate new center
    const stroke_t* strokes = get_stroke();
    md->size                = 0;
    float x                 = 0;
    float y                 = 0;
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (md->strokes[i]) {
            md->px[i] = strokes[i].x;
            x += strokes[i].x;
            md->py[i] = strokes[i].y;
            y += strokes[i].y;
            md->size++;
        }
    }
    md->cx = x / md->size;
    md->cy = y / md->size;

    // calculate new scaled offsets
    float dx[MAX_TOUCHES];
    float dy[MAX_TOUCHES];
    float a = 0;
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (md->strokes[i]) {
            dx[i] = strokes[i].x - md->cx;
            dy[i] = strokes[i].y - md->cy;
            a += SQUARE(dx[i]) + SQUARE(dy[i]);
        }
    }
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (md->strokes[i]) {
            md->sx[i] = dx[i] / a;
            md->sy[i] = dy[i] / a;
        }
    }
}

static void calculate_transform(multidrag_t* md) {
    // calculate current center
    const stroke_t* strokes = get_stroke();
    float cx                = 0;
    float cy                = 0;
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (md->strokes[i]) {
            cx += strokes[i].x;
            cy += strokes[i].y;
        }
    }
    cx /= md->size;
    cy /= md->size;

    // calculate translation
    md->dx = md->dx0 + (cx - md->cx);
    md->dy = md->dy0 + (cy - md->cy);

    // calculate rotation and scaling
    float a = 0;
    float b = 0;
    for (int i = 0; i < MAX_TOUCHES; i++) {
        if (md->strokes[i]) {
            float dx = strokes[i].x - cx;
            float dy = strokes[i].y - cy;
            a += md->sx[i] * dx + md->sy[i] * dy;
            b += md->sx[i] * dy - md->sy[i] * dx;
        }
    }
    md->rotation = md->rotation0 + atan2f(b, a);
    md->scale    = md->scale0 * sqrtf(SQUARE_SUM(a, b));
}
