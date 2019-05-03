#ifndef SOCKETCONFIG_H_
#define SOCKETCONFIG_H_
#include <string>
using namespace std;
enum
{
	TCP,
	UDP
};
class SocketConfig
{
public:
	SocketConfig();
	virtual ~SocketConfig();
public:
	string ip;
	int port;
	int backlog;
	unsigned char type;
};

#endif /* SOCKETCONFIG_H_ */