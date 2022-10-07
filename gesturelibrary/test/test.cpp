#include <gtest/gtest.h>

extern "C"
{
#include "gestures.h"
#include "singleRecog.h"
#include "twoRecog.h"
}


TEST(GestureLibraryTest, Test1)
{
    EXPECT_EQ(get_gesture(), 0);
}

TEST(SingleLibraryTest, Test2)
{
    EXPECT_EQ(recognize_single_tap(), 0);
}
