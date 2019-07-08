#include <string.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "rpc_server.h"
#include "zeg_robot_navigation_lock_point_sender.hpp"
#include "zeg_robot_basic_info_sender.hpp"
#include "udp_server.hpp"
#include "zeg_robot_command_pack.hpp"
#include "zeg_robot_command_processor.hpp"
#include "zeg_robot_command_pack.hpp"
using namespace rest_rpc;
using namespace rpc_service;
using namespace zeg_message_interface;
auto udp_server_ptr = message_communicate_entity_maker::make_unique_ptr("udp.server");
bool report_task_ack_to_scheduler(rpc_conn conn, const zeg_robot_header &header, const zeg_robot_task_ack &robot_task_ack) {
	string pack_str;
	zeg_robot_command_pack<zeg_robot_task_ack>::get_instance().pack(header, robot_task_ack, pack_str);
	int size = udp_server_ptr->send(pack_str.c_str(), pack_str.size());
	if (size > 1024) {
		LOG_CRIT << "send size over 1K.";
	}
	LOG_INFO << "send lock point size = " << size;
	if (pack_str.size() != size) {
		LOG_CRIT << "send size = " << size << " need send size = " << pack_str.size();
		return false;
	}
	return true;
}
bool send_navigation_lock_point(rpc_conn conn, const zeg_robot_header &header, const zeg_robot_navigation_lock_point &body) {
	return zeg_robot_navigation_lock_point_sender::get_instance().send(header, body, udp_server_ptr);
}
bool report_robot_basic_info(rpc_conn conn, const zeg_robot_header &header, const zeg_robot_basic_info &body) {
	return zeg_robot_basic_info_sender::get_instance().send(header, body);
}
int main() {
	if (false == zeg_config::get_instance().init()) {
		LOG_CRIT << "zeg config init failed...!";
		return -1;
	}
	if (nullptr == udp_server_ptr || (false == udp_server_ptr->init())) {
		return -1;
	}
	rpc_server server(zeg_config::get_instance().robot_rpc_message_interface_layer_port, thread::hardware_concurrency(), 0, 1);
	server.register_handler("send_navigation_lock_point", send_navigation_lock_point);
	server.register_handler("report_robot_basic_info", report_robot_basic_info);
	server.run();

	return 0;
}


