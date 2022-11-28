#pragma once

#include "gesturelib.h"
#include "recognizer.h"

typedef struct repeat_tap {
    state_t state;
    float x;
    float y;
    float t;
    int count;

    unsigned int group;
} repeat_tap_t;

void init_repeat_tap(void);

void recognize_repeat_tap(const touch_event_t* event);

const repeat_tap_t* get_ktap(void);

int set_on_ktap(void (*listener)(const repeat_tap_t*));
