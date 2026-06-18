#include <LPC11Uxx.H>                       /* LPC11Uxx definitions */
#include "rtc_api.h"
#include "i2c_platform_lpc11Uxx.h"
#include "rtc_platform_pcf8523t.h"

#define PCF8523T_NUM_OF_REGISTERS 20
int pcf8523t_set(uint8_t * dti)
{
int i;
uint8_t buf[PCF8523T_NUM_OF_REGISTERS];
  buf[0] = 0x00;
  buf[1] = 0x00;
  buf[2] = 0x00;

  buf[3] = dti[5];    // Seconds
  buf[4] = dti[4];    // Minutes
  buf[5] = dti[3];    // Hours
  buf[6] = dti[2];    // Date
  buf[7] =  0;        // Day of week not supported! -> Always 1
  buf[8] = dti[1];    // Month
  buf[9] = dti[0];    // Year
  //-- convert to BCD
  for(i=3; i<10; i++)
  {
    buf[i] = pcf8523t_toBCD(buf[i]);
  }
  //-- Set control bits
  //buf[0] &= 0x7f;    //-- Seconds  bit 7(CH)    = 0 (clock enabled)
  //buf[2] &= 0xbf;    //-- Hours    bit 6(12/24) = 0(24-hour mode)

  i2c_lpc_init(I2C_SPEED_100);
  i = pcf8523t_wr_regs(buf);
  i2c_lpc_init(I2C_SPEED_400);
  return i;
}


int pcf8523t_get(uint8_t * dti)
{
int rc;
uint8_t buf[20];
  if(dti == NULL)
    return RTC_ERR_WRONG_PARAM;
  i2c_lpc_init(I2C_SPEED_100);
  rc = pcf8523t_rd_regs(buf);
  i2c_lpc_init(I2C_SPEED_400);

  if(rc != I2C_NO_ERR)
    return rc;

  //-- Mask off the unused bits
  buf[3] &= 0x7F; // Seconds
  buf[4] &= 0x7F; // Minutes
  buf[5] &= 0x1F; // Hours
  buf[6] &= 0x3F; //  Date (day of month)
  buf[7] &= 0x00; // Day of week not supported! -> Always 1
  buf[8] &= 0x1F; // Month
  buf[9] &= 0xFF; // Year -

  //-- Convert from BCD
  dti[5] = pcf8523t_fromBCD(buf[3]); // Seconds
  dti[4] = pcf8523t_fromBCD(buf[4]); // Minutes
  dti[3] = pcf8523t_fromBCD(buf[5]); // Hours
  // dti[3] =1;
  dti[2] = pcf8523t_fromBCD(buf[6]); // Date
  dti[1] = pcf8523t_fromBCD(buf[8]); // Month
  dti[0] = pcf8523t_fromBCD(buf[9]); // Year
  return I2C_NO_ERR;
}


uint8_t pcf8523t_toBCD(uint8_t bin_val)
{
//-- this function converts an 8 bit binary value
//-- to an 8 bit BCD value. Input range from 0 to 99.
uint8_t temp;
uint8_t retval;

  temp = bin_val;
  retval = 0;

  for(;;)
  {  // get tens digit by multiple subtraction of 10 from bin_val
    if(temp >= 10)
    {
      temp -= 10;
      retval += 0x10; // increment tens digit
    }
    else  // get ones digit by adding remainder
    {
      retval += temp; // adjusted result
      break;
    }
  }
  return retval;
}

//----------------------------------------------------------------------------
uint8_t pcf8523t_fromBCD(uint8_t bcd_val)
{
  return ((bcd_val >> 4) & 0x0F)*10 + (bcd_val & 0x0F);
}

//----------------------------------------------------------------------------


//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
int pcf8523t_wr_regs(uint8_t * buf)  //---- Wr 8 control bytes to pcf8523t
{
int rc;
int num;
//---- Wr ----------
  //--- wr START + CONTROL
  rc = i2c_lpc_ctrl(0xd0);     //-- Now WR (RD/WI = 0); 0xd0 - ID of pcf8523t
  if(rc != I2C_NO_ERR)
    return rc;
  //--- wr ADDRESS
  i2c_lpc_wr_byte(0);
  //---  Write  data
  num = 10;
  while(num--)                 //-- transmit data until length>0
  {
    rc = *buf++; //---
    i2c_lpc_wr_byte(rc);
  }
  i2c_lpc_stop();
  //--------------------
  return I2C_NO_ERR;
}

//----------------------------------------------------------------------------
int pcf8523t_rd_regs(uint8_t * buf)  //---- Rd 8 control bytes from pcf8523t
{
int rc;
//---- RD ------
  //--- wr START + CONTROL
  rc = i2c_lpc_ctrl(0xd0); //-- Now WR (RD/WI = 0)
  if(rc != I2C_NO_ERR)
    return rc;
  //--- wr ADDRESS
  i2c_lpc_wr_byte(0);

  //--- wr START + CONTROL again - for read start
  rc = i2c_lpc_ctrl(0xd0 | 0x01); //-- Now RD (RD/WI = 1)
  if(rc != I2C_NO_ERR)
    return rc;

  rc = i2c_lpc_rx_to_buf(buf,10);
  if(rc != I2C_NO_ERR)
    return rc;

  i2c_lpc_stop();     //---- Set STOP ---
//-------------------------------------
  return I2C_NO_ERR;
}
