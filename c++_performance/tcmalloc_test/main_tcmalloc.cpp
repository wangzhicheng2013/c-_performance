#include <sys/time.h>
#include <string.h>
#include <iostream>
#include <string>
using namespace std;
static const int LOOP = 1000000;
static const int TRYCNT = 5;
static const size_t SIZE64 = 64;
static const size_t SIZE128 = 128;
static const size_t SIZE512 = 512;
static const size_t SIZE1024 = 1024;
static const size_t SIZE2048= 1024 * 2;
static const size_t SIZE4196 = 1024 * 4;
static const size_t SIZE8192 = 1024 * 8;
static const size_t SIZE16K = SIZE8192 * 2;
static const size_t SIZE64K = SIZE16K * 4;
static const size_t SIZE128K = SIZE64K * 2;
static const size_t SIZE256K = SIZE128K * 2;
static const size_t SIZE512K = SIZE256K * 2;
static const size_t SIZE1M = SIZE1024 * SIZE1024;
static const size_t SIZE2M = 2 * SIZE1024 * SIZE1024;
static const size_t SIZE4M = 4 * SIZE1024 * SIZE1024;

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
	struct  timeval start_time = { 0 };
	struct  timeval end_time = { 0 };
	long total_use_useconds = 0;
	long use_useconds = 0;
	for (int cnt = 0; cnt < TRYCNT; cnt++)
	{
		gettimeofday(&start_time, NULL);
		for (int i = 0; i < LOOP; i++)
		{
			//Test<SIZE64>*p = new Test<SIZE64>;		// tcmalloc -- 0.009886s no tcmalloc -- 0.012395s
			//Test<SIZE128>* p = new Test<SIZE128>;		// tcmalloc -- 0.010792s no tcmalloc -- 0.013048s
			//Test<SIZE512>* p = new Test<SIZE512>;		// tcmalloc -- 0.014859s no tcmalloc -- 0.021042s
			//Test<SIZE1024>* p = new Test<SIZE1024>;	// tcmalloc -- 0.017297s no tcmalloc -- 0.026832s
			//Test<SIZE2048>* p = new Test<SIZE2048>;	// tcmalloc -- 0.025106s no tcmalloc -- 0.037326s
			//Test<SIZE4196>* p = new Test<SIZE4196>;	// tcmalloc -- 0.048244s no tcmalloc -- 0.081024s
			//Test<SIZE8192>* p = new Test<SIZE8192>;	// tcmalloc -- 0.079766s no tcmalloc --  0.13078s
			//Test<SIZE1M>* p = new Test<SIZE1M>;		// tcmalloc -- 66.8704s no tcmalloc -- 20.6404s
			//Test<SIZE2M>* p = new Test<SIZE2M>;		// tcmalloc -- 132.338s no tcmalloc -- 40.8435s
			//Test<SIZE4M>* p = new Test<SIZE4M>;		// tcmalloc -- 132.338s no tcmalloc -- 40.8435s
			//Test<SIZE16K>* p = new Test<SIZE16K>;		// tcmalloc -- 0.144789s no tcmalloc -- 0.230068s
			//Test<SIZE64K>* p = new Test<SIZE64K>;		// tcmalloc -- 0.862808s no tcmalloc -- 1.1384s
			//Test<SIZE128K>* p = new Test<SIZE128K>;	// tcmalloc -- 1.81849s no tcmalloc -- 2.2705s
			//Test<SIZE256K>* p = new Test<SIZE256K>;	// tcmalloc -- 3.9121s no tcmalloc -- 4.65727s
			Test<SIZE512K>* p = new Test<SIZE512K>;		// tcmalloc -- 3.9121s no tcmalloc -- 4.65727s
			delete p;
		}
		gettimeofday(&end_time, NULL);
		use_useconds = 1000000 * (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec);
		total_use_useconds += use_useconds;
	}
	cout << "average elapse time = " << total_use_useconds / TRYCNT / 1e6 << "s" << endl;
	return 0;
}