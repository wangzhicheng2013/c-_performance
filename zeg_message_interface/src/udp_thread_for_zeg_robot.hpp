#ifndef SRC_UDP_THREAD_FOR_ZEG_ROBOT_HPP_
#define SRC_UDP_THREAD_FOR_ZEG_ROBOT_HPP_
#include "udp_thread.hpp"
#include "zeg_robot_command_processor.hpp"
using namespace zeg_message_interface;
class udp_thead_for_zeg_robot : public udp_thread {
public:
	udp_thead_for_zeg_robot() = default;
	virtual ~udp_thead_for_zeg_robot() = default;
public:
	virtual bool deal_udp_message(int fd) {
		char buf[buffer_size] = "";
		static socklen_t size = sizeof(struct sockaddr);
		static struct sockaddr_in client_addr = {0};
		int len = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &size);
		if (false == check_len(len)) {
			return false;
		}
		log_connection_info(client_addr, len);
		string ack_str;
		if (false == zeg_robot_command_processor::get_instance().process(buf, len, ack_str)) {
			return false;
		}
		if (false == ack_str.empty()) {
			sendto(fd, ack_str.c_str(), ack_str.size(), 0, (struct sockaddr *)&client_addr, size);
		}
		get_schedule_server_addr(client_addr);
		return true;
	}
private:
	inline bool check_len(int len) {
		if (len < 0) {
			LOG_CRIT << "server recv message error...!";
			return false;
		}
		if (0 == len) {
			LOG_CRIT << "connection closed...!";
			return false;
		}
		return true;
	}
	inline void log_connection_info(const struct sockaddr_in &client_addr, int len) {
		LOG_INFO << "connection ip = " << inet_ntoa(client_addr.sin_addr) << ":" << client_addr.sin_port;
		LOG_INFO << "server recv message len = " << len;
	}
	inline void get_schedule_server_addr(const struct sockaddr_in &client_addr) {
		if (client_addr.sin_port != scheduler_client_addr_.sin_port ||
				strcmp(inet_ntoa(client_addr.sin_addr), inet_ntoa(scheduler_client_addr_.sin_addr))) {
			memcpy(&scheduler_client_addr_, &client_addr, sizeof(client_addr));
		}
	}
public:
	static struct sockaddr_in scheduler_client_addr_;
};
struct sockaddr_in udp_thead_for_zeg_robot::scheduler_client_addr_ = {0};

#endif /* SRC_UDP_THREAD_FOR_ZEG_ROBOT_HPP_ */
