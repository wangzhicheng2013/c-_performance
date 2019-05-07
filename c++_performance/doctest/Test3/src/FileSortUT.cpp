#define DOCTEST_CONFIG_IMPLEMENT
#include <random>
#include "doctest/doctest.h"
#include "FileSort.h"
static const int LOOP = 1000;
bool MakeRandomValueToFile(const char* filepath)
{
	remove(filepath);
	ofstream writefilestream(filepath);
	if (!writefilestream)
	{
		return false;
	}
	random_device seed;
	mt19937_64 engine(seed());
	uniform_int_distribution<>uniformDist(0, LOOP);
	for (int i = 0; i < LOOP; i++)
	{
		writefilestream << uniformDist(engine) << endl;
	}
	writefilestream.close();
	return true;
}
bool ReadRandomValueFromFile(const char* filepath, vector<int>&vec)
{
	ifstream readfilestream(filepath);
	if (!readfilestream)
	{
		return false;
	}
	vec.clear();
	string line;
	while (!readfilestream.eof())
	{
		readfilestream >> line;
		if (line.empty())
		{
			continue;
		}
		int number = atoi(line.c_str());
		vec.emplace_back(number);
		line.clear();
	}
	readfilestream.close();
	return true;
}
TEST_CASE("testing File Sort MergeSort method")
{
	REQUIRE(true == MakeRandomValueToFile("./test0"));
	REQUIRE(true == MakeRandomValueToFile("./test1"));

	FilePathConfig config("./test0", "./test1", "./test");
	CHECK(true == FileSort::GetInstance().MergeSort(config));

	vector<int>vec;
	vector<int>tmp;
	REQUIRE(true == ReadRandomValueFromFile("./test", vec));
	tmp.resize(vec.size());
	copy(begin(vec), end(vec), begin(tmp));
	sort(begin(tmp), end(tmp));
	int size = vec.size();
	for (int i = 0; i < size; i++)
	{
		CHECK(vec[i] == tmp[i]);
	}
}
TEST_CASE("testing File Sort PointerCheck method")
{
	FilePathConfig config("./test0", "./test1", "./test");
	CHECK(true == FileSort::GetInstance().TestPointerCheck(config));
}
TEST_CASE("exception testing File Sort MergeSort method")
{
	FilePathConfig config("./test01", "./test11", "./test");
	CHECK(false == FileSort::GetInstance().MergeSort(config));

	FilePathConfig config1("./test0", "./test11", "./test");
	CHECK(false == FileSort::GetInstance().MergeSort(config1));
}