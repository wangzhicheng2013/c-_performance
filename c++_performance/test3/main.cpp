#include <string.h>
#include <iostream>
using namespace std;
static const int LOOP = 10000000;
class Test
{
public:
	Test() :a(10), b(20)
	{
		memset(c, 0, sizeof(c));
	}
	int GetA() const
	{
		return a;
	}
	int GetB() const
	{
		return b;
	}
private:
	int a;
	int c[4096 * 2];
	int b;
};
class Test1
{
public:
	Test1() :a(10), b(20)
	{
		memset(c, 0, sizeof(c));
	}
	int GetA() const
	{
		return a;
	}
	int GetB() const
	{
		return b;
	}
private:
	int a;
	int c[4096 * 2];
	int b;
};
void TestFun()
{
	Test test;
	int sum = 0;
	for (int i = 0; i < LOOP; i++)
	{
		sum += test.GetA();
	}
}
void TestFun1()
{
	Test test1;
	int sum = 0;
	for (int i = 0; i < LOOP; i++)
	{
		sum += test1.GetA();
	}
}
void TestFun2()
{
	Test test;
	int sum = 0;
	for (int i = 0; i < LOOP; i++)
	{
		sum += test.GetB();
	}
}
int main()
{
	//TestFun();		// 0.023s
	//TestFun1();		// 0.023s
	TestFun2();

	return 0;
}