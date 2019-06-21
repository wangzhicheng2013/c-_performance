#include <string.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "rpc_server.h"
#include "zeg_recv_command.hpp"
#include "zeg_command_processor.hpp"
using namespace rest_rpc;
using namespace rpc_service;
using namespace zeg_message_interface;
string process_command(rpc_conn conn, const string &str_buf) {
	string ack_str;
	zeg_command_processor::get_instance().process(str_buf.c_str(), str_buf.size(), ack_str);
	return ack_str;
}
zeg_recv_command zeg_recv_command_thread;
bool start_thread() {
	if (false == zeg_recv_command_thread.init()) {
		return false;
	}
	zeg_recv_command_thread.run();
	return true;
}
void join_thread() {
	zeg_recv_command_thread.join();
}
int main() {
	if (false == zeg_config::get_instance().init()) {
		LOG_CRIT << "zeg config init failed...!";
		return -1;
	}
	if (false == start_thread()) {
		return -1;
	}
	rpc_server server(zeg_config::get_instance().robot_rpc_message_interface_layer_port, thread::hardware_concurrency(), 0, 1);
	server.register_handler("process_command", process_command);
	server.run();
	join_thread();

	return 0;
}


