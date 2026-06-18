/*****************************************************************************
* $Id: cr_dsplib.h 28 2014-01-27 14:45:13Z danielru $   
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

#ifndef CR_DSPLIB_H_
#define CR_DSPLIB_H_

typedef struct
{
	int i_Coeff_a1;
	int i_yn_1;
	int i_yn_2;
}tS_ResonatorStateCoeff;
void vF_dspl_resonator(int *psi_Output, void *pS_ResonatorStateCoeff, int i_NSamples);

typedef struct
{
	int *pi_Coeff;
	int NTaps;
}tS_blockfir32_Coeff;
void vF_dspl_blockfir32(int *pi_y, int *pi_x, tS_blockfir32_Coeff *pS_Coeff, int i_nsamples);

typedef struct
{
	short int pi_Coeff[5];
	short int pi_State[2];
}tS_biquad32_StateCoeff;
void vF_dspl_biquad32(int *pi_Output, int *pi_Input, tS_biquad32_StateCoeff *pS_StateCoeff, int i_NSamples);

typedef struct
{
	short int Kp;
	short int Ki;
	short int Kd;
	short int IntegratedError;
	short int LastError;
}tS_pid_Coeff;
short int vF_dspl_pid(short int si_Error, tS_pid_Coeff *pS_Coeff);

int iF_dspl_dotproduct32(int *pi_x, int *pi_y, int i_VectorLen);
void vF_dspl_biquad32c(int *pi_y, int *pi_x, int *pi_Coeff, int *pi_State, int i_nsamples);
int iF_RandomNumber(int i_Seed);

int iF_dspl_vectsumofsquares32(int *pi_x, int i_VectorLen);
int iF_dspl_vectsumofsquares16(short int *psi_x, int i_VectorLen);

void vF_dspl_vectmulconst32(int *pi_y, int *pi_x, int i_c, int i_VectorLen);
void vF_dspl_vectmulconst16(short int *psi_y, short int *psi_x, short int si_c, int i_VectorLen);

void vF_dspl_vectmulelement32(int *pi_z, int *pi_x, int *pi_y, int i_VectorLen);
void vF_dspl_vectmulelement16(int *psi_z, int *psi_x, int *psi_y, int i_VectorLen);

void vF_dspl_vectaddconst32(int *pi_y, int *pi_x, int i_c, int i_VectorLen);
void vF_dspl_vectaddconst16(int *psi_y, int *psi_x, int si_c, int i_VectorLen);

void vF_dspl_vectsub32(int *pi_z, int *pi_x, int *pi_y, int i_VectorLen);
void vF_dspl_vectsub16(int *psi_z, int *psi_x, int *psi_y, int i_VectorLen);

void vF_dspl_vectadd32(int *pi_z, int *pi_x, int *pi_y, int i_VectorLen);
void vF_dspl_vectadd16(int *psi_z, int *psi_x, int *psi_y, int i_VectorLen);

void vF_dspl_fftR4b16N64(short int *psi_Y, short int *psi_x);
void vF_dspl_fftR4b16N256(short int *psi_Y, short int *psi_x);
void vF_dspl_fftR4b16N1024(short int *psi_Y, short int *psi_x);

#endif /*CR_DSPLIB_H_*/
