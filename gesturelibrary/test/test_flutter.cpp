#include <cmath>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

extern "C" {
#include "drag.h"
#include "gesturelib.h"
#include "recognizer.h"
#include "singleFingerTap.h"
#include "zoom_and_rotate.h"
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
            s[index] = RECOGNIZER_STATE_START;
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
                        case RECOGNIZER_STATE_START:
                            EXPECT_TRUE(drags[index].state == RECOGNIZER_STATE_START ||
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
                        EXPECT_EQ(drags[index].state, RECOGNIZER_STATE_START);
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

TEST_P(TestTap, TapPhone) {
    readTouchEvents("res/tap/phone_" + to_string(GetParam()) + ".csv");
    testTap1();
}

INSTANTIATE_TEST_SUITE_P(TapPhoneTests, TestTap, testing::Values(1, 2, 3, 4, 5, 6, 7, 8, 9, 10));

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
            s[index] = RECOGNIZER_STATE_START;
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
                        case RECOGNIZER_STATE_START:
                            EXPECT_TRUE(zoom_and_rotates[index].state == RECOGNIZER_STATE_START ||
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
                        EXPECT_EQ(zoom_and_rotates[index].state, RECOGNIZER_STATE_START);
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
