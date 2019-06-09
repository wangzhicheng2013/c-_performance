#include <iostream>
#include <fstream>
#include <experimental/filesystem>
//https://zh.cppreference.com/w/cpp/filesystem/file_time_type
//https://www.jb51.net/article/71804.htm
using namespace std;
using namespace experimental::filesystem;
int main() {
	path p = current_path();
	p += "/11.txt";
	cout << p << endl;
	ofstream(p.c_str()).put('A');
	auto ftime = last_write_time(p);
	
	return 0;
}
