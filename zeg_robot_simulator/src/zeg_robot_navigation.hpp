/*****************************************************************************
*  zeg robot navigation                                                      *
*  Copyright (C) 2019                                                        *
*                                                                            *
*  @file     zeg_robot_navigation.hpp                                        *
*  @brief    send navigation command                                         *
*  Details.                                                                  *
*                                                                            *
*  @author                                                                   *
*  @email                                                                    *
*  @version  1.0.0                                                           *
*  @date     2019-06-14                                                      *
*  @license                                                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019/06/14 | 1.0.0     |                | Create file                     *
*****************************************************************************/
#ifndef SRC_ZEG_ROBOT_NAVIGATION_HPP_
#define SRC_ZEG_ROBOT_NAVIGATION_HPP_
#include "base_thread.hpp"
#include "zeg_pose_compute.hpp"
#include "zeg_robot_vehicle.hpp"
#include "zeg_robot_poses.hpp"
#include "rpc_client.hpp"
#include "codec.h"
namespace zeg_robot_simulator {
using namespace rest_rpc;
using namespace rest_rpc::rpc_service;
class zeg_robot_navigation : public base_thread {
public:
	bool init() {
		return true;
	}
	void set_vehicle_id(int id) {
		vehicle_.vechicle_id_ = id;
	}
	bool upload_cur_pose() {
		static rpc_client client(zeg_config::get_instance().RPC_SERVER_IP, zeg_config::get_instance().RPC_SERVER_UPLOAD_POSE_PORT);
		static bool is_connect = false;
		if (false == connect_rpc(client, is_connect)) {
			return false;
		}
		bool no_exception = true;
		bool res = true;
	    try {
	    	res = client.call<bool>("upload_pose", vehicle_.vehicle_cur_pose_);
	    }
	    catch (const std::exception& e) {
	    	LOG_CRIT << e.what();
	    	no_exception = false;
	    }
		return no_exception && res;
	}
	void update_cur_pose(const unsigned char cmd) {
		switch (cmd) {
		case ROTATE_COMMAND:
			vehicle_.rotate();
			break;
		case MOVE_COMMAND:
			vehicle_.move();
			break;
		}
	}
protected:
	virtual void todo() override {
		robot_pose target_pose;
		bool target_reach = false;
		while (true) {
			/*if (zeg_robot_poses::get_instance().is_re_navigate()) {
				zeg_robot_poses::get_instance().get_robot_pose(target_pose);
			}
			else if (true == target_reach) {
				zeg_robot_poses::get_instance().get_robot_pose(target_pose);
			}
			else if (ROBOT_VEHICLE_RUN == vehicle_.vechicle_status_) {	// pose compute

			}*/
			this_thread::sleep_for(chrono::microseconds(vehicle_.msecs));
			if (false == upload_cur_pose()) {
				LOG_CRIT << "simulator upload pose failed...!";
			}
		}
	}
private:
	static inline bool connect_rpc(rpc_client &client, bool &is_connect) {
		if (false == is_connect) {
			is_connect = client.connect();
			if (false == is_connect) {
				LOG_CRIT << "connect to rpc server ip = " << zeg_config::get_instance().RPC_SERVER_IP << " port = " << zeg_config::get_instance().RPC_SERVER_UPLOAD_POSE_PORT << "failed...!";
				return false;
			}
		}
		return true;
	}
private:
	zeg_robot_vehicle vehicle_;
};
}


#endif /* SRC_ZEG_ROBOT_NAVIGATION_HPP_ */
