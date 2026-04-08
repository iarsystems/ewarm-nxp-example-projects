/*
 * File:		uart_stream.c
 * Purpose:		Main process
 *
 */

#include "common.h"

#define RED				(18)
#define RED_SHIFT		(1 << 18)
#define GREEN			(19)
#define GREEN_SHIFT		(1 << 19)
#define BLUE			(1)
#define BLUE_SHIFT		(1 << 1)

#define RED_OFF			(GPIOB_PSOR = RED_SHIFT)
#define RED_ON			(GPIOB_PCOR = RED_SHIFT)
#define RED_TOGGLE		(GPIOB_PTOR = RED_SHIFT)

#define GREEN_OFF		(GPIOB_PSOR = GREEN_SHIFT)
#define GREEN_ON		(GPIOB_PCOR = GREEN_SHIFT)
#define GREEN_TOGGLE	(GPIOB_PTOR = GREEN_SHIFT)

#define BLUE_OFF		(GPIOD_PSOR = BLUE_SHIFT)
#define BLUE_ON			(GPIOD_PCOR = BLUE_SHIFT)
#define BLUE_TOGGLE		(GPIOD_PTOR = BLUE_SHIFT)

const unsigned char TestString[] = "\nLorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n\n";

/********************************************************************/
int main (void)
{

	/* Initialize the Red LED (PTB18) */
	{
		/* Turn on clock to PortB module */
		SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

		/* Set the PTB18 pin multiplexer to GPIO mode */
		PORTB_PCR18 = PORT_PCR_MUX(1);

		/* Set the initial output state to high */
		GPIOB_PSOR |= RED_SHIFT;

		/* Set the pins direction to output */
		GPIOB_PDDR |= RED_SHIFT;
	}

	while(1)
	{
        for (int i = 0; i < sizeof(TestString); i++) {
            out_char(TestString[i]);
        }
        RED_TOGGLE;
	}
}
/********************************************************************/