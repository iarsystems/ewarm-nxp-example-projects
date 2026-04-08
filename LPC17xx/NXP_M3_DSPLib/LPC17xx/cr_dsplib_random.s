/*****************************************************************************
* $Id: cr_dsplib_random.s 28 2014-01-27 14:45:13Z danielru $
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

//	Implements a linear-congruental random number generator
//	y = (a.Xn + c) mod m
//
//	From Knuth: a=16644525, c=32767
//

#ifdef __IAR_SYSTEMS_ASM__
  SECTION .text:CODE:NOROOT(2)
#else
	AREA    |.text|, CODE, READONLY
#endif

//int iF_RandomNumber(int i_Seed)

	EXPORT iF_RandomNumber
iF_RandomNumber
	ldr r1,=16644525	//a
	ldr r2,=32767		//c
	mla r0,r1,r0,r2		//(a*seed) + c
	bx lr
	END
