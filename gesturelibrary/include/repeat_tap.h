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

int get_updated_repeat_tap(void);

const repeat_tap_t* get_repeat_tap(void);

int set_on_repeat_tap(void (*listener)(const repeat_tap_t*));
