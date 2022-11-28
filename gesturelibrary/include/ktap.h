#pragma once

#include "gesturelib.h"
#include "recognizer.h"

typedef struct ktap {
    state_t state;
    float x;
    float y;
    float t;
    int count;

    unsigned int group;
} ktap_t;

void init_ktap(void);

void recognize_ktap(const touch_event_t* event);

const ktap_t* get_ktap(void);

int set_on_ktap(void (*listener)(const ktap_t*));
