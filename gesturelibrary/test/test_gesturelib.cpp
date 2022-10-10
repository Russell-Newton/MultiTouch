#include <gtest/gtest.h>

extern "C" {
#include "singleFingerDouble.h"
#include "singleFingerHold.h"
#include "singleFingerSwipe.h"
#include "singleFingerTap.h"
}

TEST(GestureLibraryTest, TestGestureLib) {
    init_gesturelib();

    touch_event_t event;
    event.position_x = 100;
    event.position_y = 200;
    event.timestamp  = 42;

    gesture_event_t gestures[1];

    EXPECT_EQ(process_touch_event(&event, gestures, 4), 0);
}

TEST(GestureLibraryGetDataTest, TestGestureLibGetters) {
    init_gesturelib();

    touch_event_t event;
    event.position_x = 100;
    event.position_y = 200;
    event.timestamp  = 42;

    gesture_event_t gestures[1];

    ASSERT_TRUE(get_sFingerTap() != NULL);
    ASSERT_TRUE(get_sFingerSwipe() != NULL);
    ASSERT_TRUE(get_sFingerDTap() != NULL);
}

TEST(GestureLibraryHoldTest, TestGestureHoldTester) {
    init_gesturelib();

    touch_event_t event1;
    event1.position_x = 100;
    event1.position_y = 200;
    event1.timestamp  = 42;

    touch_event_t event2;
    event2.position_x = 100;
    event2.position_y = 200;
    event2.timestamp  = 47;

    gesture_event_t gestures[1];

    process_touch_event(&event1, gestures, 4);
    process_touch_event(&event2, gestures, 4);

    EXPECT_EQ(get_sFingerHold()[0].state, possible);
}
