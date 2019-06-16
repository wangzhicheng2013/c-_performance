#ifndef SRC_ZEG_ROBOT_NAVIGATION_HPP_
#define SRC_ZEG_ROBOT_NAVIGATION_HPP_
#include <atomic>
#include "base_thread.hpp"
#include "zeg_pose_compute.hpp"
#include "zeg_robot_vehicle.hpp"
#include "zeg_robot_poses.hpp"
#include "mutex_deque.hpp"
#include "rpc_client.hpp"
#include "codec.h"
namespace zeg_robot_simulator {
using namespace rest_rpc;
using namespace rest_rpc::rpc_service;
class zeg_robot_navigation : public base_thread {
public:
	zeg_robot_navigation() : client_ptr_(new rpc_client) {
	}
	bool init() {
		if (nullptr == client_ptr_) {
			return false;
		}
		return client_ptr_->connect(zeg_config::get_instance().RPC_SERVER_IP, zeg_config::get_instance().RPC_SERVER_REPORT_POSE_PORT, 5);
	}
	inline void set_vehicle_id(int id) {
		vehicle_.vechicle_id_ = id;
	}
	bool report_cur_pose() {
		bool no_exception = true;
		bool res = true;
	    try {
	    	res = client_ptr_->call<bool>("report_pose", vehicle_.vehicle_cur_pose_);
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
	inline bool update_way_points(const vector<robot_pose>&way_points) {
		return way_points_.update_deque(way_points);
	}
	void navigate() {
		update_target_pose();
		status_machine_change();
	}
	inline bool need_to_rotate() {
		if (0 == vehicle_.vehicle_speed_.w) {
			return false;
		}
		double theta = direction(vehicle_.vehicle_cur_pose_, target_pose_);
		double normalized_value = normalize(theta - vehicle_.vehicle_cur_pose_.theta);
		if (fabs(normalized_value) <= vehicle_.vehicle_speed_.w * vehicle_.msecs_ / 1000) {
			vehicle_.vehicle_cur_pose_.theta = theta;
			return false;
		}
		return true;
	}
	void status_machine_change() {
		double theta = 0;
		unsigned char cmd = ROBOT_VEHICLE_STOP;
		if (true == need_to_rotate()) {
			cmd = ROBOT_VEHICLE_ROTATE;
		}
		else {
			cmd = ROBOT_VEHICLE_MOVE;
		}
		update_cur_pose(cmd);
	}
	inline void update_target_pose() {
		if (DEQUE_IS_UPDATED == way_points_.get_status()) {
			vehicle_.vechicle_status_ = ROBOT_VEHICLE_STOP;
			way_points_.pop_front(target_pose_);
			way_points_.set_status(DEQUE_IS_UNUPDATED);
			return;
		}
		double d = distance(vehicle_.vehicle_cur_pose_, target_pose_);
		if (equal(d, 0.0)) {
			way_points_.pop_front(target_pose_);
			vehicle_.vechicle_status_ = ROBOT_VEHICLE_STOP;
		}
	}
protected:
	virtual void process() override {
		while (true) {
			if (false == way_points_.empty()){
				navigate();
			}
			this_thread::sleep_for(chrono::microseconds(vehicle_.msecs_));
			if (false == report_cur_pose()) {
				LOG_CRIT << "simulator upload pose failed...!";
			}
		}
	}
public:
	zeg_robot_vehicle vehicle_;
	robot_pose target_pose_;
	mutex_deque<robot_pose>way_points_;
	unique_ptr<rpc_client>client_ptr_;
};
}


#endif /* SRC_ZEG_ROBOT_NAVIGATION_HPP_ */
