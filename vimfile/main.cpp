#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <sys/inotify.h>  
#include <unistd.h>  
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
static const int FILE_IN_CREATE_MASK = 256; 
static const char *COMMIT_STR = "/* \n\
 *@auth:hello world\n \
*@file:hello world\n \
*@license:hello world\n \
*@change story:hello world\n \
*------------------------------------\n\
*/";
const char *monitor_path = nullptr;
inline void write_commit(struct inotify_event *event) {
	if (FILE_IN_CREATE_MASK == event->mask) {
		if (event->len > 0 && !(event->mask & IN_ISDIR)) {
			char path[128] = "";
			const char *filename = event->name;
			snprintf(path, sizeof(path), "%s%s", monitor_path, filename);
			ofstream ofs(path);
			if (!ofs) {
				cerr << path << " can not be open...!" << endl;
				return;
			}
			ofs << COMMIT_STR << endl;
			ofs.close();
		}
	}
}
int main(int argc, char **argv) {
	char buf[BUFSIZ] = "";
	int len = 0;
	int nread = 0;
	struct inotify_event *event = nullptr;
	if (argc < 2) {
		fprintf(stderr, "%s path\n", argv[0]);
		return -1;

	}
	int fd = inotify_init();
	if (fd < 0) {
		fprintf(stderr, "inotify_init failed\n");
		return -1;
	}
	monitor_path = argv[1];
 	int wd = inotify_add_watch(fd, monitor_path, IN_CREATE);
	if (wd < 0) {
		fprintf(stderr, "inotify_add_watch %s failed\n", argv[1]);
		return -1;
	}
	int n = sizeof(buf) - 1;
	while ((len = read(fd, buf, n)) > 0) {
		nread = 0;
		while (len > 0) {
			event = (struct inotify_event *)&buf[nread];
			write_commit(event);
			nread = nread + sizeof(struct inotify_event) + event->len;
			len = len - sizeof(struct inotify_event) - event->len;
		}
	}
 
	return 0;
}
