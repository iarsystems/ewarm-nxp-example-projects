/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : 06.10.2008
 *       Author      : Stoyan Choynev
 *       Description : initial vestion
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Freescale iMX31 PDK board. It shows basic use
 * of I/O, system initialization (PLL, EMI, MMU, AITC) and timers.
 *
 *
 *    $Revision: 5880 $
 **************************************************************************/
/** include files **/
#include <NXP/iomcimx31.h>
#include <intrinsics.h>
#include "board.h"
#include "arm_comm.h"
#include "drv_aitc.h"
#include "arm1136jf_cp15_drv.h"
#include "ttbl.h"
#include "drv_atlas.h"
#include "drv_lcd.h"
#include "drv_touch_scr.h"
#include "drv_i2c1.h"
#include "drv_csi.h"
#include "drv_ipu.h"

/** local definitions **/
#define BUTT_ON_UP_X      48
#define BUTT_ON_UP_Y      450
#define BUTT_ON_SIZE_X    160
#define BUTT_ON_SIZE_Y    120

#define BUTT_PAUSE_UP_X   270
#define BUTT_PAUSE_UP_Y   450
#define BUTT_PAUSE_SIZE_X 160
#define BUTT_PAUSE_SIZE_Y 120

/** default settings **/

/** external functions **/
/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** public functions **/
void Delay(Int32U time)
{
  time *= 60;
  while(time--);
}

/** private functions **/
/*************************************************************************
 * Function Name: CheckRectangle
 * Parameters: Int32U X_up, Int32U Y_up - rectangle coordinate
 *             Int32U X_size, Int32U Y_size - rectangle size
 *             Int32U X_poin, Int32U Y_point - point coordinate
 *
 * Return: Boolean
 *    TRUE  - the point is inside  from the rectangle
 *    FALSE - the point is outside from the rectangle
 *
 * Description: Check whether the coordinate of point is inside from a rectangle
 *
 *************************************************************************/
Boolean CheckRectangle (Int32U X_up, Int32U Y_up,
                        Int32U X_size, Int32U Y_size,
                        Int32U X_point, Int32U Y_point )
{
  if((X_up <= X_point) && (X_point <= X_up+X_size) &&
     (Y_up <= Y_point) && (Y_point <= Y_up+Y_size))
  {
    return (TRUE);
  }
  return (FALSE);
}
//extern void IPU_Init(void);

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
AITC_Ctrl_t AITC_CtrlInit;
Int32U XY;
Boolean Touched = FALSE, SCI_Enable = FALSE, SCI_Pause = FALSE;
  
  __disable_interrupt();
  // Init MMU
  // Privileged permissions  User permissions AP
  // Read-only               Read-only        0
  CP15_Mmu(FALSE);            // Disable MMU
  CP15_ICache(TRUE);          //Enable ICache
  CP15_SysProt(FALSE);
  CP15_RomProt(FALSE);
  CP15_InitMmuTtb(TtSB,TtTB); // Build L1 and L2 Translation tables
  CP15_SetTtb(L1Table);       // Set base address of the L1 Translation table
  CP15_SetDomain( (DomainManager << 2*1) | (DomainClient << 0)); // Set domains
  CP15_Mmu(TRUE);             // Enable MMU
  CP15_Cache(TRUE);           // Enable ICache,DCache

  //Init CPLD
  WEIM_CSCR5U = 0x0000DCF6;
  WEIM_CSCR5L = 0x444A4541;
  WEIM_CSCR5A = 0x44443302;
  // LEDs Init
  LED_OFF((LED1|LED2|LED3|LED4|LED5|LED6|LED7|LED8));

  // Init AITC
  AITC_CtrlInit.IntrPriorityMask = AITC_NOMASKED_INTR;
  AITC_CtrlInit.PrioritySticky   = FALSE;
  AITC_CtrlInit.NormIntrRisePriority = FALSE;
  AITC_CtrlInit.FastIntrRisePriority = TRUE;
  AITC_Init(&AITC_CtrlInit);
  /*Configure  CSPI2*/
  Init_Atlas_SPI();
  /*Init Tuch Scr*/
  TouchScrInit();
  /*Init CSI pins*/
  Csi_init();
  /*CSI should work in order to init camera*/
  IPU_Init();
  /*camera i2c interface init*/
  I2C1_Init();
  /*OV2640 Camera init*/
  OV2640_Init();
  /*Turn Off Camera interface*/
  StopImgCapture();
  /*Init LCD*/
  LcdSetup();
  // Enable interrupts
  AITC_EnableIRQ();
  AITC_EnableFIQ();
  __enable_interrupt();
  /*Show Menu Image*/
  ShowMenu();
  
  while(1)
  {
    // Image capture update
    // Get Touchscreen current data     
    if(TouchGet((ToushRes_t *)&XY))
    {
      if(!Touched)
      {
        Touched = TRUE;
        if(CheckRectangle(BUTT_ON_UP_X,BUTT_ON_UP_Y,
                          BUTT_ON_SIZE_X,BUTT_ON_SIZE_Y,
                         (XY&0xFFFF),((XY>>16)&0xFFFF)))
        {
          // Button "On"
          if(SCI_Enable)
          {
            SCI_Pause = FALSE;
            StopImgCapture();
            HidePicture();
          }
          else
          {
            SCI_Pause = TRUE;
            ShowPicture();
            StartImgCapture();
          }
          SCI_Enable ^= TRUE;
        }
        else if (CheckRectangle(BUTT_PAUSE_UP_X,BUTT_PAUSE_UP_Y,
                                BUTT_PAUSE_SIZE_X,BUTT_PAUSE_SIZE_Y,
                                (XY&0xFFFF),((XY>>16)&0xFFFF)) &&
                 SCI_Enable)
        {
          // Button "Pause"
          if(SCI_Pause)
          {
            StopImgCapture();
          }
          else
          {
            StartImgCapture();
          }
          SCI_Pause ^= TRUE;
        }
      }
    }
    else
    {
      Touched = FALSE;
    }
    
  }
}

