#include <iostream>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace chrono;
static const int LOOP = 100000000;
static const int ROWS = 3;
static const int COLUMNS = 3;
int array[ROWS][COLUMNS] = { 1, 2, 8, 4, 5, 9, 7, 11, 20 };
auto elem = 11;

bool TestWithSeq()
{
	for (int i = 0;i <ROWS;i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			if (elem == array[i][j])
			{
				return true;
			}
		}
	}
	return false;
}
bool BinaryFind(int row, int col)
{
	int low = 0;
	int high = col;
	int mid = 0;
	while (low <= high)
	{
		mid = (low + high) >> 1;
		if (elem == array[row][mid])
		{
			return true;
		}
		if (elem < array[row][mid])
		{
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}
	return false;

}
bool TestWithAlgorithm()
{
	int row = 0;
	int col = COLUMNS - 1;
	while (row < ROWS)
	{
		if (elem == array[row][col])
		{
			return true;
		}
		if (array[row][col] < elem)
		{
			++row;
		}
		else
		{
			return BinaryFind(row, col - 1);
		}
	}
	return false;
}
int main()
{
	auto begin = high_resolution_clock::now();
	for (int i = 0; i < LOOP; i++)
	{
		//TestWithSeq();		// 1.315s
		TestWithAlgorithm();
	}
	auto end = high_resolution_clock::now();
	cout << "elapse time = " << duration_cast<milliseconds>(end - begin).count() << "ms" << endl;


	return 0;
}