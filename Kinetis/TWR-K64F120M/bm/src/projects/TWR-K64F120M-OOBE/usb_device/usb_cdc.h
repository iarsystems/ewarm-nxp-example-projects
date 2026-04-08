/*
 * All software, source code, included documentation, and any implied know-how
 * are property of Freescale Semiconductor and therefore considered
 * CONFIDENTIAL INFORMATION.
 * This confidential information is disclosed FOR DEMONSTRATION PURPOSES ONLY.
 *
 * All Confidential Information remains the property of Freescale Semiconductor
 * and will not be copied or reproduced without the express written permission
 * of the Discloser, except for copies that are absolutely necessary in order
 * to fulfill the Purpose.
 *
 * Services performed by FREESCALE in this matter are performed AS IS and
 * without any warranty.
 * CUSTOMER retains the final decision relative to the total design and
 * functionality of the end product.
 * FREESCALE neither guarantees nor will be held liable by CUSTOMER for the
 * success of this project.
 *
 * FREESCALE disclaims all warranties, express, implied or statutory
 * including, but not limited to, implied warranty of merchantability or
 * fitness for a particular purpose on any hardware, software ore advise
 * supplied to the project by FREESCALE, and or any product resulting from
 * FREESCALE services.
 * In no event shall FREESCALE be liable for incidental or consequential
 * damages arising out of this agreement. CUSTOMER agrees to hold FREESCALE
 * harmless against any and all claims demands or actions by anyone on account
 * of any damage,or injury, whether commercial, contractual, or tortuous,
 * rising directly or indirectly as a result of the advise or assistance
 * supplied CUSTOMER in connectionwith product, services or goods supplied
 * under this Agreement.
 */

#ifndef __CDC__
#define __CDC__

/* Includes */
#include "arm_cm4.h"
#include "usb.h"
#include "ring_buffer.h"

extern uint8 u8CDCState;

/* Base Address for Customizable settings */
//#define CUSTOM_BASE_ADDRESS		0xC000

#define CDC_INPointer			gu8EP2_IN_ODD_Buffer
#define CDC_OUTPointer			gu8EP3_OUT_ODD_Buffer

#define LoaderCounter			CDC_OUT_Data[CDC_BUFFER_SIZE-1]

/* Defines */
#define CDC_BUFFER_SIZE			128
#define EP_OUT					3
#define EP_IN					2

/* USB_CDC Definitions */
#define WAITING_FOR_ENUMERATION	0x00
#define SET_LINE_CODING			0x20
#define GET_LINE_CODING			0x21
#define SET_CONTROL_LINE_STATE	0x22
#define LOADER_MODE				0xAA
#define GET_INTERFACE			0x0A
#define GET_STATUS				0x00
#define CLEAR_FEATURE			0x01
#define SET_FEATURE				0x03

/* TypeDefs */
typedef struct {
	uint32 DTERate;
	uint8 CharFormat;
	uint8 ParityType;
	uint8 Databits;
} CDC_Line_Coding;

/* Extern variables */
extern uint8 CDC_OUT_Data[];
//extern uint8 u8RecDataFlag;
extern CDC_Line_Coding LineCoding;

/* Prototypes */
void CDC_Init(void);
void CDC_Engine(void);
uint8 CDC_InterfaceReq_Handler(void);
uint32 LWordSwap(uint32);
uint8 CDC_Settings_Update(uint8 *, uint8);

#endif /* __CDC__ */
