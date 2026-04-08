/*****************************************************************************
* $Id: cr_dsplib_pid.s 28 2014-01-27 14:45:13Z danielru $
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


// short int siF_dspl_pid(short int si_Error, tS_pid_Coeff *pS_Coeff)
//
//

#define si_Error r0
#define pS_StateCoeff r1

#define Kp r3
#define Ki r4
#define Kd r5
#define IntegratedError r6
#define LastError r6
#define Temp r6
#define Acc r7

#ifdef __IAR_SYSTEMS_ASM__
  SECTION .text:CODE:NOROOT(2)
#else
	AREA    |.text|, CODE, READONLY
#endif

  EXPORT vF_dspl_pid
vF_dspl_pid
	push {r4-r7}

	ldrsh Kp,[pS_StateCoeff]
	ldrsh Ki,[pS_StateCoeff,#2]
	ldrsh Kd,[pS_StateCoeff,#4]
	ldrsh IntegratedError,[pS_StateCoeff,#6]

	mla Acc,Ki,si_Error,IntegratedError		//update integrated error
	strh Acc,[pS_StateCoeff,#6]				//store to state

	ldrsh LastError,[pS_StateCoeff,#8]
	strh si_Error,[pS_StateCoeff,#8]
	
	mla Acc,Kp,si_Error,Acc				//add in proportional part
	
	sub Temp,si_Error,LastError
	mla Acc,Kd,Temp,Acc					//add in the difference part
	
	mov Acc,Acc,ASR#16					//scale down to short int
	pop {r4-r7}
	bx lr


	END
