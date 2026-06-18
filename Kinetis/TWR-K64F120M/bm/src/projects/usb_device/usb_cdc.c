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

#include "usb_cdc.h"
#include "Settings.h"

/* CDC Global Structures */
CDC_Line_Coding LineCoding;
uint8 u8CDCState = WAITING_FOR_ENUMERATION;
uint8 CDC_OUT_Data[CDC_BUFFER_SIZE];
uint8 enum_msg = TRUE;

/* USB Variables & Flags */
extern uint8 gu8USB_Flags;
extern uint8 gu8USB_State;
extern tUSB_Setup *Setup_Pkt;
extern uint8 gu8EP2_IN_ODD_Buffer[];
extern uint8 gu8EP3_OUT_ODD_Buffer[];

extern tBDT tBDTtable[];
extern uint8 gu8Interface;

void CDC_Init(void)
{
	u8CDCState = 0;

	/* USB Initialization */
	USB_Init();

	/* Enable SOF ouput */
	PORTC_PCR7 |= PORT_PCR_MUX(3);

	/* Line Coding Initialization */
	LineCoding.DTERate = (115200);
	LineCoding.CharFormat = 0;
	LineCoding.ParityType = 0;
	LineCoding.Databits = 0x08;

	/* Initialize Data Buffers */
	Buffer_Init(CDC_OUT_Data, CDC_BUFFER_SIZE);
}

void CDC_Engine(void)
{
	/* control Stage */
	switch (u8CDCState) {
	case WAITING_FOR_ENUMERATION:
		/* Wait for USB Enumeration */
		while (gu8USB_State != uENUMERATED) {
			(void)u8CDCState;
		};

		if (enum_msg) {
			printf("USB Enumerated, check your COM ports\n");
			enum_msg = FALSE;
		}

		u8CDCState = 0;
		break;

	case SET_LINE_CODING:
		if (FLAG_CHK(EP0, gu8USB_Flags)) {
			FLAG_CLR(EP0, gu8USB_Flags);
			(void)EP_OUT_Transfer(EP0, (uint8 *) & LineCoding);
			EP_IN_Transfer(EP0, 0, 0);
			//vfnSCI1Init();
		}
		break;

	case SET_CONTROL_LINE_STATE:
		EP_IN_Transfer(EP0, 0, 0);
		break;

	}

	/* Data stage */
/*
	if(FLAG_CHK(EP_OUT,gu8USB_Flags)) {
		u8RecData=USB_EP_OUT_SizeCheck(EP_OUT);		// Check how many bytes from the PC
		usbEP_Reset(EP_OUT);
		usbSIE_CONTROL(EP_OUT);
		FLAG_CLR(EP_OUT,gu8USB_Flags);
		EP_IN_Transfer(EP2,CDC_OUTPointer,2);
		u8RecData=0;
	}
*/
}

uint8 CDC_InterfaceReq_Handler(void)
{
	uint8 u8State = uSETUP;

	switch (Setup_Pkt->bRequest) {
	case GET_INTERFACE:
		EP_IN_Transfer(EP0, &gu8Interface, 1);
		break;

	case GET_LINE_CODING:
		EP_IN_Transfer(EP0, (uint8 *) & LineCoding, 7);
		break;

	case SET_LINE_CODING:
		u8CDCState = SET_LINE_CODING;
		u8State = uDATA;
		break;

	case SET_CONTROL_LINE_STATE:
		u8CDCState = SET_CONTROL_LINE_STATE;
		u8State = uSETUP;
		break;

	case LOADER_MODE:
		Buffer_Init(CDC_OUT_Data, CDC_BUFFER_SIZE);
		FLAG_SET(LOADER, gu8USB_Flags);
		CDC_OUT_Data[0] = 0xFF;
		break;

	}
	return (u8State);
}

uint32 LWordSwap(uint32 u32DataSwap)
{
	uint32 u32Temp;
	u32Temp = (u32DataSwap & 0xFF000000) >> 24;
	u32Temp += (u32DataSwap & 0xFF0000) >> 8;
	u32Temp += (u32DataSwap & 0xFF00) << 8;
	u32Temp += (u32DataSwap & 0xFF) << 24;
	return (u32Temp);
}
