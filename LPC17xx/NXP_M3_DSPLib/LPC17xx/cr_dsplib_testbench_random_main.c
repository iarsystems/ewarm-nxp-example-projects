/*****************************************************************************
* $Id: cr_dsplib_testbench_random_main.c 4271 2015-08-17 14:00:36Z oscarmo $   
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
#include "cr_dsplib.h"


int main(void)
{
	static volatile int i = 0 ;
	int i_Random = 1;

	
	for(i=0;i<100;i++)
	{
		i_Random = iF_RandomNumber(i_Random);
		printf("Random number = %i\n", i_Random);
	}
	
	while(1) {
		i++ ;
	}
	return 0 ;
}

