#include "doctest/doctest.h"
#include "Test1.h"
TEST_CASE("testing Test1 Class Sub method")
{
	Test1 testobj;
	CHECK(17 == testobj.Sub(18, 1));
}