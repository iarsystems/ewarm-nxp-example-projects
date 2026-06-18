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
 *    1. Date        : January, 8 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR KSK-i.MX25 board. It shows basic use of I/O,
 * timers, interrupts, LCD controllers, LCD touch screen, touch screen ADC
 * and beep generator of CS43L22 codec.
 *
 *  A cursor is shown and moves when the screen is touched. RV1 control
 * transparency level of background and graphic windows.
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
#include "iar_logo.h"
#include "freescale_logo.h"
#include "drv_glcd.h"
#include "drv_touch_scr.h"
#include "drv_CS43L22.h"

extern FontType_t Terminal_6_10_6_V;
extern FontType_t Terminal_9_12_6_V;
extern FontType_t Terminal_18_24_12_V;

static volatile Boolean SysTick;

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
Int32U cursor_x = (C_GLCD_H_SIZE - CURSOR_H_SIZE)/2, cursor_y = (C_GLCD_V_SIZE - CURSOR_V_SIZE)/2;
ToushRes_t XY_Touch;
Boolean touch = FALSE;
Boolean status;
Int32U vr;

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

  // CS43L22 Init
  status = CS43L22_Init();
  assert(status);
  status = CS43L22_Vol(4);
  assert(status);
  status = CS43L22_BeepCfg(0x8,0,0);
  assert(status);

  // GLCD Init
  GLCD_Init (IarLogoPic.pPicStream, NULL);
  GLCD_InitGraphic (FreescaleLogoPic.pPicStream, NULL);

  GLCD_Cursor_Cfg(0,CURSOR_H_SIZE,CURSOR_V_SIZE,
                  0x37 | (0x37 << 6) | (0x37 << 12));

  GLCD_Move_Cursor(cursor_x, cursor_y);

  GLCD_Cursor_Ctlr(CURSOR_BACKGND_INV,TRUE);

  // Init touch screen
  TouchScrInit();

  GLCD_GraphicTransparentCtrl(TRUE,0);
  GLCD_Ctrl (TRUE);

  GLCD_SetFont(&Terminal_18_24_12_V,0xFF0000,0x000cd4ff);
  GLCD_PrintfLayerSelect(GLCD_GRAPHIC_WINDOW_LAYER);
  GLCD_SetWindow(10,10,319,33);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fFreescale Semiconductors");

  GLCD_SetFont(&Terminal_18_24_12_V,0x0000FF,0x000cd4ff);
  GLCD_PrintfLayerSelect(GLCD_BACKGROUND_LAYER);
  GLCD_SetWindow(95,10,255,33);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fIAR Systems");

  GLCD_SetFont(&Terminal_18_24_12_V,0xFF0000,0x000cd4ff);
  GLCD_PrintfLayerSelect(GLCD_GRAPHIC_WINDOW_LAYER);
  GLCD_SetWindow(55,195,268,218);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fTouch screen demo");

  GLCD_SetFont(&Terminal_18_24_12_V,0x0000FF,0x000cd4ff);
  GLCD_PrintfLayerSelect(GLCD_BACKGROUND_LAYER);
  GLCD_SetWindow(55,195,268,218);
  GLCD_TextSetPos(0,0);
  GLCD_print("\fTouch screen demo");

  while(1)
  {
    if(SysTick)
    {
      SysTick = FALSE;

      if(TouchGet(&XY_Touch))
      {
        cursor_x = XY_Touch.X;
        cursor_y = XY_Touch.Y;
        GLCD_Move_Cursor(cursor_x, cursor_y);
        if (FALSE == touch)
        {
          touch = TRUE;
          LED0_ON();
          status = CS43L22_Beep(CS43L22_BEEP_SINGLE);
          assert(status);
        }
      }
      else if(touch)
      {
        touch = FALSE;
        LED0_OFF();
      }
      vr = VR_Measure();
      vr = (255ul * vr) / 4096ul;
      GLCD_GraphicTransparentCtrl(TRUE,vr);
    }
  }
}
