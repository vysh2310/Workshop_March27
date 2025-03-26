/**
  * Polyspace example.
  *       Copyright 2012-2016 The MathWorks, Inc.
  */

#include <assert.h>
#include <math.h>
#include "include.h"

static const s32 S32MAX = 2147483647;
static const s32 S32MIN = 0x80000000U;
static const f32 F32MAX = 3.4028E+38f;
static const f32 F32MIN = -3.4028E+38f;


static int get_oil_pressure(void);
static void Close_To_Zero(void);
static int Non_Infinite_Loop(void);
static void Pointer_Arithmetic(void);
static void Recursion(int* depth);
static void Recursion_caller(void);
static void Square_Root_conv(double alpha, float* beta_pt);
static void Square_Root(void);
static void Unreachable_Code(void);
extern int get_bus_status(void);


static s32 sabs(s32 in);
static s32 sabs(s32 in)
{
    s32 lin = in;
    if (lin == S32MIN) {
        lin = S32MAX;
    }
    if (lin < 0) {
        lin = -lin;
    }
    return lin;
}
static f32 f_abs(f32 fin);
static f32 f_abs(f32 fin)
{
    f32 lfin = fin;
    if (!(lfin > F32MIN)) {
        lfin = F32MAX;
    }
    if (lfin < 0.0f) {
        lfin = -lfin;
    }
    return lfin;
}

/*============================================================================
 *  Volatiles and Polyspace
 *==========================================================================*/
static int get_oil_pressure(void)
{
    volatile int vol_i = 0;
    int i;

    i = vol_i;
    assert(i > 0);
    return i;
}


/*============================================================================
 * Here we demonstrate Polyspace Verifier's ability to
 * distinguish between values which are very close to
 * zero, and zero itself.
 ============================================================================*/
static void Close_To_Zero(void)
{
    f32 xmin = f_abs(random_float());
    f32 xmax = f_abs(random_float());
    f32 y;

    if (xmax > xmin) {
        if ((xmax - xmin) < 1.0E-37f) {
            y = 1.0f;
        } else {
            /* division by zero is impossible here */
            if (
                (xmax < (F32MAX / 100.0f))
                &&
                (
                    (xmin < (F32MAX / 1000.0f))
                    &&
                    ((xmax > (xmin + 200.0f)))
                )
            ) {
                y = (xmax + xmin) / (xmax - xmin);
            }
        }
    }
}



/*============================================================================
 * Here we demonstrate the ability to abstract out a very large number
 * of iterations.  Please note that this is done in linear time, since
 * Polyspace Verifier models the dynamic behavior, without execution.
 *
 * The loop must iterate 2**31 times before y>big allows it to break out.
 *
 * Correct operation is demonstrated because:
 * 1) x = x + 2 is shown to never generate an overflow
 * 2) the loop is not infinite
  ============================================================================*/
static int Non_Infinite_Loop(void)
{
    const int big = 1073741821 ;  /* 2**30-3 */
    int x = 0, y = 0;

    while (1)

    {
        if (y > big) { break;}
        x = x + 2;
        y = x / 2;
    }

    y = x / 100;
    return y;
}


/*============================================================================
   Here we demonstrate Polyspace Verifier's ability to track a
   pointer across numerous pointer arithmetic operations,
   including invalid addresses and back to valid addresses.
============================================================================*/
static void Pointer_Arithmetic(void)
{
    int array[100];
    int i, *p = array;

    for (i = 0; i < 100; i++) {
        *p = 0;
        p++;
    }
	p = array;

    if (get_bus_status() > 0) {
        // get oil pressure always return >0 so it can be removed
            *p = 5; /* Out of bounds */
        
          
        }
    

    i = get_bus_status();

    if (i == 0)  {*(p - i) = 10;}

    // the if that was here was unreachable because i can never be < 0
    // because p was reset at the start of the array, so it wouldn't
    // make sense to subtract a positive number from that (it couldn't be
    // inside the mapped memory
}


/*============================================================================
   Here we demonstrate Polyspace Verifier's ability to trace
   dynamic arithmetic properties across a recursion loop.

   The main function Recursion() takes a parameter, increments it,
   then divides by it.  This sequence of actions loops through
   an indirect recursive call to Recursion_recurse().

   If the initial value passed to Recursion() is negative, then
   the recursive loop will at some point attempt a division
   by zero.
============================================================================*/
static void Recursion(int* depth)
/* if depth<0, recursion will lead to division by zero */
{
    float advance;

    *depth = *depth + 1;
    advance = 1.0f / (float)(*depth); /* potential division by zero */


    if (*depth < 50) {
        Recursion(depth);
    }
}


static void Recursion_caller(void)
{
    int x = random_int();


    if ((x > 4) && (x < 10)) {
        Recursion(&x);     // always encounters a division by zero
    }


    x = 10;
    if (random_int() > 0) {
        Recursion(&x);     /* never encounters a division by zero */
    }
}


/*============================================================================
 * Here we demonstrate Polyspace Verifier's ability to trace numeric
 * constraints across many different arithmetic operations.
 * The table provided below the example shows the domain of
 * values for the expressions in the example.
 ============================================================================*/
static void Square_Root_conv(double alpha, float* beta_pt)
/* Perform arithmetic conversion of alpha to beta */
{
    *beta_pt = (float)((5.5 + cos(alpha)) / 5.0);
}


static void Square_Root(void)
{
    double alpha = random_float();
    float beta;
    float gamma;

    Square_Root_conv(alpha, &beta);

    gamma = (float)sqrt(beta - 0.75);   /* always sqrt(negative number) */
}


/*============================================================================
   * Here we demonstrate Polyspace Verifier's ability to
   * identify unreachable sections of code due to the
   * value constraints placed on the variables.
============================================================================*/
static void Unreachable_Code(void)
{
    int x = random_int();
    int y = random_int();

    if (x > y) {
        x = sabs(x) - sabs(y);
         // the code here was removed ...
        // because x > y, then x - y must be in 1+
        // so it can't be < 0
    }else 
	{
		// because, being drawn to look at the code by the overflow
        // it becomes clear that there is no point in making the
        // test and assignment in the if branch if the value of x2
        // is always overwritten after the if branch

    x = y;}
}


/*============================================================================
   * Interesting group of functions to call
============================================================================*/
void RTE(void)
{
    int tmp;
    Close_To_Zero();

    if (random_int() > 0) {
        tmp = Non_Infinite_Loop();
    }

    Pointer_Arithmetic();

    Recursion_caller();

    if (random_int() > 0) {
        Square_Root();
    }
	    Unreachable_Code();
}
