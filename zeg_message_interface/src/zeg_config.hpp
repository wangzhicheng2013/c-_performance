#ifndef SRC_ZEG_CONFIG_HPP_
#define SRC_ZEG_CONFIG_HPP_
#include <sys/stat.h>
#include <stdint.h>
#include <string>
#include <mutex>
#include "NanoLog.hpp"
#include "config_parser.hpp"
#include "zeg_robot_define.hpp"
#include "udp_unicast_server.hpp"
namespace zeg_message_interface {
using namespace std;
class zeg_config {
private:
	zeg_config() : udp_unicast_server_ptr_(message_communicate_entity_maker::make_unique_ptr("udp.unicast.server")) {
		init_log();
		init_conf();
	}
	virtual ~zeg_config() = default;
public:
	inline static zeg_config &get_instance() {
		return config_;
	}
	bool init() {
		return init_udp();
	}
public:
	void init_conf() {
		if (false == config_parser::config_parser::get_instance().init(conf_path)) {
			return;
		}
		get_udp_server_config();
		get_rpc_config();
		get_schedule_server_config();
		get_robot_test_config();
	}
private:
	inline bool init_udp() {
		reinterpret_cast<udp_unicast_server *>(udp_unicast_server_ptr_.get())->set_port(udp_server_port);
		reinterpret_cast<udp_unicast_server *>(udp_unicast_server_ptr_.get())->make_scheduler_client_addr(schedule_server_ip.c_str(), schedule_server_port);
		return udp_unicast_server_ptr_->init();
	}
	inline void init_log() {
		mkdir(zeg_log_path, 777);
		nanolog::initialize(nanolog::GuaranteedLogger(), zeg_log_path, zeg_log_file , max_log_size);
		nanolog::set_log_level(nanolog::LogLevel::INFO);
	}
	inline void get_udp_server_config() {
		vector<string>values{""};
		config_parser::config_parser::get_instance().get_value("udp_server", "port", values);
		udp_server_port = atoi(values[0].c_str());
		if (udp_server_port <= 0) {
			udp_server_port = 7780;
		}
	}
	inline void get_rpc_config() {
		vector<string>values{""};
		config_parser::config_parser::get_instance().get_value("robot_rpc", "navigation_escort_layer_port", values);
		robot_rpc_navigation_escort_layer_port = atoi(values[0].c_str());
		if (robot_rpc_navigation_escort_layer_port <= 0) {
			robot_rpc_navigation_escort_layer_port = 9003;
		}
		config_parser::config_parser::get_instance().get_value("robot_rpc", "message_interface_layer_port", values);
		robot_rpc_message_interface_layer_port = atoi(values[0].c_str());
		if (robot_rpc_message_interface_layer_port <= 0) {
			robot_rpc_message_interface_layer_port = 9001;
		}
	}
	void get_robot_test_config() {
		vector<string>values{""};
		config_parser::config_parser::get_instance().get_value("robot_test", "navigation_escort_path", values);
		robot_test_navigation_escort_path = values[0];
		if (robot_test_navigation_escort_path.empty()) {
			robot_test_navigation_escort_path = "/opt/zeg_robot_navigation_escort/zeg_robot_navigation_escort/bin/zeg_robot_navigation_escort";
		}
		config_parser::config_parser::get_instance().get_value("robot_test", "navigation_escort_name", values);
		robot_test_navigation_escort_name = values[0];
		if (robot_test_navigation_escort_name.empty()) {
			robot_test_navigation_escort_name = "zeg_robot_navigation_escort";
		}

		config_parser::config_parser::get_instance().get_value("robot_test", "message_interface_path", values);
		robot_test_message_interface_path = values[0];
		if (robot_test_message_interface_path.empty()) {
			robot_test_message_interface_path = "/opt/zeg_message_interface/zeg_message_interface/bin/zeg_message_interface";
		}
		config_parser::config_parser::get_instance().get_value("robot_test", "message_interface_name", values);
		robot_test_message_interface_name = values[0];
		if (robot_test_message_interface_name.empty()) {
			robot_test_message_interface_name = "zeg_message_interface";
		}
	}
	inline void get_schedule_server_config() {
		vector<string>values{""};
		config_parser::config_parser::get_instance().get_value("schedule_server", "ip", values);
		schedule_server_ip = values[0];
		config_parser::config_parser::get_instance().get_value("schedule_server", "port", values);
		schedule_server_port = atoi(values[0].c_str());
		if (schedule_server_port <= 0) {
			schedule_server_port = 7780;
		}
	}
private:
	static zeg_config config_;
public:
	int udp_server_port;
	int robot_rpc_navigation_escort_layer_port;
	int robot_rpc_message_interface_layer_port;
	int schedule_server_port;

	string robot_test_navigation_escort_path;
	string robot_test_navigation_escort_name;
	string robot_test_message_interface_path;
	string robot_test_message_interface_name;
	string schedule_server_ip;
	unique_ptr<message_communicate_entity>udp_unicast_server_ptr_;
public:
	const char *RPC_SERVER_IP = "127.0.0.1";
	const uint32_t buffer_size = 1024;
private:
	const char *conf_path = "../etc/zeg_message_interface.conf";
	const char *zeg_log_path = "/opt/log";
	const char *zeg_log_file = "zeg_message_interface_log";
	const uint32_t max_log_size = 1;	// 1M
};
zeg_config zeg_config::config_;
}

#endif /* SRC_ZEG_CONFIG_HPP_ */
