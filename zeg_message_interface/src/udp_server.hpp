#ifndef SRC_UDP_SERVER_HPP_
#define SRC_UDP_SERVER_HPP_
#include <vector>
#include <thread>
#include <memory>
#include "message_communicate_entity_maker.hpp"
#include "udp_thread_for_zeg_robot.hpp"
class udp_server : public message_communicate_entity {
public:
	udp_server() {
		sock_fd_ = -1;
	}
	virtual ~udp_server() {
		for (auto &it : threads_) {
			if (it.joinable()) {
				it.join();
			}
		}
		sockect_utility::get_instance().close_socket(sock_fd_);
	}
public:
	inline bool init() override {
		return init(thread::hardware_concurrency());
	}
	void set_config(const socket_config &config) {
		config_ = config;
	}
	bool init(int network_size) {
		if (network_size <= 0 || network_size > network_capacity) {
			return false;
		}
		sock_fd_ = sockect_utility::get_instance().bind_socket(config_);
		if (sock_fd_ < 0) {
			return false;
		}
		threads_.resize(network_size);
		for (int i = 0;i < network_size;i++) {
			udp_threads_.emplace_back(unique_ptr<udp_thread>(new udp_thead_for_zeg_robot));
			if (false == udp_threads_[i]->init(sock_fd_)) {
				cerr << "udp thread init failed...!" << endl;
				continue;
			}
			try {
				threads_[i] = thread(std::bind(&udp_server::start_udp_thread, this, i));
			}
			catch (...) {
				return false;
			}
		}
		return true;
	}
	virtual int send(const char *buf, int len) override {
		return sendto(sock_fd_, buf, len, 0, (struct sockaddr *)&udp_thead_for_zeg_robot::scheduler_client_addr_, sizeof(udp_thead_for_zeg_robot::scheduler_client_addr_));
	}
	virtual int recv(char *recv_buf, int buf_len) override {
		return -1;
	}
private:
	void start_udp_thread(int index) {
		udp_threads_[index]->udp_dispatch_event();
	}
private:
	socket_config config_;
	vector<unique_ptr<udp_thread> >udp_threads_;
	vector<thread>threads_;
private:
	int sock_fd_;
private:
	const int network_capacity = 128;
};
REGISTER_MESSAGE_COMMUNICATE_ENTITY(udp_server, "udp.server");

#endif /* SRC_UDP_SERVER_HPP_ */
