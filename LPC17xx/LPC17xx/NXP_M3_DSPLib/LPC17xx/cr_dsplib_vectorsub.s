/*****************************************************************************
* $Id: cr_dsplib_vectorsub.s 28 2014-01-27 14:45:13Z danielru $
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

#define pi_z r0
#define pi_x r1
#define pi_y r2
#define i_VectorLen r3

#define i_x r4
#define i_y r5

#ifdef __IAR_SYSTEMS_ASM__
  SECTION .text:CODE:NOROOT(2)
#else
	AREA    |.text|, CODE, READONLY
#endif

//void vF_dspl_vectsub32(int *pi_z, int *pi_x, int *pi_y, int i_VectorLen)
	EXTERN iF_dspl_vectsub32
vF_dspl_vectsub32
	push {i_x,i_y}
	
innerloop
	ldr i_x,[pi_x],#4
	ldr i_y,[pi_y],#4
	subs i_VectorLen,i_VectorLen,#1
	sub i_x,i_x,i_y
	str i_x,[pi_z],#4
	bne innerloop

	pop {i_x,i_y}
	bx lr
	
//void vF_dspl_vectsub16(int *psi_z, int *psi_x, int *psi_y, int i_VectorLen)
	EXTERN iF_dspl_vectsub16
vF_dspl_vectsub16
	push {i_x,i_y}
	
innerloop16
	ldrsh i_x,[pi_x],#2
	ldrsh i_y,[pi_y],#2
	subs i_VectorLen,i_VectorLen,#1
	sub i_x,i_x,i_y
	strh i_x,[pi_z],#2
	bne innerloop16

	pop {i_x,i_y}
	bx lr
	
	END
