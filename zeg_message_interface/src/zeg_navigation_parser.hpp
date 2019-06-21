#ifndef SRC_ZEG_NAVIGATION_PARSER_HPP_
#define SRC_ZEG_NAVIGATION_PARSER_HPP_
#include "zeg_command_parser_maker.hpp"
namespace zeg_message_interface {
class zeg_navigation_parser : public zeg_command_parser {
public:
public:
	virtual bool parse(zeg_command_unpack_struct *unpack_cmd, string &ack_str) override {
		zeg_robot_navigation_command cmd;
		msgpack::unpack(unpack_cmd->unpack_msg, unpack_cmd->unpack_buf, unpack_cmd->unpack_len, &unpack_cmd->unpack_offset);
		msgpack::object obj = unpack_cmd->unpack_msg->get();
		try {
			obj.convert(&cmd);
		}
		catch(const std::exception &e) {
			LOG_CRIT << e.what();
			return false;
		}
		pack_cmd_ack(unpack_cmd, cmd, ack_str);
		return true;
	}
private:
	inline void pack_cmd_ack(const zeg_command_unpack_struct *unpack_cmd,
			const zeg_robot_navigation_command &cmd,
			string &ack_str){
		zeg_robot_header header("zeg.robot.navigation.command.ack", unpack_cmd->unpack_header->robot_id, chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
		zeg_robot_navigation_command_ack cmd_ack;
		cmd_ack.task_id = cmd.task_id;

		msgpack::sbuffer buffer_header;
		msgpack::sbuffer buffer_body;
		msgpack::pack(buffer_header, header);
		msgpack::pack(buffer_body, cmd_ack);
		char buf[1024] = "";
		memcpy(buf,  buffer_header.data(), buffer_header.size());
		memcpy(buf + buffer_header.size(),  buffer_body.data(), buffer_body.size());
		int len = buffer_header.size() + buffer_body.size();
		ack_str.assign(buf, len);
	}
	void call_report_poses_to_host_rpc(const zeg_robot_navigation_command &cmd) {
		static rpc_client client(zeg_config::zeg_config::get_instance().RPC_SERVER_IP, zeg_config::get_instance().robot_rpc_host_layer_port);
		if (false == client.connect(1)) {
			LOG_CRIT << "robot host rpc connect failed...!";
			return;
		}
		// to call rpc
	}
};
REGISTER_COMMAND_PARSER_ENTITY(zeg_navigation_parser, "zeg.robot.navigation.command");
}

#endif /* SRC_ZEG_NAVIGATION_PARSER_HPP_ */
