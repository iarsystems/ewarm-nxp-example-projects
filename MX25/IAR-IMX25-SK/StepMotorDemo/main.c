/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : February, 2 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR KSK-i.MX25 board. It shows basic use of I/O,
 * timers, interrupts, touch screen ADC and step motor control.
 *
 *  The stepper motor speed and direction is controlled by the trimer.
 *
 *  Make sure that the following jumpers are correctly configured on the
 * IAR KSK-i.MX25 evaluation board:
 * Jumpers:
 *  JP19 - 1-2
 *  JP20 - 1-2
 *
 * $Revision: 16 $
 *
 **************************************************************************/
#include <intrinsics.h>
#include <assert.h>
#include <math.h>
#include "board.h"
#include "arm_comm.h"
#include "drv_asic.h"
#include "arm926ej_cp15_drv.h"
#include "ttbl.h"
#include "drv_i2c1.h"
#include "drv_MC34704.h"
#include "drv_adc.h"

static volatile Boolean SysTick;
static Boolean _Dir;

/*************************************************************************
 * Function Name: PWM4_OUT
 * Parameters: unsigned char state
 *
 * Return: none
 *
 * Description: GSet state of PWM 4 output
 *
 *************************************************************************/
void PWM4_OUT(unsigned char state)
{
	PWM4CR_bit.PRESCALER = 200;
  PWM4CR_bit.REPEAT = 0;
  PWM4CR_bit.CLKSRC = 1;
  if(state == 0)
  {
		PWM4CR_bit.POUTC  = 0;
  }
	else
  {
		PWM4CR_bit.POUTC  = 1;
  }

	PWM4CR_bit.DBGEN  = 1;
	PWM4SAR = 0x00;
  PWM4PR  = 0xFF;
	// start pwm4 output 0
	PWM4CR_bit.EN = 1;
}

/*************************************************************************
 * Function Name: GPT1_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPT1 interrupt handler
 *
 *************************************************************************/
static
void GPT1_Handler(void)
{
  // Clear interrupt flag
  GPT1SR_bit.OF1 = 1;
  SysTick = TRUE;
}

/*************************************************************************
 * Function Name: GPT2_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPT2 interrupt handler
 *
 *************************************************************************/
static
void GPT2_Handler(void)
{
static Int32U step_count;
  // Clear interrupt flag
  GPT2SR_bit.OF1 = 1;

  if(_Dir)
  {
	  step_count++;
    if(step_count>3)
    {
      step_count=0;
    }
  }
  else
  {
    step_count--;
    if(step_count>3)
    {
      step_count=3;
    }
  }
	switch(step_count)
	{
	case 0:
		PWM1_HIGH();		PWM2_LOW();	   PWM3_HIGH();   PWM4_OUT(0);    //STEP3:1010
		break;
	case 1:
		PWM1_LOW();		  PWM2_HIGH();	 PWM3_HIGH();	  PWM4_OUT(0);  	//STEP3:0110
		break;
	case 2:
		PWM1_LOW();		  PWM2_HIGH();	 PWM3_LOW();    PWM4_OUT(1);    //STEP1:0101
		break;
	default:
		PWM1_HIGH();		PWM2_LOW();		 PWM3_LOW();		PWM4_OUT(1);    //STEP4:1001
		break;
	}
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
int main(void)
{
Boolean status;
Int32S vr;
Int32U tick_per_sec = 300;

  // Init MMU
  CP15_Mmu(FALSE);            // Disable MMU
  // Privileged permissions  User permissions AP
  // Read-only               Read-only        0
  CP15_SysProt(FALSE);
  CP15_RomProt(TRUE);
  CP15_InitMmuTtb(TtSB,TtTB); // Build L1 and L2 Translation tables
  CP15_SetTtb(L1Table);       // Set base address of the L1 Translation table
  CP15_SetDomain( (DomainManager << 2*1) | (DomainClient << 0)); // Set domains
  CP15_Mmu(TRUE);             // Enable MMU
  CP15_Cache(TRUE);           // Enable ICache,DCache

  // LEDs Init
  IOMUXC_SW_MUX_CTL_PAD_A14_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A15_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A16_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A17_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A18_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A19_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A20_bit.MUX_MODE = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_A21_bit.MUX_MODE = ALT5_MUX_MODE;
  LED_PORT |= LED_MASK;
  LED_DIR  |= LED_MASK;

	// MOTOR INIT
  IOMUXC_SW_MUX_CTL_PAD_PWM_bit.MUX_MODE        = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_CSPI1_SS0_bit.MUX_MODE  = ALT5_MUX_MODE;
  IOMUXC_SW_MUX_CTL_PAD_FEC_TX_CLK_bit.MUX_MODE = ALT5_MUX_MODE;
	IOMUXC_SW_MUX_CTL_PAD_CONTRAST_bit.MUX_MODE   = ALT4_MUX_MODE;

	GPIO1_GDIR |= 0x04010044;
  GPIO3_GDIR |= 0x00002000;
	
	PWM4CR_bit.PRESCALER = 200;
  PWM4CR_bit.REPEAT = 0;
  PWM4CR_bit.CLKSRC = 1;
  PWM4CR_bit.POUTC  = 0;
  PWM4CR_bit.DBGEN  = 1;
//  PWM4SAR = 0x81;
	PWM4SAR = 0x00;
  PWM4PR  = 0xFF;
	// start pwm4 output 0
	PWM4CR_bit.EN = 1;
		
  // Init AITC
  ASIC_Init();

  // Init I2C
  I2C1_Init();

  // Init GTIM
  SysTick = FALSE;
  // Enable GPT1 clock
  CGCR0_bit.ipg_per_gpt  = 1;
  CGCR1_bit.ipg_clk_gpt1 = 1;

  // Disable timer
  GPT1CR = 0;
  // Disable interrupt
  GPT1IR_bit.OF1IE = 0;
  // Reset Timer1
  GPT1CR_bit.SWR = 1;
  while(GPT1CR_bit.SWR);
  // ipg_clk_highfreq - PERCLK5
  GPT1CR_bit.CLKSRC= 2;
  // reset counters
  GPT1CR_bit.ENMOD = 1;
  // Restart counter
  GPT1CR_bit.FRR = 0;

  // Init the Timer's period
  GPT1PR_bit.PRESCALER = 0;
  GPT1OCR1 = HCLK / ((PCDR1_bit.PERDIV5+1) * GPT1_TICK_PER_SEC);
  // Init GPT1 interrupt
  ASIC_SetupIntr(GPT1_Handler,FALSE,INT_GPT1,1);
  ASIC_EnableIntSource(INT_GPT1);
  // Enable GPT1
  GPT1CR_bit.EN = 1;
  // Enable Oput compare 1 interrupt
  GPT1IR_bit.OF1IE = 1;

  // Enable GPT2 clock
  CGCR1_bit.ipg_clk_gpt2 = 1;
  // Disable timer
  GPT2CR = 0;
  // Disable interrupt
  GPT2IR_bit.OF1IE = 0;
  // Reset Timer2
  GPT2CR_bit.SWR = 1;
  while(GPT2CR_bit.SWR);
  // ipg_clk_highfreq - PERCLK5
  GPT2CR_bit.CLKSRC= 2;
  // reset counters
  GPT2CR_bit.ENMOD = 1;
  // Restart counter
  GPT2CR_bit.FRR = 0;

  // Init the Timer's period
  GPT2PR_bit.PRESCALER = 0;
  GPT2OCR1 = HCLK / ((PCDR1_bit.PERDIV5+1) * tick_per_sec);
  // Init GPT2 interrupt
  ASIC_SetupIntr(GPT2_Handler,FALSE,INT_GPT2,1);
  ASIC_EnableIntSource(INT_GPT2);
  // Enable GPT2
  GPT2CR_bit.EN = 1;
  // Enable Oput compare 1 interrupt
  GPT2IR_bit.OF1IE = 1;

  AdcInit();

  // Enable interrupts
  ASIC_EnableIRQ();
  ASIC_EnableFIQ();
  __enable_interrupt();

  VR_Measure();

  // MC34704 init
  status = MC34704_Init();
  assert(status);

  while(1)
  {
    if(SysTick)
    {
      SysTick = FALSE;
      vr = VR_Measure() - 0x800;

      if((_Dir = vr < 0))
      {
        vr = -vr;
      }

      if(0 == vr)
      {
        tick_per_sec = 1;
      }
      else
      {
        tick_per_sec = vr*600/2048;
      }
      GPT2OCR1 = HCLK / ((PCDR1_bit.PERDIV5+1) * tick_per_sec);
    }
  }
}
