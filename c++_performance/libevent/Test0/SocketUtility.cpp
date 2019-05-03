#include "SocketUtility.h"
SocketUtility SocketUtility::socketUtility;
SocketUtility::SocketUtility()
{
}
/*
 * @purpose:init socket
 * @return socket fd
 * */
int SocketUtility::InitSocket(unsigned char type)
{
	int sock_fd = -1;
	switch (type)
	{
	case TCP:
		sock_fd = socket(AF_INET, SOCK_STREAM, 0);
		break;
	case UDP:
		sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
		break;
	default:
		cerr << "unknown socket type...!" << endl;
		return -1;
	}
	if (sock_fd < 0)
	{
		cerr << "socket fd create failed...!" << endl;
		return -1;
	}
	int on = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (char*)& on, sizeof(on)) < 0)
	{
		cerr << "set socket to resue failed...!" << endl;
		return -1;
	}
	if (setsockopt(sock_fd, SOL_SOCKET, SO_KEEPALIVE, (char*)& on, sizeof(on)) < 0)
	{
		cerr << "set socket to keep alive failed...!" << endl;
		return -1;
	}
	return sock_fd;
}
/*
 * @purpose:bind socket
 * @return socket fd
 * */
int SocketUtility::BindSocket(const SocketConfig& config)
{
	int sock_fd = InitSocket(config.type);
	if (-1 == sock_fd)
	{
		return -1;
	}
	struct sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(config.ip.c_str());
	addr.sin_port = htons(config.port);
	if (bind(sock_fd, (struct sockaddr*) & addr, sizeof(addr)) < 0)
	{
		cerr << "socket bind failed on ip = " << config.ip << " port = " << config.port << endl;
		return -1;
	}
	if (TCP == config.type)
	{
		if (listen(sock_fd, config.backlog) < 0)
		{
			cerr << "socket listen failed on backlog = " << config.backlog << endl;
			return -1;
		}
	}
	int flag = 0;
	flag = fcntl(sock_fd, F_GETFL, 0);
	if (flag < 0)
	{
		cerr << "fcntl F_GETFL on socket = " << sock_fd << " failed...!" << endl;
		return -1;
	}
	if (fcntl(sock_fd, F_SETFL, flag | O_NONBLOCK) < 0)
	{
		cerr << "fcntl F_SETFL non block on socket = " << sock_fd << " failed...!" << endl;
		return -1;
	}
	return sock_fd;
}
bool SocketUtility::CloseSocket(int& sock_fd)
{
	if (sock_fd >= 0)
	{
		close(sock_fd);
		sock_fd = -1;
		return true;
	}
	return false;
}
SocketUtility::~SocketUtility()
{
	// TODO Auto-generated destructor stub

}