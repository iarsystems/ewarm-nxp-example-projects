/*
 * File:        i2c_demo.c
 * Purpose:     I2C example of talking to MMA7660 on the Tower boards.
 *
 *              This demo uses a single byte read, but there is also a
 *                u8MMA7660ReadThreeRegisters() function available to show
 *                an example of doing multiple reads back to back.
 *
 *              TWR-K40X256 uses I2C1
 *              TWR-K60N512 uses I2C0
 *
 */

#include "common.h"

//Determine which chip is being used to determine if need to configure I2C0 or I2C1
#ifdef CPU_MK60N512VMD100
  #include "k60_i2c.h"
#else
  #include "k40_i2c.h"
#endif

//Function declarations
void delay(void);
signed char convert (signed char input);
void time_delay_ms(unsigned int count_val);
/********************************************************************/
/*

 */
void main (void)
{
  signed char resultx, resulty, resultz;

  printf("Kinetis I2C Demo\n");

  //Initialize I2C
  init_I2C();

  //Configure MMA7660 sensor
  MMA7660WriteRegister(0x09,0xE0); //Disable tap detection
  MMA7660WriteRegister(0x07,0x19); //Enable auto-sleep, auto-wake, and put in active mode

  printf("  X     Y     Z\n");

  while(1)
  {
    //Read x-axis register
    resultx = u8MMA7660ReadRegister(0x00);
    printf("%3d", convert(resultx));

    //Read y-axis register
    resulty = u8MMA7660ReadRegister(0x01);
    printf("   %3d", convert(resulty));

    //Read z-axis register
    resultz = u8MMA7660ReadRegister(0x02);
    printf("   %3d\n", convert(resultz));

    //Delay for 250ms
    time_delay_ms(250);
  }

}

/*******************************************************************/
/*
 * Convert 6-bit accelerometer result into an 8-bit signed char
 */
signed char convert(signed char input)
{
      input &=~0x40; //Turn off alert bit
      if(input & 0x20)    // update 6-bit signed format to 8-bit signed format
	input |= 0xE0;
    return (signed char) input;
}

/*
 * Delay function using the LPTMR module
 */
void time_delay_ms(unsigned int count_val)
{
  SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK; //Turn on clock to LPTMR module
  LPTMR0_CMR = count_val; //Set compare value
  LPTMR0_PSR = LPTMR_PSR_PCS(1)|LPTMR_PSR_PBYP_MASK; //Use 1Khz LPO clock and bypass prescaler
  LPTMR0_CSR |= LPTMR_CSR_TEN_MASK; //Start counting

  while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK)) {} //Wait for counter to reach compare value

  LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK; //Clear Timer Compare Flag
  return;
}
