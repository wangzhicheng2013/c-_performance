/*****************************************************************************
*  unit test source file                                                     *
*  Copyright (C) 2019                                                        *
*                                                                            *
*  @file     unit_test.cpp                                                   *
*  @brief    robot monitor unit test file                                    *
*  Details.                                                                  *
*                                                                            *
*  @author                                                                   *
*  @email                                                                    *
*  @version  1.0.0                                                           *
*  @date     2019-06-02                                                      *
*  @license                                                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019/06/02 | 1.0.0     |                | Create file                     *
*----------------------------------------------------------------------------*
*****************************************************************************/
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include "doctest.hpp"
#include "config_parser.hpp"
using namespace std;
namespace fs = std::experimental::filesystem;
bool write_test_config_file(const char *filepath) {
	fstream fs(filepath, ios::out);
	if (!fs) {
		return false;
	}
	fs << "key0=v0" << endl;
	fs << "key1=v1,v2" << endl;
	fs << "[TT]" << endl;
	fs << "key2=v3" << endl;
	fs << "key4=v4" << endl;
	fs.close();
	return true;

}
TEST_CASE("testing configuration parser") {
	const char *filepath = "./test.conf";
	REQUIRE(true == write_test_config_file(filepath));
	REQUIRE(true == config_parser::config_parser::get_instance().init(filepath));
	CHECK(true == fs::remove(filepath));
	vector<string>values;
	config_parser::config_parser::get_instance().get_value("TT", "key2", values);
	REQUIRE(1 == values.size());
	CHECK("v3" == values[0]);

	config_parser::config_parser::get_instance().get_value("TT", "key4", values);
	REQUIRE(1 == values.size());
	CHECK("v4" == values[0]);

	config_parser::config_parser::get_instance().get_value("", "key0", values);
	REQUIRE(1 == values.size());
	CHECK("v0" == values[0]);

	config_parser::config_parser::get_instance().get_value("", "key1", values);
	REQUIRE(2 == values.size());
	CHECK("v1" == values[0]);
	CHECK("v2" == values[1]);
}
