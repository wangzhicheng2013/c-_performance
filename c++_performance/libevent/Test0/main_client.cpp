#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;
static const size_t BUFF_SIZE = 1024;
static const int LOOP = 1000000;
int main()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		cerr << "socket fd init failed...!" << endl;
		return -1;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8889);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(sockfd, (struct sockaddr*) & addr, sizeof(addr)) < 0)
	{
		close(sockfd);
		cerr << "connect to server failed...!" << endl;
		return -1;
	}
	char buf[BUFF_SIZE] = "";
	memset(buf, 'A', BUFF_SIZE);
	auto begin = high_resolution_clock::now();
	for (int i = 0; i < LOOP; i++)
	{
		send(sockfd, buf, sizeof(buf), 0);
		socklen_t len = sizeof(addr);
		recv(sockfd, buf, sizeof(buf), 0);
		//cout << "client recv = " << buf[18] << endl;
	}
	auto end = high_resolution_clock::now();
	cout << "elapse time = " << duration_cast<seconds>(end - begin).count() << "s" << endl;
	close(sockfd);

	return 0;
}