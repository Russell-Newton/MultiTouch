#pragma once

// This is the maximum number of simultaneous touches supported.
// Processing more simultaneous gestures requires more memory and execution time.
#define MAX_TOUCHES 5

// This is the maximum number of gesture recognizers supported.
// Each recognizer may recognize multiple instances of the same gesture simultaneously.
#define MAX_RECOGNIZERS 100

// Touch events passed into the library should set their group field to this unless
// the user wants to specify a particular group. The touch event struct is defined in
// gesturelib.h
#define TOUCH_GROUP_UNDEFINED (MAX_TOUCHES + 1)

// When using the library to assign touch event groups, two touch events more than this
// distance apart will never be assigned the same group. Must call init_gesturelib after modifying this variable.
extern float EVENT_GROUPING_DIST_MAX;

// Time in seconds for longest tap and shortest hold. Must call init_gesturelib after modifying any of these variables.
extern float TAP_TIME_MAX;
#define HOLD_TIME_MIN TAP_TIME_MAX

// Shortest possible time difference and shortest euclidean distance for double taps. Must call init_gesturelib after
// modifying any of these variables.
extern float DOUBLE_GROUP_TIME_DIFF;
extern float DOUBLE_GROUP_DIST_DIFF;
extern float DOUBLE_DIST;
extern float DOUBLE_TIME_DIFF;

// Euclidean distance for longest tap/hold and shortest drag. Must call init_gesturelib after modifying any of these
// variables.
extern float TAP_DIST_MAX;
#define HOLD_DIST_MAX TAP_DIST_MAX
#define DRAG_DIST_MIN TAP_DIST_MAX

// Minimum scale factor difference from 1 to be a zoom. Must call init_gesturelib after modifying this variable.
extern float ZOOM_SCALE_MIN;

// Minimum rotation angle in radians to be a rotate. Must call init_gesturelib after modifying this variable.
extern float ROTATE_ANGLE_MIN;
