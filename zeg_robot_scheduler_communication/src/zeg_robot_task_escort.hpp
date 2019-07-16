#ifndef SRC_ZEG_ROBOT_TASK_ESCORT_HPP_
#define SRC_ZEG_ROBOT_TASK_ESCORT_HPP_
#include "zeg_robot_command_unpack.hpp"
#include "udp_server.hpp"
namespace zeg_robot_scheduler_communication {
class zeg_robot_task_escort {
public:
	inline static zeg_robot_task_escort &get() {
		static zeg_robot_task_escort obj;
		return obj;
	}
	void send_task_to_robot(const zeg_robot_header &header,
			const zeg_robot_task &robot_task, unique_ptr<message_communicate_entity>&udp_server_ptr) {
		string pack_str;
		zeg_robot_command_pack<zeg_robot_task>::get_instance().pack(header, robot_task, pack_str);
		auto client_ptr = zeg_robot_update_address::get().get(header.robot_id);
		reinterpret_cast<udp_server *>(udp_server_ptr.get())->send_to_client(pack_str.c_str(), pack_str.size(), *client_ptr);
	}
private:
	zeg_robot_task_escort() = default;
	~zeg_robot_task_escort() = default;
};
}



#endif /* SRC_ZEG_ROBOT_TASK_ESCORT_HPP_ */
