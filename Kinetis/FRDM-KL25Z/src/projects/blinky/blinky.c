/*
 * File:		blinky.c
 * Purpose:		Main process
 *
 */

#include "common.h"

#define SLOW_BLINK      (10000000)
#define FAST_BLINK      (1000000)
#define BLINK_DELAY     FAST_BLINK

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

void delay_time(int);

/********************************************************************/
int main (void)
{
  	printf("\nRunning the blinky project.\n");


	/*
	 * Initialize the Red LED (PTB18)
	 */
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

	/*
	 * Initialize the Green LED (PTB19)
	 */
	{
		/* Turn on clock to PortB module */
		SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

		/* Set the PTB19 pin multiplexer to GPIO mode */
		PORTB_PCR19 = PORT_PCR_MUX(1);

		/* Set the initial output state to high */
		GPIOB_PSOR |= GREEN_SHIFT;

		/* Set the pins direction to output */
		GPIOB_PDDR |= GREEN_SHIFT;
	}

	/*
	 * Initialize the Blue LED (PTD1)
	 */
	{
		/* Turn on clock to PortB module */
		SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;

		/* Set the PTD1 pin multiplexer to GPIO mode */
		PORTD_PCR1 = PORT_PCR_MUX(1);

		/* Set the initial output state to high */
		GPIOD_PSOR = BLUE_SHIFT;

		/* Set the pins direction to output */
		GPIOD_PDDR |= BLUE_SHIFT;
	}

	while(1)
    {
		RED_ON;
		delay_time(BLINK_DELAY);	/* Red */
		RED_OFF;
		GREEN_ON;
		delay_time(BLINK_DELAY);	/* Green */
		GREEN_OFF;
		BLUE_ON;
		delay_time(BLINK_DELAY);	/* Blue */
		RED_ON;
		delay_time(BLINK_DELAY);	/* Blue + Red */
		GREEN_ON;
		BLUE_OFF;
		delay_time(BLINK_DELAY);	/* Red + Green */
		RED_OFF;
		BLUE_ON;
		delay_time(BLINK_DELAY);	/* Green + Blue */
		RED_ON;
		delay_time(BLINK_DELAY);	/* Green + Blue + Red */
		GREEN_OFF;
		BLUE_OFF;
	}
}
/********************************************************************/

void delay_time(int number){
  int cnt;
  for(cnt=0;cnt<number;cnt++);
}
/********************************************************************/

/********************************************************************/
