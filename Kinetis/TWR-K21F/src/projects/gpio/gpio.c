/*
 * File:		gpio.c
 * Purpose:		LED and Switch Example
 *
 *                      Configures GPIO for the LED and push buttons on the TWR-K70F120M 
 *                      Blue LED - On
 *                      Green LED - Toggles on/off
 *                      Orange LED - On if SW2 pressed
 *                      Yellow LED - On if SW1 pressed
 *
 *                      Also configures push buttons for falling IRQ's. ISR
 *                        configured in vector table in isr.h
 *
 */


#define GPIO_PIN_MASK            0x1Fu
#define GPIO_PIN(x)              (((1)<<(x & GPIO_PIN_MASK)))

#include "common.h"
#include "lptmr.h"

//Function declarations
void portd_isr(void);
void porte_isr(void);
void init_gpio(void);

/********************************************************************/

void main (void)
{
  printf("TWR-K21D50M GPIO Example!\n");

  /* Turn on all port clocks */
  SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

  /* Enable GPIOC interrupts in NVIC */
  enable_irq(61); //GPIOC Vector is 77. IRQ# is 77-16=61
  
  /* Initialize GPIO on TWR-K70F120M */
  init_gpio();

  while(1)
  {
    //Set PTD4 to 0 (turns on green LED)
    GPIOD_PDOR&=~GPIO_PDOR_PDO(GPIO_PIN(4));

    //Toggle the yellow LED on PTD5
    GPIOD_PTOR|=GPIO_PDOR_PDO(GPIO_PIN(5));

    //Look at status of SW1 on PTC6
    if((GPIOC_PDIR & GPIO_PDIR_PDI(GPIO_PIN(6)))==0) //if pressed...
    {
      GPIOD_PDOR&=~GPIO_PDOR_PDO(GPIO_PIN(6)); //Set PTA28 to 0 (turns on orange LED)
    }
    else //else if SW1 not pressed...
    {
      GPIOD_PDOR|=GPIO_PDOR_PDO(GPIO_PIN(6)); //Set PTA28 to 1 (turns off orange LED)
    }

    //Look at status of SW2 on PTE26
    if((GPIOC_PDIR & GPIO_PDIR_PDI(GPIO_PIN(7)))==0)  //If pressed...
    {
      GPIOD_PDOR&=~GPIO_PIN(7); //Set PTA11 to 0 (turns on blue LED)
    }
    else //else if SW2 not pressed...
    {
      GPIOD_PDOR|=GPIO_PIN(7); //Set PTA11 to 1 ((turns off blue LED)
    }

    time_delay_ms(500);
  }
}

/*
 * Initialize GPIO for Tower switches and LED's
 *
 *   PTC6 - SW3
 *   PTC7 - SW2
 *
 *   PTD4 - Green (D5)
 *   PTD5 - Yellow (D6)
 *   PTD6 - Orange (D8)
 *   PTD7 - Blue (D9)
 *
 */
void init_gpio()
{
  //Set PTC6 and PTC7 (connected to SW1 and SW2) for GPIO functionality, falling IRQ,
  //   and to use internal pull-ups. (pin defaults to input state)
  PORTC_PCR6=PORT_PCR_MUX(1)|PORT_PCR_IRQC(0xA)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;
  PORTC_PCR7=PORT_PCR_MUX(1)|PORT_PCR_IRQC(0xA)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;

  //Set PTD4, PTD5, PTD6, and PTD7 (connected to LED's) for GPIO functionality
  PORTD_PCR4=(0|PORT_PCR_MUX(1));
  PORTD_PCR5=(0|PORT_PCR_MUX(1));
  PORTD_PCR6=(0|PORT_PCR_MUX(1));
  PORTD_PCR7=(0|PORT_PCR_MUX(1));

  //Change PTD4, PTD5, PTD6, and PTD7 to outputs
  GPIOD_PDDR=GPIO_PDDR_PDD(GPIO_PIN(4) | GPIO_PIN(5) | GPIO_PIN(6) | GPIO_PIN(7) );	
}

/********************************************************************/

/*
 * ISR for PORTC interrupts
 */
void portc_isr(void)
{
  if(PORTC_ISFR & GPIO_PIN(6))
  {
    printf("SW3 Pressed\n");
  }
  if(PORTC_ISFR & GPIO_PIN(7))
  {
    printf("SW2 Pressed\n");
  }  
  PORTC_ISFR=0xFFFFFFFF;  //Clear Port A ISR flags
}
