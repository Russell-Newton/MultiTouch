#pragma once

#include "math.h"

// This is the maximum number of simultaneous touches supported.
// Processing more simultaneous gestures requires more memory and execution time.
#define MAX_TOUCHES 5

// This is the maximum number of gesture recognizers supported.
// Each recognizer may recognize multiple instances of the same gesture simultaneously.
#define MAX_RECOGNIZERS 100

// Touch events passed into the library should set their group field to this unless
// the user wants to specify a particular group. The touch event struct is defined in
// gesturelib.h
#define TOUCH_ID_UNDEFINED (MAX_TOUCHES + 1)

// When using the library to assign touch event groups, two touch events more than this
// distance apart will never be assigned the same group.
#define EVENT_GROUPING_DIST_MAX 150

// Time in seconds for longest tap and shortest hold.
#define TAP_TIME_MAX  0.5f
#define HOLD_TIME_MIN TAP_TIME_MAX

// Shortest possible time difference and shortest euclidean distance for double taps.
#define DOUBLE_DIFF     5
#define DOUBLE_DISTANCE 0.2

// Euclidean distance for longest tap/hold and shortest drag.
#define TAP_DIST_MAX  10
#define HOLD_DIST_MAX TAP_DIST_MAX
#define DRAG_DIST_MIN TAP_DIST_MAX

// Minimum scale factor difference from 1 to be a zoom.
#define ZOOM_SCALE_MIN 0.15f

// Minimum rotation angle in radians to be a rotate.
#define ROTATE_ANGLE_MIN (M_PI / 12)
