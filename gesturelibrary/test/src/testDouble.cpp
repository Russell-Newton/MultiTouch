#include "testFlutter.hpp"

extern "C" {
#include "double.h"
#include "recognizer.h"
}

class TestDouble : public TestFlutter, public testing::WithParamInterface<int> {
protected:
    void testDouble() {
        state_t s = RECOGNIZER_STATE_NULL;
        for (touch_event_t event : touchEvents) {
            gesture_event_t* gestures = new gesture_event_t[MAX_RECOGNIZERS];
            bool found_hold           = false;
            process_touch_event(&event, gestures, MAX_RECOGNIZERS);
            for (size_t i = 0; i < MAX_RECOGNIZERS; i++) {  // size_t means unsigned int (positive)
                if (gestures[i].type == GESTURE_TYPE_DOUBLE_TAP && gestures[i].num_touches == 1) {
                    double_tap_t* doubles = ((double_tap_t * (*)(void)) gestures[i].get_data)();
                    bool found            = false;
                    switch (s) {
                    case RECOGNIZER_STATE_NULL:
                        for (size_t j = 0; j < MAX_TOUCHES; j++) {
                            if (doubles[j].state == RECOGNIZER_STATE_POSSIBLE) {
                                s     = doubles[j].state;
                                found = true;
                                // EXPECT_FALSE(found); //checking if it enters this if, this should make it auto fail
                                break;
                            }
                        }
                        EXPECT_TRUE(found);
                        break;
                    case RECOGNIZER_STATE_POSSIBLE:
                        for (size_t j = 0; j < MAX_TOUCHES; j++) {
                            if (doubles[j].state == RECOGNIZER_STATE_POSSIBLE ||
                                doubles[j].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                doubles[j].state == RECOGNIZER_STATE_COMPLETED) {
                                s     = doubles[j].state;
                                found = true;
                                break;
                            }
                        }
                        EXPECT_TRUE(found);
                        break;
                    case RECOGNIZER_STATE_IN_PROGRESS:
                        for (size_t j = 0; j < MAX_TOUCHES; j++) {
                            if (doubles[j].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                doubles[j].state == RECOGNIZER_STATE_COMPLETED) {
                                s     = doubles[j].state;
                                found = true;
                                break;
                            }
                        }
                        EXPECT_TRUE(found);
                        break;
                    default:
                        EXPECT_EQ("", "incorrect double state found");
                        break;
                    }
                    found_hold = true;
                    break;
                }
            }
            if (!found_hold) {
                EXPECT_EQ("", "failed to return double gesture");
            }
            delete[] gestures;
        }
    }
};

TEST_P(TestDouble, DoubleTapPhone) {
    readTouchEvents("res/double/phone_" + to_string(GetParam()) + ".csv");
    testDouble();
}

INSTANTIATE_TEST_SUITE_P(DoubleTapTests, TestDouble, testing::Values(1, 2, 3, 4, 5, 6, 7, 8, 9));
