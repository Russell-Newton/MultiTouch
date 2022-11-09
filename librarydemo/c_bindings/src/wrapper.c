#include "shared.h"

#include <stdlib.h>

EMSCRIPTEN_KEEPALIVE
touch_event_t* build_touch_event(event_type_t type, float x, float y, float t,
                                 unsigned int group) {
  touch_event_t* out = malloc(sizeof(touch_event_t));
  out->type = type;
  out->x = x;
  out->y = y;
  out->t = t;
  out->group = group;
  return out;
}

out_data_t out_data[MAX_OUT_EVENTS];
out_type_t out_types[MAX_OUT_EVENTS];
int n_out_data;

#define X(type) void CAT(my_on_, type)(const CAT(type, _t)* event) { out_data[n_out_data].type = *event; }
X_RECOGNIZERS
#undef X

void register_listeners() {
  n_out_data = 0;
  set_on_drag(my_on_drag);
}

EMSCRIPTEN_KEEPALIVE
void lib_process_touch_event(touch_event_t* ptr) {
  n_out_data = 0;
  process_touch_event(ptr, 0, 0);
}
