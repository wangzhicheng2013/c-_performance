#include <unistd.h>
#include <iostream>
using namespace std;
static const int LOOP = 100000;
long sum = 0;
int fun()
{
	for (int i = 0;i < LOOP;i++)
	{
		for (int j = 0;j < LOOP;j++) 
		{
			sum += i * j / 10;
		}
	}
	return 0;
}
int fun1()
{
	for (int i = 0;i < LOOP;i++)
	{
		for (int j = 0;j < LOOP;j++) 
		{
			sum += i + j - 10;
		}
	}
	return 0;
}
int main()
{
	fun();
	fun1();

	return 0;
}
