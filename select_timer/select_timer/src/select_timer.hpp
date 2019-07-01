#ifndef SRC_SELECT_TIMER_HPP_
#define SRC_SELECT_TIMER_HPP_
#include <sys/time.h>
#include <iostream>
#include <functional>
#include <thread>
using namespace std;
class select_timer {
public:
	select_timer(function<void()>task, int seconds) {
		thd_ = thread([this, &task, seconds] {
			while (true) {
				task();
				do_select_timeout(seconds);
			}
		});
	}
	~select_timer() {
		thd_.detach();
	}
private:
	void do_select_timeout(unsigned int seconds) {
		struct timeval tv = {0};
		tv.tv_sec = seconds;
		int err = 0;
		do {
			err = select(0, nullptr, nullptr, nullptr, &tv);
		}while (err < 0 && EINTR == errno);
	}
private:
	thread thd_;
};




#endif /* SRC_SELECT_TIMER_HPP_ */
