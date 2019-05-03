#include "TcpThreadForEcho.h"

TcpThreadForEcho::TcpThreadForEcho()
{
	// TODO Auto-generated constructor stub
}
bool TcpThreadForEcho::DealTcpMessage(struct bufferevent* bev)
{
	char buf[buffer_size] = "";
	if (bufferevent_read(bev, buf, buffer_size) > 0)
	{
		//cout << "client say: %c: " << buf[0] << endl;
		bufferevent_write(bev, buf, buffer_size);
	}
	return true;
}
TcpThreadForEcho::~TcpThreadForEcho()
{
	// TODO Auto-generated destructor stub
}