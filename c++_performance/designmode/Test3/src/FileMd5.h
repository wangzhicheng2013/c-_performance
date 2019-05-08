#pragma once
#include <stdio.h>
#include <openssl/md5.h>
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <mutex>
using namespace std;
class FileMd5
{
public:
	inline static FileMd5& GetInstance()
	{
		return fileMd5;
	}
	bool GetMd5(const char *, string &);
	void UpdateMd5Cache(const char *, const char *);
	bool FileExist(const char*);
private:
	bool ExceptionCheck(const char*);
	void GetMd5ContextFromFile(MD5_CTX &);
	void ComputeMd5Value(MD5_CTX &, string &);
private:
	FileMd5();
	virtual ~FileMd5();
private:
	const int BUFF_SIZE = 1024 * 16;
private:
	static FileMd5 fileMd5;
private:
	unordered_map<string, string>map_path_md5;
	mutex global_lock;
private:
	ifstream read_file_stream;
};

