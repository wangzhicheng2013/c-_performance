#include "select_timer.hpp"
#include <iostream>
#include <chrono>
void fun() {
    cout << "I am fun()." << endl;
}
void fun1() {
	while (true) {
		cout << "I am fun1." << endl;
		this_thread::sleep_for(chrono::seconds(1));
	}
}
int main() {
	auto f = fun;
	select_timer(f, 1);
	thread th(fun1);
	if (th.joinable()) {
		th.join();
	}

	return 0;
}




