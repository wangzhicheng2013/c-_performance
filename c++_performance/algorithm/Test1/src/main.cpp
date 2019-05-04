#include "FileSort.h"
int main()
{
	FilePathConfig config("./test0", "./test1", "./test");
	FileSort::GetInstance().MergeSort(config);
	auto x = 10;
	return 0;
}