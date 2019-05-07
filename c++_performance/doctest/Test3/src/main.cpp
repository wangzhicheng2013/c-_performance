#include "FileSort.h"
#include "doctest/doctest.h"
#include <iostream>
using namespace std;
doctest::Context g_doctest_context;
void InitDocTest(int argc, char** argv)
{
	g_doctest_context.applyCommandLine(argc, argv);
	g_doctest_context.run();
}
int main(int argc, char** argv)
{
	InitDocTest(argc, argv);
	if (g_doctest_context.shouldExit())
	{
		cout << "not run the programe again...!" << endl;
		return 0;
	}

	FilePathConfig config("./test01", "./test11", "./test");
	FileSort::GetInstance().MergeSort(config);

	return 0;
}