/*****************************************************************************
*  main process entry                                                        *
*  Copyright (C) 2019                                                        *
*                                                                            *
*  @file     main.cpp                                                        *
*  @brief    message interface main process entry                            *
*  Details.                                                                  *
*                                                                            *
*  @author                                                                   *
*  @email                                                                    *
*  @version  1.0.0                                                           *
*  @date     2019-05-27                                                      *
*  @license                                                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019/05/27 | 1.0.0     |                | Create file                     *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#include "zmq/zmq.h"
#include "msgpack/msgpack.hpp"
#include <string.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
using namespace std;
static const int SIZE = 1024;
static const int LOOP = 100;
//const char *g_server_address = "tcp://192.168.4.6:9141";
const char *g_server_address = "tcp://localhost:9141";
const char *g_local_address = "tcp://*:9141";
char g_send_buf[SIZE] = "";
char g_recv_buf[SIZE] = "";
msgpack::sbuffer buffer;
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
	cout << "my_int = " << my_obj.my_int << endl;
	cout << "my_string = " << my_obj.my_string << endl;
}
bool send_thread() {
	this_thread::sleep_for(chrono::seconds(4));
	void *context = zmq_ctx_new();
	void *client = zmq_socket(context, ZMQ_PUSH);
	if (nullptr == context || nullptr == client) {
		cerr << "client socket init failed...!" << endl;
		return false;
	}
	zmq_connect(client, g_server_address);
	int sendcnt = 0;
	for (int i = 0;i < LOOP;i++) {
		zmq_msg_t message = {0};
		pack(i);
		int size = buffer.size();
		zmq_msg_init_size(&message, size);
		memcpy(zmq_msg_data(&message), buffer.data(), size);
		if (size == zmq_msg_send(&message, client, 0)) {
			++sendcnt;
		}
		else {
			cerr << "send failed...!" << endl;
		}
		zmq_msg_close(&message);
	}
	zmq_close(client);
	zmq_term(context);
	cout << sendcnt << " message have been sent..!" << endl;
	return true;
}
bool recv_thread() {
	void *context = zmq_ctx_new();
	void *server = zmq_socket(context, ZMQ_PULL);
	if (nullptr == context || nullptr == server) {
		cerr << "server socket init failed...!" << endl;
		return false;
	}
	zmq_bind(server, g_local_address);
	this_thread::sleep_for(chrono::seconds(2));
	int recv_cnt = 0;
	while (true) {
		zmq_msg_t message = {0};
		zmq_msg_init(&message);
		int len = zmq_msg_recv(&message, server, 0);
		cout << "recv length = " << len << endl;
		cout << "recv count = " << ++recv_cnt << endl;
		unpack((char *)zmq_msg_data(&message), len);
		zmq_msg_close(&message);
	}
	zmq_close(server);
	zmq_term(context);
	return true;
}
int main() {
	thread th0(send_thread);
	thread th1(recv_thread);
	if (th0.joinable()) {
		th0.join();
	}
	if (th1.joinable()) {
		th1.join();
	}
	return 0;
}


