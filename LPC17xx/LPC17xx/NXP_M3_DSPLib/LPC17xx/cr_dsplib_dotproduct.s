/*****************************************************************************
* $Id: cr_dsplib_dotproduct.s 28 2014-01-27 14:45:13Z danielru $
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


#ifdef __IAR_SYSTEMS_ASM__
  SECTION .text:CODE:NOROOT(2)
#else
	AREA    |.text|, CODE, READONLY
#endif

//int iF_dspl_dotproduct32(int *pi_x, int *pi_y, int i_VectorLen);
// r0 = *pi_x
// r1 = *pi_y
// r2 = i_VectorLen
// r3 = Accumulated result
// r4 = x element
// r5 = y element

	EXPORT iF_dspl_dotproduct32
iF_dspl_dotproduct32
	push {r4,r5}
innerloop
	ldr r4,[r0],#4
	ldr r5,[r1],#4
	mla r3,r4,r5,r3
	subs r2,r2,#1
	bne innerloop
	pop {r4,r5}
	bx lr
	END
