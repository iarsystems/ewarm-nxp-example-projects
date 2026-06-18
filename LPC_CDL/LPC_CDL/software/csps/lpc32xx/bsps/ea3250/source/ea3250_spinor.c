
/***********************************************************************
 * Includes
 **********************************************************************/

#include <stdio.h>
#include <string.h>

#include "lpc_types.h"
#include "lpc_irq_fiq.h"
#include "lpc_arm922t_cp15_driver.h"
#include "ea3250_board.h"
#include "ea3250_spinor.h"
#include "lpc32xx_ssp_driver.h"
#include "lpc32xx_intc_driver.h"
#include "lpc32xx_gpio_driver.h"
#include "lpc32xx_spi_driver.h"
#include "lpc32xx_timer_driver.h"
#include "lpc32xx_uart_driver.h"
#include "lpc32xx_clkpwr.h"

#include "uart.h"

/***********************************************************************
 * Defines and typedefs
 **********************************************************************/

/* Manufacturer IDs */
#define MANFID_ATMEL    0x1F
#define MANFID_SPANSION    0x01

/* Common commands */

#define CMD_RDID        0x9F        /* read device ID */
#define CMD_FAST_READ   0x0B        /* read data (extra command setup time allows higher SPI clock) */
#define CMD_RES         0xAB        /* release from deep power down */

/* Atmel commands and status values   */

#define AT_CMD_RDSR     0xD7        /* read status register */
#define AT_CMD_PP       0x82        /* Page Program Through Buffer */
#define AT_CMD_BE       0x50        /* block erase */
#define AT_CMD_SE       0x7C        /* block erase */

#define STATUS_RDY      (1 << 7)
#define STATUS_COMP     (1 << 6)
#define STATUS_PROTECT  (1 << 1)
#define STATUS_P512     (1 << 0)

/* Spansion commands and status values*/

#define SP_CMD_RDSR   0x05     /* read status register */
#define SP_CMD_PP     0x02     /* Page Program  */
#define SP_CMD_WREN   0x06     /* Write enable  */
#define SP_ERASE_P4E  0x20     /* 4KB erase     */
#define SP_CMD_SE     0xD8     /* sector erase     */
#define SP_CMD_RCR    0x35     /* read configuration register */

#define SP_STATUS_SRWD   (1 << 7)
#define SP_STATUS_P_ERR  (1 << 6)
#define SP_STATUS_E_ERR  (1 << 5)
#define SP_STATUS_WEL    (1 << 1)
#define SP_STATUS_WIP    (1 << 0)


#define DUMMY_BYTE		  0xFF

#define PAGE_528       (528)
#define PAGE_512       (512)
#define PAGE_256       (256)

#define BLOCK_SZ       (4096)


/***********************************************************************
 * Local variables
 **********************************************************************/

/* SPI device handles */
static INT_32 spidev=-1;

/* SPI device configuration */
static SPI_CONFIG_T spicfg;

/* NOR flash manufacturer ID */
static UNS_32 norManufId = 0;

/* will be set to the page size of the NOR flash */
static UNS_32 pageSize = 0;

/***********************************************************************
 * Local functions
 **********************************************************************/

static void exit_deep_power_down(void)
{
  UNS_8 cmd = CMD_RES;
  spicfg.transmitter = TRUE;                        //SPI1 is a Tx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 0);
  spi_write(spidev, &cmd, 1);
  spi_ioctl(spidev, SPI_DELAY, 1);

  spicfg.transmitter = FALSE;                       //SPI1 is a Rx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  spi_read(spidev, &cmd, 1);                   //read data
  spi_ioctl(spidev, SPI_DELAY, 1);
  spi_ioctl(spidev, SPI_CLEAR_RX_BUFFER, 0);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 1);

  timer_wait_us(NULL, 50);
}

static void read_status_register(UNS_32 *status)
{
  UNS_8 datai [2], datao [2];

  if (norManufId ==  MANFID_ATMEL) {
    datao[0] = AT_CMD_RDSR;
  }
  else {
    datao[0] = SP_CMD_RDSR;
  }

  datao[1] = DUMMY_BYTE;

  spicfg.transmitter = TRUE;                        //SPI1 is a Tx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 0);
  spi_write(spidev, datao, 1);
  spi_ioctl(spidev, SPI_DELAY, 1);

  spicfg.transmitter = FALSE;                       //SPI1 is a Rx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  spi_write(spidev, &datao[1], 1);                  //dummy write
  spi_ioctl(spidev, SPI_DELAY, 1);

  spi_read(spidev, datai, 1);                       //read data
  spi_ioctl(spidev, SPI_DELAY, 1);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 1);
  spi_ioctl(spidev, SPI_DELAY, 1);
  spi_ioctl(spidev, SPI_CLEAR_RX_BUFFER, 0);

  *status = datai[0];
}


static void poll_is_busy(void)
{
  UNS_32 status;

  do
  {
    timer_wait_us(NULL, 10);
    read_status_register(&status);
  } while ((norManufId == MANFID_ATMEL && (status & STATUS_RDY) == 0)
    || (norManufId == MANFID_SPANSION && (status & SP_STATUS_WIP) != 0));


}

static void read_device_id(UNS_32 spidev, UNS_8* device_id)
{
  UNS_8 datao [2];
  INT_32 rbytes = 0;

  datao[0] = CMD_RDID;
  datao[1] = DUMMY_BYTE;

  spicfg.transmitter = TRUE;                        //SPI1 is a Tx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 0);
  spi_write(spidev, datao, 1);
  spi_ioctl(spidev, SPI_DELAY, 1);

  spicfg.transmitter = FALSE;                       //SPI1 is a Rx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  while (rbytes < 4) {

    spi_write(spidev, &datao[1], 1);                //dummy write
    spi_ioctl(spidev, SPI_DELAY, 1);

    rbytes += spi_read(spidev, &device_id[rbytes], 1);    //read data
    spi_ioctl(spidev, SPI_DELAY, 1);
  }

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 1);
  spi_ioctl(spidev, SPI_DELAY, 1);
  spi_ioctl(spidev, SPI_CLEAR_RX_BUFFER, 0);
}

static void atmel_write_cfg_reg(void)
{
  UNS_8 datao [4];

  /* 4 bytes magic sequence to program the device to 512 page device */
  datao[0] = 0x3D;
  datao[1] = 0x2A;
  datao[2] = 0x80;
  datao[3] = 0xA6;

  spicfg.transmitter = TRUE;                        //SPI1 is a Tx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 0);
  spi_write(spidev, datao, 4);
  spi_ioctl(spidev, SPI_DELAY, 1);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 1);
  spi_ioctl(spidev, SPI_DELAY, 1);
}

static void atmel_page_program(UNS_32 offset, UNS_8 *src, UNS_32 bytecount)
{
  UNS_8 datao [4];
  UNS_32 count, status;

  /* assuming sectors are not protected. */

  datao[0] = AT_CMD_PP;
  datao[1] = (offset >> 16) & 0xFF; /* 24bit address upper byte */
  datao[2] = (offset >>  8) & 0xFF; /* 24bit address middle byte */
  datao[3] = (offset >>  0) & 0xFF; /* 24bit address least significant byte */

  spicfg.transmitter = TRUE;                        //SPI1 is a Tx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 0);
  spi_write(spidev, datao, 4);
  spi_ioctl(spidev, SPI_DELAY, 1);

  while (bytecount > 0)
  {
    count = spi_write(spidev, src, bytecount);
    bytecount -= count;
    src += count;
  }
  
  /* wait for FIFO to be empty */
  do
  {
    status = spi_ioctl(spidev, SPI_GET_STATUS, SPI_RAW_INTS_ST);
  }
  while ((status & SPI_STAT_BE) == 0);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 1);
  spi_ioctl(spidev, SPI_DELAY, 1);
  spi_ioctl(spidev, SPI_CLEAR_RX_BUFFER, 0);

  poll_is_busy();
}

static void atmel_erase(UNS_8 cmd, UNS_32 offset)
{
  UNS_8 datao [4];

  /* assuming sectors are not protected. */

  datao[0] = AT_CMD_BE;
  datao[1] = (offset >> 16) & 0xFF; /* 24bit address upper byte */
  datao[2] = (offset >>  8) & 0xFF; /* 24bit address middle byte */
  datao[3] = (offset >>  0) & 0xFF; /* 24bit address least significant byte */

  spicfg.transmitter = TRUE;                        //SPI1 is a Tx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 0);
  spi_write(spidev, datao, 4);
  spi_ioctl(spidev, SPI_DELAY, 1);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 1);

  poll_is_busy();
}

static void atmel_erase_block(UNS_32 offset)
{
  atmel_erase(AT_CMD_BE, offset);
}

static void atmel_erase_sector(UNS_32 offset)
{
  atmel_erase(AT_CMD_SE, offset);
}

static void spansion_read_conf_register(UNS_32 *status)
{
  UNS_8 datai [2], datao [2];

  datao[0] = SP_CMD_RCR;
  datao[1] = DUMMY_BYTE;

  spicfg.transmitter = TRUE;                        //SPI1 is a Tx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 0);
  spi_write(spidev, datao, 1);
  spi_ioctl(spidev, SPI_DELAY, 1);

  spicfg.transmitter = FALSE;                       //SPI1 is a Rx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  spi_write(spidev, &datao[1], 1);                  //dummy write
  spi_ioctl(spidev, SPI_DELAY, 1);

  spi_read(spidev, datai, 1);                       //read data
  spi_ioctl(spidev, SPI_DELAY, 1);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 1);
  spi_ioctl(spidev, SPI_DELAY, 1);
  spi_ioctl(spidev, SPI_CLEAR_RX_BUFFER, 0);

  *status = datai[0];
}

static void  spansion_wren(void)
{
  UNS_8 cmd = SP_CMD_WREN;
  spicfg.transmitter = TRUE;                        //SPI1 is a Tx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 0);
  spi_write(spidev, &cmd, 1);
  spi_ioctl(spidev, SPI_DELAY, 1);


  spicfg.transmitter = FALSE;                       //SPI1 is a Rx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  spi_read(spidev, &cmd, 1);                   //read data
  spi_ioctl(spidev, SPI_DELAY, 1);
  spi_ioctl(spidev, SPI_CLEAR_RX_BUFFER, 0);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 1);

  timer_wait_us(NULL, 50);
}

static void spansion_page_program(UNS_32 offset, UNS_8 *src, UNS_32 bytecount)
{
  UNS_8 datao [4];
  UNS_32 count, status;

  /* assuming sectors are not protected. */

  datao[0] = SP_CMD_PP;
  datao[1] = (offset >> 16) & 0xFF; /* 24bit address upper byte */
  datao[2] = (offset >>  8) & 0xFF; /* 24bit address middle byte */
  datao[3] = (offset >>  0) & 0xFF; /* 24bit address least significant byte */


  spicfg.transmitter = TRUE;                        //SPI1 is a Tx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 0);
  spi_write(spidev, datao, 4);
  spi_ioctl(spidev, SPI_DELAY, 1);

  while (bytecount > 0)
  {
    count = spi_write(spidev, src, bytecount);
    bytecount -= count;
    src += count;
  }
  
  /* wait for FIFO to be empty */
  do
  {
    status = spi_ioctl(spidev, SPI_GET_STATUS, SPI_RAW_INTS_ST);
  }
  while ((status & SPI_STAT_BE) == 0);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 1);
  spi_ioctl(spidev, SPI_DELAY, 1);
  spi_ioctl(spidev, SPI_CLEAR_RX_BUFFER, 0);

  poll_is_busy();
}

static void spansion_erase(UNS_8 cmd, UNS_32 offset)
{
  UNS_8 datao [4];

  /* assuming sectors are not protected. */

  datao[0] = cmd;
  datao[1] = (offset >> 16) & 0xFF; /* 24bit address upper byte */
  datao[2] = (offset >>  8) & 0xFF; /* 24bit address middle byte */
  datao[3] = (offset >>  0) & 0xFF; /* 24bit address least significant byte */

  spicfg.transmitter = TRUE;                        //SPI1 is a Tx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 0);
  spi_write(spidev, datao, 4);
  spi_ioctl(spidev, SPI_DELAY, 1);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 1);

  poll_is_busy();
}


static void spansion_erase_block(UNS_32 offset)
{
  spansion_erase(SP_ERASE_P4E, offset);
}

static void spansion_erase_sector(UNS_32 offset)
{
  spansion_erase(SP_CMD_SE, offset);
}


/***********************************************************************
 * Public functions
 **********************************************************************/

/***********************************************************************
 *
 * Function: spinor_read
 *
 * Purpose: Read data from the NOR flash
 *
 * Parameters:
 *     dst : buffer for the read data
 *     offset : offset in the flash
 *     bytecount : number of bytes to read
 *
 * Outputs: None
 *
 * Returns: <0 if an error occurs
 *
 * Notes: None
 *
 **********************************************************************/
STATUS spinor_read(UNS_8 *dst, UNS_32 offset, UNS_32 bytecount)
{
  UNS_8 dummy [1], datao [5];
  INT_32 rbytes = 0;
  volatile UNS_32 status = 0;

  if (spidev == 0)
    return _ERROR;

  dummy[0] = DUMMY_BYTE;

  exit_deep_power_down();

  datao[0] = (CMD_FAST_READ);          /* FAST_READ is safe at all supported SPI speeds... */
  datao[1] = ((offset >> 16) & 0xFF);  /* 24bit address upper byte */
  datao[2] = ((offset >>  8) & 0xFF);  /* 24bit address middle byte */
  datao[3] = ((offset >>  0) & 0xFF);  /* 24bit address least significant byte */
  datao[4] = (0);                      /* dummy byte */

  spicfg.transmitter = TRUE;                        //SPI1 is a Tx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 0);
  spi_write(spidev, datao, 5);
  spi_ioctl(spidev, SPI_DELAY, 1);

  /* wait until the FIFO is empty */
  status = 0;
  while ((status & SPI_STAT_BE) == 0)
  {
    status = spi_ioctl(spidev, SPI_GET_STATUS, SPI_RAW_INTS_ST);
  }

  spicfg.transmitter = FALSE;                       //SPI1 is a Rx
  spi_ioctl(spidev, SPI_TXRX, (INT_32) & spicfg);

  while (rbytes < bytecount)
  {
    spi_write(spidev, dummy, 1);                //dummy write
    spi_ioctl(spidev, SPI_DELAY, 1);

    rbytes += spi_read(spidev, &dst[rbytes], 1);    //read data
    spi_ioctl(spidev, SPI_DELAY, 1);
  }

  spi_ioctl(spidev, SPI_DRIVE_SSEL, 1);
  spi_ioctl(spidev, SPI_DELAY, 1);
  spi_ioctl(spidev, SPI_CLEAR_RX_BUFFER, 0);

  return _NO_ERROR;
}

/***********************************************************************
 *
 * Function: spinor_write_page
 *
 * Purpose: Write one page of data to the NOR flash
 *
 * Parameters:
 *     src : buffer containing data to write to the flash
 *     offset : page offset in the flash
 *
 * Outputs: None
 *
 * Returns: <0 if an error occurs
 *
 * Notes: None
 *
 **********************************************************************/
STATUS spinor_write_page(UNS_8 *src, UNS_32 offset)
{
  if ((spidev == 0) ||
      ((offset % pageSize) != 0)) /* error if address is not sector aligned */
  {
    return _ERROR;
  }

  exit_deep_power_down();

  if(norManufId == MANFID_ATMEL) {
    atmel_page_program(offset, src, pageSize);
  } else {
    spansion_wren();
    spansion_page_program(offset, src, pageSize);
  }

  return pageSize;
}

/***********************************************************************
 *
 * Function: spinor_block_erase
 *
 * Purpose: Erase a block (4KB)
 *
 * Parameters:
 *     offset : block offset in the flash
 *
 * Outputs: None
 *
 * Returns: <0 if an error occurs
 *
 * Notes: None
 *
 **********************************************************************/
STATUS spinor_block_erase(UNS_32 offset) 
{
  if ((spidev == 0))
  {
    return _ERROR;
  }

  exit_deep_power_down();

  poll_is_busy();

  if(norManufId == MANFID_ATMEL) {
    atmel_erase_block(offset);
  } else {
    spansion_wren();
    spansion_erase_block(offset);
  }

  return _NO_ERROR;
}

/***********************************************************************
 *
 * Function: spinor_sector_erase
 *
 * Purpose: Erase a block (64KB)
 *
 * Parameters:
 *     offset : sector offset in the flash
 *
 * Outputs: None
 *
 * Returns: <0 if an error occurs
 *
 * Notes: None
 *
 **********************************************************************/
STATUS spinor_sector_erase(UNS_32 offset) 
{
  if ((spidev == 0))
  {
    return _ERROR;
  }

  exit_deep_power_down();

  poll_is_busy();

  if(norManufId == MANFID_ATMEL) {
    atmel_erase_sector(offset);
  } else {
    spansion_wren();
    spansion_erase_sector(offset);
  }

  return _NO_ERROR;
}

/***********************************************************************
 *
 * Function: spinor_setTo512Mode
 *
 * Purpose: Change to 512 byte mode (only valid for Atmel NOR flash)
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
void spinor_setTo512Mode(void)
{
  /* only valid for the Atmel NOR flash */
  if (norManufId ==  MANFID_ATMEL) {
    atmel_write_cfg_reg();
  }  
}

/***********************************************************************
 *
 * Function: spinor_init
 *
 * Purpose: SPI NOR Initialization
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns:
 *    The status of the initialization
 *
 * Notes: None
 *
 **********************************************************************/
STATUS spinor_init(void)
{
  UNS_8 device_id[4];
  UNS_32 temp;

  GPIO->p2_mux_clr = P2_GPIO05_SSEL0;
  GPIO->p2_dir_set = P3_STATE_GPIO(5);
  GPIO->p3_outp_set = P3_STATE_GPIO(5);

  /* Select SPI1 pins */
  GPIO->p_mux_clr = P_SPI1DATAIO_SSP0_MOSI |
                    P_SPI1DATAIN_SSP0_MISO | P_SPI1CLK_SCK0;
  CLKPWR->clkpwr_spi_clk_ctrl |= CLKPWR_SPICLK_USE_SPI1;

  /* Open SPI */
  spicfg.databits = 8;
  spicfg.highclk_spi_frames = TRUE;
  spicfg.usesecond_clk_spi = TRUE;
  spicfg.spi_clk = 1000000;
  spicfg.msb = FALSE;
  spicfg.transmitter = TRUE;
  spicfg.busy_halt = FALSE;
  spicfg.unidirectional = TRUE;
  spidev = spi_open(SPI1, (INT_32) & spicfg);
  if (spidev == 0)
  {
    /* Error */
    return _ERROR;
  }
  spi_ioctl(spidev, SPI_ENABLE, 1);


  exit_deep_power_down();
  read_device_id(spidev, device_id);

  /* return if it is not ATMEL device */
  if (device_id[0] ==  MANFID_ATMEL) {
    norManufId = MANFID_ATMEL;
    pageSize = PAGE_512; 

    do
    {
      read_status_register(&temp);
      temp = temp;
    }
    while ((temp & STATUS_RDY) == 0);

    if ((temp & STATUS_P512) == 0) {
      /* NOT configured to 512 byte mode */
      pageSize = PAGE_528;
    }

  }
  else if (device_id[0] ==  MANFID_SPANSION) {
    norManufId = MANFID_SPANSION;
    pageSize = PAGE_256;     

    do
    {
      read_status_register(&temp);
      temp = temp;
    }
    while ((temp & SP_STATUS_WIP) != 0);


    spansion_read_conf_register(&temp);
  } 
  else {
    uart_output("Unknown NOR flasg\r\n");
    return _ERROR;
  }

  poll_is_busy();

  return pageSize;
}

void spinor_close(void)
{
  spi_close(spidev);
}






