#include "testFlutter.hpp"

extern "C" {
#include "recognizer.h"
#include "singleFingerHold.h"
}

class TestHold : public TestFlutter, public testing::WithParamInterface<int> {
protected:
    void testHold() {
        state_t s = RECOGNIZER_STATE_NULL;
        for (touch_event_t event : touchEvents) {
            gesture_event_t* gestures = new gesture_event_t[MAX_RECOGNIZERS];
            bool found_hold           = false;
            process_touch_event(&event, gestures, MAX_RECOGNIZERS);
            for (size_t i = 0; i < MAX_RECOGNIZERS; i++) {  // size_t means unsigned int (positive)
                if (gestures[i].type == GESTURE_TYPE_HOLD && gestures[i].num_touches == 1) {
                    sFingerHold_t* holds = ((sFingerHold_t * (*)(void)) gestures[i].get_data)();
                    bool found           = false;
                    switch (s) {
                    case RECOGNIZER_STATE_NULL:
                        for (size_t j = 0; j < MAX_TOUCHES; j++) {
                            if (holds[j].state == RECOGNIZER_STATE_POSSIBLE) {
                                s     = holds[j].state;
                                found = true;
                                break;
                            }
                        }
                        // EXPECT_TRUE(found);
                        break;
                    case RECOGNIZER_STATE_POSSIBLE:
                        for (size_t j = 0; j < MAX_TOUCHES; j++) {
                            if (holds[j].state == RECOGNIZER_STATE_POSSIBLE ||
                                holds[j].state == RECOGNIZER_STATE_IN_PROGRESS) {
                                s     = holds[j].state;
                                found = true;
                                break;
                            }
                        }
                        // EXPECT_TRUE(found);
                        break;
                    case RECOGNIZER_STATE_IN_PROGRESS:
                        for (size_t j = 0; j < MAX_TOUCHES; j++) {
                            if (holds[j].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                holds[j].state == RECOGNIZER_STATE_COMPLETED) {
                                s     = holds[j].state;
                                found = true;
                                break;
                            }
                        }
                        // EXPECT_TRUE(found);
                        break;
                    default:
                        // EXPECT_EQ("", "incorrect hold state found");
                        break;
                    }
                    found_hold = true;
                    break;
                }
            }
            if (!found_hold) {
                // EXPECT_EQ("", "failed to return hold gesture");
            }
            delete[] gestures;
        }
    }
};

TEST_P(TestHold, HoldPhone) {
    readTouchEvents("res/hold/phone_" + to_string(GetParam()) + ".csv");
    testHold();
}

INSTANTIATE_TEST_SUITE_P(HoldPhoneTests, TestHold, testing::Values(1));