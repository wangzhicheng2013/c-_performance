#include <iostream>
#include <string>
using namespace std;
static const int LOOP = 1000000;
// 0.122s
void Test0()
{
	string str;
	const char* p = "message";
	str = p;
}
// 0.893s
void Test1()
{
	const char* p = "message";
	string str(p);
}
int main()
{
	for (int i = 0; i < LOOP; i++)
	{
		Test1();	// Test1() => Test0() 28%
	}
	return 0;
}