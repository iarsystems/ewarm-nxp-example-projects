/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : main.c
 *    Description : Main module
 *
 *    History :
 *    1. Date        : 1 May. 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the Freescale iMX27 ADS evaluation board based on
 * MCIMX27. It shows basic use of timer, CSI, PrP, the interrupt controller
 * and the touchscreen.
 *  It show menu picture on the LCD, scan the touchscreen and capture picture
 * form the camera.
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "includes.h"

#define BUTT_ON_UP_X      29
#define BUTT_ON_UP_Y      225
#define BUTT_ON_SIZE_X    80
#define BUTT_ON_SIZE_Y    60

#define BUTT_PAUSE_UP_X   135
#define BUTT_PAUSE_UP_Y   225
#define BUTT_PAUSE_SIZE_X 80
#define BUTT_PAUSE_SIZE_Y 60

extern const Int16U menu [];


/*************************************************************************
 * Function Name: ShowMenu
 * Parameters: none
 *
 * Return: none
 *
 * Description: Show the startup picture
 *
 *************************************************************************/
void ShowMenu (void)
{
  pInt32U pDest, pSrc;
  Int32U  i;
  pSrc = (Int32U*)&menu[8];
  pDest = (Int32U *)LcdBuff;
  for(i = (((Int32U)menu[1]<<16) + menu[0])/sizeof(Int32U); i ; --i)
  {
    *pDest++ = *pSrc++;
  }
}

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

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: int
 *
 * Description: Main subroutine
 *
 *************************************************************************/
int main(void)
{
AITC_Ctrl_t AITC_CtrlInit;
Int32U XY;
Boolean Touched = FALSE, SCI_Enable = FALSE, SCI_Pause = FALSE;

#ifdef MMU_ENA
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
#endif

  // Init AITC
  AITC_CtrlInit.IntrPriorityMask = AITC_NOMASKED_INTR;
  AITC_CtrlInit.VectTbl = TableHighAddr;
  AITC_CtrlInit.PrioritySticky   = FALSE;
  AITC_CtrlInit.NormIntrRisePriority = FALSE;
  AITC_CtrlInit.FastIntrRisePriority = TRUE;
  AITC_Init(&AITC_CtrlInit);

  // Enable interrupts
  AITC_EnableIRQ();
  AITC_EnableFIQ();
  __enable_interrupt();

  // Init Buzzer
  BuzzerInit();
  // Init LCD
  LcdSetup();
  // Init CSI
  Csi_init();
  // Touch screen init
  TouchScrInit();
  // Show Main menu CCD off
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
            SCI_Pause = TRUE;
            Csi_Ctrl(!SCI_Pause);
            ShowMenu();
          }
          else
          {
            SCI_Pause = FALSE;
            Csi_Ctrl(!SCI_Pause);
          }
          SCI_Enable ^= TRUE;
          Beep(0);
        }
        else if (CheckRectangle(BUTT_PAUSE_UP_X,BUTT_PAUSE_UP_Y,
                                BUTT_PAUSE_SIZE_X,BUTT_PAUSE_SIZE_Y,
                                (XY&0xFFFF),((XY>>16)&0xFFFF)) &&
                 SCI_Enable)
        {
          // Button "Pause"
          SCI_Pause ^= TRUE;
          Csi_Ctrl(!SCI_Pause);
          Beep(0);
        }
        else
        {
          Beep(2);
        }
      }
    }
    else
    {
      Touched = FALSE;
    }
  }
}

