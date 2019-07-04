#include <iostream>
#include <string>
#include "boost/asio.hpp"
using namespace std;
using namespace boost::asio;
static const size_t BUF_SIZE = 1024;
int main() {
	io_service ios;
	ip::udp::socket udp_server(ios);
	ip::udp::endpoint local_addr(ip::address::from_string("127.0.0.1"), 9001);
	udp_server.open(local_addr.protocol());
	udp_server.bind(local_addr);
	char buf[BUF_SIZE] = "";
	ip::udp::endpoint send_point;
	int recv_len = 0;
	while (true) {
		recv_len = udp_server.receive_from(buffer(buf, BUF_SIZE), send_point);
		cout << "server recv size = " << recv_len << endl;
		cout << "server recv message = " << buf << endl;
		cout << "server send back size = " << udp_server.send_to(buffer(buf, recv_len), send_point) << endl;
	}

	return 0;
}
