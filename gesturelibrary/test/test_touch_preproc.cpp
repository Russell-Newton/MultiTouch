#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

extern "C" {
#include "gesturelib.h"
#include "recognizer.h"
#include "singleFingerDrag.h"
}

using namespace std;

class TestPreprocessing : public testing::Test {
protected:
    void SetUp() override {
        init_gesturelib();
    }
};

// Checks that a sequence of single finger downs/ups always assigns to group 1
TEST_F(TestPreprocessing, TestStationaryDownUp) {
    touch_event_t down = {TOUCH_EVENT_DOWN, 0, 0, 0, TOUCH_ID_UNDEFINED};
    touch_event_t up   = {TOUCH_EVENT_UP, 0, 0, 0, TOUCH_ID_UNDEFINED};
    for (int i = 0; i < 5; i++) {
        down.timestamp = i * 2;
        up.timestamp   = i * 2 + 1;
        EXPECT_EQ(assign_group(&down), 0);
        EXPECT_EQ(assign_group(&up), 0);
    }
}

// Checks that downs always select the first untracked group, TOUCH_ID_UNDEFINED on no untracked groups
TEST_F(TestPreprocessing, TestStationaryDownMultiple) {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        touch_event_t newTouch = {TOUCH_EVENT_DOWN, (float)i, 0, 0, TOUCH_ID_UNDEFINED};
        EXPECT_EQ(assign_group(&newTouch), i);
    }
    touch_event_t newTouch = {TOUCH_EVENT_DOWN, (float)MAX_TOUCHES, 0, (float)MAX_TOUCHES, TOUCH_ID_UNDEFINED};
    EXPECT_EQ(assign_group(&newTouch), TOUCH_ID_UNDEFINED);
}

// Checks that moves and ups aren't assigned to any group if none are being tracked
TEST_F(TestPreprocessing, TestMoveUpUndefined) {
    EXPECT_EQ(assign_group(new touch_event_t({TOUCH_EVENT_MOVE, 0, 0, 0, TOUCH_ID_UNDEFINED})), TOUCH_ID_UNDEFINED);
    EXPECT_EQ(assign_group(new touch_event_t({TOUCH_EVENT_UP, 0, 0, 0, TOUCH_ID_UNDEFINED})), TOUCH_ID_UNDEFINED);
}

// Checks that moving sequences of downs/ups assign to the correct group
TEST_F(TestPreprocessing, TestDownUpMultiple) {
    touch_event_t downTouches[MAX_TOUCHES];
    touch_event_t upTouches[MAX_TOUCHES];
    for (int i = 0; i < MAX_TOUCHES; i++) {
        downTouches[i] = {TOUCH_EVENT_DOWN, (float)i, 0, 0, TOUCH_ID_UNDEFINED};
        upTouches[i]   = {TOUCH_EVENT_UP, (float)i, 0, 0, TOUCH_ID_UNDEFINED};
        EXPECT_EQ(assign_group(&downTouches[i]), i);
    }

    for (int round = 0; round < 5; round++) {
        for (int i = 0; i < MAX_TOUCHES; i++) {
            upTouches[i].position_y += 0.1f;
            EXPECT_EQ(assign_group(&upTouches[i]), i);
            EXPECT_EQ(assign_group(&downTouches[i]), i);
            downTouches[i].position_y += 0.1f;
        }
    }
}

// Checks that a single swipe (down, multiple moves, up) stays in the same group
TEST_F(TestPreprocessing, TestSwipeSingle) {
    EXPECT_EQ(assign_group(new touch_event_t({TOUCH_EVENT_DOWN, 0, 0, 0, TOUCH_ID_UNDEFINED})), 0);
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(assign_group(new touch_event_t({TOUCH_EVENT_MOVE, i * 0.1f, 0, 0, TOUCH_ID_UNDEFINED})), 0);
    }
    EXPECT_EQ(assign_group(new touch_event_t({TOUCH_EVENT_UP, 0, 0, 0, TOUCH_ID_UNDEFINED})), 0);
}

// Checks that multiple swipes stay within their respective groups
TEST_F(TestPreprocessing, TestSwipeMultiple) {
    int steps = 10;
    for (int touch = 0; touch < MAX_TOUCHES; touch++) {
        EXPECT_EQ(assign_group(new touch_event_t({TOUCH_EVENT_DOWN, 0, (float)touch, 0, TOUCH_ID_UNDEFINED})), touch);
    }
    for (int i = 0; i < steps; i++) {
        for (int touch = 0; touch < MAX_TOUCHES; touch++) {
            EXPECT_EQ(
                assign_group(new touch_event_t({TOUCH_EVENT_MOVE, i * 0.1f, (float)touch, 0, TOUCH_ID_UNDEFINED})),
                touch);
        }
    }
    for (int touch = 0; touch < MAX_TOUCHES; touch++) {
        EXPECT_EQ(assign_group(new touch_event_t({TOUCH_EVENT_UP, steps * 0.1f, (float)touch, 0, TOUCH_ID_UNDEFINED})),
                  touch);
    }
}

// Checks that multiple swipes with breaks of up/down stay within their respective groups
TEST_F(TestPreprocessing, TestSwipeMultipleWithUp) {
    int steps = 20;
    int ups[MAX_TOUCHES];
    for (int i = 0; i < MAX_TOUCHES; i++) {
        ups[i] = (2 * i + 4) % MAX_TOUCHES;
    }
    for (int touch = 0; touch < MAX_TOUCHES; touch++) {
        EXPECT_EQ(assign_group(new touch_event_t({TOUCH_EVENT_DOWN, 0, (float)touch, 0, TOUCH_ID_UNDEFINED})), touch);
    }
    for (int i = 0; i < steps; i++) {
        for (int touch = 0; touch < MAX_TOUCHES; touch++) {
            if (i == ups[touch]) {
                EXPECT_EQ(
                    assign_group(new touch_event_t({TOUCH_EVENT_UP, i * 0.1f, (float)touch, 0, TOUCH_ID_UNDEFINED})),
                    touch);
                EXPECT_EQ(
                    assign_group(new touch_event_t({TOUCH_EVENT_DOWN, i * 0.1f, (float)touch, 0, TOUCH_ID_UNDEFINED})),
                    touch);
            } else {
                EXPECT_EQ(
                    assign_group(new touch_event_t({TOUCH_EVENT_MOVE, i * 0.1f, (float)touch, 0, TOUCH_ID_UNDEFINED})),
                    touch);
            }
        }
    }
    for (int touch = 0; touch < MAX_TOUCHES; touch++) {
        EXPECT_EQ(assign_group(new touch_event_t({TOUCH_EVENT_UP, steps * 0.1f, (float)touch, 0, TOUCH_ID_UNDEFINED})),
                  touch);
    }
}
