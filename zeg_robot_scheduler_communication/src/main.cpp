#include "rpc_server.h"
#include "udp_server.hpp"
#include "zeg_robot_command_processor.hpp"
#include "zeg_robot_task_escort.hpp"
using namespace rest_rpc;
using namespace rpc_service;
using namespace zeg_robot_scheduler_communication;
auto udp_server_ptr = message_communicate_entity_maker::make_unique_ptr("udp.server");
bool report_task_to_robot(rpc_conn conn, const zeg_robot_header &header, const zeg_robot_task &robot_task) {
	/*string pack_str;
	zeg_robot_command_pack<zeg_robot_task>::get_instance().pack(header, robot_task, pack_str);
	auto client_ptr = zeg_robot_update_address::get().get(header.robot_id);
	if (nullptr != client_ptr) {
		udp_server_ptr->s
	}*/
	// pack(header, robot_task)
	// udp send pack
	// store pack into map <string -- header, pack>
	// return true
	// 1.store message into map key -- robot id value -- pair<header, robot_task>
	// 2.threads fetch message from qu
	// 3.send message with udp
	return true;
}
bool init_udp_server() {
	if (nullptr == udp_server_ptr) {
		return false;
	}
	socket_config config;
	config.port_ = zeg_config::get_instance().udp_server_port;
	reinterpret_cast<udp_server *>(udp_server_ptr.get())->set_config(config);
	if (false == udp_server_ptr->init()) {
		return false;
	}
	return true;
}
int main() {
	if (false == zeg_config::get_instance().init()) {
		LOG_CRIT << "zeg config init failed...!";
		return -1;
	}
	if (false == init_udp_server()) {
		return -1;
	}
	rpc_server server(zeg_config::get_instance().robot_rpc_scheduler_communication_port, thread::hardware_concurrency(), 0, 1);
	server.register_handler("report_task_to_robot", report_task_to_robot);
	server.run();

	return 0;
}



