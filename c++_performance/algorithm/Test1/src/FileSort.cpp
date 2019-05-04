#include "FileSort.h"
FileSort FileSort::fileSort;
FileSort::FileSort()
{
}
bool FileSort::MergeSort(const FilePathConfig &config)
{
	if (false == PointerCheck(config))
	{
		return false;
	}
	if (false == OpenFileStream(config))
	{
		return false;
	}
	string str;
	while (!readfilestream0.eof())
	{
		readfilestream0 >> str;
		cout << str << endl;
		str.clear();
	}
	CloseFileStream();
	return true;
}
bool FileSort::OpenFileStream(const FilePathConfig &config)
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
