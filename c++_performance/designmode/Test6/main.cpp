#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
using namespace std;

auto foo(int bar) {
	const char t = bar + 'A';
	cout << "bar = " << bar << endl;
	cout << "t = " << t << endl;
	return [=](int b)->char {
		cout << "b = " << b << endl;
		cout << "t = " << t << endl;
		const char res = t + b;
		cout << "res = " << res << endl;
		return res;
	};
}
auto gcounter(int initial) {
	shared_ptr<int>pt(new int(initial));
	cout << "use count = " << pt.use_count() << endl;
	unordered_map<string, function<int()>>functions;
	functions["reset"] = [=]()->int {
		*pt = initial;
		return *pt;
	};
	functions["next"] = [=]()->int {
		return (*pt)++;
	};
	return functions;
}
int main() {
	vector<function<char(int)>>vec;
	for (int i = 0; i < 10; i++) {
		vec.emplace_back(foo(i));
		cout << vec[i](1) << endl;
	}
	auto gcounterA = gcounter(10);
	auto gcounterB = gcounter(100);
	for (int i = 0; i < 10; i++) {
		cout << gcounterA["next"]() << endl;
		cout << gcounterB["next"]() << endl;
	}
	gcounterA["reset"]();
	gcounterB["reset"]();
	cout << gcounterA["next"]() << endl;
	cout << gcounterB["next"]() << endl;

	return 0;
}