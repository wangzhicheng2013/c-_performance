#include <string.h>
#include <iostream>
#include <exception>
#include <thread>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
using namespace std;
using namespace boost::asio;
static const int BUFF_SIZE = 1024;
io_service service;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
ip::tcp::endpoint ep(ip::tcp::v4(), 8889);
ip::tcp::acceptor acc(service, ep);
void start_accept(socket_ptr sockptr);
void handle_accept(socket_ptr sockptr, const boost::system::error_code& error)
{
	if (error)
	{
		cerr << "server accept error...!" << endl;
		return;
	}
	while (true)
	{
		char data[BUFF_SIZE] = "";
		boost::system::error_code error1;
		size_t len = sockptr->read_some(buffer(data), error1);
		if (boost::asio::error::eof == error1)
		{
			cerr << "disconnect" << endl;
			break;
		}
		else if (error1)
		{
			throw boost::system::system_error(error); // Some other error.
			break;
		}
		else
		{
		//	cout << len << endl;
		//	cout << data << endl;
			write(*sockptr, buffer(data, strlen(data)));
		}

	}
	socket_ptr sockptr1(new ip::tcp::socket(service));
	start_accept(sockptr1);
}
void start_accept(socket_ptr sockptr)
{
	acc.async_accept(*sockptr, boost::bind(handle_accept, sockptr, _1));
}
int main()
{

	socket_ptr sockptr(new ip::tcp::socket(service));
	start_accept(sockptr);
	service.run();

	return 0;
}