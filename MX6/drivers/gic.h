/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2013
 *
 *    File name   : gic.h
 *    Description : ARM GICv1 register definitions
 *
 *    History :
 *    1. Date        : July 2013
 *       Author      : Stoyan Choynev
 *       Description : Initial Revison
 *
 *    $Revision: 5855 $
 **************************************************************************/
#ifndef __GIC_H
#define __GIC_H

/** include files **/
#include <stdint.h>
#include "gic_config.h"

/** definitions **/
/* CPU interface register */
typedef struct
{
  volatile uint32_t CTLR;
  volatile uint32_t PMR;
  volatile uint32_t BPR;
  volatile uint32_t IAR;
  volatile uint32_t EOIR;
  volatile uint32_t RPR;
  volatile uint32_t HPPIR;
  volatile uint32_t ABPR;
           uint8_t  RESERVED0[220];
  volatile uint32_t IIDR;
} GICC_TypeDef;

/* Distributor register register */
typedef struct
{
  volatile uint32_t CTLR;
  volatile uint32_t TYPER;
  volatile uint32_t IIDR;
           uint8_t  RESERVED0[116];
  volatile uint32_t IGROUPR[32];
  volatile uint32_t ISENABLER[32];
  volatile uint32_t ICENABLER[32];
  volatile uint32_t ISPENDR[32];
  volatile uint32_t ICPENDR[32];
  volatile uint32_t IACTIVER[32];
           uint8_t  RESERVED1[128];
  volatile uint8_t  IPRIORITYR[1020];
           uint8_t  RESERVED2[4];
  volatile uint8_t  ITARGETSR[1020];
           uint8_t  RESERVED3[4];
  volatile uint32_t ICFGR[64];
           uint8_t  RESERVED4[512];
  volatile uint32_t SGIR;
} GICD_TypeDef;

#define GICC              ((GICC_TypeDef*)GICC_BASE_ADDR) /**/
#define GICD              ((GICD_TypeDef*)GICD_BASE_ADDR) /**/

#endif /* __GIC_H */
