#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <sched.h>
#include <numaif.h>
#define NUM_FUNCS 100000

typedef int (*Fptr) (int, int);
extern void libInit(Fptr*, int);

int driverMain(char* memAddress, int memSize)
{
    Fptr funcArr[NUM_FUNCS] = {0};
    // initializing function pointers.
    libInit(funcArr, NUM_FUNCS);
    
    // calling random functions from the library 1000 times
    for (int i = 0; i < 1000000; i++)
    {
        int funcIdx = rand() % NUM_FUNCS;
        Fptr f = funcArr[funcIdx];
        assert(f != 0);

        int arg1 = rand() % 110;
        int arg2 = rand() % 55;
        int rVal = (*f)(arg1, arg2);

        //for (int j = 0; j < 100; j++)
        //*(((memAddress) + ((j * 1009) % memSize)) += rVal;
    }
    return 0;
}

