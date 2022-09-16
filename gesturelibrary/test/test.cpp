#include <gtest/gtest.h>

extern "C"
{
#include "gestures.h"
}


TEST(GestureLibraryTest, Test1)
{
    EXPECT_EQ(get_gesture(), 0);
}
