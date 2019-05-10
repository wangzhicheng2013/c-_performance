#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <errno.h>
#include <iostream>
using namespace std;
void init_daemon() {
	pid_t pid;
	if ((pid = fork()) < 0)
	{
		exit(EXIT_FAILURE);
	}
	if (pid > 0) 		// parent must exit
	{
		fprintf(stdout, "I am parent, I will exit...!\n");
		exit(0);
	}
	if (setsid() < 0)	// set a new session and escape from current terminal
	{
		fprintf(stderr, "%s", strerror(errno));
		exit(EXIT_FAILURE);
	}
	umask(0);
	// close stdin stdout stderr
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
}
int main() {
	init_daemon();
	while (true)
	{
		cout << "hello world...!" << endl;
		sleep(10);
	}

	return 0;
}