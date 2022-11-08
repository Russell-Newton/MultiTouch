#include "testFlutter.hpp"

#include <cmath>

extern "C" {
#include "hold_and_drag.h"
#include "recognizer.h"
#include "utils.h"
}

using namespace std;

struct HoldAndDragTestParams {
    string path;
    size_t num;

    HoldAndDragTestParams(const string& path, size_t num) {
        this->path = path;
        this->num  = num;
    }
};

ostream& operator<<(ostream& stream, const HoldAndDragTestParams& params) {
    stream << params.path;
    return stream;
}

class TestHoldAndDrag : public TestFlutter, public testing::WithParamInterface<HoldAndDragTestParams> {
protected:
    void testStates(size_t num) {
        int completed   = 0;
        state_t* states = new state_t[MAX_TOUCHES];
        for (size_t index = 0; index < MAX_TOUCHES; index++) {
            states[index] = RECOGNIZER_STATE_NULL;
        }
        for (touch_event_t event : touchEvents) {
            gesture_event_t* gestures = new gesture_event_t[MAX_RECOGNIZERS];
            process_touch_event(&event, gestures, MAX_RECOGNIZERS);
            for (size_t i = 0; i < MAX_RECOGNIZERS; i++) {
                if (gestures[i].type == GESTURE_TYPE_HOLD_AND_DRAG && gestures[i].num_touches == 1) {
                    hold_and_drag_t* holdanddrags = ((hold_and_drag_t * (*)(void)) gestures[i].get_data)();
                    for (size_t index = 0; index < MAX_TOUCHES; index++) {
                        switch (states[index]) {
                        case RECOGNIZER_STATE_NULL:
                            EXPECT_TRUE(holdanddrags[index].state == RECOGNIZER_STATE_NULL ||
                                        holdanddrags[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                            break;
                        case RECOGNIZER_STATE_IN_PROGRESS:
                            EXPECT_TRUE(holdanddrags[index].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                        holdanddrags[index].state == RECOGNIZER_STATE_FAILED ||
                                        holdanddrags[index].state == RECOGNIZER_STATE_COMPLETED);
                            if (holdanddrags[index].state == RECOGNIZER_STATE_COMPLETED) {
                                completed++;
                            }
                            break;
                        case RECOGNIZER_STATE_COMPLETED:
                            EXPECT_TRUE(holdanddrags[index].state == RECOGNIZER_STATE_COMPLETED ||
                                        holdanddrags[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                            break;
                        case RECOGNIZER_STATE_FAILED:
                            EXPECT_TRUE(holdanddrags[index].state == RECOGNIZER_STATE_FAILED ||
                                        holdanddrags[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                            break;
                        default:
                            EXPECT_EQ(to_string(states[index]), "incorrect holdanddrag state found");
                            break;
                        }
                        states[index] = holdanddrags[index].state;
                    }
                    break;
                }
            }
            delete[] gestures;
        }
        EXPECT_EQ(completed, num);
        delete[] states;
    }
};

TEST_P(TestHoldAndDrag, States) {
    readTouchEvents("res/" + GetParam().path + ".csv");
    testStates(GetParam().num);
}

INSTANTIATE_TEST_SUITE_P(HoldAndDragTestParams,
                         TestHoldAndDrag,
                         testing::Values(
                             // 1 finger straight holdanddrags
                             HoldAndDragTestParams{"holdanddrag/phone_1", 1},
                             HoldAndDragTestParams{"holdanddrag/phone_2", 1},
                             HoldAndDragTestParams{"holdanddrag/phone_3", 1},
                             HoldAndDragTestParams{"holdanddrag/phone_4", 1},
                             HoldAndDragTestParams{"holdanddrag/phone_5", 1}));
