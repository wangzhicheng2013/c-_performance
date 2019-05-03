#include <string.h>
#include <iostream>
#include <string>
using namespace std;
static const int LOOP = 1000000;
static const size_t SIZE64 = 64;
static const size_t SIZE128 = 128;
static const size_t SIZE512 = 512;
static const size_t SIZE1024 = 1024;
static const size_t SIZE2048= 1024 * 2;
static const size_t SIZE4196 = 1024 * 4;
static const size_t SIZE8192 = 1024 * 8;
template<auto SIZE>
class Test
{
public:
	Test()
	{
		memset(buf, 'A', SIZE);
	}
private:
	char buf[SIZE];
};

int main()
{
	for (int i = 0; i < LOOP; i++)
	{
		//Test<SIZE64>*p = new Test<SIZE64>;		// tcmalloc -- 0.016s no tcmalloc -- 0.018s
		//Test<SIZE128>* p = new Test<SIZE128>;		// tcmalloc -- 0.017s no tcmalloc -- 0.018s~0.019s
		//Test<SIZE512>* p = new Test<SIZE512>;		// tcmalloc -- 0.021s no tcmalloc -- 0.026s
		//Test<SIZE1024>* p = new Test<SIZE1024>;	// tcmalloc -- 0.0284s no tcmalloc -- 0.0253s
		//Test<SIZE2048>* p = new Test<SIZE2048>;	// tcmalloc -- 0.0332s no tcmalloc -- 0.0424s
		//Test<SIZE4196>* p = new Test<SIZE4196>;	// tcmalloc -- 0.0568s no tcmalloc -- 0.0838s
		Test<SIZE8192>* p = new Test<SIZE8192>;		// tcmalloc -- 0.0568s no tcmalloc -- 0.0838s
		delete p;
	}

	return 0;
}