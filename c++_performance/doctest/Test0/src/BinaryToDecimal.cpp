#include "BinaryToDecimal.h"
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
