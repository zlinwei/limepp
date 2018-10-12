/**
    @file   basicTX.cpp
    @author Lime Microsystems (www.limemicro.com)
    @brief  minimal TX example
 */
#include <iostream>
#include <chrono>
#include <math.h>
#include <thread>
#include <gtest/gtest.h>

TEST(TestCPP, test_reinterpret_cast) {
    int a = 0x41414141;//4 bytes a int
    auto *b = reinterpret_cast<uint8_t *>(&a);

    for (int i = 0; i < 4; ++i) {
        std::cout << "0x" << std::hex << int(b[i]) << std::endl;
        GTEST_ASSERT_EQ(b[i], 0x41);
    }

    GTEST_ASSERT_EQ(0, 0);
}