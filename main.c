#include <stdio.h>

int hello(void);

int main(void)
{
	printf("%d",hello());
	return 0;
}

int hello(void)
{
	printf("Hello, world!\n");
	return 2;

}

