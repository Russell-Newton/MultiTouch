#include "testFlutter.hpp"

#include <cmath>
#include <iostream>

extern "C" {
#include "multidrag.h"
#include "recognizer.h"
}

using namespace std;

struct MultidragTestParams {
    string fileName;
    int num;

    MultidragTestParams(const string& fileName, int num) {
        this->fileName = fileName;
        this->num      = num;
    }
};

ostream& operator<<(ostream& stream, const MultidragTestParams& params) {
    stream << params.fileName;
    return stream;
}

class TestMultidrag : public TestFlutter, public testing::WithParamInterface<MultidragTestParams> {
protected:
    void testStates(int num) {
        int completed           = 0;
        multidrag_t* multidrags = get_multidrag();
        state_t* states         = new state_t[MAX_TOUCHES];
        for (size_t index = 0; index < MAX_TOUCHES; index++) {
            states[index] = multidrags[index].state;
        }
        for (touch_event_t event : touchEvents) {
            process_touch_event(&event, 0, 0);
            for (size_t index = 0; index < MAX_TOUCHES; index++) {
                if (multidrags[index].state != RECOGNIZER_STATE_NULL) {
                    cout << "uid_" << index << ": " << multidrags[index].uid << ",\t"
                         << "size_" << index << ": " << multidrags[index].size << ",\t"
                         << "dx_" << index << ": " << multidrags[index].dx << ",\t"
                         << "dy_" << index << ": " << multidrags[index].dy << ",\t"
                         << "s_" << index << ": " << multidrags[index].scale << ",\t"
                         << "r_" << index << ": " << multidrags[index].rotation << ",\t"
                         << "state_" << index << ": " << multidrags[index].state << endl;
                }
                switch (states[index]) {
                case RECOGNIZER_STATE_NULL:
                    EXPECT_TRUE(multidrags[index].state == RECOGNIZER_STATE_NULL ||
                                multidrags[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                    break;
                case RECOGNIZER_STATE_IN_PROGRESS:
                    EXPECT_TRUE(multidrags[index].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                multidrags[index].state == RECOGNIZER_STATE_COMPLETED);
                    if (multidrags[index].state == RECOGNIZER_STATE_COMPLETED) {
                        completed++;
                    }
                    break;
                case RECOGNIZER_STATE_COMPLETED:
                    EXPECT_TRUE(multidrags[index].state == RECOGNIZER_STATE_COMPLETED ||
                                multidrags[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                    break;
                default:
                    EXPECT_EQ("", "Multidrag: Invalid state found.");
                    break;
                }
                states[index] = multidrags[index].state;
            }
        }
        EXPECT_EQ(completed, num);
        delete[] states;
    }
};

TEST_P(TestMultidrag, State) {
    const MultidragTestParams& params = GetParam();
    readTouchEvents(params.fileName);
    testStates(params.num);
}

INSTANTIATE_TEST_SUITE_P(MultidragParamTests,
                         TestMultidrag,
                         testing::Values(
                             // 1 finger taps
                             MultidragTestParams("res/tap/phone_1.csv", 0),
                             MultidragTestParams("res/tap/phone_2.csv", 0),
                             MultidragTestParams("res/tap/phone_3.csv", 0),
                             MultidragTestParams("res/tap/phone_4.csv", 0),
                             MultidragTestParams("res/tap/phone_5.csv", 0),
                             MultidragTestParams("res/tap/phone_6.csv", 0),
                             MultidragTestParams("res/tap/phone_7.csv", 0),
                             MultidragTestParams("res/tap/phone_8.csv", 0),
                             MultidragTestParams("res/tap/phone_9.csv", 0),
                             MultidragTestParams("res/tap/phone_10.csv", 0),

                             // 2 finger drags
                             MultidragTestParams("res/drag/phone_11.csv", 1),
                             MultidragTestParams("res/drag/phone_12.csv", 1),
                             MultidragTestParams("res/drag/phone_13.csv", 1),
                             MultidragTestParams("res/drag/phone_14.csv", 1),

                             // 2 finger zooms
                             MultidragTestParams("res/zoom/phone_1.csv", 1),
                             MultidragTestParams("res/zoom/phone_2.csv", 1),
                             MultidragTestParams("res/zoom/phone_3.csv", 1),
                             MultidragTestParams("res/zoom/phone_4.csv", 1),

                             // 2 finger rotates
                             MultidragTestParams("res/rotate/phone_1.csv", 1),
                             MultidragTestParams("res/rotate/phone_2.csv", 1)));
