/*****************************************************************************
* $Id: cr_dsplib_biquad32.s 28 2014-01-27 14:45:13Z danielru $
*
* Part of the NXP LPC1000 Cortex-M3 family DSP library
* Developed for NXP by Code Red Technologies Inc.
*
*           (c) Copyright 2010, NXP Semiconductors
*                               All Rights Reserved
*
*                                 www.nxp.com
*                           www.code-red-tech.com
*
 *   History
 *   2010.01.20  version 1.00    Preliminary version, first Release
 *
*****************************************************************************/



// void vF_dspl_biquad32(int *pi_Output, int *pi_Input, tS_biquad32_StateCoeff *pS_StateCoeff, int i_NSamples)
//
// Using 2.14 x 2.14 ->4.28 arithmetic with no saturation
//
#define pi_Output r0
#define pi_Input r1
#define pS_StateCoeff r2
#define i_NSamples r3

#define a1 r4
#define a2 r5
#define b0 r6
#define b1 r7
#define b2 r8
#define yn_1 r9
#define yn_2 r10
#define Acc1 r11
#define xn r12
#define Acc2 r14

#ifdef __IAR_SYSTEMS_ASM__
  SECTION .text:CODE:NOROOT(2)
#else
	AREA    |.text|, CODE, READONLY
#endif

	EXPORT vF_dspl_biquad32
vF_dspl_biquad32
	push {r4-r12,r14}

//	ldmia pS_StateCoeff,{r4-r10}		//get the coefficients and state
	ldrsh r4,[pS_StateCoeff]
	ldrsh r5,[pS_StateCoeff,#2]
	ldrsh r6,[pS_StateCoeff,#4]
	ldrsh r7,[pS_StateCoeff,#6]
	ldrsh r8,[pS_StateCoeff,#8]
	ldrsh r9,[pS_StateCoeff,#10]
	ldrsh r10,[pS_StateCoeff,#12]
	
InnerLoop
	ldr xn,[pi_Input],#4
	
	//compute recusrive part
	mla Acc1,a1,yn_1,xn
	mla Acc1,a2,yn_2,Acc1

	//compute non-recursive part
	mov Acc1,Acc1,ASR#14				//normalize back to 2.14
	mul Acc2,b0,Acc1
	mla Acc2,b1,yn_1,Acc2
	mla Acc2,b2,yn_2,Acc2
	
	//update the state
	mov yn_2,yn_1
	mov yn_1,Acc1
	
	//store result
	str Acc2,[pi_Output],#4
	
	subs.n i_NSamples,i_NSamples,#1
	bne.n InnerLoop

	//store updated state to memory	
	strh yn_1, [pS_StateCoeff,#(2*5)]
	strh yn_2, [pS_StateCoeff,#(2*6)]
	
	pop {r4-r12,r14}
	bx lr

	END
