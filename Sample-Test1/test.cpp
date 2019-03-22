#include "pch.h"
#include "..\BSTreeNew\BStree.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(DivisionTest, Test1) {
	int a(5), b(7);
	double rez(double(a) / b);
	EXPECT_EQ(rez, division(a,b));
	EXPECT_TRUE(true);
}