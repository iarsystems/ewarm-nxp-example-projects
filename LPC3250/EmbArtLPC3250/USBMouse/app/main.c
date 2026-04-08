/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : main.c
 *
 *    Description:
 *    This example project shows how to use the IAR Embedded Workbench for ARM
 *    to develop code for a Embedded Artists LPC3250 evaluation board.
 *    It implements a USB HID mouse.
 *    The joystick controls up, down, left, right and left mouse movements.
 *    The buttons SW1 and SW2 are used as the left and right mouse buttons.
 *
 *    $Revision: 28 $
 **************************************************************************/

/** include files **/
#include "includes.h"

#define OSC           (13000000UL)  // System OSC 13MHz

#define CORE_CLK      (OSC*16)      // ARM_CLK 208MHz
#define AHB_CLK       (CORE_CLK/2)  // HCLK 104MHz
#define PER_CLK       (CORE_CLK/16) // PER_CLK 13MHz
#define RTC_CLK       (32768UL)     // RTC_CLK

#define LED1          (1UL << 1)

#define LED_ON()    (P3_OUTP_SET = LED1)
#define LED_OFF()   (P3_OUTP_CLR = LED1)

#define   TIMER1_PERIOD        5000   //in us

#define   MOUSE_DELTA          1

/** external data **/
volatile Boolean Tick = FALSE;

/*************************************************************************
 * Function Name: HSTimerHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: High speed timer handler
 *
 *************************************************************************/
static void HSTimerHandler (void)
{
  static Int32U blink_led_div = 0;
  
  HSTIM_INT_bit.MATCH0_INT   = 1;   // Clear MATCH0 interrupt flag
  
  if(100 < ++blink_led_div)
  {
    blink_led_div = 0;
    if(P3_OUTP_STATE & LED1)
      LED_OFF(); // LED OFF
    else
      LED_ON(); // LED ON
  }
  
  Tick = TRUE;
}

/*************************************************************************
 * Function Name: main
 * Parameters: None
 *
 * Return: None
 *
 * Description: Main loop
 *
 *************************************************************************/
void main(void)
{
  Buttons_t btn_state;
  Int8S X = 0, Y = 0;
  Int8U Buttons = 0, ButtonsHold = 0;  

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
  
  // Init Interrupt Controller.
  INTC_Init();

  // Init USB device Class
  HidInit();

  // Init buttons connected to the PCA9532 I2C chip
  ButtonsInit();
  
  // Init High speed timer
  TIMCLK_CTRL_bit.HST_CLK_ENA= 1;   // HSTimer clock enable
  HSTIM_CTRL_bit.COUNT_ENAB  = 0;   // Stop counting
  HSTIM_MCTRL_bit.MR0_INT    = 1;   // unmask MATCH0 intr flag
  HSTIM_MCTRL_bit.RESET_COUNT0=1;   // Enable reset of Timer Counter on Match 0
  HSTIM_MCTRL_bit.STOP_COUNT0 =0;   // Enable stop functionality on Match 0
  HSTIM_PMATCH = 13-1;              // Set prescaler to 1us
  HSTIM_CTRL_bit.RESET_COUNT = 1;   // Reset the counter
  while(HSTIM_COUNTER);
  HSTIM_CTRL_bit.RESET_COUNT = 0;   // release reset of the counter
  HSTIM_CTRL_bit.PAUSE_EN = 1;      // stop the counter in debug mode
  HSTIM_MATCH0 = TIMER1_PERIOD;     // set timer period
  HSTIM_INT_bit.MATCH0_INT   = 1;   // Clear MATHC0 interrupt flag
  
  // Init timer interrupts
  MIC_IRQInstall(HSTimerHandler,    //ISR
                 MainHSTIMER_INT,   //Interrupt index
                 HIGH,              // Interrupt is generated on a high level signal or rising edge
                 LEVEL,             // Interrupt is level sensitive
                 TYPE_IRQ);         // The interrupt is routed to the IRQ output of the interrupt controller
  MIC_IntEnable(MainHSTIMER_INT, ENABLE_IRQ);
 
  HSTIM_CTRL_bit.COUNT_ENAB  = 1;   // Enable counting
  
  // Enable interrupts
  __enable_irq();

  // Soft connection enable
  USB_ConnectRes(TRUE);

  while(1)
  {
    if (Tick)
    {
      Tick = FALSE;
      
      // Read buttons
      btn_state = GetButtons();
      
      // Mouse "moved"...
      if(btn_state.JsUp) // ...up?
      {
        Y -= MOUSE_DELTA;
      }
      else if(btn_state.JsDown) // ...down?
      {
        Y += MOUSE_DELTA;
      }

      // Mouse "moved"...
      if(btn_state.JsRight) // ...right?
      {
        X += MOUSE_DELTA;
      }
      else if(btn_state.JsLeft) // ...left?
      {
        X -= MOUSE_DELTA;
      }

      // Left mouse button pressed?
      if(btn_state.JsCenter || btn_state.B1)
      {
        Buttons |= 1;
      }
      else
      {
        Buttons &= ~1;
      }

      // Right mouse button pressed?
      if(btn_state.B2)
      {
        Buttons |= 2;
      }
      else
      {
        Buttons &=~2;
      }

      // Send mouse status if the mouse has moved, a button is being pressed
      // down or a button has just been released
      if(Y || X  || Buttons || (ButtonsHold ^ Buttons))
      {
        UsbWakeUp();  // wake-up
        
        // Send report
        if(   UsbDevStatusConfigured == UsbCoreReq(UsbCoreReqDevState)
           && !UsbCoreReq(UsbCoreReqDevSusState)
          )
        {
          HidMouseSendReport(X,Y,Buttons);
        }
        ButtonsHold = Buttons;
        Y = X = Buttons = 0;
      }
    }
  }
}
