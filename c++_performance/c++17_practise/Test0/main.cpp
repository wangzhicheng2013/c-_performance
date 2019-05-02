#include <iostream>
#include <string>
#include <variant>
#include <tuple>
using namespace std;
template<class T, auto BUFSIZE>
class buffer_t
{
private:
	T data[BUFSIZE];
};
template<auto n, typename... Args>
variant<Args...>_tuple_index(size_t i, const tuple<Args...>&tpl)
{
	if (i == n)
	{
		return get<n>(tpl);
	}
}
tuple<int, double, string>f()
{
	return make_tuple(1, 1.809, "123");
}
int main()
{
	buffer_t<int, 128>myBuffer;
	auto [x, y, z] = f();
	return 0;
}