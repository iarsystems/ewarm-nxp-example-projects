/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2009 Embedded Artists AB
 *
 * Description:
 *    Touch screen.
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/


#include <stdlib.h> 
#include <string.h>
#include "lpc313x_sysreg.h"
#include "lpc313x_spi_driver.h"
#include "lpc313x_ioconf_driver.h"

#include "touch.h"
#include "calibrate.h"


/******************************************************************************
 * Defines, macros, and typedefs
 *****************************************************************************/

typedef struct
{
  UNS_32 validatePattern1;
  MATRIX storedMatrix;
  UNS_32 validatePattern2;
} tStoredCalData;

/******************************************************************************
 * Public variables
 *****************************************************************************/


/******************************************************************************
 * Local variables
 *****************************************************************************/
static INT_32 dev_spi = 0;
static tStoredCalData storedCalData;
static BOOL_32 calibrated;


/******************************************************************************
 * Local functions
 *****************************************************************************/

/***********************************************************************
 *
 * Function: touch_cs_low
 *
 * Purpose: Chip select Low for touch panel
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: None
 *
 **********************************************************************/
static void touch_cs_low(void)
{
  gpio_set_outpin_low(IOCONF_MUX1, 3);
}

/***********************************************************************
 *
 * Function: touch_cs_high
 *
 * Purpose: Chip select High for touch panel
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: None
 *
 **********************************************************************/
static void touch_cs_high(void)
{
  gpio_set_outpin_high(IOCONF_MUX1, 3);
}

/***********************************************************************
 *
 * Function: tp_xfer
 *
 * Purpose: Transfer data to/from touch panel
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     out   : Output data
 *     in    : Input data
 *     bytes : Number of bytes to send/receive
 *
 * Outputs: None
 *
 * Returns: TRUE if the byte was transferred
 *
 * Notes: Do not use this function to transfer more than 8 bytes!
 *
 **********************************************************************/
static void tp_xfer(UNS_8 *out,
                    UNS_8 *in,
                    int bytes)
{
  INT_32 rbytes = 0;
  volatile UNS_32 status;

  /* flush the FFOS */
  spi_ioctl(dev_spi, SPI_FLUSH_RX_FIFO, 0);
  /* write the commands */
  spi_write(dev_spi, out, bytes);
  /* wait till SPI is not busy */
  do
  {
    status = spi_ioctl(dev_spi, SPI_GET_STATUS, SPI_FIFO_ST);
  }
  while ((status & SPI_ST_TX_EMPTY) != SPI_ST_TX_EMPTY);

  while (rbytes < bytes)
  {
    rbytes += spi_read(dev_spi, &in [rbytes], 1);
  }
}


/***********************************************************************
 *
 * Function: spi_read_tp
 *
 * Purpose: Read data from touch panel
 *
 * Parameters:
 *     cmd   : Touch controller command
 *
 * Outputs: None
 *
 * Returns: Read data
 *
 **********************************************************************/
static UNS_16 spi_read_tp(UNS_8 command)
{
  UNS_8 datao [6];
  UNS_8 result[6];

  datao[0] = command;
  datao[1] = 0;
  datao[2] = 0;

  touch_cs_low();
  tp_xfer(datao, &result[0], 3);
  touch_cs_high();

  return (((UNS_16)result[1]) << 8) | (UNS_16)result[2];
}

/******************************************************************************
 * Implementation of public functions
 *****************************************************************************/

/***********************************************************************
 *
 * Function: touch_calibrate
 *
 * Purpose: Store calibration data for the touch panel. 
 *
 * Parameters:
 *
 * Outputs: None
 *
 * Returns: None
 *
 **********************************************************************/
void touch_calibrate(tTouchPoint ref1, tTouchPoint ref2, tTouchPoint ref3, 
  tTouchPoint scr1, tTouchPoint scr2, tTouchPoint scr3)
{
  POINT disp[3];
  POINT scr[3];
  
  disp[0].x = ref1.x;
  disp[0].y = ref1.y;
  disp[1].x = ref2.x;
  disp[1].y = ref2.y;
  disp[2].x = ref3.x;
  disp[2].y = ref3.y;
  
  scr[0].x = scr1.x;
  scr[0].y = scr1.y;
  scr[1].x = scr2.x;
  scr[1].y = scr2.y;
  scr[2].x = scr3.x;
  scr[2].y = scr3.y;  
  
  setCalibrationMatrix(disp, scr,
                       &storedCalData.storedMatrix);

  calibrated = TRUE;
  
}

/***********************************************************************
 *
 * Function: touch_xyz
 *
 * Purpose: Get x, y and z (pressure) value from touch panel 
 *
 * Parameters: None
 *
 * Outputs:
 *    x : x value
 *    y : y value
 *    z: pressure value
 *
 * Returns: None
 *
 **********************************************************************/
void touch_xyz(INT_32 *x, INT_32* y, INT_32* z)
{
  INT_32 ix, iy, iz1, iz2 = 0;
  POINT displayPoint, screenSample;

  ix = spi_read_tp(0x83 | 0x50 | 0x00);
  ix >>= 3;

  //Read Y value
  iy = spi_read_tp(0x83 | 0x10 | 0x00);
  iy >>= 3;

  //Read Z1 value
  iz1 = spi_read_tp(0x83 | 0x30 | 0x00);
  iz1 >>= 3;

  //Read Z2 value
  iz2 = spi_read_tp(0x83 | 0x40 | 0x00);
  iz2 >>= 3;

  *z = (ix*iz2 - iz1) / iz1;
  if (*z > 40000)
    *z = 0;    

  if (calibrated) 
  {
      screenSample.x = ix;
      screenSample.y = iy;
      getDisplayPoint( &displayPoint, &screenSample, 
        &(storedCalData.storedMatrix) ) ;
      *x = displayPoint.x;
      *y = displayPoint.y;
  } 
  else 
  {
    *x = ix;
    *y = iy;
  }  
}

/***********************************************************************
 *
 * Function: touch_temp
 *
 * Purpose: Get tempature value from touch panel 
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Tempature value
 *
 **********************************************************************/
UNS_32 touch_temp(void)
{

  UNS_32 t0 = 0;
  UNS_32 t1 = 0;

  t0 = spi_read_tp(0x83 | 0x00 | 0x04);
  t0 >>= 3;

  t1  = spi_read_tp(0x83 | 0x70 | 0x04);
  t1 >>= 3;
  t0 = (2573*(t1-t0) - 273000)/1000;

  return t0;
}

/***********************************************************************
 *
 * Function: touch_init
 *
 * Purpose: Initialize touch
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: None
 *
 **********************************************************************/
void touch_init(void)
{
  SPI_SLAVE_CONFIG_T cfg;

  /* configure the slave  */
  cfg.sid = (SPI_SLAVE_ID_T)SPI_SLAVE1_ID;
  cfg.databits = 8;
  cfg.words = SPI_FIFO_DEPTH;
  cfg.mode = SPI_MODE0;
  cfg.cs_high = 0;
  cfg.pp_delay = 0;
  cfg.inter_delay = 0;
  cfg.clk = 1000000;

  /* init SPI module */
  dev_spi = spi_open((void *)SPI_BASE, 0);

  /* configure the UART/SPI pins in SPI mode. */
//AR  SYS_REGS->mux_uart_spi_sel = 1;

  touch_cs_high();
  spi_ioctl(dev_spi, SPI_SLAVE_CONFIG, (INT_32)&cfg);
  spi_ioctl(dev_spi, SPI_SET_ACTIVE_SLAVE, SPI_SLAVE1_ID);

  /* make sure SPI module is enabled */
  spi_ioctl(dev_spi, SPI_ENABLE, 1);
  
}

