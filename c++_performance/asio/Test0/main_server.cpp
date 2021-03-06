#include <string.h>
#include <iostream>
#include <exception>
#include <thread>
#include <boost/asio.hpp>
using namespace std;
using namespace boost::asio;
static const int BUFF_SIZE = 1024;
io_service service;
int main()
{
	ip::tcp::endpoint ep(ip::tcp::v4(), 8889);
	ip::tcp::acceptor acc(service, ep);
	while (true)
	{
		ip::tcp::socket sock(service);
		acc.accept(sock);
		thread thd = thread([&sock] {
			while (true)
			{
				char data[BUFF_SIZE] = "";
				boost::system::error_code error;
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
				//cout << len << endl;
				//cout << data << endl;
				write(sock, buffer(data, strlen(data)));
			}
			});
		if (thd.joinable())
		{
			thd.join();
		}
		sock.close();
	}
	
	return 0;
}