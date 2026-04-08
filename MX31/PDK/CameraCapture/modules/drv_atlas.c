/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : drv_atlas.c
 *    Description : Atlas MC13783 SPI Driver
 *
 *    History :
 *    1. Date        : 29.10.2008
 *       Author      : Stoyan Choynev
 *       Description : Inital revision. SPI driver implementaion
 *
 *    $Revision: 5880 $
 **************************************************************************/

/** include files **/
#include <NXP/iomcimx31.h>
#include "board.h"
#include "arm_comm.h"
#include "drv_atlas.h"
/** local definitions **/

/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/

/** public data **/

/** private data **/

/** public functions **/
/*************************************************************************
 * Function Name: Init_Atlas_SPI
 * Parameters: None
 *
 * Return: None
 *
 * Description: Init CSPI2 Module. Mastere mode SPI slock is 16.625MHz
 *
 *************************************************************************/
void Init_Atlas_SPI(void)
{
  IOMUX_SW_MUX_CTL29_bit.SW_MUX_OUT_EN3 = 1;  /*SCLK pin Func Enable*/
  IOMUX_SW_MUX_CTL30_bit.SW_MUX_OUT_EN0 = 1;  /*SS2 pin Func Enable*/
  IOMUX_SW_MUX_CTL30_bit.SW_MUX_IN_EN3  = 2;  /*MISO pin Func Enable*/
  IOMUX_SW_MUX_CTL31_bit.SW_MUX_OUT_EN0 = 1;  /*MOSI pin Func Enable*/
  
  CCM_CGR2 |= 3<<4;                       //Enable CSPI2 Clock
     
  CSPI2_CONREG_bit.MODE = 1;          // Master mode
  CSPI2_CONREG_bit.POL = 0;           // SPI mode 0
  CSPI2_CONREG_bit.PHA = 0; 
  CSPI2_CONREG_bit.SSPOL = 1;         // SS active high
  CSPI2_CONREG_bit.DRCTL = 0;         // Ignore RDY signal
  CSPI2_CONREG_bit.BIT_COUNT = 32-1;  // 32 bit word
  // SCLK < 20MHz (IPG_CLK/4)
  CSPI2_CONREG_bit.DATA_RATE = 0;      
  CSPI2_CONREG_bit.CS = 2;            // select CS2
  // Enable CSPI1
  CSPI2_CONREG_bit.EN = 1;
}

/*************************************************************************
 * Function Name: Atlas_Rd_Wr
 * Parameters: Int32U Data - 24 bit Data to send
 *             Int8U Addr - 6 bit Atlas Register Address
 *             Boolean Rd_Wr - 0 Read Operation
 *                           - 1 Write Operation   
 * Return: 24 Received Data
 *
 * Description: Reads or Writes Atlas Register
 *
 *************************************************************************/
Int32U Atlas_Rd_Wr(Int32U Data, Int8U Addr, Boolean Rd_Wr)
{
  Data = ((Rd_Wr)?1UL<<31:0) |
         ((Int32U)(Addr & 0x3F)<<25) |
         ((Rd_Wr)?(Data & 0x01FFFFFF):0);
         
  CSPI2_TXDATA = Data;
  // data ready to send
  CSPI2_CONREG_bit.XCH = 1;
  while(!CSPI2_STATREG_bit.RR || CSPI2_CONREG_bit.XCH);
  Data = CSPI2_RXDATA;
  return(Data & 0x00FFFFFF);
}

/** private functions **/
