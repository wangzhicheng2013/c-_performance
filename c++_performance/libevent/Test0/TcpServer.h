#ifndef TCPSERVER_H_
#define TCPSERVER_H_
#include <vector>
#include <thread>
#include <functional>
#include <memory>
#include "TcpThreadForEcho.h"

class TcpServer {
public:
	TcpServer();
	virtual ~TcpServer();
public:
	bool Init(int = 1, int = 8889);
private:
	void StartTcpThread(int);
private:
	vector<unique_ptr<TcpThread> >vecTcpthreads;
	vector<thread>vecThreads;
private:
	const int network_capacity = 128;
};

#endif /* TcpSERVER_H_ */