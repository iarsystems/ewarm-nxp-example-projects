/*****************************************************************************
* $Id: cr_dsplib_fft64.s 28 2014-01-27 14:45:13Z danielru $
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


#define psi_Y r0	//pointer to output vector
#define psi_x r1	//pointer to input vector

#define N r2
#define x0_r r4
#define x0_i r5
#define x1_r r6
#define x1_i r7
#define x2_r r8
#define x2_i r9
#define x3_r r10
#define x3_i r11
#define y3_r x3_i
#define y3_i x3_r
#define t0 r12
#define reversed_index r12
#define t1 r14
#define input_reversed_address r14
#define index r3
#define i_nSamplesPerGroup r3
#define psi_Twiddles r0
#define i_GroupSize r3
#define i_nBfy_nGrp r2	//used hi reg for num butterfy and lo reg for number of groups

#define FFTLEN 64
#define ADDRBITS 6
#define INDEX_CMP 16

#ifdef __IAR_SYSTEMS_ASM__
COMPLEX_MUL MACRO ar, ai, xr, xi, yr, yi, temp0, temp1
		mul temp0,xi,yr
		mul temp1,xr,yr
		mla ar,xi,yi,temp1
		mls ai,xr,yi,temp0
	ENDM
#else
	MACRO
	COMPLEX_MUL $ar, $ai, $xr, $xi, $yr, $yi, $temp0, $temp1
		mul $temp0,$xi,$yr
		mul $temp1,$xr,$yr
		mla $ar,$xi,$yi,$temp1
		mls $ai,$xr,$yi,$temp0
	MEND
#endif

#ifdef __IAR_SYSTEMS_ASM__
COMPLEX_LDR16 MACRO x_r, x_i, address_reg, post_offset
		ldrsh x_i,[address_reg,#2]						//imag part
		ldrsh.w x_r,[address_reg]							//real part
		add address_reg,address_reg,post_offset			//thumb has no register post-indexing
	ENDM
#else
	MACRO
	COMPLEX_LDR16 $x_r, $x_i, $address_reg, $post_offset
		ldrsh $x_i,[$address_reg,#2]						//imag part
		ldrsh.w $x_r,[$address_reg]							//real part
		add $address_reg,$address_reg,$post_offset			//thumb has no register post-indexing
	MEND
#endif

#ifdef __IAR_SYSTEMS_ASM__
COMPLEX_LDR16_PostDec MACRO	x_r, x_i, address_reg, post_offset
		ldrsh x_i,[address_reg,#2]						//imag part
		ldrsh.w x_r,[address_reg]							//real part
		sub address_reg,address_reg,post_offset			//thumb has no register post-indexing
	ENDM
#else
	MACRO	
	COMPLEX_LDR16_PostDec $x_r, $x_i, $address_reg, $post_offset
		ldrsh $x_i,[$address_reg,#2]						//imag part
		ldrsh.w $x_r,[$address_reg]							//real part
		sub $address_reg,$address_reg,$post_offset			//thumb has no register post-indexing
	MEND
#endif

#ifdef __IAR_SYSTEMS_ASM__
COMPLEX_STR16 MACRO x_r, x_i, address_reg, post_offset
		strh x_i, [address_reg,#2]
		strh x_r, [address_reg]
		add address_reg,address_reg,post_offset
 	ENDM
#else
	MACRO
	COMPLEX_STR16 $x_r, $x_i, $address_reg, $post_offset
		strh $x_i, [$address_reg,#2]
		strh $x_r, [$address_reg]
		add $address_reg,$address_reg,$post_offset
	MEND
#endif

#ifdef __IAR_SYSTEMS_ASM__
DFT4 MACRO ar, ai, br, bi, cr, ci, dr, di, Scaling
		add cr,cr,dr
		add ci,ci,di
		sub dr,cr,dr,lsl#1
		sub di,ci,di,lsl#1
		
		mov ar,ar,asr#2
		mov ai,ai,asr#2
		add ar,ar,br,asr#(2+Scaling)
		add ai,ai,bi,asr#(2+Scaling)
		sub br,ar,br,asr#(1+Scaling)
		sub bi,ai,bi,asr#(1+Scaling)
		
		add ar,ar,cr,asr#(2+Scaling)
		add ai,ai,ci,asr#(2+Scaling)
		sub cr,ar,cr,asr#(1+Scaling)
		sub ci,ai,ci,asr#(1+Scaling)
		
		add br,br,di,asr#(2+Scaling)
		sub bi,bi,dr,asr#(2+Scaling)
		sub di,br,di,asr#(1+Scaling)
		add dr,bi,dr,asr#(1+Scaling)
	ENDM
#else
	MACRO
	DFT4 $ar, $ai, $br, $bi, $cr, $ci, $dr, $di, $Scaling
		add $cr,$cr,$dr
		add $ci,$ci,$di
		sub $dr,$cr,$dr,lsl#1
		sub $di,$ci,$di,lsl#1
		
		mov $ar,$ar,asr#2
		mov $ai,$ai,asr#2
		add $ar,$ar,$br,asr#(2+$Scaling)
		add $ai,$ai,$bi,asr#(2+$Scaling)
		sub $br,$ar,$br,asr#(1+$Scaling)
		sub $bi,$ai,$bi,asr#(1+$Scaling)
		
		add $ar,$ar,$cr,asr#(2+$Scaling)
		add $ai,$ai,$ci,asr#(2+$Scaling)
		sub $cr,$ar,$cr,asr#(1+$Scaling)
		sub $ci,$ai,$ci,asr#(1+$Scaling)
		
		add $br,$br,$di,asr#(2+$Scaling)
		sub $bi,$bi,$dr,asr#(2+$Scaling)
		sub $di,$br,$di,asr#(1+$Scaling)
		add $dr,$bi,$dr,asr#(1+$Scaling)
	MEND
#endif
	
#ifdef __IAR_SYSTEMS_ASM__
  SECTION .text:CODE:NOROOT(2)
#else
	AREA    |.text|, CODE, READONLY
#endif
	
//void vF_dspl_fftR4b16N64(short int *psi_Y, short int *psi_x)
	EXPORT vF_dspl_fftR4b16N64
vF_dspl_fftR4b16N64
	push {r4-r12, lr}
	mov N,#FFTLEN
	mov index,#0
	
	//first stage uses no twiddle multiplies
first_stage
	rbit reversed_index, index
	add input_reversed_address,psi_x,reversed_index,LSR#(32-ADDRBITS)
	
	// do the loads for the bfy in bit-reversed order
	COMPLEX_LDR16 x0_r, x0_i ,input_reversed_address, N
	COMPLEX_LDR16 x2_r, x2_i ,input_reversed_address, N
	COMPLEX_LDR16 x1_r, x1_i ,input_reversed_address, N
	COMPLEX_LDR16 x3_r, x3_i ,input_reversed_address, N

	//do the 4-point transforms
	DFT4 x0_r, x0_i, x1_r, x1_i, x2_r, x2_i, x3_r, x3_i, 0

	COMPLEX_STR16 x0_r, x0_i, psi_Y, #4
	COMPLEX_STR16 x1_r, x1_i, psi_Y, #4
	COMPLEX_STR16 x2_r, x2_i, psi_Y, #4
	COMPLEX_STR16 x3_i, x3_r, psi_Y, #4

	add index,index,#1
	cmp index,#INDEX_CMP
	bne first_stage
	
	//reset the output point to start and use as new input pointer
	sub psi_x, psi_Y, N, lsl#2
	mov i_nSamplesPerGroup, #16			//16, 64, 256, 1024
	mov i_nBfy_nGrp, N, lsr#4			//nGrp=64/16=4
	adr psi_Twiddles, twiddle_table_new
	
stage_loop
	push {psi_x,i_nBfy_nGrp}
	add t0,i_nSamplesPerGroup,i_nSamplesPerGroup,lsl#1
	add psi_x,psi_x,t0
	sub i_nBfy_nGrp,i_nBfy_nGrp,#(1<<16)	//decrement number of bfy
	
group_loop
		add i_nBfy_nGrp,i_nBfy_nGrp,i_nSamplesPerGroup,lsl#(16-2)	//add in number of bfy per group
		
butterfly_loop
			//3 twiddle factors per butterfly
			COMPLEX_LDR16_PostDec x0_r, x0_i, psi_x, i_GroupSize
			COMPLEX_LDR16 x3_r, x3_i, psi_Twiddles, #4
			COMPLEX_MUL x3_r, x3_i, x0_r, x0_i, x3_r, x3_i, t0, t1
			
			COMPLEX_LDR16_PostDec x0_r, x0_i, psi_x, i_GroupSize
			COMPLEX_LDR16 x2_r, x2_i, psi_Twiddles, #4
			COMPLEX_MUL x2_r, x2_i, x0_r, x0_i, x2_r, x2_i, t0, t1
							
			COMPLEX_LDR16_PostDec x0_r, x0_i, psi_x, i_GroupSize
			COMPLEX_LDR16 x1_r, x1_i, psi_Twiddles, #4
			COMPLEX_MUL x1_r, x1_i, x0_r, x0_i, x1_r, x1_i, t0, t1
	
			COMPLEX_LDR16 x0_r ,x0_i, psi_x, #0
	
			DFT4 x0_r, x0_i, x1_r, x1_i, x2_r, x2_i, x3_r, x3_i, 14
	
			COMPLEX_STR16 x0_r, x0_i, psi_x, i_GroupSize
			COMPLEX_STR16 x1_r, x1_i, psi_x, i_GroupSize
			COMPLEX_STR16 x2_r, x2_i, psi_x, i_GroupSize
			COMPLEX_STR16 x3_i, x3_r, psi_x, #4
	
			subs i_nBfy_nGrp,i_nBfy_nGrp,#(1<<16)		//dec butterfly count
			bge butterfly_loop
	
		add t0,i_nSamplesPerGroup,i_nSamplesPerGroup,lsl#1
		add psi_x,psi_x,t0
		sub i_nBfy_nGrp,i_nBfy_nGrp,#1
		movs t1,i_nBfy_nGrp,lsl#16						//test nGrp for zero
		it ne
		subne psi_Twiddles,psi_Twiddles,t0
		bne group_loop
		
	pop {psi_x,i_nBfy_nGrp}
	mov i_nSamplesPerGroup,i_nSamplesPerGroup,lsl#2
	movs i_nBfy_nGrp,i_nBfy_nGrp,lsr#2					//nGrp=(nGrp/4) - for 64-points: 4,1
	bne stage_loop
	
	pop {r4-r12, lr}
	bx lr
	

#ifdef __IAR_SYSTEMS_ASM__
  DATA
#endif
	

twiddle_table_new	
//nPoints=16
	DCW	0x4000, 0x0000, 0x4000, 0x0000, 0x4000, 0x0000
	DCW	0x187e, 0x3b21, 0x3b21, 0x187e, 0x2d41, 0x2d41
	DCW	0xd2bf, 0x2d41, 0x2d41, 0x2d41, 0x0000, 0x4000
	DCW	0xc4df, 0xe782, 0x187e, 0x3b21, 0xd2bf, 0x2d41
//nPoints=64
	DCW	0x4000, 0x0000, 0x4000, 0x0000, 0x4000, 0x0000
	DCW	0x3d3f, 0x1294, 0x3fb1, 0x0646, 0x3ec5, 0x0c7c
	DCW	0x3537, 0x238e, 0x3ec5, 0x0c7c, 0x3b21, 0x187e
	DCW	0x289a, 0x3179, 0x3d3f, 0x1294, 0x3537, 0x238e
	DCW	0x187e, 0x3b21, 0x3b21, 0x187e, 0x2d41, 0x2d41
	DCW	0x0646, 0x3fb1, 0x3871, 0x1e2b, 0x238e, 0x3537
	DCW	0xf384, 0x3ec5, 0x3537, 0x238e, 0x187e, 0x3b21
	DCW	0xe1d5, 0x3871, 0x3179, 0x289a, 0x0c7c, 0x3ec5
	DCW	0xd2bf, 0x2d41, 0x2d41, 0x2d41, 0x0000, 0x4000
	DCW	0xc78f, 0x1e2b, 0x289a, 0x3179, 0xf384, 0x3ec5
	DCW	0xc13b, 0x0c7c, 0x238e, 0x3537, 0xe782, 0x3b21
	DCW	0xc04f, 0xf9ba, 0x1e2b, 0x3871, 0xdc72, 0x3537
	DCW	0xc4df, 0xe782, 0x187e, 0x3b21, 0xd2bf, 0x2d41
	DCW	0xce87, 0xd766, 0x1294, 0x3d3f, 0xcac9, 0x238e
	DCW	0xdc72, 0xcac9, 0x0c7c, 0x3ec5, 0xc4df, 0x187e
	DCW	0xed6c, 0xc2c1, 0x0646, 0x3fb1, 0xc13b, 0x0c7c

	END

