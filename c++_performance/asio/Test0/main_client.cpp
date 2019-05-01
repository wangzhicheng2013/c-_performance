#include <string.h>
#include <iostream>
#include <exception>
#include <chrono>
#include <boost/asio.hpp>
using namespace std;
using namespace chrono;
using namespace boost::asio;
static const int BUFF_SIZE = 1024;
static const int LOOP = 1000000;
io_service service;
int main()
{
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8889);
	ip::tcp::socket sock(service);
	char data[BUFF_SIZE] = "";
	memset(data, 'A', BUFF_SIZE - 1);
	int cnt = 0;
	try
	{
		sock.connect(ep);
		auto begin = high_resolution_clock::now();
		for (int i = 0; i < LOOP; i++)
		{
			write(sock, buffer(data, strlen(data)));
			if (sock.read_some(buffer(data)) > 0)
			{
				//cout << data << endl;
				++cnt;
			}
		}
		auto end = high_resolution_clock::now();
		cout << "elapse time = " << duration_cast<seconds>(end - begin).count() << "s" << endl;
	}
	catch (...)
	{
		cerr << "can not connect server...!" << endl;
	}
	sock.close();

	return 0;
}