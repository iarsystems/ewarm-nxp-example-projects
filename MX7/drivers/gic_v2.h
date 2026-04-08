/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2016
 *
 *    File name   : gic_v2.h
 *    Description : ARM GICv2 register definitions
 *
 *    History :
 *    1. Date        : July 2013
 *       Author      : Stoyan Choynev
 *       Description : Initial Revison
 *    2. Date        : July 2016
 *       Author      : Atanas Uzunov
 *       Description : Added GICv2 registers
 *
 *    $Revision: 5855 $
 **************************************************************************/
#ifndef __GIC_V2_H
#define __GIC_V2_H

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
  volatile uint32_t AIAR;
  volatile uint32_t AEOIR;
  volatile uint32_t AHPPIR;
           uint8_t  RESERVED0[164];
  volatile uint32_t APR[4];
  volatile uint32_t NSAPR[4];
           uint8_t  RESERVED1[12];
  volatile uint32_t IIDR;
           uint8_t  RESERVED2[3840];
  volatile uint32_t DIR;
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
  volatile uint32_t ISACTIVER[32];
  volatile uint32_t ICACTIVER[32];
  volatile uint8_t  IPRIORITYR[1020];
           uint8_t  RESERVED1[4];
  volatile uint8_t  ITARGETSR[1020];
           uint8_t  RESERVED2[4];
  volatile uint32_t ICFGR[64];
           uint8_t  RESERVED3[256];
  volatile uint32_t NSACR[64];
  volatile uint32_t SGIR;
           uint8_t  RESERVED4[12];
  volatile uint32_t CPENDSGIR[4];
  volatile uint32_t SPENDSGIR[4];
} GICD_TypeDef;

#define GICC              ((GICC_TypeDef*)GICC_BASE_ADDR) /**/
#define GICD              ((GICD_TypeDef*)GICD_BASE_ADDR) /**/

#endif /* __GIC_V2_H */
