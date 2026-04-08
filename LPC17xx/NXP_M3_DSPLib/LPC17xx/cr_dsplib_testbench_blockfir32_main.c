/*****************************************************************************
* $Id: cr_dsplib_testbench_blockfir32_main.c 4271 2015-08-17 14:00:36Z oscarmo $   
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
#define NTAPS 64

int pi_Input[TESTSIZE+NTAPS-1];
int pi_Output[TESTSIZE];

int pi_Coeffs[NTAPS];


void vF_dsplib_testbench_blockfir32(void)
{
	int j;
	
	tS_blockfir32_Coeff S_Coeff =
	{
			pi_Coeffs,
			NTAPS
	};
	
	pi_Input[0] = 1;
	pi_Coeffs[0] = 1;
	pi_Input[32] = 1;
		
	vF_dspl_blockfir32(pi_Output, pi_Input, &S_Coeff, TESTSIZE);
	
	for(j=0;j<TESTSIZE;j++)
	{	
		printf("%i, %i\n",j,pi_Output[j]);
	}
}

int main(void) {
	
	vF_dsplib_testbench_blockfir32();
	
	// Enter an infinite loop, just incrementing a counter
	static volatile int i = 0 ;
	while(1) {
		i++ ;
	}
	return 0 ;
}
