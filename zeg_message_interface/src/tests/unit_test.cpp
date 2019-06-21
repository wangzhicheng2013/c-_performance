#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include <chrono>
#include <random>
#include <experimental/filesystem>
#include "doctest.hpp"
#include "common_utility.hpp"
#include "zeg_robot_define.hpp"
#include "zeg_config.hpp"
#include "base_thread.hpp"
#include "zeg_command_processor.hpp"
#include "zeg_recv_command.hpp"
using namespace zeg_message_interface;
namespace fs = experimental::filesystem;
TEST_CASE("testing init conf") {
	CHECK(zeg_config::get_instance().robot_rpc_host_layer_port > 0);
	CHECK(zeg_config::get_instance().robot_rpc_message_interface_layer_port > 0);
	CHECK(7780 == zeg_config::get_instance().udp_server_port);
}
class my_class {
public:
    my_class() {
        my_int = 0;
    }
public:
    int my_int;
    string my_string;
public:
    MSGPACK_DEFINE(my_string, my_int);
};
TEST_CASE("testing msg pack and unpack") {
	msgpack::sbuffer buffer;
    int loop = 1000;
    char buf[1024] = "";
    for (int i = 0;i < loop;i++) {
        my_class my_obj;
        my_obj.my_int = i;
        snprintf(buf, sizeof(buf), "hello world_%d_%ld", i, time(NULL));
        my_obj.my_string = buf;
        buffer.clear();
        msgpack::pack(buffer, my_obj);

        msgpack::unpacked msg;
        msgpack::unpack(&msg, buffer.data(), buffer.size());
        msgpack::object obj = msg.get();
        my_class my_obj1;
        obj.convert(&my_obj1);
        CHECK(my_obj1.my_int == my_obj.my_int);
        CHECK(my_obj1.my_string == my_obj.my_string);
    }
    const char *str = "hello world 123ABCD edf";
    msgpack::unpacked msg;
    msgpack::unpack(&msg, str, strlen(str));
    msgpack::object obj = msg.get();
    CHECK(false == obj.is_nil());
    my_class my_obj;
    CHECK_THROWS_AS(obj.convert(&my_obj), std::exception&);
}
class add_thread : public base_thread  {
protected:
    virtual void process() override {
        sum = 0;
        for (int i = 0;i < n;i++) {
            sum += i;
        }
    }
public:
    int sum;
    int n;
};
TEST_CASE("testing thread base") {
    add_thread my_thread;
    my_thread.n = 1000000;
    my_thread.run();
    my_thread.join();
    int sum = 0;
    for (int i = 0;i < my_thread.n;i++) {
    	sum += i;
    }
    CHECK(sum == my_thread.sum);
}
TEST_CASE("testing zeg_command_processor process0") {
	const char *buf = "hello world. 12345678";
	int len = strlen(buf);
	string ack_str;
	CHECK(false == zeg_command_processor::get_instance().process(buf, len, ack_str));
}
TEST_CASE("testing zeg_command_processor process1") {
	zeg_robot_header header("zeg.robot.basic.in12fo", "zeg_robot_xx011212DD1", chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
	zeg_robot_point p0(1, 2);
	zeg_robot_point p1(11.2, 21.22);
	zeg_robot_navigation_command cmd;
	cmd.task_id = "007";
	cmd.points.emplace_back(p0);
	cmd.points.emplace_back(p1);

	msgpack::sbuffer buffer_header;
	msgpack::sbuffer buffer_body;
	msgpack::pack(buffer_header, header);
	msgpack::pack(buffer_body, cmd);

	char buf[1024] = "";
	memcpy(buf,  buffer_header.data(), buffer_header.size());
	memcpy(buf + buffer_header.size(),  buffer_body.data(), buffer_body.size());
	int len = buffer_header.size() + buffer_body.size();

	string ack_str;
	CHECK(false == zeg_command_processor::get_instance().process(buf, len, ack_str));
}
TEST_CASE("testing zeg_command_processor process2") {
	zeg_robot_header header("zeg.robot.navigation.command", "007", chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
	zeg_robot_point p0(1, 2);
	zeg_robot_point p1(11.2, 21.22);
	zeg_robot_navigation_command cmd;
	cmd.task_id = "007";
	cmd.points.emplace_back(p0);
	cmd.points.emplace_back(p1);

	msgpack::sbuffer buffer_header;
	msgpack::sbuffer buffer_body;
	msgpack::pack(buffer_header, header);
	msgpack::pack(buffer_body, cmd);

	char buf[1024] = "";
	memcpy(buf,  buffer_header.data(), buffer_header.size());
	memcpy(buf + buffer_header.size(),  buffer_body.data(), buffer_body.size());
	int len = buffer_header.size() + buffer_body.size();

	string ack_str;
	CHECK(true == zeg_command_processor::get_instance().process(buf, len, ack_str));
	CHECK(false == ack_str.empty());

	size_t offset = 0;
	msgpack::unpacked msg;
	msgpack::unpack(&msg, ack_str.c_str(), ack_str.size(), &offset);
	zeg_robot_header header1;
	zeg_robot_navigation_command_ack cmd_ack;
	msgpack::object obj = msg.get();
	bool no_excepption = true;
	try {
		obj.convert(&header1);
		CHECK("zeg.robot.navigation.command.ack" == header1.type);
		CHECK("007" == header1.robot_id);
		msgpack::unpack(&msg, ack_str.c_str(), ack_str.size(), &offset);
		obj = msg.get();
		obj.convert(&cmd_ack);
		CHECK("007" == cmd_ack.task_id);
	}
	catch(...) {
		no_excepption = false;
	}
	CHECK(true == no_excepption);
}
void start_message_interface() {
    run_program(zeg_config::get_instance().robot_test_message_interface_path.c_str());
}
TEST_CASE("testing process command") {
	thread th(start_message_interface);
	th.detach();
	this_thread::sleep_for(chrono::milliseconds(6000));

	zeg_robot_header header("zeg.robot.navigation.command", "007", chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
	zeg_robot_point p0(1, 2);
	zeg_robot_point p1(11.2, 21.22);
	zeg_robot_navigation_command cmd;
	cmd.task_id = "007";
	cmd.points.emplace_back(p0);
	cmd.points.emplace_back(p1);

	msgpack::sbuffer buffer_header;
	msgpack::sbuffer buffer_body;
	msgpack::pack(buffer_header, header);
	msgpack::pack(buffer_body, cmd);

	char buf[1024] = "";
	memcpy(buf,  buffer_header.data(), buffer_header.size());
	memcpy(buf + buffer_header.size(),  buffer_body.data(), buffer_body.size());
	int len = buffer_header.size() + buffer_body.size();
	rpc_client client(zeg_config::zeg_config::get_instance().RPC_SERVER_IP, zeg_config::get_instance().robot_rpc_message_interface_layer_port);
	CHECK(true == client.connect(1));
	bool no_exception = true;
	string ack_str;
	try {
		string str(buf, len);
		ack_str = client.call<string>("process_command", str);
	}
	catch (...) {
		no_exception = false;
	}
	CHECK(true == no_exception);
	CHECK(false == ack_str.empty());

	size_t offset = 0;
	msgpack::unpacked msg;
	msgpack::unpack(&msg, ack_str.c_str(), ack_str.size(), &offset);
	zeg_robot_header header1;
	zeg_robot_navigation_command_ack cmd_ack;
	msgpack::object obj = msg.get();
	bool no_excepption = true;
	try {
		obj.convert(&header1);
		CHECK("zeg.robot.navigation.command.ack" == header1.type);
		CHECK("007" == header1.robot_id);
		msgpack::unpack(&msg, ack_str.c_str(), ack_str.size(), &offset);
		obj = msg.get();
		obj.convert(&cmd_ack);
		CHECK("007" == cmd_ack.task_id);
	}
	catch(...) {
		no_excepption = false;
	}
	CHECK(true == no_excepption);

	kill_program(zeg_config::get_instance().robot_test_message_interface_name.c_str());
}
