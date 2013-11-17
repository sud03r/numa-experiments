#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <sched.h>
#include <numaif.h>
#define NUM_FUNCS 100

typedef int (*Fptr) (int, int, char*, int);
extern void libInit(Fptr*, int);

int driverMain(char* memAddress, int memSize)
{
    Fptr funcArr[NUM_FUNCS] = {0};
    // initializing function pointers.
    libInit(funcArr, NUM_FUNCS);
    
    Fptr f0 = funcArr[0];
    void * ptrToCheck = (void*)(f0);
    int status[1];
    int ret_code;
    status[0] = -1;
    ret_code = move_pages(0 , 1, &ptrToCheck, NULL, status, 0);
    printf("Memory for library is at %d node \n", status[0]);
    /*ptrToCheck = (void*)(funcArr[NUM_FUNCS - 1]);
    ret_code = move_pages(0, 1, &ptrToCheck, NULL, status, 0);
    printf("Memory for f99999 is at %d node \n", status[0]);

    printf("Thread running on CPU: %d, address of func0: %p\n", sched_getcpu(), funcArr[0]);
    return 0;;
    */

    // calling random functions from the library 1000 times
    for (int i = 0; i < 10000; i++)
    {
        int funcIdx = rand() % NUM_FUNCS;
        Fptr f = funcArr[funcIdx];
        assert(f != 0);

        int arg1 = rand() % 110;
        int arg2 = rand() % 55;
        (*f)(arg1, arg2, memAddress, memSize);
    }
    return 0;
}

