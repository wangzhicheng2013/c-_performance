#ifndef SRC_ZEG_ROBOT_BROADCAST_HPP_
#define SRC_ZEG_ROBOT_BROADCAST_HPP_
#include "base_thread.hpp"
#include "udp_broadcast_client.hpp"
#include "zeg_robot_define.hpp"
#include "zeg_config.hpp"
#include "rpc_client.hpp"
#include "codec.h"
namespace zeg_robot_maintainence {
using namespace rest_rpc;
using namespace rest_rpc::rpc_service;
class zeg_robot_broadcast : public base_thread {
public:
	zeg_robot_broadcast() : client_ptr_(new rpc_client) {
		rpc_connected_ = false;
	}
	bool init() {
		if (nullptr == client_ptr_) {
			return false;
		}
		rpc_connected_ = client_ptr_->connect(zeg_config::zeg_config::get_instance().RPC_SERVER_IP, zeg_config::get_instance().RPC_SERVER_ROBOT_SIMULATOR_PORT, 5);
		return udp_broadcast_client_.init();
	}
	bool get_vehicle_ids(vector<string>&ids) {
		bool no_exception = true;
		bool res = true;
		if (false == ids.empty()) {
			return true;
		}
		if (false == rpc_connected_) {
			rpc_connected_ = client_ptr_->connect(zeg_config::get_instance().RPC_SERVER_IP, zeg_config::get_instance().RPC_SERVER_ROBOT_SIMULATOR_PORT, 5);
		}
	    try {
	    	ids = client_ptr_->call<vector<string>>("get_vehicle_ids");
	    }
	    catch (const std::exception& e) {
	    	LOG_CRIT << e.what();
	    	no_exception = false;
	    }
		return no_exception && !ids.empty();
	}
	void mock_send_pack() {
		robot_basic_info info;
		info.battery_percentage = 100;
		info.protocol = "zeg_robot";
		info.state = 0;
		info.x = info.y = info.theta = NAN;
		info.vehicle_id = "0";
		info.timestamp = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
		buffer_.clear();
		msgpack::pack(buffer_, info);
		cout << "send count = " << udp_broadcast_client_.send(buffer_.data(), buffer_.size()) << endl;
	}
protected:
	virtual void process() override {
		while (true) {
			this_thread::sleep_for(chrono::microseconds(zeg_config::get_instance().robot_broadcast_interval));
			mock_send_pack();
		}
	}
public:
	vector<string>vechicle_ids_;
	unique_ptr<rpc_client>client_ptr_;
	udp_broadcast_client udp_broadcast_client_;
	msgpack::sbuffer buffer_;
private:
	bool rpc_connected_;
};
}


#endif /* SRC_ZEG_ROBOT_BROADCAST_HPP_ */
