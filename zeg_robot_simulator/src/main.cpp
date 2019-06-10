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
#include "zeg_robot_define.hpp"
#include "zeg_config.hpp"
#include "zeg_data_define.h"
#include "rpc_server.h"
using namespace rest_rpc;
using namespace rpc_service;
using namespace zeg_robot_simulator;
using namespace zeg_message_interface;
pose_compute pose_compute_obj;
class zeg_mock_navigate_server {
public:
        uint64_t get_taskid(rpc_conn conn, const znavigate_command &cmd) {
                return cmd.task_id;
        }
};
const robot_pose &get_cur_pose(rpc_conn conn) {
	return pose_compute_obj.cur_pose_;
}
int get_robot_msecs(rpc_conn conn) {
	return pose_compute_obj.msecs;
}
vector<robot_pose> get_pose_trace(rpc_conn conn, const vector<robot_pose>&pose_set) {
	vector<robot_pose>pose_trace;
	if (false == pose_compute_obj.get_pose_trace_with_angle(pose_set, pose_trace)) {
		pose_trace.clear();
	}
	return pose_trace;
}
int main() {
	pose_compute_obj.msecs = 100;
	pose_compute_obj.speed_ = {1, 0, 0.1};
	rpc_server server(zeg_config::get_instance().RPC_SERVER_PORT, thread::hardware_concurrency());
	server.register_handler("get_cur_pose", get_cur_pose);
	server.register_handler("get_robot_msecs", get_robot_msecs);
	server.register_handler("get_pose_trace", get_pose_trace);
	zeg_mock_navigate_server obj;
	server.register_handler("get_taskid", &zeg_mock_navigate_server::get_taskid, &obj);
	server.run();

	return 0;
}




