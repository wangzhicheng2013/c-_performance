#ifndef SRC_UDP_BROADCAST_CLIENT_HPP_
#define SRC_UDP_BROADCAST_CLIENT_HPP_
#include "message_communicate_entity.hpp"
class udp_broadcast_client : public message_communicate_entity {
public:
	udp_broadcast_client() {
		sock_fd_ = -1;
		port_ = 17789;
		broadcast_address_ = nullptr;
		memset(&client_addr_, 0, sizeof(client_addr_));
	}
	~udp_broadcast_client() {
		if (sock_fd_ >= 0) {
			close(sock_fd_);
		}
	}
public:
	inline bool init() override {
		return init_sock_fd();
	}
	inline bool init_sock_fd() {
		sock_fd_ = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock_fd_ < 0) {
			return false;
		}
		int so_broadcast = 1;
		return setsockopt(sock_fd_, SOL_SOCKET, SO_BROADCAST, (char *)&so_broadcast, sizeof(so_broadcast)) >= 0;
	}
	inline void set_port(int port) {
		port_ = port;
	}
	inline void set_broadcast_address(const char *address) {
		broadcast_address_ = address;
	}
	virtual int send(const char *buf, int len) override {
		struct sockaddr_in dest_addr = {0};
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(port_);
		dest_addr.sin_addr.s_addr = inet_addr(broadcast_address_);
		return sendto(sock_fd_, buf, len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
	}
	virtual int recv(char *recv_buf, int buf_len) override {
		if (buf_len <= 0 || buf_len > BUFSIZ) {
			return -1;
		}
		char buf[BUFSIZ] = "";
		socklen_t size = sizeof(struct sockaddr);
		int len = recvfrom(sock_fd_, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr_, &size);
		if (len <= BUFSIZ && len > 0) {
			len = min(len, buf_len);
			memcpy(recv_buf, buf, len);
		}
		return len;
	}
private:
	int sock_fd_;
	int port_;
	const char *broadcast_address_;
	struct sockaddr_in client_addr_;
};

#endif /* SRC_UDP_BROADCAST_CLIENT_HPP_ */
