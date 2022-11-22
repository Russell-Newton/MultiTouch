#include <gtest/gtest.h>

extern "C" {
#include "double.h"
#include "hold.h"
#include "tap.h"
}

TEST(GestureLibraryTest, TestInitGestureLib) {
    init_gesturelib();
}
