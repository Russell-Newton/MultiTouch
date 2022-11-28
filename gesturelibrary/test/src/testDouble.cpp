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
    size_t groups;

    DoubleTapTestParams(const string& path, size_t num, size_t groups) {
        this->path   = path;
        this->num    = num;
        this->groups = groups;
    }
};

ostream& operator<<(ostream& stream, const DoubleTapTestParams& params) {
    stream << params.path;
    return stream;
}

class TestDouble : public TestFlutter, public testing::WithParamInterface<DoubleTapTestParams> {
protected:
    void testDouble(size_t num, size_t groups) {
        (void)groups;
        int completed   = 0;
        state_t* states = new state_t[MAX_TOUCHES];
        for (size_t index = 0; index < MAX_TOUCHES; index++) {
            states[index] = RECOGNIZER_STATE_NULL;
        }
        const double_tap_t* taps;

        for (touch_event_t event : touchEvents) {
            process_touch_event(&event);
            taps = get_double_tap();

            for (size_t index = 0; index < MAX_TOUCHES; index++) {
                switch (states[index]) {
                case RECOGNIZER_STATE_NULL:
                    EXPECT_TRUE(taps[index].state == RECOGNIZER_STATE_NULL ||
                                taps[index].state == RECOGNIZER_STATE_POSSIBLE);
                    break;

                case RECOGNIZER_STATE_POSSIBLE:
                    EXPECT_TRUE(taps[index].state == RECOGNIZER_STATE_POSSIBLE ||
                                taps[index].state == RECOGNIZER_STATE_FAILED ||
                                taps[index].state == RECOGNIZER_STATE_COMPLETED);
                    break;

                case RECOGNIZER_STATE_COMPLETED:
                    EXPECT_TRUE(taps[index].state == RECOGNIZER_STATE_COMPLETED ||
                                taps[index].state == RECOGNIZER_STATE_POSSIBLE);
                    break;

                case RECOGNIZER_STATE_FAILED:
                    EXPECT_TRUE(taps[index].state == RECOGNIZER_STATE_FAILED ||
                                taps[index].state == RECOGNIZER_STATE_POSSIBLE);
                    break;

                default:
                    EXPECT_EQ(to_string(states[index]), "incorrect double tap state found");
                    break;
                }

                states[index] = taps[index].state;
            }
        }

        for (int i = 0; i < MAX_TOUCHES; i++) {
            if (taps[i].state == RECOGNIZER_STATE_COMPLETED) {
                completed++;
            }
        }

        EXPECT_EQ(completed, num);

        delete[] states;
    }
};

TEST_P(TestDouble, States) {
    readTouchEvents("res/" + GetParam().path + ".csv");
    testDouble(GetParam().num, GetParam().groups);
}

INSTANTIATE_TEST_SUITE_P(DoubleTapTests,
                         TestDouble,
                         testing::Values(DoubleTapTestParams{"double/phone_1", 1, 5},
                                         DoubleTapTestParams{"double/phone_2", 1, 5},
                                         DoubleTapTestParams{"double/phone_3", 1, 5},
                                         /* Check this data */
                                         DoubleTapTestParams{"double/phone_4", 2, 4},
                                         DoubleTapTestParams{"double/phone_5", 2, 4},
                                         DoubleTapTestParams{"double/phone_6", 2, 4},
                                         DoubleTapTestParams{"double/phone_7", 3, 3},
                                         /* */
                                         DoubleTapTestParams{"double/phone_8", 1, 5},
                                         DoubleTapTestParams{"double/phone_9", 1, 5},
                                         DoubleTapTestParams{"double/fail_case", 0, 5},
                                         DoubleTapTestParams{"double/fail_case_2", 0, 5},
                                         DoubleTapTestParams{"double/phone_10", 1, 4}));
