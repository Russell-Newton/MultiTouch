#include "testFlutter.hpp"

#include <cmath>

extern "C" {
#include "hold.h"
#include "recognizer.h"
#include "utils.h"
}

using namespace std;

struct HoldTestParams {
    string path;
    size_t num;

    HoldTestParams(const string& path, size_t num) {
        this->path = path;
        this->num  = num;
    }
};

ostream& operator<<(ostream& stream, const HoldTestParams& params) {
    stream << params.path;
    return stream;
}

class TestHold : public TestFlutter, public testing::WithParamInterface<HoldTestParams> {
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
                if (gestures[i].type == GESTURE_TYPE_HOLD && gestures[i].num_touches == 1) {
                    hold_t* holds = ((hold_t * (*)(void)) gestures[i].get_data)();
                    for (size_t index = 0; index < MAX_TOUCHES; index++) {
                        switch (states[index]) {
                        case RECOGNIZER_STATE_NULL:
                            EXPECT_TRUE(holds[index].state == RECOGNIZER_STATE_NULL ||
                                        holds[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                            break;
                        case RECOGNIZER_STATE_IN_PROGRESS:
                            EXPECT_TRUE(holds[index].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                        holds[index].state == RECOGNIZER_STATE_FAILED ||
                                        holds[index].state == RECOGNIZER_STATE_COMPLETED);
                            if (holds[index].state == RECOGNIZER_STATE_COMPLETED) {
                                completed++;
                            }
                            break;
                        case RECOGNIZER_STATE_COMPLETED:
                            EXPECT_TRUE(holds[index].state == RECOGNIZER_STATE_COMPLETED ||
                                        holds[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                            break;
                        case RECOGNIZER_STATE_FAILED:
                            EXPECT_TRUE(holds[index].state == RECOGNIZER_STATE_FAILED ||
                                        holds[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                            break;
                        default:
                            EXPECT_EQ(to_string(states[index]), "incorrect hold state found");
                            break;
                        }
                        states[index] = holds[index].state;
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

TEST_P(TestHold, States) {
    readTouchEvents("res/" + GetParam().path + ".csv");
    testStates(GetParam().num);
}

INSTANTIATE_TEST_SUITE_P(HoldTests,
                         TestHold,
                         testing::Values(
                             // 1 finger straight holds
                             HoldTestParams{"hold/phone_1", 1},
                             HoldTestParams{"hold/phone_2", 1},
                             HoldTestParams{"hold/phone_3", 1},
                             HoldTestParams{"hold/phone_4", 1},
                             HoldTestParams{"hold/phone_5", 1},
                             // This one has weird data --> Do we want this to count
                             // HoldTestParams{"hold/phone_6", 1},
                             HoldTestParams{"hold/phone_7", 1}));
