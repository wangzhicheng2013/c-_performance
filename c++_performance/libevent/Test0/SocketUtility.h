#ifndef SocketUtility_H_
#define SocketUtility_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include "SocketConfig.h"
class SocketUtility
{
public:
	static SocketUtility& GetInstance()
	{
		return socketUtility;
	}
	/*
	 * @purpose:bind socket
	 * @return socket fd
	 * */
	int BindSocket(const SocketConfig& config);
	bool CloseSocket(int& sock_fd);
private:
	SocketUtility();
	virtual ~SocketUtility();
private:
	/*
	 * @purpose:init socket
	 * @return socket fd
	 * */
	int InitSocket(unsigned char type = TCP);
private:
	static SocketUtility socketUtility;
};

#endif /* SOCKECTOP_H_ */
