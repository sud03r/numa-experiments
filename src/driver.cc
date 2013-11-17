#include <cstdlib>
#include <cstdio>
#include <cassert>
#define NUM_FUNCS 100000

typedef int (*Fptr) (int, int);
extern void libInit(Fptr*, int);

int driverMain()
{
    Fptr funcArr[NUM_FUNCS] = {0};
    // initializing function pointers.
    libInit(funcArr, NUM_FUNCS);
    
    // calling random functions from the library 1000 times
    for (int i = 0; i < 10000000; i++)
    {
        int funcIdx = rand() % NUM_FUNCS;
        Fptr f = funcArr[funcIdx];
        assert(f != 0);

        int arg1 = rand() % 110;
        int arg2 = rand() % 55;
        (*f)(arg1, arg2);
    }
    printf("driver.cc done!\n");
    return 0;
}

