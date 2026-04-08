
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lpc_types.h"
#include "lpc_irq_fiq.h"
#include "lpc_arm922t_cp15_driver.h"
#include "lpc313x_chip.h"
#include "ea3131_board.h"
#include "lpc313x_sysreg.h"
#include "lpc313x_mpmc.h"
#include "lpc313x_intc_driver.h"
#include "lpc313x_cgu_driver.h"
#include "lpc313x_i2c_driver.h"
#include "lpc313x_uart_driver.h"
#include "lpc313x_timer_driver.h"
#include "lpc313x_adc10b_driver.h"

#include "lcd_hw.h"
#include "lcd_grph.h"


/* device handles */
static INT_32 dev_uart;
static INT_32 dev_adc;


/* buffer for the uart when printing strings */
static char buff[512];       

/***********************************************************************
 *
 * Function: uart_string_write
 *
 * Purpose: Output a string on the UART
 *
 * Parameters:
 *    devid : UART device ID
 *    pbuff : string to output
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
void uart_string_write(INT_32 devid, char *pbuff)
{
  UNS_32 len = strlen(pbuff);
  UNS_32 written;

  while (len > 0)
  {
    written = uart_write(devid, pbuff, len);
    len -= written;
    pbuff += written;
  }
}



/***********************************************************************
 *
 * Function: uart_init
 *
 * Purpose: Initialize UART
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
static void uart_init(void)
{
  UART_CONTROL_T ucntl;

  /* Setup UART for 115.2K, 8 data bits, no parity, 1 stop bit */
  ucntl.baud_rate = 115200;
  ucntl.parity = UART_PAR_NONE;
  ucntl.databits = 8;
  ucntl.stopbits = 1;

  dev_uart = uart_open((void *) UART_BASE, (INT_32) & ucntl);
}

/***********************************************************************
 *
 * Function: board_lcd_init
 *
 * Purpose: Initialize LCD
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
static void board_lcd_init(void)
{

//  MPMC->static_mem[0].config = MPMC_STATIC_CFG_MW16 | MPMC_STATIC_CFG_BLS | MPMC_STATIC_CFG_B;
  /* Static Memory Configuration for nCS0 */
  MPMC->static_mem[0].config = MPMC_STATIC_CFG_MW16 | MPMC_STATIC_CFG_BLS ;
  /* Static Memory Write Enable Delay for nCS0 */
  MPMC->static_mem[0].wtwen  = 2;
  /* Static Memory Output Enable Delay for nCS0 */
  MPMC->static_mem[0].wtoen  = 1;
  /* Static Memory Read Delay for nCS0 */
  MPMC->static_mem[0].wtrd   = 15;
  /* Static Memory Page Mode Read Delay for nCS0 */
  MPMC->static_mem[0].wtpg   = 2;
  /* Static Memory Write Delay for nCS0 */
  MPMC->static_mem[0].wtwr   = 4 /*2*/;
  /* Static Memory Turn Around Delay for nCS0 */
  MPMC->static_mem[0].wtturn = 2;

  lcd_init();
}

/***********************************************************************
 *
 * Function: adcRead
 *
 * Purpose: ADC read
 *
 * Parameters:
 *    devid : ADC device ID
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
static void adcRead(INT_32 devid)
{
  ADC_REGS_T *adcregsptr;
  ADC_CFG_T *adccfgptr = (ADC_CFG_T *) devid;
  adcregsptr = adccfgptr->regptr;

  /* start conversion scan */
  adc_ioctl(devid, ADC_START_CONVERSION, 0);

  /* Wait until a conversion is done*/
  while(adcregsptr->adc_int_status_reg == 0);
 
  /* clear interrupt */
  adcregsptr->adc_int_clear_reg = 1;

  if (adcregsptr->adc_csel_reg & 0x0000000F)
    adccfgptr->rx[0] = (UNS_16) adcregsptr->adc_r0_reg & ADCDAT_VALUE_MASK;
  if (adcregsptr->adc_csel_reg & 0x000000F0)
    adccfgptr->rx[1] = (UNS_16) adcregsptr->adc_r1_reg & ADCDAT_VALUE_MASK;
  if (adcregsptr->adc_csel_reg & 0x00000F00)
    adccfgptr->rx[2] = (UNS_16) adcregsptr->adc_r2_reg & ADCDAT_VALUE_MASK;


  while(adcregsptr->adc_int_status_reg == 0);
  
  adc_ioctl(dev_adc, ADC_STOP_CONVERSION, 0);
}

/***********************************************************************
 *
 * Function: adc_init
 *
 * Purpose: Initialize ADC
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: None
 *
 * Notes: None
 *
 **********************************************************************/
static void adc_init(void)
{
  dev_adc = adc_open((void*)ADC_BASE, 0);

  adc_ioctl(dev_adc, ADC_SELECT_CHANNEL_0, 10); 
  adc_ioctl(dev_adc, ADC_SELECT_CHANNEL_1, 10); 
  adc_ioctl(dev_adc, ADC_SELECT_CHANNEL_2, 10); 
  adc_ioctl(dev_adc, ADC_SELECT_CONTINUOUS_SCAN_MODE, 0); 
}

static UNS_16 lastx = 0;
static INT_32 lasty = 0;
static INT_32 lastz = 0;

/***********************************************************************
 *
 * Function: getSamples
 *
 * Purpose: Get sample from ADC (accelerometer)
 *
 * Parameters:
 *      data : array of three values
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
static void getSamples(UNS_16* data)
{
#define NUM_SAMPLES 20

  ADC_CFG_T *adc = NULL;
  UNS_16 x = 0;
  UNS_16 y = 0;
  UNS_16 z = 0;
  int i = 0;

  adc = (ADC_CFG_T*)dev_adc;

  /* simple filtering (average) */
  for (i = 0; i < NUM_SAMPLES; i++) {
    adcRead(dev_adc);

    x += adc->rx[0];
    y += adc->rx[1];
    z += adc->rx[2];

    //timer_wait_ms(NULL, 100/NUM_SAMPLES);
  }

  x /= NUM_SAMPLES;
  y /= NUM_SAMPLES;
  z /= NUM_SAMPLES;

  if (abs(x-lastx) > 3)
    lastx = x;

  if (abs(y-lasty) > 3)
    lasty = y;

  if (abs(z-lastz) > 3)
    lastz = z;

  data[0] = lastx;
  data[1] = lasty;
  data[2] = lastz;

}

#define SLIDER_WIDTH 50

#define RECT_MARGIN 6
#define RECT_WIDTH (DISPLAY_WIDTH-SLIDER_WIDTH-RECT_MARGIN)
#define RECT_X     (SLIDER_WIDTH + RECT_MARGIN/2)
#define RECT_Y     (DISPLAY_HEIGHT/2 - RECT_WIDTH/2)

#define MID_X (RECT_X + RECT_WIDTH/2)
#define MID_Y (RECT_Y + RECT_WIDTH/2)


static INT_16 last_rx = 0;
static INT_16 last_ry = 0;
static INT_16 last_rz = 0;

/***********************************************************************
 *
 * Function: drawXY
 *
 * Purpose: Draw x and y values on the display
 *
 * Parameters:
 *      x : x value to draw
 *      y : y value to draw
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
static void drawXY(INT_16 x, INT_16 y)
{
  x = x/2;
  y = y/2;


  if (x >= RECT_WIDTH/2)
    x = RECT_WIDTH/2-1;

  if (-x >= RECT_WIDTH/2)
    x = -(RECT_WIDTH/2)+1;

  if (y >= RECT_WIDTH/2)
    y = RECT_WIDTH/2-1;

  if (-y >= RECT_WIDTH/2)
    y = -(RECT_WIDTH/2)+1;

  /* erase last line */
  lcd_line(MID_X, MID_Y, MID_X+last_rx, MID_Y+last_ry, LIGHT_GRAY);

  lcd_line(MID_X, MID_Y, MID_X+x, MID_Y+y, BLUE);

  last_rx = x;
  last_ry = y;  
}

/***********************************************************************
 *
 * Function: drawZ
 *
 * Purpose: Draw z values on the display
 *
 * Parameters:
 *      z : z value to draw
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
static void drawZ(INT_16 z)
{
  INT_16 x0 = SLIDER_WIDTH/2-10;
  INT_16 x1 = SLIDER_WIDTH/2+10;
  INT_16 yc = DISPLAY_HEIGHT/2;

  z = z/2;

  if (yc+z <= SLIDER_WIDTH)
    z = SLIDER_WIDTH-yc;

  if (yc+z >= DISPLAY_HEIGHT-SLIDER_WIDTH)
    z = DISPLAY_HEIGHT-SLIDER_WIDTH-yc;

  lcd_fillRect(x0, yc+last_rz, x1, yc+last_rz+2, WHITE);
  lcd_fillRect(SLIDER_WIDTH/2-1, yc+last_rz, SLIDER_WIDTH/2+1, yc+last_rz+2, BLACK);

  lcd_fillRect(x0, yc+z, x1, yc+z+2, GREEN);
  
  last_rz = z;  
}

/***********************************************************************
 *
 * Function: drawBackground
 *
 * Purpose: Draw background on display
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
static void drawBackground(void)
{
  lcd_fillScreen(WHITE);

  /* area for x, and y values */
  lcd_drawRect(RECT_X, RECT_Y, RECT_X+RECT_WIDTH, RECT_Y+RECT_WIDTH, BLACK);
  lcd_fillRect(RECT_X+1, RECT_Y+1, RECT_X+RECT_WIDTH-1, RECT_Y+RECT_WIDTH-1, LIGHT_GRAY);

  /* area for z values */

  lcd_fillRect(SLIDER_WIDTH/2-1, SLIDER_WIDTH, SLIDER_WIDTH/2+1, DISPLAY_HEIGHT-SLIDER_WIDTH, BLACK);
}

/***********************************************************************
 *
 * Function: c_entry
 *
 * Purpose: Application entry point from the startup code
 *
 * Processing:
 *     See function.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
void c_entry(void)
{
  UNS_16 origValues[3];
  UNS_16 values[3];

  /* Disable interrupts in ARM core */
  disable_irq_fiq();

  /* Setup miscellaneous board functions */
  ea3131_board_init();

  /* Set virtual address of MMU table */
  cp15_set_vmmu_addr((void *)ISROM_MMU_TTB_BASE);

  /* Initialize interrupt system */
  int_initialize(0xFFFFFFFF);

  /* Enable IRQ interrupts in the ARM core */
  enable_irq();

  /* initialize uart */
  uart_init();

  /* setup LCD */
  board_lcd_init();

  /* initialize adc */
  adc_init();

  sprintf(buff, "Starting\r\n");
  uart_string_write(dev_uart, buff); 


  drawBackground();

  /* get starting values */
  getSamples(origValues);
  getSamples(origValues);
  getSamples(origValues);

  while (1) {

    getSamples(values);
    drawXY(origValues[0]-values[0], origValues[1]-values[1]);
    drawZ(origValues[2]-values[2]);

  }

}
