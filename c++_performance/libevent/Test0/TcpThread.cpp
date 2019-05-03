#include "TcpThread.h"

TcpThread::TcpThread()
{
	// TODO Auto-generated constructor stub
	pBase = nullptr;
	pListener = nullptr;
}
bool TcpThread::Init(int port)
{
	struct sockaddr_in serv = { 0 };
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	pBase = event_base_new();
	if (nullptr == pBase)
	{
		return false;
	}
	pListener = evconnlistener_new_bind(pBase, ListenerCallBackFun, this, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 1024, (struct  sockaddr *) & serv, sizeof(serv));
	return pListener != nullptr;
}
void TcpThread::TcpDispatchEvent()
{
	if (pBase != nullptr)
	{
		event_base_dispatch(pBase);
	}
}
void TcpThread::ListenerCallBackFun(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int len, void* ptr)
{
	TcpThread* pTcpThread = reinterpret_cast<TcpThread*>(ptr);
	if (nullptr == pTcpThread)
	{
		return;
	}
	evutil_make_socket_nonblocking(fd);
	struct sockaddr_in* client = (sockaddr_in*)addr;
	cout << "connect new client: " << inet_ntoa(client->sin_addr) << "::" << ntohs(client->sin_port) << endl;
	struct bufferevent* bev = bufferevent_socket_new(pTcpThread->pBase, fd, BEV_OPT_CLOSE_ON_FREE);
	if (nullptr == bev)
	{
		cerr << "bufferevent_socket_new failed...!" << endl;
		return;
	}
	bufferevent_setcb(bev, ReadCallBackFun, WriteCallBackFun, EventCallBackFun, ptr);
	bufferevent_enable(bev, EV_READ);
}
void TcpThread::ReadCallBackFun(struct bufferevent* bev, void* arg)
{
	TcpThread* pTcpThread = reinterpret_cast<TcpThread*>(arg);
	pTcpThread->DealTcpMessage(bev);
}
void TcpThread::WriteCallBackFun(struct bufferevent* bev, void* arg)
{
}
void TcpThread::EventCallBackFun(struct bufferevent* bev, short events, void* arg)
{
	if (events & BEV_EVENT_EOF)
	{
		cout << "connection closed:" << endl;
	}
	else if (events & BEV_EVENT_ERROR)
	{
		cout << "some other error !" << endl;
	}

	bufferevent_free(bev);
	cout << "buffer event is released...!" << endl;
}
TcpThread::~TcpThread()
{
	// TODO Auto-generated destructor stub
	if (pBase != nullptr)
	{
		event_base_loopexit(pBase, NULL);
		if (pListener != nullptr)
		{
			evconnlistener_free(pListener);
			pListener = nullptr;
		}
		event_base_free(pBase);
		pBase = nullptr;
	}
}