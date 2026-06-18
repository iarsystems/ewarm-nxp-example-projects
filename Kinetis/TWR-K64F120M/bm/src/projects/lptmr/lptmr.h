/****************************************************************************************************/
/* All software, source code, included documentation, and any implied know-how are property of      */
/* Freescale Semiconductor and therefore considered CONFIDENTIAL INFORMATION.                       */
/* This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.                      */
/*                                                                                                  */
/* All Confidential Information remains the property of Freescale Semiconductor and will not be     */
/* copied or reproduced without the express written permission of the Discloser, except for copies  */
/* that are absolutely necessary in order to fulfill the Purpose.                                   */
/*                                                                                                  */
/* Services performed by FREESCALE in this matter are performed AS IS and without any warranty.     */
/* CUSTOMER retains the final decision relative to the total design and functionality of the end    */
/* product.                                                                                         */
/* FREESCALE neither guarantees nor will be held liable by CUSTOMER for the success of this project.*/
/*                                                                                                  */
/* FREESCALE disclaims all warranties, express, implied or statutory including, but not limited to, */
/* implied warranty of merchantability or fitness for a particular purpose on any hardware,         */
/* software ore advise supplied to the project by FREESCALE, and or any product resulting from      */
/* FREESCALE services.                                                                              */
/* In no event shall FREESCALE be liable for incidental or consequential damages arising out of     */
/* this agreement. CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands or */
/* actions by anyone on account of any damage,or injury, whether commercial, contractual, or        */
/* tortuous, rising directly or indirectly as a result of the advise or assistance supplied CUSTOMER*/
/* in connectionwith product, services or goods supplied under this Agreement.                      */
/****************************************************************************************************/

#ifndef __lptmr_H__
#define __lptmr_H__

#include "common.h"

//Pin Select constants for CSR[TPS]
#define HSCMP    0x0
#define LPTMR_ALT1 0x1
#define LPTMR_ALT2 0x2

//CSR[TPP] constants
#define RISING 0
#define FALLING LPTMR_CSR_TPP_MASK

uint16 get_counter_value();
void lptmr_clear_registers();

void lptmr_time_counter();
void lptmr_prescale();

void lptmr_internal_ref_input();
void lptmr_lpo_input();
void lptmr_32khz_input();
void lptmr_external_clk_input();

void lptmr_interrupt();

void lptmr_pulse_counter(char pin_select);

#endif /* __UART_H__ */
