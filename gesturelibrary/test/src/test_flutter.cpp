#include "testFlutter.hpp"

extern "C" {
#include "drag.h"
#include "recognizer.h"
#include "tap.h"
}

TEST_F(TestFlutter, TapPhone1) {
    readTouchEvents("res/tap/phone_1.csv");
    for (touch_event_t event : touchEvents) {
        process_touch_event(&event, nullptr, 0);
    }
}
