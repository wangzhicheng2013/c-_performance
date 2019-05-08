#include<stdio.h>
union Ip
{
	unsigned int num;
	struct
	{
		unsigned char c1;
		unsigned char c2;
		unsigned char c3;
		unsigned char c4;
	}ip;
};
int main()
{
	union Ip my_ip;
	my_ip.num = 678273849;
	printf("%d.%d.%d.%d", my_ip.ip.c1, my_ip.ip.c2, my_ip.ip.c3, my_ip.ip.c4);
	return 0;
}