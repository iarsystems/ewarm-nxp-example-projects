/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2005
 *
 *    File name   : main.c
 *    Description : main module
 *
 *    History :
 *    1. Date        : June, 04 2005
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 * Jumpers:
 *  PWR_J   - depend of power source
 *  PWM/DAC - 2-3 (PWM)
 *  DBG     - present
 *
 *  LCD Backlight - indicate disks activity
 *
 *    $Revision: 28 $
 **************************************************************************/
#include "includes.h"

// Sys timer tick per seconds
#define UPDATE_SHOW_DLY       ((Int32U)(0.5*TICK_PER_SECOND))
#define STARTUP_SHOW_DLY      ((Int32U)(2 *TICK_PER_SECOND))
#define LCD_BACKLIGHT_ON()    (IO0SET_bit.P0_21 = 1)
#define LCD_BACKLIGHT_OFF()   (IO0CLR_bit.P0_21 = 1)
#define LCD_BACKLIGHT_INIT()  (PINSEL1_bit.P0_21 = 0, IO0DIR_bit.P0_21 = 1, IO0CLR_bit.P0_21 = 1)

#pragma location="DMA_RAM"
#pragma data_alignment=4
__no_init Int8U Lun0Buffer[512];

const Int8U HexToCharStr [] = "0123456789ABCDEF";
volatile Int32U Update;

/*************************************************************************
 * Function Name: fiq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description:
 *
 *************************************************************************/
__fiq __arm void fiq_handler (void)
{
   while(1);
}

/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ handler
 *
 *************************************************************************/
__irq __arm void irq_handler (void)
{
void (*interrupt_function)();
unsigned int vector;

  vector = VICVectAddr;     // Get interrupt vector.
  interrupt_function = (void(*)())vector;
  if(interrupt_function != NULL)
  {
    interrupt_function();  // Call vectored interrupt function.
  }
  else
  {
    VICVectAddr = 0;      // Clear interrupt in VIC.
  }
}

/*************************************************************************
 * Function Name: SetSysTickFlag
 * Parameters: none
 *
 * Return: none
 *
 * Description: Set arg
 *		
 *************************************************************************/
void SetSysTickFlag (void* arg)
{
Int32U* pFlag = arg;
  *pFlag = 1;
}
/*************************************************************************
 * Function Name: DelayResolution100us
 * Parameters: Int32U Delay
 * Return: void
 *
 * Description: Timer1 CH0 subroutine - delay [100us]
 *		
 *************************************************************************/
void DelayResolution100us(Int32U Delay)
{
volatile Int32U Flag = 0;
  // Stop Timer 1
  TIMER_Stop(TIMER1);
  // Reset Timer 1 counter
  TIMER_Reset(TIMER1);
  // Set action - match module CH0
  TIMER_SetMatchAction(TIMER1, CH0, TimerAction_Interrupt | TimerAction_StopTimer,
  Delay usec_T1*100, SetSysTickFlag, (void *)&Flag, DONOTHING);
  // Start Timer 1
  TIMER_Start(TIMER1);
  // Wait expire of delay
  while(Flag == 0);
}

/*************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *
 *************************************************************************/

int main(void)
 {
Int32U Dly = UPDATE_SHOW_DLY;
DiskStatusCode_t StatusHold = (DiskStatusCode_t) -1;
Int8U Message[17];
Int32U Tmp, Size;

  // Initialize the system
  if (SYS_Init(FOSC, FCCLK, VPBDIV1, USER_FLASH, 0xB27E7FFF,0x80FFFFFF,0xFFFFFFFF,0))
    return 1;

  VIC_Init();
  VIC_SetProtectionMode(UserandPrivilegedMode);

  // Init System Timer - Timer0
  PM_OpenPeripheral(PC_TIMER0);
  TIMER_Init(TIMER0, TIMER_PRECISION);
  TIMER_SetMatchAction(TIMER0, CH0, TimerAction_Interrupt | TimerAction_ResetTimer,
  1sec_T0/TICK_PER_SECOND, SetSysTickFlag, (void *)&Update, DONOTHING);
  // System Timer interrupt registered
  VIC_SetVectoredIRQ(TIMER0_ISR,VIC_Slot2,VIC_TIMER0);
  VIC_EnableInt(1<<VIC_TIMER0);

  // Init Dly 100 ms - Timer1
  PM_OpenPeripheral(PC_TIMER1);
  TIMER_Init(TIMER1, TIMER_PRECISION);
  //  Dly 100 ms interrupt registered
  VIC_SetVectoredIRQ(TIMER1_ISR,VIC_Slot3,VIC_TIMER1);
  VIC_EnableInt(1<<VIC_TIMER1);

  // LUNs Init
  LunInit(RAM_DISK_LUN,RamDiskInit,RamDiskStatus,RamDiskIO);
  LunInit(SD_DISK_LUN,SdDiskInit,SdGetDiskCtrlBkl,SdDiskIO);

  // Init SCSI module
  ScsiInit();

  // Enable interrupts
  __enable_interrupt();

  // Init SD card driver
  // SCSI init push init SD driver message for an each LUN
  for(Int32U i = 0; i < SCSI_LUN_NUMB; i++)
  {
    // Implement LUNs messages
    if(LunImp(i))
    {
      LCD_BACKLIGHT_ON();
    }
    else
    {
      LCD_BACKLIGHT_OFF();
    }
  }

  // Soft connection enable
  USB_ConnectRes(TRUE);

  // Start System timer
  TIMER_Start(TIMER0);

  LCD_BACKLIGHT_INIT();
  // LCD Powerup init
  HD44780_PowerUpInit();
  // Show meassges on LCD
  HD44780_StrShow(1, 1,  "IAR Systems ARM ");
  HD44780_StrShow(1, 2,  "USB Mass Storage");

  // Set the startup message time out
  Dly = STARTUP_SHOW_DLY;
  while(1)
  {
    for(Int32U i = 0; i < SCSI_LUN_NUMB; i++)
    {
      // Implement LUNs messages
      if(LunImp(i))
      {
        LCD_BACKLIGHT_ON();
      }
      else
      {
        LCD_BACKLIGHT_OFF();
      }
    }

    if (Update)
    {
      Update = FALSE;
      // Update MMC/SD card status
      SdStatusUpdate();
      if(Dly-- == 0)
      {
        // LCD show
        Dly = UPDATE_SHOW_DLY;
        // Current state of MMC/SD show
        pDiskCtrlBlk_t pMMCDiskCtrlBlk = SdGetDiskCtrlBkl();
        if(StatusHold != pMMCDiskCtrlBlk->DiskStatus)
        {

          StatusHold = pMMCDiskCtrlBlk->DiskStatus;
          switch (pMMCDiskCtrlBlk->DiskStatus)
          {
          case DiskCommandPass:
            // Calculate MMC/SD size [MB]
            Size = (pMMCDiskCtrlBlk->BlockNumb * pMMCDiskCtrlBlk->BlockSize);
            Tmp  = Size/1000000;
            Tmp += ((Size%1000000)<1000000/2)?0:1;

            switch(pMMCDiskCtrlBlk->DiskType)
            {
            case DiskMMC:
              sprintf((char*)Message,"MMC Card - %dMB",Tmp);
              break;
            case DiskSD_Spec1_x:
            case DiskSD_Spec2_0:
              sprintf((char*)Message,"SD Card - %dMB",Tmp);
              break;
            default:
              sprintf((char*)Message,"Card - %dMB",Tmp);
            }
            for(Int32U i = strlen((char*)Message); i < 16; ++i)
            {
              Message[i] = ' ';
            }
            Message[strlen((char*)Message)+1] = 0;
            break;
          default:
            strcpy((char*)Message,"Pls, Insert Card");
          }
          HD44780_StrShow(1, 2, (pInt8S)Message);
        }
      }
    }
  }
}
