#include <exception>
#include "zeg_robot_broadcast.hpp"
#include "udp_unicast_server.hpp"
#include "udp_unicast_client.hpp"
#include "rpc_server.h"
using namespace zeg_robot_maintainence;
zeg_robot_broadcast zeg_robot_broadcast_thread;
bool start_thread() {
	if (false == zeg_robot_broadcast_thread.init()) {
		LOG_CRIT << "zeg_robot_broadcast_thread init failed";
		return false;
	}
	zeg_robot_broadcast_thread.run();
	return true;
}
void join_thread() {
	zeg_robot_broadcast_thread.join();
}
char udp_recv_buf[BUFSIZ] = "";
udp_unicast_server mock_server;
zeg_robot_header obj;
msgpack::sbuffer buffer_header;
msgpack::sbuffer buffer_body;
void recv_thread() {
	msgpack::sbuffer buffer;
	while (true) {
		int len = mock_server.recv(udp_recv_buf, sizeof(udp_recv_buf));
		cout << "recv = " << udp_recv_buf << endl;
		cout << "recv len = " << len << endl;
		msgpack::unpacked msg;
		size_t offset = 0;
		msgpack::unpack(&msg, udp_recv_buf, len, &offset);
		cout << "offset = " << offset << endl;
		msgpack::object obj = msg.get();
		zeg_robot_header header;
		robot_basic_info1 info;
		try {
			obj.convert(&header);
			cout << header.robot_id << endl;
			msgpack::unpack(&msg, udp_recv_buf, len, &offset);
			cout << "offset  = " << offset << endl;
			msgpack::object obj = msg.get();
			obj.convert(&info);
		}
		catch(const std::exception &e) {
			cout << e.what() << endl;
		}
	}
}
char buf[1024] = "";
void send_thread() {
	//const char *buf = "zeg robot process ok.";
	while (true) {
		this_thread::sleep_for(chrono::seconds(1));
		memcpy(buf, buffer_header.data(), buffer_header.size());
		memcpy(buf + buffer_header.size(), buffer_body.data(), buffer_body.size());
		//cout << "size = " << buffer_header.size() << endl;
		cout << "send back = " << mock_server.send(buf, buffer_header.size() + buffer_body.size()) << endl;
	}
}
void make_uniform_pack() {
	robot_basic_info1 obj0 = {1.2, 12, 0.01, 0, 98.899};
	msgpack::pack(buffer_body, obj0);
	obj.type = "zeg.robot.basic.info";
	obj.robot_id = "zeg_robot_xx011212DD1";
	obj.timestamp = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	msgpack::pack(buffer_header, obj);
}
int main() {
	/*
	if (false == start_thread()) {
		return -1;
	}
	join_thread();
*/
	make_uniform_pack();
	mock_server.set_port(17789);
	if (false == mock_server.init()) {
		cout << "mock server init failed...!" << endl;
		return -1;
	}
	thread th0(recv_thread);
	thread th1(send_thread);
	th0.join();
	th1.join();
	/*make_uniform_pack();
	udp_broadcast_agent client;
	if (client.init_sock_fd() < 0) {
		return -1;
	}
	while (true) {
		client.send_broadcast(buffer.data(), buffer.size(), zeg_config::get_instance().robot_broadcast_address.c_str());
		sleep(2);
	}*/
	return 0;
}



