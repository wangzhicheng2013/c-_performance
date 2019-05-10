#include <stdio.h>
#include <iostream>
using namespace std;
void test(int count) {
	int i;
	for (i = 1;i < count;i++)	{
		if (i % 3 == 0)
			printf("%d is divisible by 3\n", i);
		if (i % 11 == 0)
			printf("%d is divisible by 11\n", i);
		if (i % 13 == 0)
			printf("%d is divisible by 13\n", i);
	}
}
int main(int argc, char* argv[])
{
	int i = 0;
	if (argc == 2)
		i = atoi(argv[1]);
	else
		i = 10;

	printf("arg is %d\n", i);

	test(i);

	return EXIT_SUCCESS;
}