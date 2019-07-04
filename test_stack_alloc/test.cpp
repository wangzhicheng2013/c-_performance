#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <new>
using namespace std;
static const uint64_t ONE_G = 1024 * 1024 * 1024;
bool test_system_stack_memory(size_t size) {
	char *p = nullptr;
	uint64_t capacity = ONE_G * size;
	bool succ = true;
	try {
		p = new char[capacity];
	}
	//catch (std::exception &e) {
	catch (std::bad_alloc &e) {
		succ = false;
		cerr << e.what() << endl;
		cerr << size << "G" << " alloc failed...!" << endl;
	}
	if (nullptr != p) {
		delete []p;
		p = nullptr;
	}
	return succ;
}
bool test_system_stack_memory1(size_t size) {
    uint64_t capacity = ONE_G * size;
    bool succ = true;
	char *p = new(nothrow) char[capacity];
    if (nullptr != p) {
        delete []p;
        p = nullptr;
    }
	else {
		succ = false;
		cerr << size << "G" << " alloc failed...!" << endl;
	}
    return succ;
}

int main() {
	for (size_t i = 0;i < 1000;i++) {
//		if (false == test_system_stack_memory(i)) {			// 0.004s
		if (false == test_system_stack_memory1(i)) {		// 0.004s
			return -1;
		}
	}

    return 0;
}
