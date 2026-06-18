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
#include "lpc32xx_spi_driver.h"
#include "lpc32xx_gpio_driver.h"

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
  gpio_set_gpo_state(0x00, P3_STATE_GPO(11));
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
  gpio_set_gpo_state(P3_STATE_GPO(11), 0x00);
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

extern INT_32 uartdev;
extern char buff[];

static UNS_16 spi_read_tp(UNS_8 command)
{
  SPI_CONFIG_T spicfg;
  volatile UNS_32 status = 0;
  INT_32 rbytes = 0;
  UNS_8 result[3] = {0,0,0};
  UNS_8 dummy = 00;

  result[0] = command;

  spicfg.transmitter = TRUE;                        //SPI1 is a Tx
  spi_ioctl(dev_spi, SPI_TXRX, (INT_32) & spicfg);

  touch_cs_low();

  spi_write(dev_spi, &command, 1);
  spi_ioctl(dev_spi, SPI_DELAY, 1);

  /* wait until the FIFO is empty */
  status = 0;
  while ((status & SPI_STAT_BE) == 0)
  {
    status = spi_ioctl(dev_spi, SPI_GET_STATUS, SPI_RAW_INTS_ST);
  }

  spicfg.transmitter = FALSE;                       //SPI1 is a Rx
  spi_ioctl(dev_spi, SPI_TXRX, (INT_32) & spicfg);


  result[0] = 0;
  while (rbytes < 2)
  {
    spi_write(dev_spi, &dummy, 1);                //dummy write
    spi_ioctl(dev_spi, SPI_DELAY, 1);

    rbytes += spi_read(dev_spi, &result[rbytes], 1);    //read data
    spi_ioctl(dev_spi, SPI_DELAY, 1);
  }


                   
  spi_ioctl(dev_spi, SPI_DELAY, 1);
  spi_ioctl(dev_spi, SPI_CLEAR_RX_BUFFER, 0);

  touch_cs_high();

  return (((UNS_16)result[0]) << 8) | (UNS_16)result[1];


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
  SPI_CONFIG_T spicfg;

  /* Open SPI */
  spicfg.databits = 8;
  spicfg.highclk_spi_frames = FALSE;
  spicfg.usesecond_clk_spi = FALSE;//TRUE;
  spicfg.spi_clk = 1500000;
  spicfg.msb = FALSE;
  spicfg.transmitter = TRUE;
//  spicfg.busy_halt = FALSE;
  spicfg.unidirectional = TRUE;

  dev_spi = spi_open(SPI1, (INT_32) & spicfg);

  spi_ioctl(dev_spi, SPI_ENABLE, 1);


  touch_cs_high();
  
}

