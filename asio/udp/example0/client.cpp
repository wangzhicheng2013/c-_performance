#include <iostream>
#include "boost/asio.hpp"
using namespace std;
using namespace boost::asio;
static const size_t BUF_SIZE = 1024;
int main() {
	io_service ios;
	ip::udp::socket sock(ios);
	ip::udp::endpoint end_point(ip::address::from_string("127.0.0.1"), 9001);
	sock.open(end_point.protocol());
	char buf[BUF_SIZE] = "";
	string str;
	while (true) {
		cout << "input str = ";
		cin >> str;
		try {
			cout << "client send size = " << sock.send_to(buffer(str.c_str(), str.size()), end_point) << endl;
			cout << "client recv size = " << sock.receive_from(buffer(buf, BUF_SIZE), end_point) << endl;
			cout << "client recv message = " << buf << endl;
		}
		catch (boost::system::system_error &e) {
			cerr << e.what() << endl;
		}
	}


	return 0;
}

