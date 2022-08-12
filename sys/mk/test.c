#include <stdio.h>
#include "mylib.h"

int init(int a, int b)
{
	printf("---\n");
}

int main()
{
	init(0, 1);

	int a = add(5, 3);
	int b = sub(5, 3);

	printf("5+3=%d\n", a);
	printf("5-3=%d\n", b);

	return 0;
}
