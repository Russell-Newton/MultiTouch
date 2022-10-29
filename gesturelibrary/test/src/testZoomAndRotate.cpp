#include "testFlutter.hpp"

extern "C" {
#include "recognizer.h"
#include "zoom_and_rotate.h"
}

struct ZoomAndRotateTestParams {
    string path;
    size_t num;

    ZoomAndRotateTestParams(const string& path, size_t num) {
        this->path = path;
        this->num  = num;
    }
};

ostream& operator<<(ostream& stream, const ZoomAndRotateTestParams& params) {
    stream << params.path;
    return stream;
}

class TestZoomAndRotate : public TestFlutter, public testing::WithParamInterface<ZoomAndRotateTestParams> {
protected:
    void testStates(size_t num) {
        state_t* s = new state_t[num];
        for (size_t index = 0; index < num; index++) {
            s[index] = RECOGNIZER_STATE_NULL;
        }
        for (touch_event_t event : touchEvents) {
            bool found                = false;
            gesture_event_t* gestures = new gesture_event_t[MAX_RECOGNIZERS];
            process_touch_event(&event, gestures, MAX_RECOGNIZERS);
            for (size_t i = 0; i < MAX_RECOGNIZERS; i++) {
                if (gestures[i].type == GESTURE_TYPE_ZOOM_AND_ROTATE && gestures[i].num_touches == 2) {
                    zoom_and_rotate_t* zoom_and_rotates = ((zoom_and_rotate_t * (*)(void)) gestures[i].get_data)();
                    for (size_t index = 0; index < num; index++) {
                        cout << "z_" << index << ": " << zoom_and_rotates[index].zoom << ",\t"
                             << "r_" << index << ": " << zoom_and_rotates[index].rotate << ",\t"
                             << "state_" << index << ": " << zoom_and_rotates[index].state << endl;
                        switch (s[index]) {
                        case RECOGNIZER_STATE_NULL:
                            EXPECT_TRUE(zoom_and_rotates[index].state == RECOGNIZER_STATE_NULL ||
                                        zoom_and_rotates[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                            s[index] = zoom_and_rotates[index].state;
                            break;
                        case RECOGNIZER_STATE_IN_PROGRESS:
                            EXPECT_TRUE(zoom_and_rotates[index].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                        zoom_and_rotates[index].state == RECOGNIZER_STATE_COMPLETED);
                            s[index] = zoom_and_rotates[index].state;
                            break;
                        default:
                            EXPECT_EQ(s[index], RECOGNIZER_STATE_COMPLETED);
                            break;
                        }
                    }
                    for (size_t index = num; index < MAX_TOUCHES; index++) {
                        EXPECT_EQ(zoom_and_rotates[index].state, RECOGNIZER_STATE_NULL);
                    }
                    found = true;
                    break;
                }
            }
            EXPECT_TRUE(found);
            delete[] gestures;
        }
        for (size_t index = 0; index < num; index++) {
            EXPECT_EQ(s[index], RECOGNIZER_STATE_COMPLETED);
        }
        delete[] s;
    }
};

TEST_P(TestZoomAndRotate, States) {
    readTouchEvents("res/" + GetParam().path + ".csv");
    testStates(GetParam().num);
}

INSTANTIATE_TEST_SUITE_P(ZoomAndRotateTests,
                         TestZoomAndRotate,
                         testing::Values(
                             // rotates
                             ZoomAndRotateTestParams{"rotate/phone_1", 1},
                             ZoomAndRotateTestParams{"rotate/phone_2", 1},
                             // zooms
                             ZoomAndRotateTestParams{"zoom/phone_1", 1},
                             ZoomAndRotateTestParams{"zoom/phone_2", 1},
                             ZoomAndRotateTestParams{"zoom/phone_3", 1},
                             ZoomAndRotateTestParams{"zoom/phone_4", 1}));
