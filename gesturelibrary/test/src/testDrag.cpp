#include "testFlutter.hpp"

#include <cmath>

extern "C" {
#include "drag.h"
#include "recognizer.h"
#include "utils.h"
}

using namespace std;

struct DragTestParams {
    string path;
    size_t num;

    DragTestParams(const string& path, size_t num) {
        this->path = path;
        this->num  = num;
    }
};

ostream& operator<<(ostream& stream, const DragTestParams& params) {
    stream << params.path;
    return stream;
}

class TestDrag : public TestFlutter, public testing::WithParamInterface<DragTestParams> {
protected:
    void testStates(size_t num) {
        int completed   = 0;
        state_t* states = new state_t[MAX_TOUCHES];
        for (size_t index = 0; index < MAX_TOUCHES; index++) {
            states[index] = RECOGNIZER_STATE_NULL;
        }
        for (touch_event_t event : touchEvents) {
            process_touch_event(&event);
            drag_t* drags = get_drag();
            for (size_t index = 0; index < MAX_TOUCHES; index++) {
                // if (drags[index].state != RECOGNIZER_STATE_NULL) {
                //     float dx = drags[index].x - drags[index].x0;
                //     float dy = drags[index].y - drags[index].y0;
                //     cout << "dist_" << index << ": " << sqrtf(SQUARE_SUM(dx, dy)) << ",\t"
                //          << "angle_" << index << ": " << atan2f(dy, dx) << ",\t"
                //          << "state_" << index << ": " << drags[index].state << endl;
                // }
                switch (states[index]) {
                case RECOGNIZER_STATE_NULL:
                    EXPECT_TRUE(drags[index].state == RECOGNIZER_STATE_NULL ||
                                drags[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                    break;
                case RECOGNIZER_STATE_IN_PROGRESS:
                    EXPECT_TRUE(drags[index].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                drags[index].state == RECOGNIZER_STATE_FAILED ||
                                drags[index].state == RECOGNIZER_STATE_COMPLETED);
                    if (drags[index].state == RECOGNIZER_STATE_COMPLETED) {
                        completed++;
                    }
                    break;
                case RECOGNIZER_STATE_COMPLETED:
                    EXPECT_TRUE(drags[index].state == RECOGNIZER_STATE_COMPLETED ||
                                drags[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                    break;
                case RECOGNIZER_STATE_FAILED:
                    EXPECT_TRUE(drags[index].state == RECOGNIZER_STATE_FAILED ||
                                drags[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                    break;
                default:
                    EXPECT_EQ(to_string(states[index]), "incorrect drag state found");
                    break;
                }
                states[index] = drags[index].state;
            }
        }
        EXPECT_EQ(completed, num);
        delete[] states;
    }
};

TEST_P(TestDrag, States) {
    readTouchEvents("res/" + GetParam().path + ".csv");
    testStates(GetParam().num);
}

INSTANTIATE_TEST_SUITE_P(DragTests,
                         TestDrag,
                         testing::Values(
                             // 1 finger straight drags
                             DragTestParams{"drag/phone_1", 1},
                             DragTestParams{"drag/phone_2", 1},
                             DragTestParams{"drag/phone_3", 1},
                             DragTestParams{"drag/phone_4", 1},
                             DragTestParams{"drag/phone_5", 1},
                             DragTestParams{"drag/phone_6", 1},
                             DragTestParams{"drag/phone_7", 1},
                             DragTestParams{"drag/phone_8", 1},
                             DragTestParams{"drag/phone_9", 1},
                             DragTestParams{"drag/phone_10", 1},

                             // 2 finger drags
                             DragTestParams{"drag/phone_11", 2},
                             DragTestParams{"drag/phone_12", 2},
                             DragTestParams{"drag/phone_13", 2},
                             DragTestParams{"drag/phone_14", 2},

                             // 1 finger curved drags
                             DragTestParams{"drag/phone_15", 1},
                             DragTestParams{"drag/phone_16", 1},
                             DragTestParams{"drag/phone_17", 1},
                             DragTestParams{"drag/phone_18", 1},
                             DragTestParams{"drag/phone_19", 1},
                             DragTestParams{"drag/phone_20", 1},
                             DragTestParams{"drag/phone_21", 1},
                             DragTestParams{"drag/phone_22", 1},
                             DragTestParams{"drag/phone_23", 1},
                             DragTestParams{"drag/phone_24", 1},
                             DragTestParams{"drag/phone_25", 1},
                             DragTestParams{"drag/phone_26", 1},

                             // 1 finger swipes
                             DragTestParams{"swipe/phone_1", 1},
                             DragTestParams{"swipe/phone_2", 1},
                             DragTestParams{"swipe/phone_3", 1},
                             DragTestParams{"swipe/phone_4", 1},
                             DragTestParams{"swipe/phone_5", 1},
                             DragTestParams{"swipe/phone_6", 1},
                             DragTestParams{"swipe/phone_7", 1},
                             DragTestParams{"swipe/phone_8", 1},
                             DragTestParams{"swipe/phone_9", 1},

                             // rotates
                             DragTestParams{"rotate/phone_1", 2},
                             DragTestParams{"rotate/phone_2", 2},

                             // zooms
                             DragTestParams{"zoom/phone_1", 2},
                             DragTestParams{"zoom/phone_2", 2},
                             DragTestParams{"zoom/phone_3", 2},
                             DragTestParams{"zoom/phone_4", 2},

                             // 1 taps
                             DragTestParams{"tap/phone_1", 0},
                             DragTestParams{"tap/phone_2", 0},
                             DragTestParams{"tap/phone_3", 0},
                             DragTestParams{"tap/phone_4", 0},
                             DragTestParams{"tap/phone_5", 0},
                             DragTestParams{"tap/phone_6", 0},
                             DragTestParams{"tap/phone_7", 0},
                             DragTestParams{"tap/phone_8", 0},
                             DragTestParams{"tap/phone_9", 0},
                             DragTestParams{"tap/phone_10", 0}));
