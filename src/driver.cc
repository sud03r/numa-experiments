#include <stdlib.h>
#include <stdio.h>

extern int func0(int, int);

int main()
{

    int t = func0(10, 100);
	printf(" result %d",t);	
    return t;
}
