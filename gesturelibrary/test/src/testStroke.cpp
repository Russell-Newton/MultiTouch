#include "testFlutter.hpp"

#include <iostream>

extern "C" {
#include "recognizer.h"
#include "stroke.h"
}

using namespace std;

struct StrokeTestParams {
    string fileName;
    int num;

    StrokeTestParams(const string& fileName, int num) {
        this->fileName = fileName;
        this->num      = num;
    }
};

ostream& operator<<(ostream& stream, const StrokeTestParams& params) {
    stream << params.fileName;
    return stream;
}

class TestStroke : public TestFlutter, public testing::WithParamInterface<StrokeTestParams> {
protected:
    void checkVelocity(stroke_t* stroke) {
        if (stroke->cache_size > 0) {
            float vx = 0;
            float vy = 0;
            for (int i = 0; i < stroke->cache_size; i++) {
                vx += stroke->cache_vx[i];
                vy += stroke->cache_vy[i];
            }
            EXPECT_LT(fabsf(stroke->vx - vx / stroke->cache_size), 0.01f);
            EXPECT_LT(fabsf(stroke->vy - vy / stroke->cache_size), 0.01f);
        } else {
            EXPECT_FLOAT_EQ(stroke->vx, 0);
            EXPECT_FLOAT_EQ(stroke->vy, 0);
        }
    }

    void testStates(int num) {
        int completed   = 0;
        state_t* states = new state_t[MAX_TOUCHES];
        for (size_t index = 0; index < MAX_TOUCHES; index++) {
            states[index] = RECOGNIZER_STATE_NULL;
        }
        for (touch_event_t event : touchEvents) {
            process_touch_event(&event, 0, 0);
            stroke_t* strokes = get_stroke();
            for (size_t index = 0; index < MAX_TOUCHES; index++) {
                checkVelocity(strokes + index);
                switch (states[index]) {
                case RECOGNIZER_STATE_NULL:
                    EXPECT_TRUE(strokes[index].state == RECOGNIZER_STATE_NULL ||
                                strokes[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                    break;
                case RECOGNIZER_STATE_IN_PROGRESS:
                    EXPECT_TRUE(strokes[index].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                strokes[index].state == RECOGNIZER_STATE_COMPLETED);
                    if (strokes[index].state == RECOGNIZER_STATE_COMPLETED) {
                        completed++;
                    }
                    break;
                case RECOGNIZER_STATE_COMPLETED:
                    EXPECT_TRUE(strokes[index].state == RECOGNIZER_STATE_COMPLETED ||
                                strokes[index].state == RECOGNIZER_STATE_IN_PROGRESS);
                    break;
                default:
                    EXPECT_EQ(to_string(states[index]), "incorrect stroke state found");
                    break;
                }
                states[index] = strokes[index].state;
            }
        }
        EXPECT_EQ(completed, num);
        delete[] states;
    }
};

TEST_P(TestStroke, State) {
    const StrokeTestParams& params = GetParam();
    readTouchEvents(params.fileName);
    testStates(params.num);
}

INSTANTIATE_TEST_SUITE_P(StrokeParamTests,
                         TestStroke,
                         testing::Values(
                             // 1 finger taps
                             StrokeTestParams("res/tap/phone_1.csv", 1),
                             StrokeTestParams("res/tap/phone_2.csv", 1),
                             StrokeTestParams("res/tap/phone_3.csv", 1),
                             StrokeTestParams("res/tap/phone_4.csv", 1),
                             StrokeTestParams("res/tap/phone_5.csv", 1),
                             StrokeTestParams("res/tap/phone_6.csv", 1),
                             StrokeTestParams("res/tap/phone_7.csv", 1),
                             StrokeTestParams("res/tap/phone_8.csv", 1),
                             StrokeTestParams("res/tap/phone_9.csv", 1),
                             StrokeTestParams("res/tap/phone_10.csv", 1),

                             // 1 finger drags
                             StrokeTestParams("res/drag/phone_1.csv", 1),
                             StrokeTestParams("res/drag/phone_2.csv", 1),
                             StrokeTestParams("res/drag/phone_3.csv", 1),
                             StrokeTestParams("res/drag/phone_4.csv", 1),
                             StrokeTestParams("res/drag/phone_5.csv", 1),
                             StrokeTestParams("res/drag/phone_6.csv", 1),
                             StrokeTestParams("res/drag/phone_7.csv", 1),
                             StrokeTestParams("res/drag/phone_8.csv", 1),
                             StrokeTestParams("res/drag/phone_9.csv", 1),
                             StrokeTestParams("res/drag/phone_10.csv", 1),

                             // 2 finger drags
                             StrokeTestParams("res/drag/phone_11.csv", 2),
                             StrokeTestParams("res/drag/phone_12.csv", 2),
                             StrokeTestParams("res/drag/phone_13.csv", 2),
                             StrokeTestParams("res/drag/phone_14.csv", 2),

                             // 2 finger zooms
                             StrokeTestParams("res/zoom/phone_1.csv", 2),
                             StrokeTestParams("res/zoom/phone_2.csv", 2),
                             StrokeTestParams("res/zoom/phone_3.csv", 2),
                             StrokeTestParams("res/zoom/phone_4.csv", 2),

                             // 2 finger rotates
                             StrokeTestParams("res/rotate/phone_1.csv", 2),
                             StrokeTestParams("res/rotate/phone_2.csv", 2)));
