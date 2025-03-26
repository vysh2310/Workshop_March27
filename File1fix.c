#include "include.h"
void bug_memleak(void);
unsigned int function_one(void);
unsigned short bug_shiftovfl(void);

/*============================================================================
 *  MEMORY LEAK
 *==========================================================================*/
void bug_memleak(void);
unsigned int function_one(void);
unsigned short bug_shiftovfl(void);

void bug_memleak(void) {
    int* pi = (int*)malloc(sizeof(int));
    if (pi == NULL) return;

    *pi = 42;
    free(pi);
}                               /* Defect: We forgot to free the pointer    */


/*============================================================================
 *  Division by Zero & Overflow correction
 *==========================================================================*/

unsigned int function_one(void)
{
        signed char a = -1;
        unsigned int divisor= 0;
        
        divisor = a + 2;        	
		
        return 100/divisor;
}

/*============================================================================
 *  Shifting within limits
 *==========================================================================*/
unsigned short bug_shiftovfl(void) {
    const unsigned short mask2bits = 0x0003U;
    const int num = 12;
    int size1;
    unsigned short res;

    size1 = sizeof(mask2bits);        

    res = mask2bits << num;         /* Defect: Capacity overflow correction */

    return res + size1;
}

