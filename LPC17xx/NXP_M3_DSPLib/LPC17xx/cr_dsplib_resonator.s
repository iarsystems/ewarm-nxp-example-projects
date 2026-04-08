/*****************************************************************************
* $Id: cr_dsplib_resonator.s 28 2014-01-27 14:45:13Z danielru $
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


// void vF_dspl_resonator(int *psi_Output, void *pS_StateCoeff, int i_NSamples)

#define psi_Output r0
#define pS_StateCoeff r1
#define i_NSamples r2

#define a1 r3
#define yn_1 r4
#define yn_2 r5
#define Acc r6

#ifdef __IAR_SYSTEMS_ASM__
  SECTION .text:CODE:NOROOT(2)
#else
	AREA    |.text|, CODE, READONLY
#endif

	EXPORT vF_dspl_resonator
vF_dspl_resonator
	push {r3-r6}

	ldmia pS_StateCoeff,{r3-r5}

	mvn yn_2,yn_2,LSL#14
InnerLoop
	mla Acc,a1,yn_1,yn_2
	mvn yn_2,yn_1,LSL#14
	mov yn_1,Acc,ASR#14
	str yn_1,[psi_Output],#4
	subs.n i_NSamples,i_NSamples,#1
	bne.n InnerLoop
	
	str yn_1, [pS_StateCoeff,#8]
	mvn yn_2,yn_2,ASR#14
	str yn_2, [pS_StateCoeff,#12]
	
	pop {r3-r6}
	bx lr

	END
