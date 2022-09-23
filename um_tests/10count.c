#include <stdio.h>

int main()
{
	for (int i = 20; i < 30; i++) {
		printf("%c", i);
	}
	
	printf("\0");
	return 0;
}
