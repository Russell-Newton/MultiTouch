#pragma once

#include "math.h"

#define EVENT_GROUPING_DIST_MAX 150

// time in seconds for longest tap and shortest hold
#define TAP_TIME_MAX  0.5f
#define HOLD_TIME_MIN TAP_TIME_MAX

// shortest possible time diff and shortest euclidean distance
#define DOUBLE_DIFF     5
#define DOUBLE_DISTANCE 0.2

// euclidean distance for longest tap/hold and shortest drag/swipe
#define TAP_DIST_MAX   10
#define HOLD_DIST_MAX  TAP_DIST_MAX
#define DRAG_DIST_MIN  TAP_DIST_MAX
#define SWIPE_DIST_MIN TAP_DIST_MAX

// max angle in radians before a gesture is considered not straight
#define SWIPE_ANGLE_MAX (M_PI / 6)
#define DRAG_ANGLE_MIN  SWIPE_ANGLE_MAX

// euclidean distance for largest distance between touches for
// one zoom & rotate gesture
#define ZOOM_AND_ROTATE_SEPARATION_MAX 500
