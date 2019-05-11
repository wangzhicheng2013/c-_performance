#include <stdio.h>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
using namespace std;
static const int LOOP = 1000000;
void string_to_lower(string& str) {
	transform(begin(str), end(str), begin(str), ::tolower);
}
void string_to_lower1(string& str) {
	transform(begin(str), end(str), begin(str), [](char ch) {
		ch |= 0x20;
		return ch;
		});
}
void string_to_lower2(string& str) {
	int size = str.size();
	for (int i = 0; i < size; i++) {
		str[i] |= 0x20;
	}
}
std::string strtolower(std::string s)
{
	size_t len = s.size();
	size_t ec = len / 8;
	uint64_t* p8 = (uint64_t*)s.data();
	for (size_t i = 0; i < ec; ++i) {
		p8[i] |= 0x2020202020202020;
	}
	uint8_t* p1 = (uint8_t*)(p8 + ec);
	len %= 8;
	for (size_t i = 0; i < len; ++i) {
		p1[i] |= 0x20;
	}
	return s;
}
int main() {
	for (int i = 0; i < LOOP; i++) {
		string str = "abcAAscccdBBjjjhaashKKlmmmJHbbbhJJJAAdsddddmmnBNB";	
		//string_to_lower(str);		// 0.694s
		//string_to_lower1(str);	// 0.706s
		//string_to_lower2(str);	// 0.374s
		str = strtolower(str);
	}
	/*
	string str = "AAA";
	cout << strtolower(str) << endl;
	*/

	return 0;
}