/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2005 Embedded Artists AB
 *
 *****************************************************************************/

#ifndef _ETHAPI_H_
#define _ETHAPI_H_

#include <lpc_types.h>

#define INBUF_LEN 1500

/*****************************************************************************
 *
 * Description:
 *    Initialize the ethernet driver
 *
 * Params:
 *    [in] pEthAddr - the Ethernet (MAC) address. Must be 6 bytes long.
 *
 ****************************************************************************/
BOOL_32 ethIf_init(UNS_8* pEthAddr);

/*****************************************************************************
 *
 * Description:
 *    Send an ethernet packet. 
 *
 * Params:
 *    [in] pData - the data to send
 *    [in] len   - length of the data to send
 *
 ****************************************************************************/
void ethIf_send(UNS_8* pData, 
                UNS_16 len);

/*****************************************************************************
 *
 * Description:
 *    Poll the driver for incoming packets. 
 *
 * Params:
 *    [in/out] pBuf - allocated buffer to hold the incoming packet
 *    [in]     len  - length of the buffer
 *
 * Returns:
 *    Length of the received packet.
 *
 ****************************************************************************/
UNS_16 ethIf_poll(UNS_8* pBuf, 
                  UNS_16 len);


void
ethInput(UNS_8* pBuf, 
         UNS_16 len);

#endif
