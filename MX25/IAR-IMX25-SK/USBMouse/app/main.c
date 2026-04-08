/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : November 10, 2009
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR KSK-i.MX25 board. It implements USB HID mouse.
 *  The mouse cursor moves depending of slope of the board.
 *
 *  Set the jumpers and the switches regarding the IAR KSK-i.MX25 User’s Manual
 *
 *    $Revision: 16 $
 *
 **************************************************************************/
#include <intrinsics.h>
#include "includes.h"

static volatile Boolean _SysTick;

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
  _SysTick = TRUE;
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
Int32U count;
Acc_Data_t acc_data;

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

  // Init AITC
  ASIC_Init();

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

  // Init GTIM
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

  // I2C1 Init
  I2C1_Init();

  // Initialize USB
  // HID USB
  HidInit();

  // Enable interrupts
  ASIC_EnableIRQ();
  ASIC_EnableFIQ();
  __enable_interrupt();

  // MC34704 init
  MC34704_Init();
  // Init MMA7660FC
  MMA7660FC_Init();
  // Soft connection enable
  USB_ConnectRes(TRUE);

  count     = 0;

  while(1)
  {
    if(_SysTick)
    {
      _SysTick = FALSE;
      if(++count > 2)
      {
        count = 0;
        LED_PORT ^= LED0;
      }

      if(UsbCoreReq(UsbCoreReqDevState) == UsbDevStatusConfigured)
      {
        if(!UsbCoreReq(UsbCoreReqDevSusState))
        {
          MMA7660FC_GetAccData(&acc_data);
          if(    (-4 < acc_data.Y)
              && ( 4 > acc_data.Y)
            )
          {
            acc_data.Y = 0;
          }
          if(    (-4 < acc_data.X)
              && ( 4 > acc_data.X)
            )
          {
            acc_data.X = 0;
          }
          HidMouseSendReport(-acc_data.Y,-acc_data.X,0);
          LED_PORT &= ~LED0;
        }
      }
    }
  }
}
