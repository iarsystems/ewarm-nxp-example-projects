/*****************************************************************************\
*              efs - General purpose Embedded Filesystem library              *
*          --------------------- -----------------------------------          *
*                                                                             *
* Filename : sd.c                                                             *
* Revision : Initial developement                                             *
* Description : This file contains the functions needed to use efs for        *
*               accessing files on an SD-card.                                *
*                                                                             *
* This program is free software; you can redistribute it and/or               *
* modify it under the terms of the GNU General Public License                 *
* as published by the Free Software Foundation; version 2                     *
* of the License.                                                             *
                                                                              *
* This program is distributed in the hope that it will be useful,             *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
* GNU General Public License for more details.                                *
*                                                                             *
* As a special exception, if other files instantiate templates or             *
* use macros or inline functions from this file, or you compile this          *
* file and link it with other works to produce a work based on this file,     *
* this file does not by itself cause the resulting work to be covered         *
* by the GNU General Public License. However the source code for this         *
* file must still be made available in accordance with section (3) of         *
* the GNU General Public License.                                             *
*                                                                             *
* This exception does not invalidate any other reasons why a work based       *
* on this file might be covered by the GNU General Public License.            *
*                                                                             *
*                                                    (c)2006 Lennart Yseboodt *
*                                                    (c)2006 Michael De Nil   *
\*****************************************************************************/

/*****************************************************************************/
#include "sd.h"
#include "type.h"
/*****************************************************************************/
esint8 sd_Init()
{
  esint16 i;
  euint8 resp;

  /* Try to send reset command up to 100 times */
  i=100;
  do{
    sd_Command(0, 0, 0);
    resp=sd_Resp8b();
  }
  while(resp!=1 && i--);

  if(resp!=1){
    if(resp==0xff){
      return(-1);
    }
    else{
      sd_Resp8bError(resp);
      return(-2);
    }
  }

  /* Wait till card is ready initialising (returns 0 on CMD1) */
  /* Try up to 32000 times. */
  i=32000;
  do{
    sd_Command(1, 0, 0);

    resp=sd_Resp8b();
    if(resp!=0)
      sd_Resp8bError(resp);
  }
  while(resp==1 && i--);

  if(resp!=0){
    sd_Resp8bError(resp);
    return(-3);
  }

  return(0);
}
esint8 sd_State()
{
  eint16 value;

  sd_Command(13, 0, 0);
  value=sd_Resp16b();

  switch(value)
  {
    case 0x000:
      return(1);
//      break;
    case 0x0001:
//      DBG((TXT("Card is Locked.\n")));
      break;
    case 0x0002:
//      DBG((TXT("WP Erase Skip, Lock/Unlock Cmd Failed.\n")));
      break;
    case 0x0004:
//      DBG((TXT("General / Unknown error -- card broken?.\n")));
      break;
    case 0x0008:
//      DBG((TXT("Internal card controller error.\n")));
      break;
    case 0x0010:
//      DBG((TXT("Card internal ECC was applied, but failed to correct the data.\n")));
      break;
    case 0x0020:
//      DBG((TXT("Write protect violation.\n")));
      break;
    case 0x0040:
//      DBG((TXT("An invalid selection, sectors for erase.\n")));
      break;
    case 0x0080:
//      DBG((TXT("Out of Range, CSD_Overwrite.\n")));
      break;
    default:
      if(value>0x00FF)
        sd_Resp8bError((euint8) (value>>8));
      else
//        DBG((TXT("Unknown error: 0x%x (see SanDisk docs p5-14).\n"),value));
      break;
  }
  return(-1);
}


/*****************************************************************************/

void sd_Command(euint8 cmd, euint16 paramx, euint16 paramy)
{
  if_spiSend(0xff);

  if_spiSend(0x40 | cmd);
  if_spiSend((euint8) (paramx >> 8)); /* MSB of parameter x */
  if_spiSend((euint8) (paramx)); /* LSB of parameter x */
  if_spiSend((euint8) (paramy >> 8)); /* MSB of parameter y */
  if_spiSend((euint8) (paramy)); /* LSB of parameter y */

  if_spiSend(0x95); /* Checksum (should be only valid for first command (0) */

  if_spiSend(0xff); /* eat empty command - response */
}
/*****************************************************************************/

euint8 sd_Resp8b()
{
  euint8 i;
  euint8 resp;

  /* Respone will come after 1 - 8 pings */
  for(i=0;i<8;i++){
    resp = if_spiSend(0xff);
    if(resp != 0xff)
      return(resp);
  }

  return(resp);
}
/*****************************************************************************/

euint16 sd_Resp16b()
{
  euint16 resp;

  resp = ( sd_Resp8b() << 8 ) & 0xff00;
  resp |= if_spiSend(0xff);

  return(resp);
}
/*****************************************************************************/


void sd_Resp8bError(euint8 value)
{
  switch(value)
  {
    case 0x40:
//      DBG((TXT("Argument out of bounds.\n")));
      break;
    case 0x20:
//      DBG((TXT("Address out of bounds.\n")));
      break;
    case 0x10:
//      DBG((TXT("Error during erase sequence.\n")));
      break;
    case 0x08:
//      DBG((TXT("CRC failed.\n")));
      break;
    case 0x04:
//      DBG((TXT("Illegal command.\n")));
      break;
    case 0x02:
//      DBG((TXT("Erase reset (see SanDisk docs p5-13).\n")));
      break;
    case 0x01:
//      DBG((TXT("Card is initialising.\n")));
      break;
    default:
//      DBG((TXT("Unknown error 0x%x (see SanDisk docs p5-13).\n"),value));
      break;
  }
}
/*****************************************************************************/
/*****************************************************************************/

/* ****************************************************************************
 * WAIT ?? -- FIXME
 * CMDWRITE
 * WAIT
 * CARD RESP
 * WAIT
 * DATA BLOCK OUT
 *      START BLOCK
 *      DATA
 *      CHKS (2B)
 * BUSY...
 */

int8_t sd_writeSector(uint32_t address, uint8_t* buf)
{
  uint32_t place;
  uint16_t i;
  uint16_t t=0;

  /*DBG((TXT("Trying to write %u to sector %u.\n"),(void *)&buf,address));*/
  place=512*address;
  sd_Command(CMDWRITE, (uint16_t) (place >> 16), (uint16_t) place);

  sd_Resp8b(); /* Card response */

  if_spiSend(0xfe); /* Start block */
  for(i=0;i<512;i++)
    if_spiSend(buf[i]); /* Send data */
  if_spiSend(0xff); /* Checksum part 1 */
  if_spiSend(0xff); /* Checksum part 2 */

  if_spiSend(0xff);

  while(if_spiSend(0xff)!=0xff){
    t++;
    /* Removed NOP */
  }
  /*DBG((TXT("Nopp'ed %u times.\n"),t));*/

  return(0);
}
/*****************************************************************************/

/* ****************************************************************************
 * WAIT ?? -- FIXME
 * CMDCMD
 * WAIT
 * CARD RESP
 * WAIT
 * DATA BLOCK IN
 *    START BLOCK
 *    DATA
 *    CHKS (2B)
 */

int8_t sd_readSector(uint32_t address, uint8_t* buf, uint16_t len)
{
  uint8_t cardresp;
  uint8_t firstblock;
  uint8_t c;
  uint16_t fb_timeout=0xffff;
  uint32_t i;
  uint32_t place;

  /*DBG((TXT("sd_readSector::Trying to read sector %u and store it at %p.\n"),address,&buf[0]));*/
  place=512*address;
  sd_Command(CMDREAD, (uint16_t) (place >> 16), (uint16_t) place);

  cardresp=sd_Resp8b(); /* Card response */

  /* Wait for startblock */
  do
    firstblock=sd_Resp8b();
  while(firstblock==0xff && fb_timeout--);

  if(cardresp!=0x00 || firstblock!=0xfe){
    sd_Resp8bError(firstblock);
    return(-1);
  }

  for(i=0;i<512;i++){
    c = if_spiSend(0xff);
    if(i<len)
      buf[i] = c;
  }

  /* Checksum (2 byte) - ignore for now */
  if_spiSend(0xff);
  if_spiSend(0xff);

  return(0);
}
/*****************************************************************************/

/* ****************************************************************************
 calculates size of card from CSD
 (extension by Martin Thomas, inspired by code from Holger Klabunde)
 */
int8_t sd_getDriveSize(uint32_t* drive_size )
{
  uint8_t cardresp, i, by;
  uint8_t iob[16];
  uint16_t c_size, c_size_mult, read_bl_len;

  sd_Command(CMDREADCSD, 0, 0);

  do {
    cardresp = sd_Resp8b();
  } while ( cardresp != 0xFE );

  //DBG((TXT("CSD:")));
  for( i=0; i<16; i++) {
    iob[i] = sd_Resp8b();
    //DBG((TXT(" %02x"), iob[i]));
  }
  //DBG((TXT("\n")));

  if_spiSend(0xff);
  if_spiSend(0xff);

  c_size = iob[6] & 0x03; // bits 1..0
  c_size <<= 10;
  c_size += (uint16_t)iob[7]<<2;
  c_size += iob[8]>>6;

  by= iob[5] & 0x0F;
  read_bl_len = 1;
  read_bl_len <<= by;

  by=iob[9] & 0x03;
  by <<= 1;
  by += iob[10] >> 7;

  c_size_mult = 1;
  c_size_mult <<= (2+by);

  *drive_size = (uint32_t)(c_size+1) * (uint32_t)c_size_mult * (uint32_t)read_bl_len;

  return 0;
}

/*****************************************************************************/


