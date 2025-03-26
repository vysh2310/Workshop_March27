#include "include.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include <limits.h>
#include <errno.h>
#define SIZEN100 100
#define BUFF_SIZE 128
extern int tab1[SIZEN100];
static int tainted_int_source(void);
enum {
    SIZE10  =  10,
    SIZE100 = 100,
    SIZE128 = 128
};

#define fatal_error() abort()

/*============================================================================
 *  Main() 
 *==========================================================================*/
void main(void)
{
    volatile int temp;
	
    PowerLevel = -10000;

    RTE();
    
    switch (temp) {
    case 0:
        temp = reset_temperature(v3);
        break;
    case 1: 
       //do something    
    break;
    default:
        //do nothing
        break;
    }

}
/*============================================================================
 *  Tainted Index- Did you verify what comes from an external source?
 *==========================================================================*/

static int tainted_int_source(void) {
  return strtol(getenv("INDEX"),NULL,10);
}
int taintedarrayindex(void) {
	int num = tainted_int_source();
    return tab1[num];  
}


/*============================================================================
 *  Dangerous function- Did you not forsee the danger?
 *==========================================================================*/

int dangerous_func(char *str)
{
    char dst[BUFF_SIZE];
    int r = 0;

    if (sprintf(dst, "%s", str) == 1)
    {
        r += 1;
        dst[BUFF_SIZE-1] = '\0';
    }
    
    return r;
}

/*============================================================================
 *  DATA RACE
 *==========================================================================*/
int bad_glob1;      /* Defect Data race                                     */
int good_glob1;     /* Fix: Use a critical section to protect all accesses  */


void bug_datarace_task1(void) {
    bad_glob1 = 1;
}

void bug_datarace_task2(void) {
    int local_var;
    local_var = bad_glob1;
    (void)printf("%d", local_var);
}