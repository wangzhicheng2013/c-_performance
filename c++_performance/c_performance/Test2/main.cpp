#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string>
using namespace std;
inline void run_program(const char* path) {
	system(path);
	usleep(5000);
}
void kill_program(const char* program_name) {
	char cmd[128] = "";
	snprintf(cmd, sizeof(cmd), "ps x | grep ./%s | grep -v grep | awk \'{print $1}\'", program_name);
	FILE* fp = popen(cmd, "r");
	if (!fp) {
		cerr << cmd << " failed...!" << endl;
		return;
	}
	char buf[64] = "";
	while (fgets(buf, sizeof(buf), fp)) {
	}
	pclose(fp);
	snprintf(cmd, sizeof(cmd), "kill -9  %s", buf);
	cout << cmd << endl;
	system(cmd);
	usleep(5000);
}

int main() {
	while (true) {
		kill_program("client");
		sleep(10);
		run_program("./home/wangbin/file_transfer_test/octopus/client/client");
		sleep(100);
		kill_program("server");
		run_program("./home/wangbin/file_transfer_test/octopus/server/server");
		sleep(1000);
	}
}
