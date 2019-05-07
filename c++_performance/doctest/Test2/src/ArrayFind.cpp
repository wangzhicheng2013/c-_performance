#include "ArrayFind.h"
static int array[ROWS][COLUMNS] = { 1, 2, 8, 4, 5, 9, 7, 11, 20 };
bool BinaryFind(int row, int col, int elem)
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
bool TestWithAlgorithm(int elem)
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
			if (false == BinaryFind(row, col - 1, elem))
			{
				++row;
			}
			else
			{
				return true;
			}

		}
	}
	return false;
}