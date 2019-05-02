#include <unistd.h>
#include <iostream>
#include <string>
#include <zmq.h>
using namespace std;
static const char* address = "tcp://*:8889";
static const size_t BUFF_SIZE = 1024;
int main()
{
	void* context = zmq_ctx_new();
	void* server = zmq_socket(context, ZMQ_PULL);
	if (nullptr == context || nullptr == server)
	{
		cerr << "server socket init failed...!" << endl;
		return -1;
	}
	if (zmq_bind(server, address))
	{
		cerr << "server bind failed...!" << endl;
		return -1;
	}
	sleep(2);
	while (true)
	{
		zmq_msg_t msg;
		if (zmq_msg_init(&msg))
		{
			cerr << "message init failed...!" << endl;
			break;
		}
		int len = zmq_msg_recv(&msg, server, 0);
		if (len > 0)
		{
			zmq_msg_send(&msg, server, 0);
		}
		else
		{
			cerr << "server recv message failed...!" << endl;
		}
		zmq_msg_close(&msg);
	}
	return 0;
}