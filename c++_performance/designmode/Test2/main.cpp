#include <iostream>
using namespace std;
template<class T, int rows, int cols>
class Matrix
{
	static_assert(rows > 0, "rows must be greater than 0");
	static_assert(cols > 0, "cols must be greater than 0");
};
class Test
{
public:
	void f()
	{

	}
};
template<class Function>
enable_if<!is_member_function_pointer_v<Function>>foo(Function &&f)
{

}
int main()
{
	static_assert(8 == sizeof(void *), "expect 64 bits system");
	Matrix<int, 10, 12>matrix;
	foo([]() {});
	foo(&Test::f);

	return 0;
}