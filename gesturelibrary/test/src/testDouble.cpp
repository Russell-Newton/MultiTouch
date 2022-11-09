#include "testFlutter.hpp"

#include <cmath>

extern "C" {
#include "double.h"
#include "recognizer.h"
#include "utils.h"
}

using namespace std;

struct DoubleTapTestParams {
    string path;
    size_t num;

    DoubleTapTestParams(const string& path, size_t num) {
        this->path = path;
        this->num  = num;
    }
};

ostream& operator<<(ostream& stream, const DoubleTapTestParams& params) {
    stream << params.path;
    return stream;
}

class TestDouble : public TestFlutter, public testing::WithParamInterface<DoubleTapTestParams> {
protected:
    void testDouble(size_t num) {
        int completed   = 0;
        state_t* states = new state_t[MAX_TOUCHES];
        for (size_t index = 0; index < MAX_TOUCHES; index++) {
            states[index] = RECOGNIZER_STATE_NULL;
        }
        for (touch_event_t event : touchEvents) {
            gesture_event_t* gestures = new gesture_event_t[MAX_RECOGNIZERS];
            process_touch_event(&event, gestures, MAX_RECOGNIZERS);
            for (size_t i = 0; i < MAX_RECOGNIZERS; i++) {
                if (gestures[i].type == GESTURE_TYPE_DOUBLE_TAP) {
                    double_tap_t* taps = ((double_tap_t * (*)(void)) gestures[i].get_data)();
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
                            EXPECT_EQ(to_string(states[index]), "incorrect double tap state found");
                            break;
                        }
                        states[index] = taps[index].state;
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

TEST_P(TestDouble, States) {
    readTouchEvents("res/" + GetParam().path + ".csv");
    testDouble(GetParam().num);
}

INSTANTIATE_TEST_SUITE_P(DoubleTapTests,
                         TestDouble,
                         testing::Values(DoubleTapTestParams{"double/phone_1", 1},
                                         DoubleTapTestParams{"double/phone_2", 1},
                                         DoubleTapTestParams{"double/phone_3", 1},
                                         /* Check this data
                                         DoubleTapTestParams{"double/phone_4", 1},
                                         DoubleTapTestParams{"double/phone_5", 1},
                                         DoubleTapTestParams{"double/phone_6", 1},
                                         DoubleTapTestParams{"double/phone_7", 1},
                                         */
                                         DoubleTapTestParams{"double/phone_8", 1},
                                         DoubleTapTestParams{"double/phone_9", 1}));
