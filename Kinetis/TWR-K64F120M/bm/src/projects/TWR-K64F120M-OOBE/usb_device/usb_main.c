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

/* Includes */
#include "common.h"
#include "usb_cdc.h"
#include "usb_reg.h"
#include "Settings.h"

/* Extern Variables */
extern uint8 gu8USB_Flags;
extern uint8 gu8EP3_OUT_ODD_Buffer[];
extern tBDT tBDTtable[];
extern uint32_t irq_sw;

void usb_device_demo(void)
{
	extern uint32 __VECTOR_RAM[];
	int benum = 0;

	printf("USB Device Module demo using internal reference 48MHz clock\n"
		   "- Connect to PC, the device wil enumurate as serial console\n"
		   "- Remove J26 and connect micro USB to the Tower\n"
		   "- Set terminal baudrate to 115200, 8-bits and no parity\n"
		   "- Press SW3 to exit\n");

	/* disable MPU */
	MPU_CESR = 0;
	USB_REG_SET_ENABLE;
	USB_REG_SET_STDBY_STOP;
	USB_REG_SET_STDBY_VLPx;

	__VECTOR_RAM[INT_USB0] = (uint32)&USB_ISR;

	/* USB CDC Initialization */
	CDC_Init();

	while (!benum) {

		/* USB CDC service routine */
		CDC_Engine();

		if (irq_sw == 0x01) {
			irq_sw &= ~0x01;		/* clear for next use */
			/* if SW3 (PTA4) pressed..., exit this demo */
			break;
		}

		/* If data transfer arrives */
		if (FLAG_CHK(EP_OUT, gu8USB_Flags)) {
			benum++;

			(void)USB_EP_OUT_SizeCheck(EP_OUT);
			usbEP_Reset(EP_OUT);
			usbSIE_CONTROL(EP_OUT);
			FLAG_CLR(EP_OUT, gu8USB_Flags);

			/* Send it back to the PC */
			EP_IN_Transfer(EP2, CDC_OUTPointer, 1);
			break;
		}
	}

	while (benum) {
		if (irq_sw == 0x01) {
			irq_sw &= ~0x01;		/* clear for next use */
			/* if SW3 (PTA4) pressed..., exit this demo */
			break;
		}

		/* If data transfer arrives */
		if (FLAG_CHK(EP_OUT, gu8USB_Flags)) {
			benum++;

			(void)USB_EP_OUT_SizeCheck(EP_OUT);
			usbEP_Reset(EP_OUT);
			usbSIE_CONTROL(EP_OUT);
			FLAG_CLR(EP_OUT, gu8USB_Flags);

			/* Send it back to the PC */
			EP_IN_Transfer(EP2, CDC_OUTPointer, 1);
			break;
		}
	}

	disable_irq(INT_USB0 - 16);

	printf("Exit USB device demo\n\n");
}
