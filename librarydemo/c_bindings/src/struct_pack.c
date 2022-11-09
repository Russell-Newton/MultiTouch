#include <stdio.h>
#include <stdlib.h>

#include "double.h"
#include "drag.h"
#include "gesturelib.h"
#include "hold.h"
#include "hold_and_drag.h"
#include "recognizer.h"
#include "rotate.h"
#include "tap.h"
#include "zoom.h"
#include <emscripten.h>

#include "struct_pack.h"

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

typedef struct pass_out_data {
  gesture_event_t *out_events;
  int num_out_events;
} pass_out_data_t;

EMSCRIPTEN_KEEPALIVE
int unpack_num_out_events(pass_out_data_t *ptr) { return ptr->num_out_events; }

EMSCRIPTEN_KEEPALIVE
gesture_event_t *unpack_out_events(pass_out_data_t *ptr) {
  return ptr->out_events;
}

EMSCRIPTEN_KEEPALIVE
pass_out_data_t *lib_process_touch_event(touch_event_t *ptr) {
  gesture_event_t *out_events =
      malloc(sizeof(gesture_event_t) * MAX_OUT_EVENTS);
  int num_out_events = process_touch_event(ptr, out_events, MAX_OUT_EVENTS);

  pass_out_data_t *out = malloc(sizeof(pass_out_data_t));
  out->out_events = out_events;
  out->num_out_events = num_out_events;

  return out;
}

//////////////////////////////////////////////////////////////////////
// touch_event_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define STRUCT_NAME touch_event
UNPACKER_FUNCTION(unsigned int, type)
UNPACKER_FUNCTION(unsigned int, group)
UNPACKER_FUNCTION(float, x)
UNPACKER_FUNCTION(float, y)
UNPACKER_FUNCTION(float, t)
#undef STRUCT_NAME

//////////////////////////////////////////////////////////////////////
// gesture_event_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define STRUCT_NAME gesture_event
ARRAY_GETTER_FUNCTION
UNPACKER_FUNCTION(unsigned int, type)
UNPACKER_FUNCTION(unsigned int, index)
UNPACKER_FUNCTION(void *, get_data)
#undef STRUCT_NAME

//////////////////////////////////////////////////////////////////////
// tap_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define STRUCT_NAME tap
UNPACKER_FUNCTION(state_t, state)
UNPACKER_FUNCTION(float, x0)
UNPACKER_FUNCTION(float, y0)
UNPACKER_FUNCTION(float, t0)
UNPACKER_FUNCTION(float, x)
UNPACKER_FUNCTION(float, y)
UNPACKER_FUNCTION(float, t)
#undef STRUCT_NAME

//////////////////////////////////////////////////////////////////////
// hold_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define STRUCT_NAME hold
UNPACKER_FUNCTION(state_t, state)
UNPACKER_FUNCTION(float, x0)
UNPACKER_FUNCTION(float, y0)
UNPACKER_FUNCTION(float, t0)
UNPACKER_FUNCTION(float, x)
UNPACKER_FUNCTION(float, y)
UNPACKER_FUNCTION(float, t)
#undef STRUCT_NAME

//////////////////////////////////////////////////////////////////////
// hold_and_drag_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define STRUCT_NAME hold_and_drag
UNPACKER_FUNCTION(state_t, state)
UNPACKER_FUNCTION(float, x0)
UNPACKER_FUNCTION(float, y0)
UNPACKER_FUNCTION(float, x)
UNPACKER_FUNCTION(float, y)
UNPACKER_FUNCTION(float, vx)
UNPACKER_FUNCTION(float, vy)
#undef STRUCT_NAME

//////////////////////////////////////////////////////////////////////
// double_tap_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define STRUCT_NAME double_tap
UNPACKER_FUNCTION(state_t, state)
UNPACKER_FUNCTION(float, x)
UNPACKER_FUNCTION(float, y)
UNPACKER_FUNCTION(float, t)
#undef STRUCT_NAME

//////////////////////////////////////////////////////////////////////
// drag_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define STRUCT_NAME drag
UNPACKER_FUNCTION(state_t, state)
UNPACKER_FUNCTION(int, group)
UNPACKER_FUNCTION(float, x0)
UNPACKER_FUNCTION(float, y0)
UNPACKER_FUNCTION(float, x)
UNPACKER_FUNCTION(float, y)
UNPACKER_FUNCTION(float, vx)
UNPACKER_FUNCTION(float, vy)
#undef STRUCT_NAME

//////////////////////////////////////////////////////////////////////
// zoom_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define STRUCT_NAME zoom
UNPACKER_FUNCTION(state_t, state)
UNPACKER_FUNCTION(int, uid)
UNPACKER_FUNCTION(int, size)
UNPACKER_FUNCTION(float, scale)
#undef STRUCT_NAME

//////////////////////////////////////////////////////////////////////
// rotate_t unpacking functions
//////////////////////////////////////////////////////////////////////
#define STRUCT_NAME rotate
UNPACKER_FUNCTION(state_t, state)
UNPACKER_FUNCTION(int, uid)
UNPACKER_FUNCTION(int, size)
UNPACKER_FUNCTION(float, rotation)
#undef STRUCT_NAME
