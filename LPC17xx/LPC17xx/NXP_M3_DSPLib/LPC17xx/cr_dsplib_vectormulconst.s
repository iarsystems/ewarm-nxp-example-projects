/*****************************************************************************
* $Id: cr_dsplib_vectormulconst.s 28 2014-01-27 14:45:13Z danielru $
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

//int iF_dspl_vectmulconst32(int *pi_y, int *pi_x, int i_c, int i_VectorLen)
#define pi_y r0
#define pi_x r1
#define i_c r2
#define i_VectorLen r3
#define i_Acc r4
#ifdef __IAR_SYSTEMS_ASM__
  SECTION .text:CODE:NOROOT(2)
#else
	AREA    |.text|, CODE, READONLY
#endif

	EXPORT iF_dspl_vectmulconst32
iF_dspl_vectmulconst32
	push {r4}
innerloop
	ldr i_Acc,[pi_x],#4
	subs i_VectorLen,i_VectorLen,#1
	mul i_Acc,i_Acc,i_c
	str i_Acc,[pi_y],#4
	bne innerloop
	
	pop {r4}
	bx lr
	

//int iF_dspl_vectmulconst32(int *psi_y, int *psi_x, int si_c, int i_VectorLen)
#define psi_y r0
#define psi_x r1
#define si_c r2

	EXPORT iF_dspl_vectmulconst16
iF_dspl_vectmulconst16
	push {r4}
innerloop16
	ldrsh i_Acc,[psi_x],#2
	subs i_VectorLen,i_VectorLen,#1
	mul i_Acc,i_Acc,si_c
	strh i_Acc,[psi_y],#2
	bne innerloop16
	
	pop {r4}
	bx lr
	END
