#define _CRT_SECURE_NO_WARNINGS
#include "rpc_server.h"
#include "NanoLog.hpp"
#include <iostream>
using namespace std;
using namespace rest_rpc;
using namespace rpc_service;
int main(int argc, char * argv[]) {
	rpc_server server(9000, thread::hardware_concurrency());
	cout << "hello world...!" << endl;
	nanolog::initialize(nanolog::GuaranteedLogger(), "/home/wangbin/client_status_manager/log/", "XX11", 1);
	nanolog::set_log_level(nanolog::LogLevel::INFO);
	int a = 100;
	LOG_INFO << "Logging " << a << "\n";

	return 0;
}
