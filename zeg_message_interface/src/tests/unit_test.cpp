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
#include <string>
#include <queue>
#include <thread>
#include <chrono>
#include "msgpack.hpp"
#include "doctest.hpp"
#include "zmq.h"
using namespace std;
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
queue<my_class>g_queue;
void pack(int i) {
	snprintf(g_send_buf, SIZE, "AAA%d", i);
	my_class obj;
	obj.my_int = i;
	obj.my_string = g_send_buf;
	buffer.clear();
	msgpack::pack(buffer, obj);
}
void unpack(const char *buf, int size) {
	msgpack::unpacked msg;
	msgpack::unpack(&msg, buf, size);
	msgpack::object obj = msg.get();
	my_class my_obj;
	obj.convert(&my_obj);
	g_queue.push(my_obj);
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
bool recv_thread() {
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
		unpack((char *)zmq_msg_data(&message), len);
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
	thread th0(send_thread);
	thread th1(recv_thread);
	if (th0.joinable()) {
		th0.join();
	}
	if (th1.joinable()) {
		th1.join();
	}
	CHECK(send_cnt == LOOP);
	CHECK(recv_cnt == LOOP);
	CHECK(g_queue.size() == LOOP);
	int cnt = 0;
	char buf[SIZE] = "";
	while (g_queue.empty()) {
		my_class obj;
		obj = g_queue.front();
		g_queue.pop();
		snprintf(buf, SIZE, "AAA%d", cnt);
		CHECK(obj.my_int == cnt);
		CHECK(obj.my_string == buf);
		++cnt;
	}
}
