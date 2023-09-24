#ifndef OPTIMIZATION_TESTS_H
#define OPTIMIZATION_TESTS_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(TestGroupName, Subtest_1) {
    ASSERT_TRUE(1 == 1);
}

int run_tests(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}

#endif //OPTIMIZATION_TESTS_H
