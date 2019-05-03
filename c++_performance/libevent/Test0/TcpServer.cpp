#include "TcpServer.h"

TcpServer::TcpServer()
{
	// TODO Auto-generated constructor stub
}
bool TcpServer::Init(int network_size, int port)
{
	if (network_size <= 0 || network_size > network_capacity)
	{
		return false;
	}
	vecThreads.resize(network_size);
	for (int i = 0; i < network_size; i++)
	{
		vecTcpthreads.emplace_back(unique_ptr<TcpThread>(new TcpThreadForEcho));
		if (false == vecTcpthreads[i]->Init(port))
		{
			cerr << "Tcp thread init failed...!" << endl;
			continue;
		}
		try
		{
			vecThreads[i] = thread(std::bind(&TcpServer::StartTcpThread, this, i));
		}
		catch (...)
		{
			return false;
		}
	}

	return true;
}
void TcpServer::StartTcpThread(int index)
{
	vecTcpthreads[index]->TcpDispatchEvent();
}
TcpServer::~TcpServer()
{
	// TODO Auto-generated destructor stub
	for (auto& it : vecThreads)
	{
		if (it.joinable())
		{
			it.join();
		}
	}
}