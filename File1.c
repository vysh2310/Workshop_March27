#include "include.h"

/*============================================================================
 *  MEMORY LEAK-Did the developer forget something?
 *==========================================================================*/
void bug_memleak(void) {
    int* pi = (int*)malloc(sizeof(int));
    if (pi == NULL) return;

    *pi = 42;
}      


/*============================================================================
 *  Overflow - Did not expect that?
 *==========================================================================*/

unsigned int function_one(void)
{
        char a = -1;
        unsigned int divisor= 0;

        if(a< 0u)
        {
                divisor = a;
        }
        return 100/divisor;
}

/*============================================================================
 *  Shifting beyond - watchout!!!
 *==========================================================================*/

unsigned short bug_shiftovfl(void) {
    const unsigned short mask2bits = 0x0003U;
    const int num = 42;
        int size1, size2, size3;
    unsigned short res;

        size1 = sizeof(mask2bits + num);
        size2 = sizeof(num + size1);
        size3 = sizeof(num + 3);

    res = mask2bits << num;       

    return res;
}
