/*****************************************************************************
* $Id: cr_dsplib_vectsumofsquares.s 28 2014-01-27 14:45:13Z danielru $
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

#define pi_x r0
#define i_VectorLen r1
#define i_x r2
#define i_Acc r3

#ifdef __IAR_SYSTEMS_ASM__
  SECTION .text:CODE:NOROOT(2)
#else
	AREA    |.text|, CODE, READONLY
#endif

//int iF_dspl_vectsumofsquares32(int *pi_x, int i_VectorLen)
	EXPORT iF_dspl_vectsumofsquares32
iF_dspl_vectsumofsquares32
	mov i_Acc,#0
innerloop
	ldr i_x,[pi_x],#4
	subs i_VectorLen,i_VectorLen,#1
	mla i_Acc,i_x,i_x,i_Acc
	bne innerloop

	mov r0,i_Acc	//return value			
	bx lr

//int iF_dspl_vectsumofsquares16(short int *psi_x, int i_VectorLen)
	EXPORT iF_dspl_vectsumofsquares32
iF_dspl_vectsumofsquares16
	mov i_Acc,#0
innerloop16
	ldrsh i_x,[pi_x],#4
	subs i_VectorLen,i_VectorLen,#1
	mla i_Acc,i_x,i_x,i_Acc
	bne innerloop16

	mov r0,i_Acc	//return value			
	bx lr
	
	END
