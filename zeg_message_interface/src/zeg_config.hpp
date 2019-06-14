/*****************************************************************************
*  zeg configuration entry                                                   *
*  Copyright (C) 2019                                                        *
*                                                                            *
*  @file     zeg_config.hpp                                                  *
*  @brief    zeg configuration entry                                         *
*  Details.                                                                  *
*                                                                            *
*  @author                                                                   *
*  @email                                                                    *
*  @version  1.0.0                                                           *
*  @date     2019-06-11                                                      *
*  @license                                                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019/05/27 | 1.0.0     |                | Create file                     *
*----------------------------------------------------------------------------*
*  2019/05/29 | 1.0.0     |                | Add stat info                   *
*----------------------------------------------------------------------------*
*  2019/05/30 | 1.0.0     |                | Add thread pool                 *
*----------------------------------------------------------------------------*
*  2019/06/11 | 1.0.0     |                | Add simulator queue             *
*----------------------------------------------------------------------------*                                                                   *
*****************************************************************************/
#ifndef SRC_ZEG_CONFIG_HPP_
#define SRC_ZEG_CONFIG_HPP_
#include <sys/stat.h>
#include <stdint.h>
#include <string>
#include <atomic>
#include <fstream>
#include <future>
#include "blockingconcurrentqueue.h"
#include "NanoLog.hpp"
#include "zeg_data_define.h"
#include "thread_pool.hpp"
#include "config_parser.hpp"
#include "zeg_robot_define.hpp"
namespace zeg_message_interface {
using namespace std;
using namespace moodycamel;
using namespace tp;
using namespace zeg_robot_simulator;
class zeg_config {
private:
	zeg_config() {
		robot_simulator_msecs = 100;
		init_log();
		init_conf();
	}
	virtual ~zeg_config() = default;
public:
	inline static zeg_config &get_instance() {
		return config_;
	}
public:
	void init_conf() {
		if (false == config_parser::config_parser::get_instance().init(conf_path)) {
			return;
		}
		vector<string>values{""};
		config_parser::config_parser::get_instance().get_value("scheduler_server", "pose_upload_address", values);
		pose_upload_address = values[0];
		if (pose_upload_address.empty()) {
			pose_upload_address = SCHEDULER_SERVER_SIMULATOR_ADDRESS;
		}
		config_parser::config_parser::get_instance().get_value("robot_simulator", "msecs", values);
		robot_simulator_msecs = atoi(values[0].c_str());
		check_msecs();
	}
private:
	inline void init_log() {
		mkdir(zeg_log_path, 777);
		nanolog::initialize(nanolog::GuaranteedLogger(), zeg_log_path, zeg_log_file , max_log_size);
		nanolog::set_log_level(nanolog::LogLevel::INFO);
	}
	inline void check_msecs() {
		if (robot_simulator_msecs <= 0 || robot_simulator_msecs > 10000) {
			robot_simulator_msecs = 100;
		}
	}
public:
	static zeg_config config_;
public:
	const char *g_server_address = "tcp://localhost:9141";
	const char *g_local_address = "tcp://*:9142";
	const char *local_navigate_address = "tcp://*:9143";
	const char *test_navigate_address = "tcp://localhost:9143";
	const char *SCHEDULER_SERVER_SIMULATOR_ADDRESS = "tcp://192.168.4.6:9144";
	const char *zeg_log_path = "/opt/log";
	const char *zeg_log_file = "zeg_message_interface_log";
	const uint32_t max_log_size = 1;	// 1M
	const int max_queue_size = 1000;
	const int stat_log_interval = 10;
public:
	const int RPC_SERVER_UPLOAD_POSE_PORT = 9001;
	int robot_simulator_msecs;
	string pose_upload_address;
	atomic<uint64_t>recv_navigate_cmd_counter_;
public:
	BlockingConcurrentQueue<string>navigate_cmd_queue;
	ConcurrentQueue<robot_pose>simulator_pose_queue;
	ThreadPool thread_pool;
private:
	const char *conf_path = "../etc/zeg_message_interface.conf";
};
zeg_config zeg_config::config_;
}

#endif /* SRC_ZEG_CONFIG_HPP_ */
