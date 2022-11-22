#include "testFlutter.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>

extern "C" {
#include "recognizer.h"
#include "rotate.h"
}

using namespace std;

struct RotateTestParams {
    string fileName;
    int num;

    RotateTestParams(const string& fileName, int num) {
        this->fileName = fileName;
        this->num      = num;
    }
};

ostream& operator<<(ostream& stream, const RotateTestParams& params) {
    stream << params.fileName;
    return stream;
}

class TestRotate : public TestFlutter, public testing::WithParamInterface<RotateTestParams> {
protected:
    void testStates(int num) {
        int completed           = 0;
        const rotate_t* rotates = get_rotate();
        state_t* states         = new state_t[MAX_TOUCHES];
        for (size_t index = 0; index < MAX_TOUCHES; index++) {
            states[index] = rotates[index].state;
        }
        for (touch_event_t event : touchEvents) {
            process_touch_event(&event);
            for (size_t index = 0; index < 1; index++) {
                // if (rotates[index].state != RECOGNIZER_STATE_NULL) {
                cout << setprecision(5);
                cout << "uid_" << index << ": " << rotates[index].uid << ",\t"
                     << "state_" << index << ": " << rotates[index].state << ",\t"
                     << "size_" << index << ": " << rotates[index].size << ",\t"
                     << "r_" << index << ": " << rotates[index].rotation << endl;
                // }
                switch (states[index]) {
                case RECOGNIZER_STATE_NULL:
                    EXPECT_TRUE(rotates[index].state == RECOGNIZER_STATE_NULL ||
                                rotates[index].state == RECOGNIZER_STATE_POSSIBLE);
                    break;
                case RECOGNIZER_STATE_POSSIBLE:
                    EXPECT_TRUE(rotates[index].state == RECOGNIZER_STATE_POSSIBLE ||
                                rotates[index].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                rotates[index].state == RECOGNIZER_STATE_FAILED);
                    break;
                case RECOGNIZER_STATE_IN_PROGRESS:
                    EXPECT_TRUE(rotates[index].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                rotates[index].state == RECOGNIZER_STATE_FAILED ||
                                rotates[index].state == RECOGNIZER_STATE_COMPLETED);
                    if (rotates[index].state == RECOGNIZER_STATE_COMPLETED) {
                        completed++;
                    }
                    break;
                case RECOGNIZER_STATE_COMPLETED:
                    EXPECT_TRUE(rotates[index].state == RECOGNIZER_STATE_COMPLETED ||
                                rotates[index].state == RECOGNIZER_STATE_POSSIBLE);
                    break;
                case RECOGNIZER_STATE_FAILED:
                    EXPECT_TRUE(rotates[index].state == RECOGNIZER_STATE_FAILED ||
                                rotates[index].state == RECOGNIZER_STATE_POSSIBLE);
                    break;
                default:
                    EXPECT_EQ("", "Rotate: Invalid state found.");
                    break;
                }
                states[index] = rotates[index].state;
            }
        }
        EXPECT_EQ(completed, num);
        delete[] states;
    }
};

TEST_P(TestRotate, State) {
    const RotateTestParams& params = GetParam();
    readTouchEvents(params.fileName);
    testStates(params.num);
}

INSTANTIATE_TEST_SUITE_P(RotateParamTests,
                         TestRotate,
                         testing::Values(
                             // 2 finger followed by 1 finger drag
                             RotateTestParams("res/drag/phone_27.csv", 0),
                             // drag, drag, zoom, drag
                             RotateTestParams("res/drag/laptop_1.csv", 0),

                             // 2 finger zooms
                             RotateTestParams("res/zoom/phone_1.csv", 0),
                             RotateTestParams("res/zoom/phone_2.csv", 0),
                             RotateTestParams("res/zoom/phone_3.csv", 0),
                             RotateTestParams("res/zoom/phone_4.csv", 0),

                             // 2 finger rotates
                             RotateTestParams("res/rotate/phone_1.csv", 1),
                             RotateTestParams("res/rotate/phone_2.csv", 1)));
