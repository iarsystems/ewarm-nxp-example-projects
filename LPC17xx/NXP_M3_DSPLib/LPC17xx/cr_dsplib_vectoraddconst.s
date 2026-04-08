/*****************************************************************************
* $Id: cr_dsplib_vectoraddconst.s 28 2014-01-27 14:45:13Z danielru $
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

#define pi_y r0
#define pi_x r1
#define i_c r2
#define i_VectorLen r3

#define i_x r4
#ifdef __IAR_SYSTEMS_ASM__
  SECTION .text:CODE:NOROOT(2)
#else
	AREA    |.text|, CODE, READONLY
#endif

//void vF_dspl_vectaddconst32(int *pi_y, int *pi_x, int i_c, int i_VectorLen)
	EXPORT vF_dspl_vectaddconst32
vF_dspl_vectaddconst32
	push {i_x}
innerloop
	ldr i_x,[pi_x],#4
	subs i_VectorLen,i_VectorLen,#1
	add i_x,i_x,i_c
	str i_x,[pi_y],#4
	bne innerloop

	pop {i_x}
	bx lr
	
	//void vF_dspl_vectaddconst16(int *psi_y, int *psi_x, int si_c, int i_VectorLen)
	EXPORT vF_dspl_vectaddconst16
vF_dspl_vectaddconst16
	push {i_x}
innerloop16
	ldrsh i_x,[pi_x],#4
	subs i_VectorLen,i_VectorLen,#1
	add i_x,i_x,i_c
	strh i_x,[pi_y],#4
	bne innerloop16

	pop {i_x}
	bx lr
	
	END
