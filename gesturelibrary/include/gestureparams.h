#pragma once

#define EVENT_GROUPING_DIST_MAX 150

// float time in seconds for longest tap and shortest hold/drag
#define TAP_TIME_MAX  1
#define HOLD_TIME_MIN 1.5
#define DRAG_TIME_MIN TAP_TIME_MAX

// float euclidean distance for longest tap/hold and shortest drag/swipe
#define TAP_DIST_MAX   100
#define HOLD_DIST_MAX  7
#define DRAG_DIST_MIN  TAP_DIST_MAX
#define SWIPE_DIST_MIN TAP_DIST_MAX
