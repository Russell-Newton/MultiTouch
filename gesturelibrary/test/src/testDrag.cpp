#include "testFlutter.hpp"

extern "C" {
#include "recognizer.h"
#include "singleFingerDrag.h"
}

class TestDrag : public TestFlutter, public testing::WithParamInterface<int> {
protected:
    void testDrag1() {
        state_t s = RECOGNIZER_STATE_NULL;
        for (touch_event_t event : touchEvents) {
            bool drag_found           = false;
            gesture_event_t* gestures = new gesture_event_t[MAX_RECOGNIZERS];
            process_touch_event(&event, gestures, MAX_RECOGNIZERS);
            for (size_t i = 0; i < MAX_RECOGNIZERS; i++) {
                if (gestures[i].type == GESTURE_TYPE_DRAG && gestures[i].num_touches == 1) {
                    sFingerDrag_t* drags = ((sFingerDrag_t * (*)(void)) gestures[i].get_data)();
                    bool found           = false;
                    switch (s) {
                    case RECOGNIZER_STATE_NULL:
                        for (size_t j = 0; j < MAX_TOUCHES; j++) {
                            if (drags[j].state == RECOGNIZER_STATE_POSSIBLE) {
                                s     = drags[j].state;
                                found = true;
                                break;
                            }
                        }
                        EXPECT_TRUE(found);
                        break;
                    case RECOGNIZER_STATE_POSSIBLE:
                        for (size_t j = 0; j < MAX_TOUCHES; j++) {
                            if (drags[j].state == RECOGNIZER_STATE_POSSIBLE ||
                                drags[j].state == RECOGNIZER_STATE_IN_PROGRESS) {
                                s     = drags[j].state;
                                found = true;
                                break;
                            }
                        }
                        EXPECT_TRUE(found);
                        break;
                    case RECOGNIZER_STATE_IN_PROGRESS:
                        for (size_t j = 0; j < MAX_TOUCHES; j++) {
                            if (drags[j].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                drags[j].state == RECOGNIZER_STATE_COMPLETED) {
                                s     = drags[j].state;
                                found = true;
                                break;
                            }
                        }
                        EXPECT_TRUE(found);
                        break;
                    default:
                        EXPECT_EQ("", "incorrect drag state found");
                        break;
                    }
                    drag_found = true;
                    break;
                }
            }
            if (!drag_found) {
                EXPECT_EQ("", "failed to return drag gesture");
            }
            delete[] gestures;
        }
    }
};

TEST_P(TestDrag, DragPhone) {
    readTouchEvents("res/drag/phone_" + to_string(GetParam()) + ".csv");
    testDrag1();
}

INSTANTIATE_TEST_SUITE_P(DragPhoneTests, TestDrag, testing::Values(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
