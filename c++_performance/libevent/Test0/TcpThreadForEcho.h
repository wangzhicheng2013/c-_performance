#ifndef TCPTHREADFORECHO_H_
#define TCPTHREADFORECHO_H_

#include "TcpThread.h"

class TcpThreadForEcho : public TcpThread {
public:
	TcpThreadForEcho();
	virtual ~TcpThreadForEcho();
public:
	virtual bool DealTcpMessage(struct bufferevent* bev) override;
};

#endif /* TCPTHREADFORECHO_H_ */