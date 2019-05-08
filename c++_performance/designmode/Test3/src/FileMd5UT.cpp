#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"
#include "FileMd5.h"
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <random>
static const int LOOP = 100;
bool MakeRandomValueToFile(const char* filepath)
{
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
bool GetMD5WithShell(const char* filepath, string& md5str)
{
	const char* tmp_file = "./tmp.md5";
	string cmd = "md5sum ";
	cmd += filepath;
	cmd += " > ";
	cmd += tmp_file;
	if (system(cmd.c_str()))
	{
		remove(tmp_file);
		return false;
	}
	usleep(5000);
	ifstream readfilestream(tmp_file);
	if (!readfilestream)
	{
		return false;
	}
	string line;
	while (!readfilestream.eof())
	{
		readfilestream >> line;
		if (line.empty())
		{
			continue;
		}
		md5str = line;
		break;
	}
	readfilestream.close();
	remove(tmp_file);
	return true;
}
TEST_CASE("testing File MD5  GetMd5 method")
{
	for (int i = 0; i < 100; i++)
	{
		const char* filepath = "./test";
		string md5str;
		REQUIRE(true == MakeRandomValueToFile(filepath));
		REQUIRE(true == GetMD5WithShell(filepath, md5str));
		string str;
		CHECK(true == FileMd5::GetInstance().GetMd5(filepath, str));
		CHECK(str == md5str);
		remove(filepath);
	}
}
TEST_CASE("exception testing File MD5  GetMd5 method")
{
	const char* filepath = "./test1";
	string str;
	CHECK(false == FileMd5::GetInstance().GetMd5(filepath, str));
	remove(filepath);
}
TEST_CASE("testing File MD5 FileExist method")
{
	const char* filepath = "./test";
	string md5str;
	REQUIRE(true == MakeRandomValueToFile(filepath));
	CHECK(true == FileMd5::GetInstance().GetMd5(filepath, md5str));
	FileMd5::GetInstance().UpdateMd5Cache(filepath, md5str.c_str());
	CHECK(true == FileMd5::GetInstance().FileExist(filepath));
	remove(filepath);

	const char* filepath1 = "./test1";
	REQUIRE(true == MakeRandomValueToFile(filepath1));
	CHECK(false == FileMd5::GetInstance().FileExist(filepath1));
	remove(filepath1);

	const char* filepath2 = "./test2";
	CHECK(true == FileMd5::GetInstance().FileExist(filepath2));
}
TEST_CASE("testing File MD5 FileExist method with loop")
{
	char filepath[64] = "";
	for (int i = 0; i < LOOP; i++)
	{
		snprintf(filepath, sizeof(filepath), "test_%d", i);
		string md5str;
		REQUIRE(true == MakeRandomValueToFile(filepath));
		if (i % 2)
		{
			CHECK(true == FileMd5::GetInstance().GetMd5(filepath, md5str));
			FileMd5::GetInstance().UpdateMd5Cache(filepath, md5str.c_str());
			CHECK(true == FileMd5::GetInstance().FileExist(filepath));
		}
		else
		{
			CHECK(false == FileMd5::GetInstance().FileExist(filepath));
		}
		remove(filepath);
	}
}