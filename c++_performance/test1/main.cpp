#include <iostream>
#include <string>
using namespace std;
static const int LOOP = 1000000;
class Test
{
public:
	Test(const char* p)
	{
		str = p;
	}
private:
	string str;
};
class Test1
{
public:
	Test1(const char* p) : str(p)
	{
	}
private:
	string str;
};
int main()
{
	for (int i = 0; i < LOOP; i++)
	{
		//Test test("Hleoo11111111111111111212JJJK");	// 0.207s
		Test1 test("Hleoo11111111111111111212JJJK");	// 0.207s
	}

	return 0;
}