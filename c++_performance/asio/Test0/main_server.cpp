#include <string.h>
#include <iostream>
#include <exception>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
using namespace std;
using namespace boost::asio;
static const int BUFF_SIZE = 1024 * 64;
io_service service;
int main()
{
	ip::tcp::endpoint ep(ip::tcp::v4(), 8889);
	ip::tcp::acceptor acc(service, ep);
	boost::system::error_code error;
	while (true)
	{
		ip::tcp::socket sock(service);
		acc.accept(sock);
		while (true)
		{
			char data[BUFF_SIZE] = "";
			size_t len = sock.read_some(buffer(data), error);
			if (boost::asio::error::eof == error)
			{
				cerr << "disconnect" << endl;
				break;
			}
			else if (error)
			{
				throw boost::system::system_error(error); // Some other error.
				break;
			}
			//cout << "len = " << len << endl;
			write(sock, buffer(data, strlen(data)));
		}
		sock.close();
	}
	
	return 0;
}
