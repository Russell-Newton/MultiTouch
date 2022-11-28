#pragma once

#include "math.h"

// This is the maximum number of simultaneous touches supported.
// Processing more simultaneous gestures requires more memory and execution time.
#define MAX_TOUCHES 5

// This is the maximum number of gesture recognizers supported.
// Each recognizer may recognize multiple instances of the same gesture simultaneously.
#define MAX_RECOGNIZERS 100  // do we wanna reduce this?

// Touch events passed into the library should set their group field to this unless
// the user wants to specify a particular group. The touch event struct is defined in
// gesturelib.h
#define TOUCH_GROUP_UNDEFINED (MAX_TOUCHES + 1)

// When using the library to assign touch event groups, two touch events more than this
// distance apart will never be assigned the same group.
#define EVENT_GROUPING_DIST_MAX 150

// Time in seconds for longest tap and shortest hold.
#define TAP_TIME_MAX  0.5f
#define HOLD_TIME_MIN TAP_TIME_MAX

// Shortest possible time difference and shortest euclidean distance for double taps.
#define DOUBLE_GROUP_TIME_DIFF 0.02  // in phone_6, the difference reaches a max of 0.015
#define DOUBLE_GROUP_DIST_DIFF                                                                                         \
    25000  // largest diff between any two points I saw was in phone_7 with a diff of 147**2 + 51**2
#define DOUBLE_MAX_X     16  // greatest x_diff I got from testing
#define DOUBLE_MAX_Y     19  // greatest y_diff I got from testing
#define DOUBLE_DIST      362  // using 19**2 + 0 + 1
#define DOUBLE_TIME_DIFF 0.5

// Euclidean distance for longest tap/hold and shortest drag.
#define TAP_DIST_MAX  10
#define HOLD_DIST_MAX TAP_DIST_MAX
#define DRAG_DIST_MIN TAP_DIST_MAX

// Minimum scale factor difference from 1 to be a zoom.
#define ZOOM_SCALE_MIN 0.15f

// Minimum rotation angle in radians to be a rotate.
#define ROTATE_ANGLE_MIN (M_PI / 12)
