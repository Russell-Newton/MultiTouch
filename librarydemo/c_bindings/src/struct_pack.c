#include <stdio.h>
#include <stdlib.h>

#include "gesturelib.h"
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
touch_event_t *build_touch_event(event_type_t type, float x, float y, float t,
                                 unsigned int group) {
  touch_event_t *out = malloc(sizeof(touch_event_t));
  out->type = type;
  out->x = x;
  out->y = y;
  out->t = t;
  out->group = group;
  return out;
}

EMSCRIPTEN_KEEPALIVE
unsigned int unpack_touch_type(touch_event_t *ptr) {
  return (unsigned int)ptr->type;
}

EMSCRIPTEN_KEEPALIVE
float unpack_touch_x(touch_event_t *ptr) { return ptr->x; }

EMSCRIPTEN_KEEPALIVE
float unpack_touch_y(touch_event_t *ptr) { return ptr->y; }

EMSCRIPTEN_KEEPALIVE
float unpack_touch_t(touch_event_t *ptr) { return ptr->t; }

EMSCRIPTEN_KEEPALIVE
float unpack_touch_group(touch_event_t *ptr) { return ptr->group; }

EMSCRIPTEN_KEEPALIVE
unsigned int unpack_gesture_type(gesture_event_t *ptr) {
  return (unsigned int)ptr->type;
}

EMSCRIPTEN_KEEPALIVE
unsigned int unpack_gesture_num_touches(gesture_event_t *ptr) {
  return ptr->num_touches;
}

EMSCRIPTEN_KEEPALIVE
unsigned int unpack_gesture_index(gesture_event_t *ptr) { return ptr->index; }

EMSCRIPTEN_KEEPALIVE
void *unpack_gesture_get_data(gesture_event_t *ptr) { return ptr->get_data; }
