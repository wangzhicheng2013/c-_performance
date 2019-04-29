#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <iostream>
using namespace std;
static const char* terminal = "/dev/tty";
static const size_t BUFF_SIZE = 256;
int main()
{
	int fd = open(terminal, O_RDWR | O_NONBLOCK);
	if (fd <= 0)
	{
		cerr << "open " << terminal << " failed...!" << endl;
		return -1;
	}
	char buf[BUFF_SIZE] = "";
	while (true)
	{
		if (read(fd, buf, BUFF_SIZE - 1) <= 0)
		{
			cerr << "read none...!" << endl;
			int flags = fcntl(fd, F_GETFL);
			flags |= O_NONBLOCK;
			fcntl(fd, F_SETFL);
			continue;
		}
		cout << buf << endl;
	}
	close(fd);
	return 0;
}