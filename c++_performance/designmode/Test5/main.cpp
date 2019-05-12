#include <iostream>
#include <vector>
#include <memory>
#include <functional>
using namespace std;
class Bad {
public:
	Bad() {
	}
	~Bad() {
		cout << "~Bad() is called...!" << endl;
	}
	shared_ptr<Bad>getptr() {
		return shared_ptr<Bad>(this);
	}
};
struct Good : enable_shared_from_this<Good> {
public:
	Good() {
	}
	~Good() {
		cout << "~Good() is called...!" << endl;
	}
	shared_ptr<Good>getptr() {
		return shared_from_this();
	}
};
template<class T>
class ObjectPool {
public:
	using DelType = function<void(T*)>;
public:
	void add(unique_ptr<T>ptr) {
		pool.emplace_back(move(ptr));
	}
	unique_ptr<T, DelType>get() {
		if (pool.empty()) {
			return nullptr;
		}
		unique_ptr<T, DelType>ptr(pool.back().release(), [this](T *t) {
			pool.emplace_back(unique_ptr<T>(t));
			});
		pool.pop_back();
		return move(ptr);
	}
	int size() {
		return pool.size();
	}
private:
	vector<unique_ptr<T>>pool;

};
class Test {
public:
	Test() {
		cout << "Test()" << endl;
	}
	~Test() {
		cout << "~Test()" << endl;
	}
};
int main() {
	
	shared_ptr<Bad>bp1(new Bad);
	cout << bp1.use_count() << endl;
	shared_ptr<Bad>bp2(bp1->getptr());
	cout << bp2.use_count() << endl;
	
	shared_ptr<Good>gp1(new Good);
	cout << gp1.use_count() << endl;
	shared_ptr<Good>gp2(gp1->getptr());
	cout << gp2.use_count() << endl;

	ObjectPool<Test>test_pool;
	test_pool.add(unique_ptr<Test>(new Test));
	{
		auto ptr = test_pool.get();
		cout << test_pool.size() << endl;
	}
	cout << test_pool.size() << endl;

	return 0;
}