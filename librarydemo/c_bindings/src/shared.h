#pragma once

#include "double.h"
#include "drag.h"
#include "gesturelib.h"
#include "hold.h"
#include "hold_and_drag.h"
#include "repeat_tap.h"
#include "recognizer.h"
#include "rotate.h"
#include "tap.h"
#include "zoom.h"

#include <emscripten.h>


#define CAT_NOEXPAND(A, B) A##B
#define CAT(A, B)          CAT_NOEXPAND(A, B)

#define TYPE_POINTER CAT(TYPE_NAME, _t)*

#define MAX_OUT_EVENTS 50

#define X_RECOGNIZERS                                                                                                  \
    X(tap, TAP)                                                                                                        \
    X(hold, HOLD)                                                                                                      \
    X(hold_and_drag, HOLD_AND_DRAG)                                                                                    \
    X(double_tap, DOUBLE_TAP)                                                                                          \
    X(drag, DRAG)                                                                                                      \
    X(zoom, ZOOM)                                                                                                      \
    X(rotate, ROTATE)                                                                                                  \
    X(ktap, KTAP)

typedef union out_data_structs {
#define X(type, ...) CAT(type, _t) type;
    X_RECOGNIZERS
#undef X
} out_data_t;

typedef enum gesture_type {
    GESTURE_TYPE_TAP,
    GESTURE_TYPE_HOLD,
    GESTURE_TYPE_HOLD_AND_DRAG,
    GESTURE_TYPE_DOUBLE_TAP,
    GESTURE_TYPE_DRAG,
    GESTURE_TYPE_ZOOM,
    GESTURE_TYPE_ROTATE,
    GESTURE_TYPE_KTAP
} out_type_t;

extern out_data_t out_data[];
extern out_type_t out_types[];
extern int n_out_data;
