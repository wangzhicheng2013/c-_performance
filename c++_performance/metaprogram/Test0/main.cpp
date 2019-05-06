#include <iostream>
using namespace std;
static const int LOOP = 1000000;
template <unsigned long N>
struct binary
{
	static unsigned const value = binary<N / 10>::value * 2 + N % 10;
};
template<>
struct binary<0>
{
	static unsigned const value = 0;
};
unsigned binary1(unsigned long N)
{
	return (0 == N) ? 0 : (N % 10 + 2 * binary1(N / 10));
}
unsigned binary2(unsigned long N)
{
	unsigned mode = 0;
	unsigned base = 1;
	unsigned num = 0;
	while (N)
	{
		mode = N % 10;
		N /= 10;
		num += mode * base;
		base <<= 1;
	}
	return num;
}
int main()
{
	for (int i = 0; i < LOOP; i++)
	{
		//unsigned const three = binary<1101>::value;	// 0.07s
		//unsigned const three = binary1(1101);			// 0.020s
		unsigned const three = binary2(1101);			// 0.020s
	}

	return 0;
}