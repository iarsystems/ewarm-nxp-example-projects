/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : July 25, 2011
 *       Author      : Atanas Uzunov
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the MC13260-EVK board. It shows basic use
 * of I/O, system initialization (MMU, ASIC), timer and PWM FIFO.
 *
 *  Jumpers:
 *    J74: 1-2   (Enable high intensity LEDs)
 *    J75: 2-3
 *    J65: 3-5, 4-6
 *  
 *  Switches:
 *    SW2: 01000 (Enable SPI flash)
 *    SW3: 00100 (Enable Red and Green LEDs)
 *    SW5: 11111 (Boot from SPI flash, use internal bootloader)
 *
 *    $Revision: 16 $
 *
 **************************************************************************/
#include <intrinsics.h>
#include <math.h>
#include "board.h"
#include "arm_comm.h"
#include "drv_asic.h"
#include "arm926ej_cp15_drv.h"
#include "ttbl.h"

#define PWM_EXP_LOW    2
#define PWM_EXP_HIGH  11

static volatile Int32U flags0=0;

#define flPwmDir             (flags0 &   ( 1 << 0 ))
#define flPwmDirSet()        (flags0 |=  ( 1 << 0 ))
#define flPwmDirClear()      (flags0 &= ~( 1 << 0 ))

#define flPwmFIFOEmpty          (flags0 &   ( 1 << 1 ))
#define flPwmFIFOEmptySet()     (flags0 |=  ( 1 << 1 ))
#define flPwmFIFOEmptyClear()   (flags0 &= ~( 1 << 1 ))

#define flUpdateLeds         (flags0 &   ( 1 << 2 ))
#define flUpdateLedsSet()    (flags0 |=  ( 1 << 2 ))
#define flUpdateLedsClear()  (flags0 &= ~( 1 << 2 ))

static volatile float pwmExpVal=PWM_EXP_LOW;

/*************************************************************************
 * Function Name: GPT_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPT interrupt handler
 *
 *************************************************************************/
static void GPT_Handler(void)
{
  // Clear interrupt flag
  GPTSR_bit.OF1 = 1;
  flUpdateLedsSet();
}

/*************************************************************************
 * Function Name: PWM1_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: PWM1 (Roll-over) interrupt handler
 *
 *************************************************************************/
static void PWM1_Handler(void)
{
  // Clear interrupt flag   
  if (PWM1_PWMSR_bit.FE)
  {
    PWM1_PWMSR_bit.FE = 1;
    flPwmFIFOEmptySet();  
  }
}

/*************************************************************************
 * Function Name: getNewPwmValue
 * Parameters: none
 *
 * Return: Int16U
 *
 * Description: Caluculates new 16 bit, unsigned value, ready to be written to PWM 
 *              SAR register
 *
 *************************************************************************/
static Int16U getNewPwmValue(void)
{
    // Update brightness value
    if (!flPwmDir)
      pwmExpVal += 0.1;
    else 
      pwmExpVal -= 0.1;
      
    // Change brightness direction if needed
    if (pwmExpVal > PWM_EXP_HIGH) flPwmDirSet();
    else if (pwmExpVal < PWM_EXP_LOW) flPwmDirClear();
  
    // Return new PWM Value
    return (Int16U)exp(pwmExpVal);
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
  /* Init MMU */
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
    
  /* LEDs Init */
  
  LED_PORT |= LED_MASK;
  LED_DIR  |= LED_MASK;
      
  LED0_ON();
  LED1_OFF();
  
  /* Init ASIC */
  
  ASIC_Init();

  /* Init GPTIMER */
    
  // Disable timer 
  GPTCR = 0;
  // Disable interrupt 
  GPTIR_bit.OF1IE = 0;
  // Select clock source - ipg_clk 
  GPTCR_bit.CLKSRC= 1;
  // Reset status register 
  GPTSR = 0xFFFF;  
  // Enable interrupt register (Output Compare 1) 
  GPTIR_bit.OF1IE = 1;
  // Reset counters 
  GPTCR_bit.ENMOD = 1;
  // Restart counter 
  GPTCR_bit.FRR = 0;
  // Init the Timer's period 
  GPTPR_bit.PRESCALER = 0;
  GPTOCR1 = IPCLK / GPT_TICK_PER_SEC;
  // Init GPT interrupt 
  ASIC_SetupInterrupt(GPT_Handler,FALSE,INT_GPT,1);
  ASIC_EnableIntSource(INT_GPT);
  // Enable GPT 
  GPTCR_bit.EN = 1;

  /* Init PWM */ 
  
  // Connect pad 26 to PWM1 
  IOMUX_SW_MUX_CTL_26_bit.mux_out = 4;    
  // Software reset PWM1
  PWM1_PWMCR_bit.SWR = 1;   
  // Disable PWM1
  PWM1_PWMCR_bit.EN = 0;  
  // Select clock source - ipg_clk 
  PWM1_PWMCR_bit.CLKSRC = 1;
  // Set prescaler value - divide by 6 
  PWM1_PWMCR_bit.PRESCALER = 5;
  // FIFO-Empty interrupt enable   
  PWM1_PWMIR_bit.FIE = 1;
  // Clear Sample register 
  PWM1_PWMSAR = 0;  
  // Enable PWM in debug mode 
  PWM1_PWMCR_bit.DBGEN = 1;  
  // Set up PWM1 Interrupt
  ASIC_SetupInterrupt(PWM1_Handler,FALSE,INT_PWM1,2);
  ASIC_EnableIntSource(INT_PWM1);
  // Enable PWM1
  PWM1_PWMCR_bit.EN = 1;
  
  /* Enable Interrupts */
  
  ASIC_EnableIRQ();
  ASIC_EnableFIQ();  
  __enable_interrupt();

  while(1)
  {
    if(flUpdateLeds)
    {      
      // Clear flag
      flUpdateLedsClear();      
      // Toggle LEDs
      LED0_TOGGLE();
      LED1_TOGGLE();     
    }
    
    // When FIFO is empty
    if (flPwmFIFOEmpty)
    {
      // Clear flag
      flPwmFIFOEmptyClear();            
      // Fill FIFO with new brightness values
      while (PWM1_PWMSR_bit.FIFOAV < 4){      
        PWM1_PWMSAR = getNewPwmValue();            
      }
    }
    
  }
}
