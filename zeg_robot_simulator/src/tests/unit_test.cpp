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
*  @date     2019-06-06                                                      *
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
*  2019/06/04 | 1.0.0     |                | Add get next pose test case     *
*----------------------------------------------------------------------------*
*  2019/06/05 | 1.0.0     |                | Add get pose trace test case    *
*****************************************************************************/
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <fstream>
#include <random>
#include <experimental/filesystem>
#include "doctest.hpp"
#include "config_parser.hpp"
#include "zeg_robot_define.hpp"
#include "zeg_config.hpp"
#include "rpc_client.hpp"
#include "codec.h"
using namespace std;
using namespace zeg_robot_simulator;
using namespace rest_rpc;
using namespace rest_rpc::rpc_service;
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
TEST_CASE("testing pose compute0") {
	pose_compute pose_compute_obj;
	pose_compute_obj.msecs = 1000;
	pose_compute_obj.cur_pose_ = {10, 10, 0.1};
	pose_compute_obj.speed_ = {1, 0, 10};
	robot_pose next_pose;
	pose_compute_obj.get_next_pose(next_pose);
	CHECK(next_pose.x > 10);
	CHECK(next_pose.y > 10);
	CHECK(next_pose.theta > 0);

	pose_compute_obj.cur_pose_ = {10, 10, 10.1};
	pose_compute_obj.speed_ = {0, 0, 0};
	pose_compute_obj.get_next_pose(next_pose);
	CHECK(10 == next_pose.x);
	CHECK(10 == next_pose.y);
	CHECK(true == equal(atan2(sin(10.1), cos(10.1)), next_pose.theta));

	pose_compute_obj.msecs = 1000;
	pose_compute_obj.cur_pose_ = {10, 0, 0};
	pose_compute_obj.speed_ = {1, 0, 0};
	pose_compute_obj.get_next_pose(next_pose);
	CHECK(11 == next_pose.x);
	CHECK(0 == next_pose.y);
	CHECK(true == equal(0.0, next_pose.theta));
}
TEST_CASE("testing pose compute1") {
	pose_compute pose_compute_obj;
	robot_pose next_pose;
	pose_compute_obj.msecs = 1000;
	pose_compute_obj.cur_pose_ = {10, 10, 0};
	pose_compute_obj.speed_ = {0, 0, 1};
	pose_compute_obj.get_next_pose(next_pose);
	CHECK(10 == next_pose.x);
	CHECK(10 == next_pose.y);
	CHECK(next_pose.theta > 0);

	pose_compute_obj.msecs = 1000;
	pose_compute_obj.cur_pose_ = {10, 10, 0};
	pose_compute_obj.speed_ = {0, 1, 0};
	pose_compute_obj.get_next_pose(next_pose);
	CHECK(10 == next_pose.x);
	CHECK(10 == next_pose.y);
	CHECK(true == equal(0.0, next_pose.theta));
}
TEST_CASE("testing get pose trace") {
	pose_compute pose_compute_obj;
	pose_compute_obj.msecs = 100;
	pose_compute_obj.cur_pose_ = {10, 10, 0};
	pose_compute_obj.destination_pose_ = {11, 10, 0};
	pose_compute_obj.speed_ = {1, 0, 0};

	vector<robot_pose>pose_trace;
	CHECK(true == pose_compute_obj.get_pose_trace(pose_trace));
	CHECK(10 == pose_trace.size());

	pose_compute_obj.cur_pose_ = {10, 10, 0.88};
	pose_compute_obj.destination_pose_ = {10, 10, 1.88};
	pose_compute_obj.speed_ = {0, 0, 1};

	CHECK(true == pose_compute_obj.get_pose_trace(pose_trace));
	pose_compute_obj.get_pose_trace(pose_trace);
	CHECK(0 == pose_trace.size());

	double x =  atan2(1.0, 1.0);
	pose_compute_obj.cur_pose_ = {10, 10, x};
	pose_compute_obj.destination_pose_ = {11, 11, 0};
	pose_compute_obj.speed_ = {1, 0, 0.1};

	CHECK(true == pose_compute_obj.get_pose_trace(pose_trace));
	CHECK(pose_trace.size() > 0);

	x =  atan2(1.0, 0.0);
	pose_compute_obj.cur_pose_ = {10, 10, x};
	pose_compute_obj.destination_pose_ = {10, 11, 0};
	pose_compute_obj.speed_ = {1, 0, 0.1};

	CHECK(true == pose_compute_obj.get_pose_trace(pose_trace));
	CHECK(pose_trace.size() > 0);

	pose_compute_obj.cur_pose_ = {10, 10, 0};
	pose_compute_obj.destination_pose_ = {10, 11, 0};
	pose_compute_obj.speed_ = {1, 0, 0.1};

	CHECK(false == pose_compute_obj.get_pose_trace(pose_trace));
}
TEST_CASE("testing adjust pose angle") {
	pose_compute pose_compute_obj;
	pose_compute_obj.msecs = 100;
	pose_compute_obj.cur_pose_ = {10, 10, 0};
	pose_compute_obj.destination_pose_ = {11, 10, 0};
	pose_compute_obj.speed_ = {1, 0, 10};
	CHECK(false == pose_compute_obj.adjust_pose_angle());

	pose_compute_obj.cur_pose_ = {10, 10, 2.1};
	pose_compute_obj.destination_pose_ = {11, 10, 10.90};
	pose_compute_obj.speed_ = {1, 0, 0.1};
	CHECK(true == pose_compute_obj.adjust_pose_angle());

	pose_compute_obj.cur_pose_ = {10, 10, 2.1};
	pose_compute_obj.destination_pose_ = {11, 10, 10.90};
	pose_compute_obj.speed_ = {1, 0, 0};
	CHECK(false == pose_compute_obj.adjust_pose_angle());
}
TEST_CASE("testing rotate robot pose") {
	pose_compute pose_compute_obj;
	pose_compute_obj.msecs = 100;
	pose_compute_obj.cur_pose_ = {10, 10, 2.1};
	pose_compute_obj.destination_pose_ = {11, 10, 10.90};
	pose_compute_obj.speed_ = {1, 0, 0.1};
	CHECK(true == pose_compute_obj.adjust_pose_angle());

	vector<robot_pose>pose_trace;
	pose_compute_obj.rotate_robot_pose(pose_trace);
	CHECK(pose_trace.size() > 0);
	CHECK(10 == pose_compute_obj.destination_pose_.y);
	CHECK(10.90 == pose_compute_obj.destination_pose_.theta);
	CHECK(1 == pose_compute_obj.speed_.vx);
}
TEST_CASE("testing merge vector") {
	vector<int>v0{1, 2, 3};
	vector<int>v1;
	merge_vector(v0, v1);
	CHECK(3 == v1.size());
	merge_vector(v0, v1);
	CHECK(6 == v1.size());
}
TEST_CASE("testing get pose trace with angle") {
	pose_compute pose_compute_obj;
	pose_compute_obj.msecs = 100;
	pose_compute_obj.speed_ = {1, 0, 0.1};
	const int n = 1000;
	vector<robot_pose>pose_set;
	vector<robot_pose>pose_trace;
	robot_pose tmp;
	default_random_engine e;
	uniform_real_distribution<double>u(-M_PI_, M_PI_);
	for (int i = 0;i < n;i++) {
		robot_pose pose(i + 1, i * 2 + 1, u(e));
		pose_set.emplace_back(pose);
		tmp = pose;
	}
	CHECK(true == pose_compute_obj.get_pose_trace_with_angle(pose_set, pose_trace));
	REQUIRE(pose_trace.size() > 0);
	bool r = (tmp == *(end(pose_trace) - 1));
	CHECK(true == r);
	for (auto &pose : pose_trace) {
		cout << "(" << pose.x << "," << pose.y << "," << pose.theta << ")" << endl;
	}
}
void start_server() {
	run_program("./zeg_robot_monitor");
}
TEST_CASE("testing rest rpc get cur pose") {
	thread th(start_server);
	th.detach();
	bool no_exception = true;
    try {
    	sleep(1);
    	rpc_client client("127.0.0.1", zeg_config::get_instance().RPC_SERVER_PORT);
    	bool r = client.connect();
    	REQUIRE(true == r);
        auto pose = client.call<robot_pose>("get_cur_pose");
        CHECK(robot_pose(0, 0, 0) == pose);
    }
    catch (const std::exception& e) {
    	cout << e.what() << std::endl;
    	no_exception = false;
    }
    CHECK(true == no_exception);
}
TEST_CASE("testing rest rpc get pose trace") {
	bool no_exception = true;
    try {
    	sleep(1);
    	rpc_client client("127.0.0.1", zeg_config::get_instance().RPC_SERVER_PORT);
    	bool r = client.connect();
    	REQUIRE(true == r);

    	const int n = 1000;
    	vector<robot_pose>pose_set;
    	robot_pose tmp;
    	default_random_engine e;
    	uniform_real_distribution<double>u(-M_PI_, M_PI_);
    	for (int i = 0;i < n;i++) {
    		robot_pose pose(i + 1, i * 2 + 1, u(e));
    		pose_set.emplace_back(pose);
    		tmp = pose;
    	}
        auto pose_trace = client.call<vector<robot_pose>>("get_pose_trace", pose_set);
       	REQUIRE(pose_trace.size() > 0);
       	r = (tmp == *(end(pose_trace) - 1));
        CHECK(true == r);
        for (auto &pose : pose_trace) {
        	cout << "(" << pose.x << "," << pose.y << "," << pose.theta << ")" << endl;
        }
        auto msecs = client.call<int>("get_robot_msecs");
        CHECK(msecs == zeg_config::get_instance().msecs);
    }
    catch (const std::exception& e) {
    	cout << e.what() << std::endl;
    	no_exception = false;
    }
    CHECK(true == no_exception);
	kill_program("zeg_robot_monitor");
}
TEST_CASE("testing init conf") {
	zeg_config::get_instance().init_conf();
	CHECK(100 == zeg_config::get_instance().msecs);
	bool r = (zeg_config::get_instance().speed_ == robot_speed{1, 0, 0.1});
	CHECK(true == r);
	r = (zeg_config::get_instance().cur_pose_ == robot_pose{0, 0, 0});
	CHECK(true == r);
}
