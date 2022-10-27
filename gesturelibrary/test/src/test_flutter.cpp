#include "testFlutter.hpp"

extern "C" {
#include "recognizer.h"
#include "singleFingerDrag.h"
#include "singleFingerTap.h"
}

TEST_F(TestFlutter, TapPhone1) {
    readTouchEvents("res/tap/phone_1.csv");
    for (touch_event_t event : touchEvents) {
        process_touch_event(&event, nullptr, 0);
    }
}

class TestTap : public TestFlutter, public testing::WithParamInterface<int> {
protected:
    void testTap1() {
        state_t s = RECOGNIZER_STATE_NULL;
        for (touch_event_t event : touchEvents) {
            bool tap_found            = false;
            gesture_event_t* gestures = new gesture_event_t[MAX_RECOGNIZERS];
            process_touch_event(&event, gestures, MAX_RECOGNIZERS);
            for (size_t i = 0; i < MAX_RECOGNIZERS; i++) {
                if (gestures[i].type == GESTURE_TYPE_TAP && gestures[i].num_touches == 1) {
                    sFingerTap_t* taps = ((sFingerTap_t * (*)(void)) gestures[i].get_data)();
                    switch (s) {
                    case RECOGNIZER_STATE_NULL:
                        EXPECT_TRUE(s == taps[0].state);
                        s = RECOGNIZER_STATE_POSSIBLE;
                        break;
                    case RECOGNIZER_STATE_POSSIBLE:
                        EXPECT_TRUE(s == taps[0].state || taps[0].state == RECOGNIZER_STATE_COMPLETED);
                        s = taps[0].state;
                        break;
                    default:
                        EXPECT_EQ("", "incorrect tap state found");
                        break;
                    }
                    tap_found = true;
                    break;
                }
            }
            if (!tap_found) {
                EXPECT_EQ("", "failed to return tap gesture");
            }
            delete[] gestures;
        }
        if (!(s == RECOGNIZER_STATE_COMPLETED)) {
            EXPECT_EQ("", "failed to return complete tap gesture");
        }
    }
};

TEST_P(TestTap, TapPhone) {
    readTouchEvents("res/tap/phone_" + to_string(GetParam()) + ".csv");
    testTap1();
}

INSTANTIATE_TEST_SUITE_P(TapPhoneTests, TestTap, testing::Values(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
