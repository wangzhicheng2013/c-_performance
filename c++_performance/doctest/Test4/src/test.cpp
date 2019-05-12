#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include <iostream>
#include <vector>
using namespace std;
TEST_CASE("vectors can be sized and resized") {
	cout << "OK" << endl;
	vector<int>v(5);
	REQUIRE(5 == v.size());
	REQUIRE(v.capacity() >= 5);
	SUBCASE("11") {
		v.emplace_back(10);
		REQUIRE(6 == v.size());
		REQUIRE(v.capacity() >= 6);
		SUBCASE("22") {
			cout << "hello world...!" << endl;
		}
	}
	SUBCASE("22") {
		v.reserve(6);
		CHECK(5 == v.size());
		CHECK(v.capacity() >= 6);
	}
}