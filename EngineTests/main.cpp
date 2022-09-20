//
// Created by boris on 7/6/2022.
//
#include "spdlog/spdlog.h"
#include "gtest/gtest.h"

TEST(List, PushBack) {
	// Arrange
	int a = 10;

	// Act
	a = a - 1 + 1;
	// Assert
	ASSERT_EQ(a, 11);
}

int main(int agrc, char** argv)
{
	testing::InitGoogleTest(&agrc, argv);

	return RUN_ALL_TESTS();
}