#include "shared.h"

#include <stdio.h>
#include <stdlib.h>

EMSCRIPTEN_KEEPALIVE touch_event_t*
build_touch_event(event_type_t type, float x, float y, float t, unsigned int group) {
    touch_event_t* out = malloc(sizeof(touch_event_t));
    out->type          = type;
    out->x             = x;
    out->y             = y;
    out->t             = t;
    out->group         = group;
    out->uid           = 0;
    return out;
}

out_data_t out_data[MAX_OUT_EVENTS];
out_type_t out_types[MAX_OUT_EVENTS];
int n_out_data;

#define X(type, TYPE)                                                                                                  \
    void CAT(my_on_, type)(const CAT(type, _t) * event) {                                                              \
        out_types[n_out_data]       = CAT(GESTURE_TYPE_, TYPE);                                                        \
        out_data[n_out_data++].type = event;                                                                           \
    }                                                                                                                  \
    EMSCRIPTEN_KEEPALIVE                                                                                               \
    const CAT(type, _t) * CAT(out_data_as_, type)(out_data_t * data) {                                                 \
        return data->type;                                                                                             \
    }
X_RECOGNIZERS
#undef X

EMSCRIPTEN_KEEPALIVE
void register_listeners() {
    n_out_data = 0;
#define X(type, TYPE) CAT(set_on_, type)(CAT(my_on_, type));
    X_RECOGNIZERS
#undef X
}

EMSCRIPTEN_KEEPALIVE
int lib_process_touch_event(touch_event_t* ptr) {
    n_out_data = 0;
    process_touch_event(ptr);
    return n_out_data;
}
