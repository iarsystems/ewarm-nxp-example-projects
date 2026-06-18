/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2009 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 **************************************************************************//*!
 *
 * @file user_config.h
 *
 * @author
 *
 * @version
 *
 * @date May-28-2009
 *
 * @brief The file contains User Modifiable Macros for PHDC Application
 *
 *****************************************************************************/
#ifndef _USER_CONFIG_H
#define _USER_CONFIG_H

#if ((defined __MK_xxx_H__)||(defined MCU_mcf51jf128))
	#define  KEY_PRESS_SIM_TMR_INTERVAL	  (1000)		/* 2s between simulated key press events */
#else
#ifdef __MCF52277_H__
#define  HIGH_SPEED_DEVICE            (1) 
#define  BUTTON_PRESS_SIMULATION      (1)
#define  KEY_PRESS_SIM_TMR_INTERVAL	  (2000)		/* 2s between simulated key press events */
#else
#define  HIGH_SPEED_DEVICE            (0)
#endif
#endif

/* User Defined MACRO to set number of Timer Objects */
#define MAX_TIMER_OBJECTS		    15

#define MAX_SERIAL_RECV_BUFFER      64
#define EVB51JM128                  1

#if MAX_TIMER_OBJECTS
/* When Enabled Timer Callback is invoked with an argument */

#define TIMER_CALLBACK_ARG 

#endif

#endif /* _USER_CONFIG_H */

