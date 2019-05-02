#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string>
#include <chrono>
#include <zmq.h>
using namespace std;
using namespace chrono;
static const char* address = "tcp://127.0.0.1:8889";
static const int LOOP = 1000000;
static const size_t BUFF_SIZE = 1024;

int main()
{
	void* context = zmq_ctx_new();
	void* client = zmq_socket(context, ZMQ_REQ);
	if (nullptr == context || nullptr == client)
	{
		cerr << "client socket init failed...!" << endl;
		return -1;
	}
	if (zmq_connect(client, address))
	{
		cerr << "server bind failed...!" << endl;
		return -1;
	}
	int sendcnt = 0;
	int recvcnt = 0;
	string str(BUFF_SIZE, 'A');
	auto begin = high_resolution_clock::now();
	for (int i = 0; i < LOOP; i++)
	{
		zmq_msg_t msg;
		if (zmq_msg_init_size(&msg, BUFF_SIZE))
		{
			cerr << "message init failed...!" << endl;
			break;
		}
		memcpy(zmq_msg_data(&msg), str.c_str(), BUFF_SIZE);
		if (BUFF_SIZE == zmq_msg_send(&msg, client, 0))
		{
			sendcnt++;
		}
		int len = zmq_msg_recv(&msg, client, 0);
		if (BUFF_SIZE == len)
		{
			recvcnt++;
		}
		else
		{
			cerr << "client recv message failed...!" << endl;
		}
		zmq_msg_close(&msg);
	}
	auto end = high_resolution_clock::now();
	cout << "elapse time = " << duration_cast<seconds>(end - begin).count() << "s" << endl;
	cout << "send count = " << sendcnt << endl;
	cout << "recv count = " << recvcnt << endl;

	return 0;
}