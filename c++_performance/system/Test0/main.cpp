#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <iostream>
using namespace std;
unsigned char* p;
unsigned char* stack_buffer;
void sig_start(int signo)
{
	unsigned long a = 0x1234567811223344;
	p = (unsigned char*)& a;
	stack_buffer = (unsigned char*)& a;
	cout << "-----begin stack-----------" << endl;
	int k = 0;
	for (int i = 0; i < 32; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			printf("%.2x", stack_buffer[k++]);
		}
		cout << endl;
	}
	cout << "-----end stack-----------" << endl;
	if (SIGINT == signo)
	{
		signal(SIGINT, nullptr);
	}
	else if (SIGHUP == signo)
	{
		signal(SIGHUP, nullptr);
	}
}
int main()
{
	printf("process id = %d %p %p\n", getpid(), main, sig_start);
	signal(SIGINT, sig_start);
	signal(SIGHUP, sig_start);
	while (true)
	{
		sleep(10);
	}
	return 0;
}
