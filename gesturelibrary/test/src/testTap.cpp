#include "testFlutter.hpp"

#include <cmath>

extern "C" {
#include "recognizer.h"
#include "tap.h"
#include "utils.h"
}

using namespace std;

struct TapTestParams {
    string path;
    size_t num;

    TapTestParams(const string& path, size_t num) {
        this->path = path;
        this->num  = num;
    }
};

ostream& operator<<(ostream& stream, const TapTestParams& params) {
    stream << params.path;
    return stream;
}

class TestTap : public TestFlutter, public testing::WithParamInterface<TapTestParams> {
protected:
    void testStates(size_t num) {
        int completed   = 0;
        state_t* states = new state_t[MAX_TOUCHES];
        for (size_t index = 0; index < MAX_TOUCHES; index++) {
            states[index] = RECOGNIZER_STATE_NULL;
        }
        for (touch_event_t event : touchEvents) {
            process_touch_event(&event);
            const tap_t* taps = get_tap();
            for (size_t index = 0; index < MAX_TOUCHES; index++) {
                switch (states[index]) {
                case RECOGNIZER_STATE_NULL:
                    EXPECT_TRUE(taps[index].state == RECOGNIZER_STATE_NULL ||
                                taps[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                    break;
                case RECOGNIZER_STATE_IN_PROGRESS:
                    EXPECT_TRUE(taps[index].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                taps[index].state == RECOGNIZER_STATE_FAILED ||
                                taps[index].state == RECOGNIZER_STATE_COMPLETED);
                    if (taps[index].state == RECOGNIZER_STATE_COMPLETED) {
                        completed++;
                    }
                    break;
                case RECOGNIZER_STATE_COMPLETED:
                    EXPECT_TRUE(taps[index].state == RECOGNIZER_STATE_COMPLETED ||
                                taps[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                    break;
                case RECOGNIZER_STATE_FAILED:
                    EXPECT_TRUE(taps[index].state == RECOGNIZER_STATE_FAILED ||
                                taps[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                    break;
                default:
                    EXPECT_EQ(to_string(states[index]), "incorrect tap state found");
                    break;
                }
                states[index] = taps[index].state;
            }
        }
        EXPECT_EQ(completed, num);
        delete[] states;
    }
};

TEST_P(TestTap, States) {
    readTouchEvents("res/" + GetParam().path + ".csv");
    testStates(GetParam().num);
}

INSTANTIATE_TEST_SUITE_P(TapTests,
                         TestTap,
                         testing::Values(
                             // 1 finger straight taps
                             TapTestParams{"tap/phone_1", 1},
                             TapTestParams{"tap/phone_2", 1},
                             TapTestParams{"tap/phone_3", 1},
                             TapTestParams{"tap/phone_4", 1},
                             TapTestParams{"tap/phone_5", 1},
                             TapTestParams{"tap/phone_6", 1},
                             TapTestParams{"tap/phone_7", 1},
                             TapTestParams{"tap/phone_8", 1},
                             TapTestParams{"tap/phone_9", 1},
                             TapTestParams{"tap/phone_10", 1},

                             // 1 finger swipes
                             TapTestParams{"swipe/phone_1", 0},
                             TapTestParams{"swipe/phone_2", 0},
                             TapTestParams{"swipe/phone_3", 0},
                             TapTestParams{"swipe/phone_4", 0},
                             TapTestParams{"swipe/phone_5", 0},
                             TapTestParams{"swipe/phone_6", 0},
                             TapTestParams{"swipe/phone_7", 0},
                             TapTestParams{"swipe/phone_8", 0},
                             TapTestParams{"swipe/phone_9", 0},

                             // drags
                             TapTestParams{"drag/phone_1", 0},
                             TapTestParams{"drag/phone_2", 0},
                             TapTestParams{"drag/phone_3", 0},
                             TapTestParams{"drag/phone_4", 0},
                             TapTestParams{"drag/phone_5", 0},
                             TapTestParams{"drag/phone_6", 0},
                             TapTestParams{"drag/phone_7", 0},
                             TapTestParams{"drag/phone_8", 0},
                             TapTestParams{"drag/phone_9", 0},
                             TapTestParams{"drag/phone_10", 0},
                             TapTestParams{"drag/phone_11", 0},
                             TapTestParams{"drag/phone_12", 0},
                             TapTestParams{"drag/phone_13", 0},
                             TapTestParams{"drag/phone_14", 0},
                             TapTestParams{"drag/phone_15", 0},
                             TapTestParams{"drag/phone_16", 0},
                             TapTestParams{"drag/phone_17", 0},
                             TapTestParams{"drag/phone_18", 0},
                             TapTestParams{"drag/phone_19", 0},
                             TapTestParams{"drag/phone_20", 0},
                             TapTestParams{"drag/phone_21", 0},
                             TapTestParams{"drag/phone_22", 0},
                             TapTestParams{"drag/phone_23", 0},
                             TapTestParams{"drag/phone_24", 0},
                             TapTestParams{"drag/phone_25", 0},
                             TapTestParams{"drag/phone_26", 0}));
