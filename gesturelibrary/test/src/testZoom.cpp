#include "testFlutter.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>

extern "C" {
#include "recognizer.h"
#include "zoom.h"
}

using namespace std;

struct ZoomTestParams {
    string fileName;
    int num;

    ZoomTestParams(const string& fileName, int num) {
        this->fileName = fileName;
        this->num      = num;
    }
};

ostream& operator<<(ostream& stream, const ZoomTestParams& params) {
    stream << params.fileName;
    return stream;
}

class TestZoom : public TestFlutter, public testing::WithParamInterface<ZoomTestParams> {
protected:
    void testStates(int num) {
        int completed       = 0;
        const zoom_t* zooms = get_zoom();
        state_t* states     = new state_t[MAX_TOUCHES];
        for (size_t index = 0; index < MAX_TOUCHES; index++) {
            states[index] = zooms[index].state;
        }
        for (touch_event_t event : touchEvents) {
            process_touch_event(&event);
            for (size_t index = 0; index < 1; index++) {
                // if (zooms[index].state != RECOGNIZER_STATE_NULL) {
                cout << setprecision(5);
                cout << "uid_" << index << ": " << zooms[index].uid << ",\t"
                     << "state_" << index << ": " << zooms[index].state << ",\t"
                     << "size_" << index << ": " << zooms[index].size << ",\t"
                     << "s_" << index << ": " << zooms[index].scale << endl;
                // }
                switch (states[index]) {
                case RECOGNIZER_STATE_NULL:
                    EXPECT_TRUE(zooms[index].state == RECOGNIZER_STATE_NULL ||
                                zooms[index].state == RECOGNIZER_STATE_POSSIBLE);
                    break;
                case RECOGNIZER_STATE_POSSIBLE:
                    EXPECT_TRUE(zooms[index].state == RECOGNIZER_STATE_POSSIBLE ||
                                zooms[index].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                zooms[index].state == RECOGNIZER_STATE_FAILED);
                    break;
                case RECOGNIZER_STATE_IN_PROGRESS:
                    EXPECT_TRUE(zooms[index].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                zooms[index].state == RECOGNIZER_STATE_FAILED ||
                                zooms[index].state == RECOGNIZER_STATE_COMPLETED);
                    if (zooms[index].state == RECOGNIZER_STATE_COMPLETED) {
                        completed++;
                    }
                    break;
                case RECOGNIZER_STATE_COMPLETED:
                    EXPECT_TRUE(zooms[index].state == RECOGNIZER_STATE_COMPLETED ||
                                zooms[index].state == RECOGNIZER_STATE_POSSIBLE);
                    break;
                case RECOGNIZER_STATE_FAILED:
                    EXPECT_TRUE(zooms[index].state == RECOGNIZER_STATE_FAILED ||
                                zooms[index].state == RECOGNIZER_STATE_POSSIBLE);
                    break;
                default:
                    EXPECT_EQ("", "Zoom: Invalid state found.");
                    break;
                }
                states[index] = zooms[index].state;
            }
        }
        EXPECT_EQ(completed, num);
        delete[] states;
    }
};

TEST_P(TestZoom, State) {
    const ZoomTestParams& params = GetParam();
    readTouchEvents(params.fileName);
    testStates(params.num);
}

INSTANTIATE_TEST_SUITE_P(ZoomParamTests,
                         TestZoom,
                         testing::Values(
                             // 2 finger followed by 1 finger drag
                             ZoomTestParams("res/drag/phone_27.csv", 0),
                             // drag, drag, zoom, drag
                             ZoomTestParams("res/drag/laptop_1.csv", 1),

                             // 2 finger zooms
                             ZoomTestParams("res/zoom/phone_1.csv", 1),
                             ZoomTestParams("res/zoom/phone_2.csv", 1),
                             ZoomTestParams("res/zoom/phone_3.csv", 1),
                             ZoomTestParams("res/zoom/phone_4.csv", 1),

                             // 2 finger rotates
                             ZoomTestParams("res/rotate/phone_1.csv", 0),
                             ZoomTestParams("res/rotate/phone_2.csv", 0)));
