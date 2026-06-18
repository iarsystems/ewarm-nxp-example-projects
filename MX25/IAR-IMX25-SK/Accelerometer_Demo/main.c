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
 *    1. Date        : Fabruary, 4 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR KSK-i.MX25 board. It shows basic use of I/O,
 * timers, interrupts, LCD controllers and 3-Axis Accelerometer.
 *
 *  The example shows accelerations on 3 axis.
 *
 *  Set the jumpers and the switches regarding the IAR KSK-i.MX25 User’s Manual
 *
 *    $Revision: 16 $
 *
 **************************************************************************/
#include <intrinsics.h>
#include <assert.h>
#include "board.h"
#include "arm_comm.h"
#include "drv_asic.h"
#include "arm926ej_cp15_drv.h"
#include "ttbl.h"
#include "drv_i2c1.h"
#include "drv_MC34704.h"
#include "drv_glcd.h"
#include "drv_MMA7660FC.h"

extern FontType_t Terminal_6_10_6_V;
extern FontType_t Terminal_9_12_6_V;
extern FontType_t Terminal_18_24_12_V;

static volatile Boolean SysTick;

typedef struct _XY_t
{
  Int32U X;
  Int32U Y;
} XY_t;

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
 * Function Name: ShowBackgraund
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init Background pictures
 *
 *************************************************************************/
static
void ShowBackgraund (void)
{
  GLCD_PrintfLayerSelect(GLCD_BACKGROUND_LAYER);

  GLCD_SetFont(&Terminal_18_24_12_V,0xFFFFFF,0x00000000);
  GLCD_SetWindow(95,0,255,23);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fIAR Systems");

  GLCD_SetWindow(55,196,268,219);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fAccelerometer demo");

  GLCD_SetWindow(240,240/2+21*3-24/2,311,240/2+21*3+24/2);
  GLCD_SetFont(&Terminal_18_24_12_V,0x000FF00,0x00000000);
  GLCD_TextSetPos(0,0);
  GLCD_print("X");
  GLCD_SetFont(&Terminal_18_24_12_V,0x00000FF,0x00000000);
  GLCD_TextSetPos(2,0);
  GLCD_print("Y");
  GLCD_SetFont(&Terminal_18_24_12_V,0x0FF0000,0x00000000);
  GLCD_TextSetPos(4,0);
  GLCD_print("Z");

  GLCD_SetFont(&Terminal_9_12_6_V,0xFFFFFF,0x00000000);
  GLCD_SetWindow(0,240/2-12/2,29,240/2+12/2);
  GLCD_TextSetPos(3,0);
  GLCD_print("0g");

  GLCD_SetWindow(0,240/2-21*1-12/2,29,240/2-21*1+12/2);
  GLCD_TextSetPos(1,0);
  GLCD_print("0.5g");

  GLCD_SetWindow(0,240/2-21*2-12/2,29,240/2-21*2+12/2);
  GLCD_TextSetPos(1,0);
  GLCD_print("1.0g");

  GLCD_SetWindow(0,240/2-21*3-12/2,29,240/2-21*3+12/2);
  GLCD_TextSetPos(1,0);
  GLCD_print("1.5g");

  GLCD_SetWindow(0,240/2+21*1-12/2,29,240/2+21*1+12/2);
  GLCD_TextSetPos(0,0);
  GLCD_print("-0.5g");

  GLCD_SetWindow(0,240/2+21*2-12/2,29,240/2+21*2+12/2);
  GLCD_TextSetPos(0,0);
  GLCD_print("-1.0g");

  GLCD_SetWindow(0,240/2+21*3-12/2,29,240/2+21*3+12/2);
  GLCD_TextSetPos(0,0);
  GLCD_print("-1.5g");

  GLCD_Line(33,240/2,318,240/2,0xFFFFFF,2);
  GLCD_Line(33,240/2-75,33,240/2+75,0xFFFFFF,2);

  GLCD_Line(34,240/2-21*1,318,240/2-21*1,0x808080,1);
  GLCD_Line(34,240/2+21*1,318,240/2+21*1,0x808080,1);
  GLCD_Line(34,240/2-21*2,318,240/2-21*2,0x808080,1);
  GLCD_Line(34,240/2+21*2,318,240/2+21*2,0x808080,1);
  GLCD_Line(34,240/2-21*3,318,240/2-21*3,0x808080,1);
  GLCD_Line(34,240/2+21*3,318,240/2+21*3,0x808080,1);


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
Boolean first;
Acc_Data_t acc_data;
Int32U count;
XY_t ax1,ax2, ay1, ay2, az1, az2;

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

  // Enable interrupts
  ASIC_EnableIRQ();
  ASIC_EnableFIQ();
  __enable_interrupt();

  // MC34704 init
  status = MC34704_Init();
  assert(status);

  // GLCD Init
  GLCD_Init (NULL, NULL);
  GLCD_InitGraphic(NULL, NULL);
  GLCD_GraphicColorKeying(TRUE,0,0,0);
  GLCD_GraphicTransparentCtrl(TRUE,255);
  GLCD_Ctrl (TRUE);

  ShowBackgraund();

  // Init MMA7660FC
  status = MMA7660FC_Init();
  assert(status);

  GLCD_PrintfLayerSelect(GLCD_GRAPHIC_WINDOW_LAYER);

  first = TRUE;
  count = 35;

  while(1)
  {
    if(SysTick)
    {
      SysTick = FALSE;
      MMA7660FC_GetAccData(&acc_data);
      ax2.X = count;
      ay2.X = count;
      az2.X = count;

      ax2.Y = 240/2-(acc_data.X*2);
      ay2.Y = 240/2-(acc_data.Y*2);
      az2.Y = 240/2-(acc_data.Z*2);

      if(!first)
      {
        // Show
        GLCD_Line(ax1.X,ax1.Y,ax2.X,ax2.Y,0x0000FF,1);
        GLCD_Line(ay1.X,ay1.Y,ay2.X,ay2.Y,0x00FF00,1);
        GLCD_Line(az1.X,az1.Y,az2.X,az2.Y,0xFF0000,1);
      }

      first = FALSE;
      ax1 = ax2;
      ay1 = ay2;
      az1 = az2;

      count += 2;
      if(C_GLCD_H_SIZE-10 <= ++count)
      {
        count = 35;
        first = TRUE;
      }
      GLCD_Line(count,240/2-75,count,240/2+75,0x000000,3);
    }
  }
}
