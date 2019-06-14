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
*  @date     2019-06-12                                                      *
*  @license                                                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019/05/27 | 1.0.0     |                | Create file                     *
*----------------------------------------------------------------------------*
*  2019/05/28 | 1.0.0     |                | Delete reluctant variables      *
*----------------------------------------------------------------------------*
*  2019/06/11 | 1.0.0     |                | Add send simulator thread       *
*----------------------------------------------------------------------------*
*  2019/06/12 | 1.0.0     |                | Add rpc server                  *
*****************************************************************************/
#include <string.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "zmq_agent.hpp"
#include "zeg_recv_navigate.hpp"
#include "zeg_stat_output.hpp"
#include "zeg_post_navigate.hpp"
#include "zeg_send_simulator.hpp"
#include "zeg_upload_pose.hpp"
#include "rpc_server.h"
using namespace rest_rpc;
using namespace rpc_service;
using namespace zmq_self_agent;
using namespace zeg_message_interface;
zeg_recv_navigate zeg_recv_navigate_thread;
zeg_stat_output zeg_stat_output_thread;
zeg_post_navigate zeg_post_navigate_thread;
zeg_send_simulator zeg_send_simulator_thread;
zeg_upload_pose zeg_upload_pose_obj;
bool start_thread() {
	if (false == zeg_recv_navigate_thread.init()) {
		return false;
	}
	if (false == zeg_post_navigate_thread.init()) {
		return false;
	}
	if (false == zeg_send_simulator_thread.init()) {
		return false;
	}
	zeg_recv_navigate_thread.run();
	zeg_stat_output_thread.run();
	zeg_post_navigate_thread.run();
	zeg_send_simulator_thread.run();
	return true;
}
void join_thread() {
	zeg_recv_navigate_thread.join();
	zeg_stat_output_thread.join();
	zeg_post_navigate_thread.join();
	zeg_send_simulator_thread.join();
}
bool init_object() {
	if (zeg_upload_pose_obj.init()) {
		return false;
	}
	return true;
}
int main() {
	/*if (false == start_thread()) {
		return -1;
	}*/
	if (init_object()) {
		return -1;
	}
	rpc_server zeg_upload_pose_server(zeg_config::get_instance().RPC_SERVER_UPLOAD_POSE_PORT, thread::hardware_concurrency(), 0);
	zeg_upload_pose_server.register_handler("upload_pose", &zeg_upload_pose::upload_pose, &zeg_upload_pose_obj);
	zeg_upload_pose_server.run();
	//join_thread();
	return 0;
}



