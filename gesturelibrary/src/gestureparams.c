#include "gestureparams.h"

#include "math.h"

float EVENT_GROUPING_DIST_MAX = 150;

float TAP_TIME_MAX = 0.5f;

float DOUBLE_GROUP_TIME_DIFF = 0.02;  // in phone_6, the difference reaches a max of 0.015
float DOUBLE_GROUP_DIST_DIFF =
    25000;  // largest diff between any two points I saw was in phone_7 with a diff of 147**2 + 51**2
float DOUBLE_DIST      = 362;  // using 19**2 + 0 + 1
float DOUBLE_TIME_DIFF = 0.5;

float TAP_DIST_MAX = 10;

float ZOOM_SCALE_MIN = 0.15f;

float ROTATE_ANGLE_MIN = M_PI / 12;
