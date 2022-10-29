#include "testFlutter.hpp"

#include <cmath>
#include <iostream>

extern "C" {
#include "multistroke.h"
#include "recognizer.h"
}

using namespace std;

struct MultistrokeTestParams {
    string fileName;
    int num;

    MultistrokeTestParams(const string& fileName, int num) {
        this->fileName = fileName;
        this->num      = num;
    }
};

ostream& operator<<(ostream& stream, const MultistrokeTestParams& params) {
    stream << params.fileName;
    return stream;
}

class TestMultistroke : public TestFlutter, public testing::WithParamInterface<MultistrokeTestParams> {
protected:
    void testStates(int num) {
        int completed   = 0;
        state_t* states = new state_t[MAX_TOUCHES];
        for (size_t index = 0; index < MAX_TOUCHES; index++) {
            states[index] = RECOGNIZER_STATE_NULL;
        }
        for (touch_event_t event : touchEvents) {
            process_touch_event(&event, 0, 0);
            multistroke_t* multistrokes = get_multistroke();
            for (size_t index = 0; index < MAX_TOUCHES; index++) {
                if (multistrokes[index].state != RECOGNIZER_STATE_NULL) {
                    cout << "dx_" << index << ": " << multistrokes[index].dx << ",\t"
                         << "dy_" << index << ": " << multistrokes[index].dy << ",\t"
                         << "s_" << index << ": " << multistrokes[index].scale << ",\t"
                         << "r_" << index << ": " << multistrokes[index].rotation << ",\t"
                         << "state_" << index << ": " << multistrokes[index].state << endl;
                }
                switch (states[index]) {
                case RECOGNIZER_STATE_NULL:
                    EXPECT_TRUE(multistrokes[index].state == RECOGNIZER_STATE_NULL ||
                                multistrokes[index].state == RECOGNIZER_STATE_POSSIBLE);
                    break;
                case RECOGNIZER_STATE_POSSIBLE:
                    EXPECT_TRUE(multistrokes[index].state == RECOGNIZER_STATE_POSSIBLE ||
                                multistrokes[index].state == RECOGNIZER_STATE_FAILED ||
                                multistrokes[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                    break;
                case RECOGNIZER_STATE_IN_PROGRESS:
                    EXPECT_TRUE(multistrokes[index].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                multistrokes[index].state == RECOGNIZER_STATE_COMPLETED);
                    if (multistrokes[index].state == RECOGNIZER_STATE_COMPLETED) {
                        completed++;
                    }
                    break;
                case RECOGNIZER_STATE_FAILED:
                    EXPECT_TRUE(multistrokes[index].state == RECOGNIZER_STATE_FAILED ||
                                multistrokes[index].state == RECOGNIZER_STATE_POSSIBLE);
                    break;
                case RECOGNIZER_STATE_COMPLETED:
                    EXPECT_TRUE(multistrokes[index].state == RECOGNIZER_STATE_COMPLETED ||
                                multistrokes[index].state == RECOGNIZER_STATE_POSSIBLE);
                    break;
                }
                states[index] = multistrokes[index].state;
            }
        }
        EXPECT_EQ(completed, num);
        delete[] states;
    }
};

TEST_P(TestMultistroke, State) {
    const MultistrokeTestParams& params = GetParam();
    readTouchEvents(params.fileName);
    testStates(params.num);
}

INSTANTIATE_TEST_SUITE_P(MultistrokeParamTests,
                         TestMultistroke,
                         testing::Values(
                             // 1 finger taps
                             MultistrokeTestParams("res/tap/phone_1.csv", 0),
                             MultistrokeTestParams("res/tap/phone_2.csv", 0),
                             MultistrokeTestParams("res/tap/phone_3.csv", 0),
                             MultistrokeTestParams("res/tap/phone_4.csv", 0),
                             MultistrokeTestParams("res/tap/phone_5.csv", 0),
                             MultistrokeTestParams("res/tap/phone_6.csv", 0),
                             MultistrokeTestParams("res/tap/phone_7.csv", 0),
                             MultistrokeTestParams("res/tap/phone_8.csv", 0),
                             MultistrokeTestParams("res/tap/phone_9.csv", 0),
                             MultistrokeTestParams("res/tap/phone_10.csv", 0),

                             // 1 finger drags
                             MultistrokeTestParams("res/drag/phone_1.csv", 0),
                             MultistrokeTestParams("res/drag/phone_2.csv", 0),
                             MultistrokeTestParams("res/drag/phone_3.csv", 0),
                             MultistrokeTestParams("res/drag/phone_4.csv", 0),
                             MultistrokeTestParams("res/drag/phone_5.csv", 0),
                             MultistrokeTestParams("res/drag/phone_6.csv", 0),
                             MultistrokeTestParams("res/drag/phone_7.csv", 0),
                             MultistrokeTestParams("res/drag/phone_8.csv", 0),
                             MultistrokeTestParams("res/drag/phone_9.csv", 0),
                             MultistrokeTestParams("res/drag/phone_10.csv", 0),

                             // 2 finger drags
                             MultistrokeTestParams("res/drag/phone_11.csv", 1),
                             MultistrokeTestParams("res/drag/phone_12.csv", 1),
                             MultistrokeTestParams("res/drag/phone_13.csv", 1),
                             MultistrokeTestParams("res/drag/phone_14.csv", 1),

                             // 2 finger zooms
                             MultistrokeTestParams("res/zoom/phone_1.csv", 1),
                             MultistrokeTestParams("res/zoom/phone_2.csv", 1),
                             MultistrokeTestParams("res/zoom/phone_3.csv", 1),
                             MultistrokeTestParams("res/zoom/phone_4.csv", 1),

                             // 2 finger rotates
                             MultistrokeTestParams("res/rotate/phone_1.csv", 1),
                             MultistrokeTestParams("res/rotate/phone_2.csv", 1)));
