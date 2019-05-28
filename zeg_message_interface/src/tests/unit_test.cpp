
/*
 * unit_test.cpp
 *
 *  Created on: 2019年5月28日
 *      Author: root
 */
/*****************************************************************************
*  unit test source file                                                     *
*  Copyright (C) 2019                                                        *
*                                                                            *
*  @file     unit_test.cpp                                                   *
*  @brief    message interface unit test file                                *
*  Details.                                                                  *
*                                                                            *
*  @author                                                                   *
*  @email                                                                    *
*  @version  1.0.0                                                           *
*  @date     2019-05-28                                                      *
*  @license                                                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019/05/28 | 1.0.0     |                | Create file                     *
*----------------------------------------------------------------------------*
*----------------------------------------------------------------------------*                                                                   *
*****************************************************************************/
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include <chrono>
#include "doctest.hpp"
#include "zmq_agent.hpp"
#include "zeg_data_define.h"
#include "zeg_config.hpp"
using namespace zeg_message_interface;
using namespace zmq_self_agent;
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
msgpack::sbuffer buffer;
TEST_CASE("testing msg pack and unpack") {
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
}
static const int SIZE = 64;
static const int LOOP = 100;
const char *g_server_address = "tcp://localhost:9141";
const char *g_local_address = "tcp://*:9141";
char g_send_buf[SIZE] = "";
void pack(int i) {
	snprintf(g_send_buf, SIZE, "AAA%d", i);
	my_class obj;
	obj.my_int = i;
	obj.my_string = g_send_buf;
	buffer.clear();
	msgpack::pack(buffer, obj);
}
void unpack(const char *buf, int size, queue<my_class>&my_queue) {
	msgpack::unpacked msg;
	msgpack::unpack(&msg, buf, size);
	msgpack::object obj = msg.get();
	my_class my_obj;
	obj.convert(&my_obj);
	my_queue.push(my_obj);
}
int send_cnt = 0;
bool send_thread() {
	this_thread::sleep_for(chrono::seconds(4));
	void *context = zmq_ctx_new();
	void *client = zmq_socket(context, ZMQ_PUSH);
	if (nullptr == context || nullptr == client) {
		return false;
	}
	zmq_connect(client, g_server_address);
	for (int i = 0;i < LOOP;i++) {
		zmq_msg_t message = {0};
		pack(i);
		int size = buffer.size();
		zmq_msg_init_size(&message, size);
		memcpy(zmq_msg_data(&message), buffer.data(), size);
		if (size == zmq_msg_send(&message, client, 0)) {
			++send_cnt;
		}
		zmq_msg_close(&message);
	}
	zmq_close(client);
	zmq_term(context);
	return true;
}
int recv_cnt = 0;
bool recv_thread(queue<my_class>&my_queue) {
	void *context = zmq_ctx_new();
	void *server = zmq_socket(context, ZMQ_PULL);
	if (nullptr == context || nullptr == server) {
		return false;
	}
	zmq_bind(server, g_local_address);
	this_thread::sleep_for(chrono::seconds(2));
	while (true) {
		zmq_msg_t message = {0};
		zmq_msg_init(&message);
		int len = zmq_msg_recv(&message, server, 0);
		++recv_cnt;
		unpack((char *)zmq_msg_data(&message), len, my_queue);
		zmq_msg_close(&message);
		if (recv_cnt >= LOOP) {
			break;
		}
	}
	zmq_close(server);
	zmq_term(context);
	return true;
}
TEST_CASE("testing zmq send and recv") {
	queue<my_class>my_queue;
	thread th0(send_thread);
	thread th1(recv_thread, ref(my_queue));
	if (th0.joinable()) {
		th0.join();
	}
	if (th1.joinable()) {
		th1.join();
	}
	CHECK(send_cnt == LOOP);
	CHECK(recv_cnt == LOOP);
	CHECK(my_queue.size() == LOOP);
	int cnt = 0;
	char buf[SIZE] = "";
	while (my_queue.empty()) {
		my_class obj;
		obj = my_queue.front();
		my_queue.pop();
		snprintf(buf, SIZE, "AAA%d", cnt);
		CHECK(obj.my_int == cnt);
		CHECK(obj.my_string == buf);
		++cnt;
	}
}
TEST_CASE("testing znavigate_command pack and unpack") {
	int loop = 1000;
	int points = 3;
	char buf[1024] = "";
	for (int i = 0;i < loop;i++) {
		znavigate_command my_obj;
		my_obj.task_id = i;
		for (int i = 0;i < points;i++) {
			my_obj.task_id = i;
			zwaypoint way_point = {{{1000, 1000, 1000, 1000}, 1000}, {1000, 1000, 1000, 1000, 1000}};
			my_obj.waypoints_.emplace_back(way_point);
		}
		buffer.clear();
		msgpack::pack(buffer, my_obj);

		msgpack::unpacked msg;
		msgpack::unpack(&msg, buffer.data(), buffer.size());
		msgpack::object obj = msg.get();
		znavigate_command my_obj1;
		obj.convert(&my_obj1);
		CHECK(my_obj1.task_id == my_obj.task_id);
		CHECK(my_obj1.waypoints_.size()== my_obj.waypoints_.size());
		ztolerance t = {1000, 1000, 1000, 1000, 1000};
		for (size_t i = 0;i < my_obj1.waypoints_.size();i++) {
			CHECK(1000 == my_obj1.waypoints_[i].tolerance_.x_thres);
			CHECK(1000 == my_obj1.waypoints_[i].posetime_.time);
			CHECK(1000 == my_obj1.waypoints_[i].posetime_.pose_.theta);
			CHECK(1000 == my_obj1.waypoints_[i].posetime_.pose_.blief);
		}
	}
}
TEST_CASE("testing zeg configuration") {
	CHECK(0 != zeg_config::get_instance().g_server_address[0]);
	CHECK(0 != zeg_config::get_instance().g_local_address[0]);
}
bool send_thread1() {
	this_thread::sleep_for(chrono::seconds(4));
	send_cnt = 0;
	zmq_config config;
	zmq_agent zmq_client;
	config.sock_type = ZMQ_PUSH;
	config.addr = g_server_address;
	if (zmq_client.init(config)) {
		return false;
	}
	for (int i = 0;i < LOOP;i++) {
		pack(i);
		int size = buffer.size();
		if (size == zmq_client.send(buffer.data(), buffer.size())) {
			++send_cnt;
		}
	}
	return true;
}
bool recv_thread1(queue<my_class>&my_queue) {
	recv_cnt = 0;
	zmq_config config;
	zmq_agent zmq_server;
	config.sock_type = ZMQ_PULL;
	config.addr = g_local_address;
	if (zmq_server.init(config)) {
			return false;
	}
	this_thread::sleep_for(chrono::seconds(2));
	string recv_str;
	while (true) {
		recv_str.clear();
		if (true == zmq_server.recv(recv_str)) {
			++recv_cnt;
			int len = recv_str.size();
			unpack(recv_str.c_str(), len, my_queue);
		}
		if (recv_cnt >= LOOP) {
			break;
		}
	}
	return true;
}
TEST_CASE("testing zmq agent send and recv") {
	queue<my_class>my_queue;
	thread th0(send_thread1);
	thread th1(recv_thread1, ref(my_queue));
	if (th0.joinable()) {
		th0.join();
	}
	if (th1.joinable()) {
		th1.join();
	}
	CHECK(send_cnt == LOOP);
	CHECK(recv_cnt == LOOP);
	CHECK(my_queue.size() == LOOP);
	int cnt = 0;
	char buf[SIZE] = "";
	while (my_queue.empty()) {
		my_class obj;
		obj = my_queue.front();
		my_queue.pop();
		snprintf(buf, SIZE, "AAA%d", cnt);
		CHECK(obj.my_int == cnt);
		CHECK(obj.my_string == buf);
		++cnt;
	}
}
