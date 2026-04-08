/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2016
 *
 *    File name   : A9_timers.h
 *    Description : Cortex A9 Timers register definitions
 *
 *    History :
 *    1. Date        : January 2016
 *       Author      : Stoyan Choynev
 *       Description : Initial Revison
 *
 *    $Revision: 5936 $
 **************************************************************************/
#ifndef __A9_TIMERS_H
#define __A9_TIMERS_H

/** include files **/
#include <stdint.h>
#include "timers_config.h"

/** definitions **/
/* Private timer registers */
typedef struct
{
  volatile uint32_t LDR;
  volatile uint32_t CNTR;
  volatile uint32_t CTLR;
  volatile uint32_t ISR;
} PTMR_TypeDef;

#define PTMR_CTLR_TMR_ENABLE     (1ul<<0)
#define PTMR_CTLR_AUTO_RELOAD    (1ul<<1)
#define PTMR_CTLR_IRQ_ENABLE     (1ul<<2)
#define PTMR_CTLR_PRESCALER_MASK (0xFFul<<8)

#define PTMR_ISR_EVENT           (1ul<<0)

/* Watchdog timer registers */
typedef struct
{
  volatile uint32_t LDR;
  volatile uint32_t CNTR;
  volatile uint32_t CTLR;
  volatile uint32_t ISR;
  volatile uint32_t RSR;
  volatile uint32_t DBLR;
} WDTMR_TypeDef;

#define WDTMR_CTLR_TMR_ENABLE     (1ul<<0)
#define WDTMR_CTLR_AUTO_RELOAD    (1ul<<1)
#define WDTMR_CTLR_IRQ_ENABLE     (1ul<<2)
#define WDTMR_CTLR_WD_MODE        (1ul<<3)
#define WDTMR_CTLR_PRESCALER_MASK (0xFFul<<8)

#define WDTMR_ISR_EVENT           (1ul<<0)

#define WDTMR_RSR_RESET           (1ul<<0)

/* Global timer registers */
typedef struct
{
  volatile uint32_t CNTRL;
  volatile uint32_t CNTRH;
  volatile uint32_t CTLR;
  volatile uint32_t ISR;
  volatile uint32_t CVRL;
  volatile uint32_t CVRH;
  volatile uint32_t AIR;
} GTMR_TypeDef;

#define PTMR              ((PTMR_TypeDef*)PTMR_BASE_ADDR)   /**/
#define WDTMR             ((WDTMR_TypeDef*)WDTMR_BASE_ADDR) /**/
#define GTMR              ((GTMR_TypeDef*)GTMR_BASE_ADDR)   /**/

#endif /* __A9_TIMERS_H */
