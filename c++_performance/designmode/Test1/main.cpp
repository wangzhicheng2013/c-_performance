#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;
template<class T> using add_pointer = T *;

template<typename... Values> struct meta_list {};
using list_of_ints = meta_list<char, short, int, long>;

template<class List> struct list_size;

template<template<class...> class List, class... Elements>
struct list_size<List<Elements...>>
	: integral_constant<size_t, sizeof...(Elements)> {};
constexpr auto size1 = list_size<list_of_ints>::value;
static const int LOOP = 10000000;
constexpr int fact(int n)
{
	return n <= 1 ? 1 : fact(n - 1);
}
template<size_t I>
auto& get()
{
	if constexpr (0 == I)
	{
		return "123";
	}
	else if constexpr (1 == I)
	{
		return "456";
	}
}
template<size_t I>
auto& get1()
{
	if constexpr (0 == I)
	{
		return "123";
	}
	else if constexpr (1 == I)
	{
		return "456";
	}
}
auto get2(size_t I)
{
	if (0 == I)
	{
		return "123";
	}
	else if (1 == I)
	{
		return "456";
	}
}
template<typename T>
string to_string(T t)
{
	if constexpr (is_same_v<T, string>)
	{
		return t;
	}
	return std::to_string(t);
}

int main()
{
	/*auto begin = high_resolution_clock::now();
	for (int i = 0; i < LOOP; i++)
	{
		fact(100);
	}
	auto end = high_resolution_clock::now();
	cout << "elapse time = " << duration_cast<seconds>(end - begin).count() << "s" << endl;	// 2s
	*/

	for (int i = 0; i < LOOP; i++)
	{
		get2(1);// get1 0.023s => get2 0.028s
	}
	cout << to_string(123) << endl;

	return 0;
}