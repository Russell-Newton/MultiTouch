#include "testFlutter.hpp"
#include "utils.h"

#include <cmath>

extern "C" {
#include "recognizer.h"
#include "swipe.h"
}

using namespace std;

struct SwipeTestParams {
    string path;
    size_t num;

    SwipeTestParams(const string& path, size_t num) {
        this->path = path;
        this->num  = num;
    }
};

ostream& operator<<(ostream& stream, const SwipeTestParams& params) {
    stream << params.path;
    return stream;
}

class TestSwipe : public TestFlutter, public testing::WithParamInterface<SwipeTestParams> {
protected:
    void testStates(size_t num) {
        int completed   = 0;
        state_t* states = new state_t[MAX_TOUCHES];
        for (size_t index = 0; index < MAX_TOUCHES; index++) {
            states[index] = RECOGNIZER_STATE_NULL;
        }
        for (touch_event_t event : touchEvents) {
            gesture_event_t* gestures = new gesture_event_t[MAX_RECOGNIZERS];
            process_touch_event(&event, gestures, MAX_RECOGNIZERS);
            for (size_t i = 0; i < MAX_RECOGNIZERS; i++) {
                if (gestures[i].type == GESTURE_TYPE_SWIPE && gestures[i].num_touches == 1) {
                    swipe_t* swipes = ((swipe_t * (*)(void)) gestures[i].get_data)();
                    for (size_t index = 0; index < MAX_TOUCHES; index++) {
                        if (swipes[index].state != RECOGNIZER_STATE_NULL) {
                            cout << "spd_" << index << ": " << sqrtf(SQUARE_SUM(swipes[index].vx, swipes[index].vy))
                                 << ",\t"
                                 << "dir_" << index << ": " << atan2f(swipes[index].vy, swipes[index].vx) << ",\t"
                                 << "ste_" << index << ": " << swipes[index].state << endl;
                        }
                        switch (states[index]) {
                        case RECOGNIZER_STATE_NULL:
                            EXPECT_TRUE(swipes[index].state == RECOGNIZER_STATE_NULL ||
                                        swipes[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                            break;
                        case RECOGNIZER_STATE_IN_PROGRESS:
                            EXPECT_TRUE(swipes[index].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                        swipes[index].state == RECOGNIZER_STATE_FAILED ||
                                        swipes[index].state == RECOGNIZER_STATE_COMPLETED);
                            if (swipes[index].state == RECOGNIZER_STATE_COMPLETED) {
                                completed++;
                            }
                            break;
                        case RECOGNIZER_STATE_COMPLETED:
                            EXPECT_TRUE(swipes[index].state == RECOGNIZER_STATE_COMPLETED ||
                                        swipes[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                            break;
                        case RECOGNIZER_STATE_FAILED:
                            EXPECT_TRUE(swipes[index].state == RECOGNIZER_STATE_FAILED ||
                                        swipes[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                            break;
                        default:
                            EXPECT_EQ(to_string(states[index]), "incorrect swipe state found");
                            break;
                        }
                        states[index] = swipes[index].state;
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

TEST_P(TestSwipe, States) {
    readTouchEvents("res/" + GetParam().path + ".csv");
    testStates(GetParam().num);
}

INSTANTIATE_TEST_SUITE_P(SwipeTests,
                         TestSwipe,
                         testing::Values(
                             // 1 finger swipes
                             SwipeTestParams{"swipe/phone_1", 1},
                             SwipeTestParams{"swipe/phone_2", 1},
                             SwipeTestParams{"swipe/phone_3", 1},
                             SwipeTestParams{"swipe/phone_4", 1},
                             SwipeTestParams{"swipe/phone_5", 1},
                             SwipeTestParams{"swipe/phone_6", 1},
                             SwipeTestParams{"swipe/phone_7", 1},
                             SwipeTestParams{"swipe/phone_8", 1},
                             SwipeTestParams{"swipe/phone_9", 1},

                             // 1 finger straight drags
                             SwipeTestParams{"drag/phone_1", 0},
                             SwipeTestParams{"drag/phone_2", 0},
                             SwipeTestParams{"drag/phone_3", 0},
                             SwipeTestParams{"drag/phone_4", 0},
                             SwipeTestParams{"drag/phone_5", 0},
                             SwipeTestParams{"drag/phone_6", 0},
                             SwipeTestParams{"drag/phone_7", 0},
                             SwipeTestParams{"drag/phone_8", 0},
                             SwipeTestParams{"drag/phone_9", 0},
                             SwipeTestParams{"drag/phone_10", 0},

                             // 2 finger drags
                             SwipeTestParams{"drag/phone_11", 0},
                             SwipeTestParams{"drag/phone_12", 0},
                             SwipeTestParams{"drag/phone_13", 0},
                             SwipeTestParams{"drag/phone_14", 0},

                             // 1 finger curved drags
                             SwipeTestParams{"drag/phone_15", 0},
                             SwipeTestParams{"drag/phone_16", 0},
                             SwipeTestParams{"drag/phone_17", 0},
                             SwipeTestParams{"drag/phone_18", 0},
                             SwipeTestParams{"drag/phone_19", 0},
                             SwipeTestParams{"drag/phone_20", 0},
                             SwipeTestParams{"drag/phone_21", 0},
                             SwipeTestParams{"drag/phone_22", 0},
                             SwipeTestParams{"drag/phone_23", 0},
                             SwipeTestParams{"drag/phone_24", 0},
                             SwipeTestParams{"drag/phone_25", 0},
                             SwipeTestParams{"drag/phone_26", 0}));
