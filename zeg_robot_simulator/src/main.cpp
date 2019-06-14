/*****************************************************************************
*  main process entry                                                        *
*  Copyright (C) 2019                                                        *
*                                                                            *
*  @file     main.cpp.cpp                                                    *
*  @brief    rpc server						                                 *
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
*  2019/06/03 | 1.0.0     |                | Create file                     *
*----------------------------------------------------------------------------*
*  2019/06/06 | 1.0.0     |                | Add get robot msecs             *
*****************************************************************************/
#include <mutex>
#include "zeg_pose_compute.hpp"
#include "zeg_config.hpp"
#include "zeg_data_define.h"
#include "rpc_server.h"
#include "zeg_robot_navigation.hpp"
#include "zeg_robot_poses.hpp"
using namespace rest_rpc;
using namespace rpc_service;
using namespace zeg_robot_simulator;
pose_compute pose_compute_obj;
mutex global_lock;
class zeg_mock_navigate_server {
public:
	uint64_t get_taskid(rpc_conn conn, const znavigate_command &cmd) {
		return cmd.task_id;
	}
};
const robot_pose &get_cur_pose(rpc_conn conn) {
	return pose_compute_obj.cur_pose_;
}
void set_cur_pose(rpc_conn conn) {
	lock_guard<mutex>lk(global_lock);
	pose_compute_obj.cur_pose_ = zeg_config::get_instance().cur_pose_;
}
int get_robot_msecs(rpc_conn conn) {
	return pose_compute_obj.msecs;
}
vector<robot_pose> get_pose_trace(rpc_conn conn, const vector<robot_pose>&pose_set) {
	vector<robot_pose>pose_trace;
	lock_guard<mutex>lk(global_lock);
	if (false == pose_compute_obj.get_pose_trace_with_angle(pose_set, pose_trace)) {
		pose_trace.clear();
	}
	return pose_trace;
}
vector<robot_pose> get_pose_trace1(rpc_conn conn, const robot_pose &target_pose) {
	vector<robot_pose>pose_trace;
	lock_guard<mutex>lk(global_lock);
	if (false == pose_compute_obj.get_pose_trace_with_angle(target_pose, pose_trace)) {
		pose_trace.clear();
	}
	return pose_trace;
}
// todo
bool post_poses_to_simulator(rpc_conn conn, const vector<robot_pose>&pose_set) {
	return zeg_robot_poses::get_instance().update_robot_poses(pose_set);
}
vector<zeg_robot_navigation>zeg_robot_navigation_objs;
bool start_simulator() {
	int num = zeg_config::get_instance().vechile_num;
	zeg_robot_navigation_objs.resize(num);
	for (int i = 0;i < num;++i) {
		if (false == zeg_robot_navigation_objs[i].init()) {
			return false;
		}
		zeg_robot_navigation_objs[i].set_vehicle_id(i);
		zeg_robot_navigation_objs[i].run();
	}
	return true;
}
void end_simulator() {
	for (auto &zeg_robot_navigation_obj : zeg_robot_navigation_objs) {
		zeg_robot_navigation_obj.join();
	}
}
int main() {
	zeg_config::get_instance().init();
	pose_compute_obj.msecs = zeg_config::get_instance().msecs;
	pose_compute_obj.speed_ = zeg_config::get_instance().speed_;
	rpc_server robot_simulator_server(zeg_config::get_instance().RPC_SERVER_ROBOT_SIMULATOR_PORT, thread::hardware_concurrency(), 0);

	robot_simulator_server.register_handler("get_cur_pose", get_cur_pose);
	robot_simulator_server.register_handler("set_cur_pose", set_cur_pose);
	robot_simulator_server.register_handler("get_robot_msecs", get_robot_msecs);
	robot_simulator_server.register_handler("get_pose_trace", get_pose_trace);
	robot_simulator_server.register_handler("get_pose_trace1", get_pose_trace1);
	robot_simulator_server.register_handler("post_poses_to_simulator", post_poses_to_simulator);

	zeg_mock_navigate_server obj;
	robot_simulator_server.register_handler("get_taskid", &zeg_mock_navigate_server::get_taskid, &obj);
	if (false == start_simulator()) {
		LOG_CRIT << "start simulator failed...!";
		return -1;
	}
	robot_simulator_server.run();
	end_simulator();

	return 0;
}

