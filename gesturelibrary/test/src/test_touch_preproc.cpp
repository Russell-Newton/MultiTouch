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
    static void DisableRecognizers() {
        for (int i = 0; i < num_recognizers; i++) {
            disable_recognizer(i);
        }
    }

    void SetUp() override {
        init_gesturelib();
        DisableRecognizers();
    }
};

// Checks that a sequence of single finger downs/ups always assigns to group 1
TEST_F(TestPreprocessing, TestStationaryDownUp) {
    touch_event_t down = {TOUCH_EVENT_DOWN, 0, 0, 0, TOUCH_ID_UNDEFINED};
    touch_event_t up   = {TOUCH_EVENT_UP, 0, 0, 0, TOUCH_ID_UNDEFINED};
    for (int i = 0; i < 5; i++) {
        down.t = i * 2;
        up.t   = i * 2 + 1;
        process_touch_event(&down, nullptr, 0);
        process_touch_event(&up, nullptr, 0);
        EXPECT_EQ(down.group, 0);
        EXPECT_EQ(up.group, 0);
    }
}

// Checks that downs always select the first untracked group, TOUCH_ID_UNDEFINED on no untracked groups
TEST_F(TestPreprocessing, TestStationaryDownMultiple) {
    for (int i = 0; i < MAX_TOUCHES; i++) {
        touch_event_t newTouch = {TOUCH_EVENT_DOWN, (float)i, 0, 0, TOUCH_ID_UNDEFINED};
        process_touch_event(&newTouch, nullptr, 0);
        EXPECT_EQ(newTouch.group, i);
    }
    touch_event_t newTouch = {TOUCH_EVENT_DOWN, (float)MAX_TOUCHES, 0, (float)MAX_TOUCHES, TOUCH_ID_UNDEFINED};
    process_touch_event(&newTouch, nullptr, 0);
    EXPECT_EQ(newTouch.group, TOUCH_ID_UNDEFINED);
}

// Checks that moves and ups aren't assigned to any group if none are being tracked
TEST_F(TestPreprocessing, TestMoveUpUndefined) {
    touch_event_t badEvent = {TOUCH_EVENT_MOVE, 0, 0, 0, TOUCH_ID_UNDEFINED};
    process_touch_event(&badEvent, nullptr, 0);
    EXPECT_EQ(badEvent.group, TOUCH_ID_UNDEFINED);

    badEvent.group = TOUCH_ID_UNDEFINED;
    badEvent.type  = TOUCH_EVENT_UP;
    process_touch_event(&badEvent, nullptr, 0);
    EXPECT_EQ(badEvent.group, TOUCH_ID_UNDEFINED);
}

// Checks that moving sequences of downs/ups assign to the correct group
TEST_F(TestPreprocessing, TestDownUpMultiple) {
    touch_event_t downTouches[MAX_TOUCHES];
    touch_event_t upTouches[MAX_TOUCHES];
    for (int i = 0; i < MAX_TOUCHES; i++) {
        downTouches[i] = {TOUCH_EVENT_DOWN, (float)i, 0, 0, TOUCH_ID_UNDEFINED};
        upTouches[i]   = {TOUCH_EVENT_UP, (float)i, 0, 0, TOUCH_ID_UNDEFINED};
        process_touch_event(&downTouches[i], nullptr, 0);
        EXPECT_EQ(downTouches[i].group, i);
    }

    for (int round = 0; round < 5; round++) {
        for (int i = 0; i < MAX_TOUCHES; i++) {
            upTouches[i].y += 0.1f;
            process_touch_event(&upTouches[i], nullptr, 0);
            EXPECT_EQ(upTouches[i].group, i);
            process_touch_event(&downTouches[i], nullptr, 0);
            EXPECT_EQ(downTouches[i].group, i);
            downTouches[i].y += 0.1f;
        }
    }
}

// Checks that a single swipe (down, multiple moves, up) stays in the same group
TEST_F(TestPreprocessing, TestSwipeSingle) {
    touch_event_t downEvent = {TOUCH_EVENT_DOWN, 0, 0, 0, TOUCH_ID_UNDEFINED};
    process_touch_event(&downEvent, nullptr, 0);
    EXPECT_EQ(downEvent.group, 0);
    for (int i = 0; i < 10; i++) {
        touch_event_t moveEvent = {TOUCH_EVENT_MOVE, i * 0.1f, 0, 0, TOUCH_ID_UNDEFINED};
        process_touch_event(&moveEvent, nullptr, 0);
        EXPECT_EQ(moveEvent.group, 0);
    }
    touch_event_t upEvent = {TOUCH_EVENT_UP, 0, 0, 0, TOUCH_ID_UNDEFINED};
    process_touch_event(&upEvent, nullptr, 0);
    EXPECT_EQ(upEvent.group, 0);
}

// Checks that multiple swipes stay within their respective groups
TEST_F(TestPreprocessing, TestSwipeMultiple) {
    int steps = 10;
    for (int touch = 0; touch < MAX_TOUCHES; touch++) {
        touch_event_t downEvent = {TOUCH_EVENT_DOWN, 0, (float)touch, 0, TOUCH_ID_UNDEFINED};
        process_touch_event(&downEvent, nullptr, 0);
        EXPECT_EQ(downEvent.group, touch);
    }
    for (int i = 0; i < steps; i++) {
        for (int touch = 0; touch < MAX_TOUCHES; touch++) {
            touch_event_t moveEvent = {TOUCH_EVENT_MOVE, i * 0.1f, (float)touch, 0, TOUCH_ID_UNDEFINED};
            process_touch_event(&moveEvent, nullptr, 0);
            EXPECT_EQ(moveEvent.group, touch);
        }
    }
    for (int touch = 0; touch < MAX_TOUCHES; touch++) {
        touch_event_t upEvent = {TOUCH_EVENT_UP, steps * 0.1f, (float)touch, 0, TOUCH_ID_UNDEFINED};
        process_touch_event(&upEvent, nullptr, 0);
        EXPECT_EQ(upEvent.group, touch);
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
        touch_event_t event = touch_event_t({TOUCH_EVENT_DOWN, 0, (float)touch, 0, TOUCH_ID_UNDEFINED});
        process_touch_event(&event, nullptr, 0);
        EXPECT_EQ(event.group, touch);
    }
    for (int i = 0; i < steps; i++) {
        for (int touch = 0; touch < MAX_TOUCHES; touch++) {
            if (i == ups[touch]) {
                touch_event_t upEvent = {TOUCH_EVENT_UP, i * 0.1f, (float)touch, 0, TOUCH_ID_UNDEFINED};
                process_touch_event(&upEvent, nullptr, 0);
                EXPECT_EQ(upEvent.group, touch);
                touch_event_t downEvent = {TOUCH_EVENT_DOWN, i * 0.1f, (float)touch, 0, TOUCH_ID_UNDEFINED};
                process_touch_event(&downEvent, nullptr, 0);
                EXPECT_EQ(downEvent.group, touch);
            } else {
                touch_event_t event = {TOUCH_EVENT_MOVE, i * 0.1f, (float)touch, 0, TOUCH_ID_UNDEFINED};
                process_touch_event(&event, nullptr, 0);
                EXPECT_EQ(event.group, touch);
            }
        }
    }
    for (int touch = 0; touch < MAX_TOUCHES; touch++) {
        touch_event_t event = {TOUCH_EVENT_UP, steps * 0.1f, (float)touch, 0, TOUCH_ID_UNDEFINED};
        process_touch_event(&event, nullptr, 0);
        EXPECT_EQ(event.group, touch);
    }
}
