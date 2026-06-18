/*****************************************************************************
 *   qei.h:  Header file for NXP LPC18xx Family Microprocessors
 *
 *   Copyright(C) 2010, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2010.04.22  ver 1.00    Preliminary version, first Release
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 *****************************************************************************/

#ifndef __QEI_H
#define __QEI_H
  
#define MAXPOS_Int	(1<<15)
#define REV2_Int	(1<<14)
#define REV1_Int	(1<<13)
#define POS2REV_Int	(1<<12)
#define POS1REV_Int	(1<<11)
#define POS0REV_Int	(1<<10)
#define REV0_Int	(1<<9)
#define POS2_Int	(1<<8)
#define POS1_Int	(1<<7)
#define POS0_Int	(1<<6)
#define ENCLK_Int	(1<<5)	
#define ERR_Int		(1<<4)
#define DIR_Int		(1<<3)	
#define VELC_Int	(1<<2)
#define TIM_Int		(1<<1)	
#define INX_Int		(1<<0)	

#define QEI_CNTS	500
#define QEI_MAX_POS (2000-1)					// Counts per turn X4
#define QEI_CLK		120000000					// [Hz] peripheral clockspeed
#define QEI_RPM_RESOLUTION	1

// The original formula for QEI RPM is: 
// QEI_RPM = (QEI_CLK * LPC_QEI->CAP * 60) / (QEI_LOAD * (QEI_CNTS+1) * 4)
// QEI_LOAD can now be calculated with the needed resoluation
// QEI_LOAD = Match value for velocity timer
// A velocity capture of 1 has to correspond to 'QEI_RPM_RESOLUTION' 
#define QEI_RPM (LPC_QEI->CAP*QEI_RPM_RESOLUTION)

// QEI_RPM_RESOLUTION = (QEI_CLK * CAP=1 * 60) / (QEI_LOAD * QEI_CNTS * 4)
// so:
#define QEI_LOAD (QEI_CLK * 15ul) / (QEI_RPM_RESOLUTION * QEI_CNTS)	 

#define QEI_INXGATE(N)	(N<<16)	// Index gating configuration.		
#define QEI_CRESPI	(1<<4)		// Continuously reset position counter on index
#define QEI_INVINX 	(1<<3)		// Invert Index. When set, inverts the sense of the index input. 
#define QEI_CAPMODE	(1<<2)		// Capture Mode. When = 0, only PhA edges are counted (2X). When = 1, 
								// BOTH PhA and PhB edges are counted (4X), increasing resolution but decreasing range.
#define QEI_SIGMODE (1<<1)		// Signal Mode. When = 0, PhA and PhB function as quadrature encoder inputs. When = 1, 
								// PhA functions as the direction signal and PhB functions as the clock signal.
#define QEI_DIRINV 	(1<<0)		// Direction invert. When = 1, complements the DIR bit. 

// clock is the controller clock rate
// ppr is the number of pulses per revolution of the physical encoder
// edges is 2 or 4, based on the capture mode set in the QEICON register (2 for
// CapMode set to 0 and 4 for CapMode set to 1)

void QEI_Init(void);
void QEI_SetDirection(uint8_t dir);
void QEI_IRQHandler(void);

#endif
