#ifndef OPTIMIZATION_TESTS_H
#define OPTIMIZATION_TESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(SimplexTests, Test_1) {
    ASSERT_TRUE(1 == 1);
}

TEST(SimplexTests, Test_2) {
    ASSERT_TRUE(2 == 2);
}

int run_tests(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}

#endif //OPTIMIZATION_TESTS_H
