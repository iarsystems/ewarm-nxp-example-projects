/*****************************************************************************
* $Id: dsplib_testbench_fft_main.c 4271 2015-08-17 14:00:36Z oscarmo $   
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

//#include "local_target.h"
#include "cr_dsplib.h"

#include <stdio.h>
#include <math.h>

#define NPOINTS 1024

short psi_Input[NPOINTS*2];
short psi_Output[NPOINTS*2];


#define SINUSOID
//#define DC
//#define UNIT_IMPULSE

int main(void)
{
	int j;
	double pi = 4.0*atan(1.0);
	
//	TargetResetInit();

#ifdef DC
	for(j=0;j<NPOINTS;j++)
	{
		psi_Input[2*j] = 0x4000;
		psi_Input[(2*j)+1] = 0;
	}
#endif
	
#ifdef SINUSOID
	double Omega = 2*pi*2.0/NPOINTS;
	double x;
	
	for(j=0;j<NPOINTS;j++)
	{
		x = pow(2.0,14)*cos(Omega*j);
		psi_Input[2*j] = (short int)x;
		psi_Input[(2*j)+1] = 0;		
	}	
#endif

#ifdef UNIT_IMPULSE
	psi_Input[0] = 0x4000;
	
	for(j=1;j<NPOINTS;j++)
	{
		psi_Input[2*j] = 0;
		psi_Input[(2*j)+1] = 0;
	}
#endif
	
	vF_dspl_fftR4b16N1024(psi_Output, psi_Input);
//	vF_dspl_fftR4b16N256(psi_Output, psi_Input);
//	vF_dspl_fftR4b16N64(psi_Output, psi_Input);
	
	
	for(j=0;j<NPOINTS;j++)
	{
		printf("%i, %f\n",j, (pow(psi_Output[2*j],2)+pow(psi_Output[(2*j)+1],2)));
	}

	// Enter an infinite loop, just incrementing a counter
	static volatile int i = 0 ;
	while(1) {
		i++ ;
	}
	return 0 ;
}
