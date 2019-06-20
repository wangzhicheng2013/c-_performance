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
	zeg_robot_broadcast() : rpc_client_ptr_(new rpc_client) ,
				udp_broadcast_client_ptr_(message_communicate_entity_maker::make_unique_ptr("udp.broadcast.client")) {
		rpc_connected_ = false;

	}
	bool init() {
		if (nullptr == rpc_client_ptr_ || nullptr == udp_broadcast_client_ptr_) {
			return false;
		}
		rpc_connected_ = rpc_client_ptr_->connect(zeg_config::zeg_config::get_instance().RPC_SERVER_IP, zeg_config::get_instance().RPC_SERVER_ROBOT_SIMULATOR_PORT, 5);
		return udp_broadcast_client_ptr_->init();
	}
    bool mock_get_robot_basic_info(robot_basic_info &info) {
        bool no_exception = true;
        bool res = true;
        if (false == rpc_connected_) {
            rpc_connected_ = rpc_client_ptr_->connect(zeg_config::get_instance().RPC_SERVER_IP, zeg_config::get_instance().RPC_SERVER_ROBOT_SIMULATOR_PORT, 5);
        }
        try {
            info = rpc_client_ptr_->call<robot_basic_info>("get_robot_basic_info");
        }
        catch (const std::exception& e) {
            LOG_CRIT << e.what();
            no_exception = false;
        }
        return no_exception;
    }
	void mock_send_pack() {
		zeg_robot_header header("zeg.robot.basic.info", "zeg_robot_xx011212DD1", chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
		msgpack::pack(buffer_header_, header);
		robot_basic_info body(0, 0, 0.1, 0, 99.8);
		msgpack::pack(buffer_body_, body);
		char buf[1024] = "";
		memcpy(buf,  buffer_header_.data(), buffer_header_.size());
		memcpy(buf + buffer_header_.size(),  buffer_body_.data(), buffer_body_.size());
		int len = buffer_header_.size() + buffer_body_.size();
		LOG_INFO << udp_broadcast_client_ptr_->send(buf, len);
	}
protected:
	virtual void process() override {
		while (true) {
			this_thread::sleep_for(chrono::microseconds(zeg_config::get_instance().robot_broadcast_interval));
			mock_send_pack();
		}
	}
private:
	unique_ptr<rpc_client>rpc_client_ptr_;
	unique_ptr<message_communicate_entity>udp_broadcast_client_ptr_;
	msgpack::sbuffer buffer_header_;
	msgpack::sbuffer buffer_body_;
private:
	bool rpc_connected_;
};
}


#endif /* SRC_ZEG_ROBOT_BROADCAST_HPP_ */
