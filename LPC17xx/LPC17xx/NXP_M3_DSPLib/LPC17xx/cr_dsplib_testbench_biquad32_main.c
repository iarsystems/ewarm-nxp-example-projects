/*****************************************************************************
* $Id: cr_dsplib_testbench_biquad32_main.c 4271 2015-08-17 14:00:36Z oscarmo $   
*
* Part of the NXP LPC1000 Cortex-M3 family DSP library
* Developed for NXP by Code Red Technologies Inc.
*
*           (c) Copyright 2010, NXP Semiconductors
*                               All Rights Reserved
*
*                                 www.nxp.com
*                           www.code-red-tech.com
*
 *   History
 *   2010.01.20  version 1.00    Preliminary version, first Release
 *
*****************************************************************************/


#include <stdio.h>
#include <math.h>
#include "cr_dsplib.h"

#define TESTSIZE 128

int pi_Input[TESTSIZE];
int pi_Output[TESTSIZE];

void vF_dsplib_testbench_biquad32(void)
{
	int j;
	
	tS_biquad32_StateCoeff S_StateCoeff =
	{
			{0x2000,0,0x4000,0,0},
			{0,0}
	};
	
	pi_Input[0] = 0x10000000;

		
	vF_dspl_biquad32(pi_Output, pi_Input, &S_StateCoeff, TESTSIZE);
	
	for(j=0;j<TESTSIZE;j++)
	{	
		printf("%i, %8.8x\n",j,pi_Output[j]);
	}
}

int main(void) {
	
	vF_dsplib_testbench_biquad32();
	
	// Enter an infinite loop, just incrementing a counter
	static volatile int i = 0 ;
	while(1) {
		i++ ;
	}
	return 0 ;
}
