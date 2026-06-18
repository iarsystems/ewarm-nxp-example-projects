/*****************************************************************************
* $Id: testbench_resonator_main.c 4271 2015-08-17 14:00:36Z oscarmo $   
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

#define FRACTBITS 14
#define SCALEFACTOR pow(2,FRACTBITS)
#define Amplitude 1.0
#define TESTSIZE 512

int pi_Output[TESTSIZE];

void vF_dsplib_testbench_resonator(void)
{
	double pi = 4.0*atan(1.0);
	double Omega = 2*pi*1.0/100.0;
	int j;
	
	tS_ResonatorStateCoeff S_ResonatorStateCoeff =
	{
			((2.0 * cos(Omega)) * SCALEFACTOR),
			0,
			((-Amplitude * sin(Omega)) * SCALEFACTOR)
	};
		
	vF_dspl_resonator(pi_Output, &S_ResonatorStateCoeff, 512);
	
	for(j=0;j<512;j++)
	{	
		printf("%i, %i\n",j,pi_Output[j]);
	}
}

int main(void) {
	
	vF_dsplib_testbench_resonator();
	
	// Enter an infinite loop, just incrementing a counter
	static volatile int i = 0 ;
	while(1) {
		i++ ;
	}
	return 0 ;
}
