/*******************************************************************************
                                Copyright 2021 IAR Systems. All rights reserved.
Function: Plot of the Mandelbrot set

Note: Evaluation Kit for the i.MX 8M Nano
*******************************************************************************/

/******************************************************************************/
#include <stdint.h>
#include <intrinsics.h>
#include <stdio.h>
#include "misc.h"

#define RESOLUTION 100
#define IM_MIN -2.0
#define RE_MIN -2.0

/******************************************************************************/
__noreturn int main( void );
__noreturn int core1_main( void );
__noreturn int core2_main( void );
__noreturn int core3_main( void );
static int mandel(double re0, double im0, int limit);

/******************************************************************************/
char buffer[RESOLUTION + 1] = {'\0'};
/******************************************************************************/
__noreturn int main( void )
{
    const double step = 4.0 / (double) RESOLUTION;
    double im = IM_MIN;

    enable_caches_el3();

    for (int x = 0; x < RESOLUTION; im = im + step, x++)
    {
        double re = RE_MIN;
        for (int y = 0; y < RESOLUTION; re = re + step, y++)
        {
            int n = mandel(re, im, 100);
            if (n < 0)
              buffer[y] = 'x';
            else
              buffer[y] = '.';
        }
        puts(buffer);
    }

    for(;;);
}

/******************************************************************************/
__noreturn int core1_main( void )
{
    for(;;);
}

/******************************************************************************/
__noreturn int core2_main( void )
{
    for(;;);
}

/******************************************************************************/
__noreturn int core3_main( void )
{
    for(;;);
}

/******************************************************************************/
static int mandel(double re0, double im0, int limit)
{
    int n = 0;
    double re = re0;
    double im = im0;
    double reN;

    while ((re*re + im*im) < 4.0 && n <= limit)
    {
        reN = re*re - im*im + re0;
        im = 2.0*re*im + im0;
        re = reN;
        n = n + 1;
    }

    if (n >= limit)
        return -1;
    else
        return n;
}
