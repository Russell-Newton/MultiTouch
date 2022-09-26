#include <gtest/gtest.h>

extern "C" {
#include "gesturelib.h"
}

TEST(GestureLibraryTest, TestGestureLib) {
    init_gesturelib();

    touch_event_t event;
    event.position_x = 100;
    event.position_y = 200;
    event.timestamp  = 42;

    gesture_event_t gestures[1];

    EXPECT_EQ(process_touch_event(&event, gestures, 1), 0);
}