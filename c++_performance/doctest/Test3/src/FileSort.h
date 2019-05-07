#pragma once
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;
class FilePathConfig
{
public:
	FilePathConfig(const char* path0, const char* path1, const char* path) : filepath0(path0), filepath1(path1), filepath(path)
	{
	}
public:
	const char* filepath0;
	const char* filepath1;
	const char* filepath;
};
class FileSort
{
public:
	static inline FileSort& GetInstance()
	{
		return fileSort;
	}
	bool MergeSort(const FilePathConfig&);
	bool TestPointerCheck(const FilePathConfig& config)
	{
		return PointerCheck(config);
	}
private:
	inline bool PointerCheck(const FilePathConfig& config)
	{
		return nullptr != config.filepath0 && nullptr != config.filepath1 && nullptr != config.filepath;
	}
	bool OpenFileStream(const FilePathConfig&);
	bool ReadFileStream();
	void ReadFileStream(ifstream& readfilestream, vector<int>&);
	inline void CloseFileStream()
	{
		readfilestream0.close();
		readfilestream1.close();
		writefilestream.close();
	}
	void CoreSort();
private:
	FileSort();
	virtual ~FileSort();
private:
	static FileSort fileSort;
private:
	ifstream readfilestream0;
	ifstream readfilestream1;
	ofstream writefilestream;
private:
	vector<int>v;
private:
	size_t curr_size;
private:
	const size_t MAXFILESIZE = 1024 * 1024;
};

