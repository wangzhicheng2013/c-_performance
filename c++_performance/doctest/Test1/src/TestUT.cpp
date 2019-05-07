#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "Test.h"
TEST_CASE("testing Test Class Add method")
{
	Test testobj;
	CHECK(2 == testobj.Add(1, 1));
}
