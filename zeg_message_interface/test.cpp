#include "msgpack.hpp"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
int main() {
	msgpack::type::tuple<int, bool, string>src(1, true, "example");
	stringstream buffer;
	msgpack::pack(buffer, src);
	buffer.seekg(0);
	string str(buffer.str());
	msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());
	msgpack::object deserialized = oh.get();
	cout << deserialized << std::endl;
	msgpack::type::tuple<int, bool, string>dst;
	deserialized.convert(dst);
	cout << "a = " << get<0>(dst) << endl;  

	

	return 0;
}
