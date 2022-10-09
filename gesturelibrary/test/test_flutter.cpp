#include <fstream>
#include <gtest/gtest.h>
#include <sstream>

extern "C" {
#include "gesturelib.h"
}

using namespace std;

class TestFlutter : public testing::Test {
protected:
    vector<touch_event_t> touchEvents;

    void readTouchEvents(string fileName) {
        ifstream file;
        file.open(fileName);
        EXPECT_TRUE(file.is_open());

        string header;
        file >> header;
        vector<string> columns = split(header);
        size_t indexX          = string::npos;
        size_t indexY          = string::npos;
        size_t indexT          = string::npos;
        for (size_t index = 0; index < columns.size(); index++) {
            if (columns[index] == "position.dx") {
                indexX = index;
            } else if (columns[index] == "position.dy") {
                indexY = index;
            } else if (columns[index] == "timeStamp") {
                indexT = index;
            }
        }
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
            touchEvent.position_x = stof(data[indexX]);
            touchEvent.position_y = stof(data[indexY]);
            size_t index1         = data[indexT].find(':');
            size_t index2         = data[indexT].rfind(':');
            int hours             = stoi(data[indexT].substr(0, index1));
            int minutes           = stoi(data[indexT].substr(index1 + 1, index2));
            float seconds         = stof(data[indexT].substr(index2 + 1));
            touchEvent.timestamp  = 60 * 60 * hours + 60 * minutes + seconds;
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
