#include <stdio.h>
#include <stdlib.h>

int main()
{
        for (int i = 0; i < 256; i++) {
                printf("%c", i);
        }
	printf("%c", -1);
	printf("%c", 256);
        return 0;
}
