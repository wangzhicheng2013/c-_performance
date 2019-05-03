#ifndef TCPTHREAD_H_
#define TCPTHREAD_H_
#include "SocketUtility.h"
#include <event.h>
#include <event2/listener.h>
class TcpThread {
public:
	TcpThread();
	virtual ~TcpThread();
public:
	bool Init(int port = 8889);
	void TcpDispatchEvent();
	virtual bool DealTcpMessage(struct bufferevent* bev) = 0;
	static void ListenerCallBackFun(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int len, void* ptr);
	static void ReadCallBackFun(struct bufferevent* bev, void* arg);
	static void WriteCallBackFun(struct bufferevent* bev, void* arg);
	static void EventCallBackFun(struct bufferevent* bev, short events, void* arg);
protected:
	event_base* pBase;
	struct evconnlistener* pListener;
protected:
	const int buffer_size = 1024;
};

#endif /* TCPTHREAD_H_ */