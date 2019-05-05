#include <stdlib.h>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include <fstream>
#include <random>
using namespace std;
static const size_t MAXSIZE = 1024 * 1024 * 10;
static const int LOOP = 1000000;
void* operator new (size_t size)
{
	if (size > MAXSIZE)
	{
		return NULL;
	}
	//cout << "new is called...!" << endl;
	return malloc(size);
}
void getString(const string& str)
{
}
void getString(string_view & str)
{
}
void TestStringView()
{
	ifstream readfileStream("./main.cpp", ios::in);
	if (!readfileStream)
	{
		cerr << "file read failed...!" << endl;
		return;
	}
	stringstream ss;
	ss << readfileStream.rdbuf();
	readfileStream.close();
	string_view filecontent = ss.str();
	cout << "file content as following:" << endl;
	cout << filecontent << endl;
	cout << "file size = " << filecontent.size() << endl;
	random_device seed;
	mt19937_64 engine(seed());
	uniform_int_distribution<>uniformDist(0, 1500);
	vector<int>vec;
	for (int i = 0; i < LOOP; i++)
	{
		vec.emplace_back(uniformDist(engine));
	}
	for (int i = 0; i < LOOP; i++)
	{
		filecontent.substr(vec[i]);
	}
}

int main()
{
	getString("01AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA2345667898");
	char p[128] = "1BBBBBBBBBBBBBBBBBBBBBNNNNNNNNNNNNNNNNNN212";
	getString(p);
	string str("121AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA2");
	string_view largeString(str.c_str(), str.size());
	getString(str);
	getString(largeString);
	cout << largeString.substr(0) << endl;
	TestStringView();

	return 0;
}