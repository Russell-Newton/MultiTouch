#include "gesturelib.h"

#include <emscripten.h>
#include <stdio.h>

EMSCRIPTEN_KEEPALIVE
void say_hi() {
    printf("Hello, World!\n");
}

EMSCRIPTEN_KEEPALIVE
float square(float x) {
    return x * x;
}

EMSCRIPTEN_KEEPALIVE
void init_lib() {
    if (!init_gesturelib()) {
        fprintf(stderr, "Gesture library already initialized!\n");
    }
    for (int i = 0; i < num_recognizers; i++) {
        if (!disable_recognizer(i)) {
            fprintf(stderr, "Couldn't disable recognizer %d\n", i);
        }
    }
}

void print_memory(void* ptr, unsigned long size) {
    unsigned char* char_ptr = (unsigned char*)ptr;
    for (unsigned long i = 0; i < size; i++) {
        printf("Memory at offset %lu: 0x%x\n", i, *(char_ptr + i));
    }
}

void print_breakdown_touch_event(touch_event_t* ptr) {
    printf("sizeof type: %lu\n", sizeof(ptr->type));
    printf("sizeof x: %lu\n", sizeof(ptr->x));
    printf("sizeof y: %lu\n", sizeof(ptr->y));
    printf("sizeof t: %lu\n", sizeof(ptr->t));
    printf("sizeof group: %lu\n", sizeof(ptr->group));
}

EMSCRIPTEN_KEEPALIVE
unsigned long test_create_touch_event(touch_event_t* outptr) {
    static touch_event_t default_out = {.type = TOUCH_EVENT_UP, .x = 1, .y = 2, .t = 5, .group = TOUCH_ID_UNDEFINED};
    print_breakdown_touch_event(outptr);
    // print_memory(outptr, sizeof(touch_event_t));
    *outptr = default_out;
    print_memory(outptr, sizeof(touch_event_t));
    printf("Size of touch_event_t: %lu bytes\n", sizeof(touch_event_t));
    return sizeof(touch_event_t);
}
