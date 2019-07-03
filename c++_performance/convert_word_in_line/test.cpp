#include <unistd.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <stack>
using namespace std;
string str = "Have a nice day";
void fun(string &res) {
	stack<string>ss;
	stringstream os(str);
	string word;
	while (os >> word) {
		ss.push(word);
	}
	while (false == ss.empty()) {
		res += ss.top();
		res += " ";
		ss.pop();
	}
}
void fun1(string &res) {
    stack<string>ss;
    stringstream os(str);
    string word;
    while (os >> word) {
        ss.push(word);
    }
	static string blank = " ";
    while (false == ss.empty()) {
        res += ss.top();
        res += blank;
        ss.pop();
    }
}
void fun2(string &res) {
	string word;
    stack<string>ss;
	for (auto &ch : str) {
		if (' ' == ch)  {
			ss.push(word);
			word.clear();
			continue;
		}
		word += ch;
	}
	ss.push(word);
    static string blank = " ";
    while (false == ss.empty()) {
        res += ss.top();
        res += blank;
        ss.pop();
    }
}
void fun3(string &res) {
    char word[64] = "";
	int index = 0;
	int size = str.size();
    for(int i = size - 1;i >= 0;i--) {
		if (' ' != str[i]) {
			word[index++] = str[i];
		}
		else {
			for (int i = index - 1;i >= 0;i--) {
				res += word[i];
			}
			res += str[i];
			index = 0;
		}
	}
	for (int i = index - 1;i >= 0;i--) {
		res += word[i];
	}
}


int main() {
	string res;
	for (int i = 0;i < 1000000;i++) {
		//fun(res);			// 2.05s
		//fun1(res);		// 1.970s
		//fun2(res);		// 1.875s
		fun3(res);			// 0.675s
		res.clear();
	}

    return 0;
}
