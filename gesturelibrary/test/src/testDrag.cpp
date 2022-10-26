#include "testFlutter.hpp"

#include <cmath>

extern "C" {
#include "drag.h"
#include "recognizer.h"
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
        state_t* s = new state_t[num];
        for (size_t index = 0; index < num; index++) {
            s[index] = RECOGNIZER_STATE_NULL;
        }
        for (touch_event_t event : touchEvents) {
            bool found                = false;
            gesture_event_t* gestures = new gesture_event_t[MAX_RECOGNIZERS];
            process_touch_event(&event, gestures, MAX_RECOGNIZERS);
            for (size_t i = 0; i < MAX_RECOGNIZERS; i++) {
                if (gestures[i].type == GESTURE_TYPE_DRAG && gestures[i].num_touches == 1) {
                    drag_t* drags = ((drag_t * (*)(void)) gestures[i].get_data)();
                    for (size_t index = 0; index < num; index++) {
                        cout << "vx_" << index << ": " << drags[index].vx << ",\t"
                             << "vy_" << index << ": " << drags[index].vy << ",\t"
                             << "m_" << index << ": " << sqrt(pow(drags[index].vx, 2) + pow(drags[index].vy, 2))
                             << ",\t"
                             << "a_" << index << ": " << atan2(drags[index].vy, drags[index].vx) << ",\t"
                             << "state_" << index << ": " << drags[index].state << endl;
                        switch (s[index]) {
                        case RECOGNIZER_STATE_NULL:
                            EXPECT_TRUE(drags[index].state == RECOGNIZER_STATE_NULL ||
                                        drags[index].state == RECOGNIZER_STATE_POSSIBLE);
                            s[index] = drags[index].state;
                            break;
                        case RECOGNIZER_STATE_POSSIBLE:
                            EXPECT_TRUE(drags[index].state == RECOGNIZER_STATE_POSSIBLE ||
                                        drags[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                            s[index] = drags[index].state;
                            break;
                        case RECOGNIZER_STATE_IN_PROGRESS:
                            EXPECT_TRUE(drags[index].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                        drags[index].state == RECOGNIZER_STATE_COMPLETED);
                            s[index] = drags[index].state;
                            break;
                        default:
                            EXPECT_EQ(s[index], RECOGNIZER_STATE_COMPLETED);
                            break;
                        }
                    }
                    for (size_t index = num; index < MAX_TOUCHES; index++) {
                        EXPECT_EQ(drags[index].state, RECOGNIZER_STATE_NULL);
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

TEST_P(TestDrag, States) {
    readTouchEvents("res/" + GetParam().path + ".csv");
    testStates(GetParam().num);
}

INSTANTIATE_TEST_SUITE_P(DragTests,
                         TestDrag,
                         testing::Values(
                             // 1 finger drags
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
                             // rotates
                             DragTestParams{"rotate/phone_1", 2},
                             DragTestParams{"rotate/phone_2", 2},
                             // zooms
                             DragTestParams{"zoom/phone_1", 2},
                             DragTestParams{"zoom/phone_2", 2},
                             DragTestParams{"zoom/phone_3", 2},
                             DragTestParams{"zoom/phone_4", 2}));
