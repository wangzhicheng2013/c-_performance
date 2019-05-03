/********************************************************
Copyright (C),  2016-2018,
FileName:		main
Author:        	woniu201
Email:         	wangpengfei.201@163.com
Created:       	2018/11/21
Description:    ʹ��libeventʵ��TCP Server
********************************************************/
#include <iostream>
#include <string.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>

#ifdef _WIN32
#include <windows.h>
#include <time.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#endif

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "libevent.lib")
#pragma comment(lib, "libevent_core.lib")
#pragma comment(lib, "libevent_extras.lib")

using namespace std;

/************************************
@ Brief:		���������ص�
@ Author:		woniu201
@ Created:		2018/11/21
@ Return:
************************************/
void read_cb(struct bufferevent* bev, void* arg)
{
	char buf[1024] = { 0 };

	bufferevent_read(bev, buf, sizeof(buf));
	cout << "client say: %s: " << buf << endl;

	const char* p = "���Ƿ���";

	//д���ݸ��ͻ���
	bufferevent_write(bev, p, strlen(p) + 1);
}


/************************************
@ Brief:		д�������ص�
@ Author:		woniu201
@ Created:		2018/11/21
@ Return:
************************************/
void write_cb(struct bufferevent* bev, void* arg)
{
	cout << "I'm ���������ɹ�д���ݸ��ͻ��ˣ�д����ص�����������..." << endl;
}

/************************************
@ Brief:		�¼��ص�
@ Author:		woniu201
@ Created:		2018/11/21
@ Return:
************************************/
void event_cb(struct bufferevent* bev, short events, void* arg)
{
	char* ip = (char*)arg;
	if (events & BEV_EVENT_EOF)
	{
		cout << "connection closed:" << ip << endl;
	}
	else if (events & BEV_EVENT_ERROR)
	{
		cout << "some other error !" << endl;
	}

	bufferevent_free(bev);
	cout << "bufferevent ��Դ�Ѿ����ͷ�..." << endl;
}

/************************************
@ Brief:		�����ص�
@ Author:		woniu201
@ Created:		2018/11/21
@ Return:
************************************/
void cb_listener(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int len, void* ptr)
{
	struct sockaddr_in* client = (sockaddr_in*)addr;
	cout << "connect new client: " << inet_ntoa(client->sin_addr) << "::" << ntohs(client->sin_port) << endl;

	struct event_base* base = (struct event_base*)ptr;

	//������¼�
	struct bufferevent* bev;
	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

	//��bufferevent���������ûص�
	bufferevent_setcb(bev, read_cb, write_cb, event_cb, inet_ntoa(client->sin_addr));

	//���� bufferevent�� ����������Ĭ����disable ��
	bufferevent_enable(bev, EV_READ);
}

int main()
{
#ifdef WIN32
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	(void)WSAStartup(wVersionRequested, &wsaData);
#endif
	//init server
	struct sockaddr_in serv;

	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(8889);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);

	//���� event_base
	struct event_base* base;
	base = event_base_new();

	//�����׽���
	//��
	//������������
	struct evconnlistener* listener;
	listener = evconnlistener_new_bind(base, cb_listener, base, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, 36, (struct  sockaddr*) & serv, sizeof(serv));

	//����ѭ������
	event_base_dispatch(base);
	evconnlistener_free(listener);
	event_base_free(base);
	return 0;
}