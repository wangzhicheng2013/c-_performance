#include "FileSort.h"
FileSort FileSort::fileSort;
FileSort::FileSort()
{
}
bool FileSort::MergeSort(const FilePathConfig& config)
{
	if (false == PointerCheck(config))
	{
		return false;
	}
	remove(config.filepath);
	bool succ = true;
	if (true == OpenFileStream(config) && true == ReadFileStream())
	{
		CoreSort();
	}
	else
	{
		succ = false;
	}
	CloseFileStream();
	return succ;
}
bool FileSort::ReadFileStream()
{
	curr_size = 0;
	v.clear();
	ReadFileStream(readfilestream0, v);
	ReadFileStream(readfilestream1, v);
	return !v.empty();
}
void FileSort::ReadFileStream(ifstream& readfilestream, vector<int>& v)
{
	string line;
	while (!readfilestream.eof())
	{
		readfilestream >> line;
		curr_size += line.size();
		if (curr_size >= MAXFILESIZE)
		{
			return;
		}
		if (line.empty())
		{
			continue;
		}
		int number = atoi(line.c_str());
		v.emplace_back(number);
		line.clear();
	}
}
void FileSort::CoreSort()
{
	sort(begin(v), end(v));
	for (auto& e : v)
	{
		writefilestream << e << endl;
	}
}
bool FileSort::OpenFileStream(const FilePathConfig& config)
{
	readfilestream0.open(config.filepath0, ios::in);
	if (!readfilestream0.is_open())
	{
		cerr << config.filepath0 << " open failed...!" << endl;
		return false;
	}
	readfilestream1.open(config.filepath1, ios::in);
	if (!readfilestream1.is_open())
	{
		cerr << config.filepath1 << " open failed...!" << endl;
		return false;
	}
	writefilestream.open(config.filepath, ios::out);
	if (!writefilestream.is_open())
	{
		cerr << config.filepath << " open failed...!" << endl;
		return false;
	}
	return true;
}
FileSort::~FileSort()
{
}
