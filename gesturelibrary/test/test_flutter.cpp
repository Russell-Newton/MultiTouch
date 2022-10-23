#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

extern "C" {
#include "gesturelib.h"
#include "recognizer.h"
#include "singleFingerDrag.h"
#include "singleFingerTap.h"
}

using namespace std;

class TestFlutter : public testing::Test {
protected:
    vector<touch_event_t> touchEvents;

    void readTouchEvents(string fileName) {
        touchEvents.clear();

        ifstream file;
        file.open(fileName);
        EXPECT_TRUE(file.is_open());

        string header;
        file >> header;
        vector<string> columns = split(header);
        size_t indexType       = string::npos;
        size_t indexX          = string::npos;
        size_t indexY          = string::npos;
        size_t indexT          = string::npos;
        for (size_t index = 0; index < columns.size(); index++) {
            if (columns[index] == "type") {
                indexType = index;
            } else if (columns[index] == "position.dx") {
                indexX = index;
            } else if (columns[index] == "position.dy") {
                indexY = index;
            } else if (columns[index] == "timeStamp") {
                indexT = index;
            }
        }
        EXPECT_NE(indexType, string::npos);
        EXPECT_NE(indexX, string::npos);
        EXPECT_NE(indexY, string::npos);
        EXPECT_NE(indexT, string::npos);

        do {
            string event;
            file >> event;
            if (event.size() == 0) {
                break;
            }
            vector<string> data = split(event);
            touch_event_t touchEvent;
            if (data[indexType] == "down") {
                touchEvent.event_type = TOUCH_EVENT_DOWN;
            } else if (data[indexType] == "move") {
                touchEvent.event_type = TOUCH_EVENT_MOVE;
            } else if (data[indexType] == "up") {
                touchEvent.event_type = TOUCH_EVENT_UP;
            }
            touchEvent.position_x = stof(data[indexX]);
            touchEvent.position_y = stof(data[indexY]);
            size_t index1         = data[indexT].find(':');
            size_t index2         = data[indexT].rfind(':');
            int hours             = stoi(data[indexT].substr(0, index1));
            int minutes           = stoi(data[indexT].substr(index1 + 1, index2));
            float seconds         = stof(data[indexT].substr(index2 + 1));
            touchEvent.timestamp  = 60 * 60 * hours + 60 * minutes + seconds;
            touchEvent.id         = TOUCH_ID_UNDEFINED;
            touchEvents.push_back(touchEvent);
        } while (!file.eof());
    }

    void SetUp() override {
        init_gesturelib();
    }

private:
    vector<string> split(string s) {
        vector<string> words;
        size_t index = s.find(',');
        while (index != string::npos) {
            words.push_back(s.substr(0, index));
            s.erase(0, index + 1);
            index = s.find(',');
        }
        words.push_back(s);
        return words;
    }
};

TEST_F(TestFlutter, TapPhone1) {
    readTouchEvents("res/tap/phone_1.csv");
    for (touch_event_t event : touchEvents) {
        process_touch_event(&event, nullptr, 0);
    }
}

class TestDrag : public TestFlutter, public testing::WithParamInterface<int> {
protected:
    void testDrag1() {
        state_t s = RECOGNIZER_STATE_START;
        for (touch_event_t event : touchEvents) {
            bool drag_found           = false;
            gesture_event_t* gestures = new gesture_event_t[MAX_RECOGNIZERS];
            process_touch_event(&event, gestures, MAX_RECOGNIZERS);
            for (size_t i = 0; i < MAX_RECOGNIZERS; i++) {
                if (gestures[i].type == GESTURE_TYPE_DRAG && gestures[i].num_touches == 1) {
                    sFingerDrag_t* drags = ((sFingerDrag_t * (*)(void)) gestures[i].get_data)();
                    bool found           = false;
                    switch (s) {
                    case RECOGNIZER_STATE_START:
                        for (size_t j = 0; j < MAX_TOUCHES; j++) {
                            if (drags[j].state == RECOGNIZER_STATE_POSSIBLE) {
                                s     = drags[j].state;
                                found = true;
                                break;
                            }
                        }
                        EXPECT_TRUE(found);
                        break;
                    case RECOGNIZER_STATE_POSSIBLE:
                        for (size_t j = 0; j < MAX_TOUCHES; j++) {
                            if (drags[j].state == RECOGNIZER_STATE_POSSIBLE ||
                                drags[j].state == RECOGNIZER_STATE_IN_PROGRESS) {
                                s     = drags[j].state;
                                found = true;
                                break;
                            }
                        }
                        EXPECT_TRUE(found);
                        break;
                    case RECOGNIZER_STATE_IN_PROGRESS:
                        for (size_t j = 0; j < MAX_TOUCHES; j++) {
                            if (drags[j].state == RECOGNIZER_STATE_IN_PROGRESS ||
                                drags[j].state == RECOGNIZER_STATE_COMPLETED) {
                                s     = drags[j].state;
                                found = true;
                                break;
                            }
                        }
                        EXPECT_TRUE(found);
                        break;
                    default:
                        EXPECT_EQ("", "incorrect drag state found");
                        break;
                    }
                    drag_found = true;
                    break;
                }
            }
            if (!drag_found) {
                EXPECT_EQ("", "failed to return drag gesture");
            }
            delete[] gestures;
        }
    }
};

class TestTap : public TestFlutter, public testing::WithParamInterface<int> {
protected:
    void testTap1() {
        state_t s = RECOGNIZER_STATE_START;
        for (touch_event_t event : touchEvents) {
            bool tap_found            = false;
            gesture_event_t* gestures = new gesture_event_t[MAX_RECOGNIZERS];
            process_touch_event(&event, gestures, MAX_RECOGNIZERS);
            for (size_t i = 0; i < MAX_RECOGNIZERS; i++) {
                if (gestures[i].type == GESTURE_TYPE_TAP && gestures[i].num_touches == 1) {
                    sFingerTap_t* taps = ((sFingerTap_t * (*)(void)) gestures[i].get_data)();
                    switch (s) {
                    case RECOGNIZER_STATE_START:
                        EXPECT_TRUE(s == taps[0].state);
                        s = RECOGNIZER_STATE_POSSIBLE;
                        break;
                    case RECOGNIZER_STATE_POSSIBLE:
                        EXPECT_TRUE(s == taps[0].state || taps[0].state == RECOGNIZER_STATE_COMPLETED);
                        s = taps[0].state;
                        break;
                    default:
                        EXPECT_EQ("", "incorrect tap state found");
                        break;
                    }
                    tap_found = true;
                    break;
                }
            }
            if (!tap_found) {
                EXPECT_EQ("", "failed to return tap gesture");
            }
            delete[] gestures;
        }
        if (!(s == RECOGNIZER_STATE_COMPLETED)) {
            EXPECT_EQ("", "failed to return complete tap gesture");
        }
    }
};

TEST_P(TestDrag, DragPhone) {
    readTouchEvents("res/drag/phone_" + to_string(GetParam()) + ".csv");
    testDrag1();
}

TEST_P(TestTap, TapPhone) {
    readTouchEvents("res/tap/phone_" + to_string(GetParam()) + ".csv");
    testTap1();
}

INSTANTIATE_TEST_SUITE_P(DragPhoneTests, TestDrag, testing::Values(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));

INSTANTIATE_TEST_SUITE_P(TapPhoneTests, TestTap, testing::Values(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
