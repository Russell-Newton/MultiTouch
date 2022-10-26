#pragma once

#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <unistd.h>

extern "C" {
#include "gesturelib.h"
}

using namespace std;

class TestFlutter : public testing::Test {
protected:
    vector<touch_event_t> touchEvents;

    void readTouchEvents(string fileName) {
        touchEvents.clear();

        char path[80];
        getcwd(path, sizeof(path));
        cout << "trying to open " << fileName << " from " << path << endl;

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
