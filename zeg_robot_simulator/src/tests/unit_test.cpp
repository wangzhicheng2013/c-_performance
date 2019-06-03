
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
*  @date     2019-06-03                                                      *
*  @license                                                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019/06/02 | 1.0.0     |                | Create file                     *
*----------------------------------------------------------------------------*
*  2019/06/03 | 1.0.0     |                | Add config parser test case     *
*----------------------------------------------------------------------------*
*****************************************************************************/
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include "doctest.hpp"
#include "config_parser.hpp"
#include "zeg_robot_define.hpp"
using namespace std;
using namespace zeg_robot_simulator;
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
	fs << "[TT1]" << endl;
	fs << "key2=v3" << endl;
	fs << "key4=v4" << endl;
	fs << "key5=v5" << endl;
	fs << "key6=v6,v7,v8" << endl;
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

	config_parser::config_parser::get_instance().get_value("TT1", "key2", values);
	REQUIRE(1 == values.size());
	CHECK("v3" == values[0]);

	config_parser::config_parser::get_instance().get_value("TT1", "key6", values);
	REQUIRE(3 == values.size());
	bool succ = true;
	for (auto &v : values) {
		if ("v6" != v && "v7" != v && "v8" != v) {
			succ = false;
			break;
		}
	}
	CHECK(true == succ);
}
TEST_CASE("testing pose compute") {
	pose_compute pose_compute_obj;
	pose_compute_obj.msecs = 100;
	pose_compute_obj.cur_pose = {10, 10, 0.1};
	pose_compute_obj.speed = {100, 0, 10};
	pose_compute_obj();
	CHECK(pose_compute_obj.next_pose.x > 0);
	CHECK(pose_compute_obj.next_pose.y > 0);

	pose_compute_obj.cur_pose = {10, 10, 10.1};
	pose_compute_obj.speed = {0, 0, 0};
	pose_compute_obj();
	CHECK(10 == pose_compute_obj.next_pose.x);
	CHECK(10 == pose_compute_obj.next_pose.y);

	pose_compute_obj.msecs = 1000;
	pose_compute_obj.cur_pose = {10, 0, 0};
	pose_compute_obj.speed = {1, 0, 0};
	pose_compute_obj();
	CHECK(11 == pose_compute_obj.next_pose.x);
	CHECK(0 == pose_compute_obj.next_pose.y);

	pose_compute_obj.msecs = 1000;
	pose_compute_obj.cur_pose = {10, 10, 0};
	pose_compute_obj.speed = {0, 0, 1};
	pose_compute_obj();
	cout << pose_compute_obj.next_pose.x << endl;
	cout << pose_compute_obj.next_pose.y << endl;
	CHECK(pose_compute_obj.next_pose.x > 0);
	CHECK(pose_compute_obj.next_pose.y > 0);

	pose_compute_obj.msecs = 1000;
	pose_compute_obj.cur_pose = {10, 10, 0};
	pose_compute_obj.speed = {0, 1, 0};
	pose_compute_obj();
	CHECK(10 == pose_compute_obj.next_pose.x);
	CHECK(10 == pose_compute_obj.next_pose.y);
}
