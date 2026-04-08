/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : ea_lpc314x_if.h
 *    Description : This file is dervied from the ``ethernetif.h'' skeleton
 *                  Ethernet network interface driver for lwIP.
 *
 *    History :
 *    1. Date        : 27.8.2009
 *       Author      : Stanimir Bonev
 *       Description : initial revision.
 *    2. Date        : 2.10.2009
 *       Author      : Stanimir Bonev
 *       Description : Adapt for an Embedded Artsists LPC314x board
 *
 *    $Revision: 28 $
 **************************************************************************/

/**
 * @file - stellarisif.h
 * Prototypes for Stellaris Ethernet interface.
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */


#ifndef __EA_LPC314X_IF_H__
#define __EA_LPC314X_IF_H__

extern int ea_lpc314xif_input(struct netif *netif);
extern err_t ea_lpc314xif_init(struct netif *netif);
extern void ea_lpc314xif_interrupt(struct netif *netif);

#if NETIF_DEBUG
void ea_lpc314xif_debug_print(struct pbuf *p);
#else
#define ea_lpc314xif_debug_print(p)
#endif /* NETIF_DEBUG */

#endif // __EA_LPC314X_IF_H__
