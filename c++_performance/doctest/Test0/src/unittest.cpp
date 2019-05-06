#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN		// run with default main by doctest
//#define DOCTEST_CONFIG_IMPLEMENT				// remove the default main by doctest
#include "doctest/doctest.h"
#include "BinaryToDecimal.h"
TEST_CASE("testing binary1")
{
	CHECK(7 == binary1(111));
	CHECK(14 == binary1(1110));
	CHECK(15 == binary1(1111));
}
TEST_CASE("testing binary2")
{
	CHECK(7 == binary2(111));
	CHECK(14 == binary2(1110));
	CHECK(15 == binary2(1111));
}
