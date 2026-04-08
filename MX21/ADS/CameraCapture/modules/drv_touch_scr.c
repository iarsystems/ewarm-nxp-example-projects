/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : drv_touch_scr.c
 *    Description : Touchscreen driver module
 *
 *    History :
 *    1. Date        : 14 May, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "drv_touch_scr.h"

volatile Boolean Touch;
volatile Boolean TouchResReady;
volatile Boolean AcquireTo;
volatile Int32U  X,Y;

/*************************************************************************
 * Function Name: Gpt2_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: GPT2 interrupt handler
 *
 *************************************************************************/
void Gpt2_Handler (void)
{
  LED_STAT1_ON();
  // Disable Timer
  TCTL2_bit.TEN = 0;
  // Clear interrupt flag
  TSTAT2_bit.COMP = 1;
  // set flag
  AcquireTo = TRUE;
  LED_STAT1_OFF();
}

/*************************************************************************
 * Function Name: Touch_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Touchscreen interrupt handler
 *
 *************************************************************************/
void Touch_Handler (void)
{
Int32U IntrStat = PTE_ISR;
  // interrupt from the touchscreen controller
  Touch = TRUE;
  // disable PE.10 interrupt
  PTE_IMR_bit.PIN10 = 0;
  // clear the interrupt flag
  PTE_ISR = IntrStat;
}

/*************************************************************************
 * Function Name: AD7873_Transfer
 * Parameters: Int32U Data
 *
 * Return: Int32U
 *
 * Description: Send a data by SPI1 and return the received data from SPI1
 *
 *************************************************************************/
Int32U AD7873_Transfer(Int32U Data)
{
  // send 6 bit of command
  CONTROLREG1_bit.BIT_COUNT = 6-1;
  // Set pin level - 0
  PTD_DR_bit.PIN28 = 0;
  // load data into FIFO
  TXDATAREG1 = Data>>2;
  // data ready to send
  CONTROLREG1_bit.XCH = 1;
  // Wait acquire time 100 us
  // clear flag
  AcquireTo = FALSE;
  // Starting timer
  TCTL2_bit.TEN = 1;
  while(!AcquireTo);
  Data = RXDATAREG1;
  // send 18 bit of command
  CONTROLREG1_bit.BIT_COUNT = 18-1;
  TXDATAREG1 = 0;
  // data ready to send
  CONTROLREG1_bit.XCH = 1;
  __no_operation();
  __no_operation();
  __no_operation();
  while(!INTREG1_bit.RR);
  __no_operation();
  __no_operation();
  __no_operation();
  Data = RXDATAREG1;
  // Set pin level - 1
  PTD_DR_bit.PIN28 = 1;
  Data >>= 3;
  return(Data & 0x1FFF);
}

/*************************************************************************
 * Function Name: TouchScrInit
 * Parameters: none
 *
 * Return: Int32U 1 - pass
 *                0 - fault
 *
 * Description: Init SPI, Ad7873 and PE10 intr by falling edge
 *
 *************************************************************************/
Int32U TouchScrInit (void)
{
Int32U Id;
  // Init variable
  TouchResReady = Touch = FALSE;
  X = Y = 0;
  // Init GPT2 - define acquire time ~100us
  PCCR1_bit.GPT2_EN = 1;
  // Timer disable
  TCTL2_bit.TEN = 0;
  // clear tmr counter when time is disable
  TCTL2_bit.CC = 1;
  // Timer clock source PERCLK1 ~ 16MHz
  TCTL2_bit.CLKSOURCE = 1;
  // Comparator interrupt enable
  TCTL2_bit.COMP_EN = 1;
  // Set prescaler
  TPRER2_bit.PRESCALER = 0;
  // Set compare period ACQUIRE_DLY [us]
  TCMP2 = MPLL_CLK / (PERDIV1 * (1000000 / ACQUIRE_DLY));
  // Clear interrupt flag
  TSTAT2_bit.COMP = 1;
  // Init AITC GPT2 interrupt
  AITC_SetupIntr(Gpt2_Handler,FALSE,INT_GPT2,5);
  AITC_EnableIntSource(INT_GPT2);

  // PD31 assign to CSPI1 MOSI
  PTD_GIUS_bit.PIN31 = 0;PTD_GPR_bit.PIN31 = 0;
  // PD30 assign to CSPI1 MISO
  PTD_GIUS_bit.PIN30 = 0;PTD_GPR_bit.PIN30 = 0;
  // PD29 assign to CSPI1 CLK
  PTD_GIUS_bit.PIN29 = 0;PTD_GPR_bit.PIN29 = 0;
  // PD28 is GPIO
  PTD_GIUS_bit.PIN28 = 1;
  // Connect to data register
  PTD_OCR2_bit.PIN28 = 3;
  // Set direction - output
  PTD_DDIR_bit.PIN28 = 1;
  // Set pin level - 1
  PTD_DR_bit.PIN28 = 1;

  // Init PE10 input, GPIO
  PTE_DDIR_bit.PIN10 = 0; PTE_GIUS_bit.PIN10 = 0;
  // Enable SPI1 clock
  PCCR0_bit.CSPI1_EN = 1;
  // Set PERCLK2 divider  16.625MHz = MPLL_CLK / 16
  PCDR1_bit.PCLK_DIV2 = PERDIV2 - 1;
  // Init SPI POL, PHA = 0, MASTER, ~1MHz - /16, 24bit
  CONTROLREG1 = 0x00018C17;
  // Init Ad7873
  Id = AD7873_Transfer(AD7873_READ_ID);
  if(Id != AD7873_ID)
  {
    return(0);
  }
  // GPIO clock enable
  PCCR0_bit.GPIO_EN = 1;
  // flailing edge interrupt, clear interrupt
  PTE_ICR1_bit.PIN10 = 1; PTE_ISR_bit.PIN10 = 1;
  // Enable port E interrupts
  PMASK_bit.PTE = 1;
  // Init AITC
  AITC_SetupIntr(Touch_Handler,FALSE,INT_GPIO,7);
  AITC_EnableIntSource(INT_GPIO);
  // enable interrupt
  PTE_IMR_bit.PIN10 = 1;
  return(1);
}

/*************************************************************************
 * Function Name: TouchImp
 * Parameters: none
 *
 * Return: none
 *
 * Description: Implement touch - adc result filtering and set touch flag
 *
 *************************************************************************/
void TouchImp (void)
{
static Int32S MeasureCount = MEASURE_REPEAT*2;
static Int16U TouchTmp[MEASURE_REPEAT*2];
  if(Touch)
  {
    // check state of touch interrupt pin
    if(!PTE_SSR_bit.PIN10)
    {
      TouchTmp[--MeasureCount] = AD7873_Transfer(AD7873_READ_X);
      TouchTmp[--MeasureCount] = AD7873_Transfer(AD7873_READ_Y);
      if(MeasureCount == 0)
      {
        for(X = Y = 0; MeasureCount < MEASURE_REPEAT*2;)
        {
          X += TouchTmp[MeasureCount++];
          Y += TouchTmp[MeasureCount++];
        }
        X /= MEASURE_REPEAT;
        Y /= MEASURE_REPEAT;
        TouchResReady = TRUE;
      }
    }
    else
    {
      // Result is not valid
      TouchResReady =  Touch = FALSE;
      // clear pending interrupt
      PTE_ISR_bit.PIN10 = 1;
      // enable interrupt
      PTE_IMR_bit.PIN10 = 1;
      // Init filter
      MeasureCount = MEASURE_REPEAT*2;
    }
  }
}

/*************************************************************************
 * Function Name: TouchGet
 * Parameters: ToushRes_t * pData X,Y data
 *
 * Return: Int32U 0 - untouched
 *                1 - touched
 *
 * Description: Return current state of the touchscreen
 *
 *************************************************************************/
Boolean TouchGet (ToushRes_t * pData)
{
Boolean  TouchResReadyHold = TouchResReady;
Int32U X_coordinate, Y_coordinate;

  if(TouchResReadyHold)
  {
    X_coordinate = X;
    Y_coordinate = Y;
    // Calculate X coordinate in pixels
    if (X_coordinate <= LEFT_UP_X)
    {
      X_coordinate = 0;
    }
    else if (X_coordinate >= RIGHT_DOWN_X)
    {
      X_coordinate = IMAGE_WIDTH;
    }
    else
    {
      X_coordinate = ((X_coordinate - LEFT_UP_X) * IMAGE_WIDTH)/(RIGHT_DOWN_X-LEFT_UP_X);
    }
    // Calculate Y coordinate in pixels
    if (Y_coordinate <= LEFT_UP_Y)
    {
      Y_coordinate = 0;
    }
    else if (Y_coordinate >= RIGHT_DOWN_Y)
    {
      Y_coordinate = IMAGE_HEIGHT;
    }
    else
    {
      Y_coordinate = ((Y_coordinate - LEFT_UP_Y) * IMAGE_HEIGHT)/(RIGHT_DOWN_Y-LEFT_UP_Y);
    }
    pData->X = X_coordinate;
    pData->Y = Y_coordinate;
  }
  return(TouchResReadyHold);
}
