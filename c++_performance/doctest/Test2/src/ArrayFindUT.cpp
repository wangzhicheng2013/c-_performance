#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "ArrayFind.h"
// white test
// 语句覆盖 判定覆盖 条件覆盖 判定条件覆盖 条件组合覆盖 路径覆盖 
TEST_CASE("testing Array Find TestWithAlgorithm function ")
{
	CHECK(true == TestWithAlgorithm(1));
	CHECK(true == TestWithAlgorithm(2));
	CHECK(true == TestWithAlgorithm(8));
	CHECK(true == TestWithAlgorithm(4));
	CHECK(true == TestWithAlgorithm(5));
	CHECK(true == TestWithAlgorithm(9));
	CHECK(true == TestWithAlgorithm(7));
	CHECK(true == TestWithAlgorithm(11));
	CHECK(true == TestWithAlgorithm(20));
	
	CHECK(false == TestWithAlgorithm(-1));
	CHECK(false == TestWithAlgorithm(200));
}