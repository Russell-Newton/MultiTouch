#pragma once

#include <emscripten.h>

#include "double.h"
#include "drag.h"
#include "gesturelib.h"
#include "hold.h"
#include "hold_and_drag.h"
#include "recognizer.h"
#include "rotate.h"
#include "tap.h"
#include "zoom.h"


#define CAT_NOEXPAND(A, B) A##B
#define CAT(A, B) CAT_NOEXPAND(A, B)

#define TYPE_POINTER CAT(TYPE_NAME, _t)*

#define MAX_OUT_EVENTS 50

#define X_RECOGNIZERS \
  X(tap) \
  X(hold) \
  X(hold_and_drag) \
  X(double_tap) \
  X(drag) \
  X(zoom) \
  X(rotate)

typedef union out_data_structs {
  #define X(type) CAT(type, _t) type;
  X_RECOGNIZERS
  #undef X
} out_data_t;

typedef gesture_type_t out_type_t;

extern out_data_t out_data[];
extern out_type_t out_types[];
extern int n_out_data;

