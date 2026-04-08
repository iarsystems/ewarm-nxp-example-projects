/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2016
 *
 *    File name   : drv_gic.c
 *    Description : GICv1/v2 driver.
 *
 *    History :
 *    1. Date        : July 2013
 *       Author      : Stoyan Choynev
 *       Description : Initial revision.
 *
 *    $Revision: 5855 $
 *
 **************************************************************************/

#ifndef __DRV_GIC_H
#define __DRV_GIC_H

/** include files **/
#include <stdint.h>
#include "gic_v2.h"

/** definitions **/
typedef void (*Sgi_Isr_t)(uint32_t call_id);
typedef void (*Ppi_Isr_t)(void);
typedef void (*Spi_Isr_t)(void);

/** public data **/


/** public functions **/
/*************************************************************************
 * Function Name: GIC_Init(uint32_t gicc_bpr);
 * Parameters: uint32_t gicc_bpr - Binary Point value
 *
 * Return: none
 *
 * Description: Interrupt Controller init
 *
 *************************************************************************/
void GIC_Init(uint32_t gicc_bpr);

/*************************************************************************
 * Function Name: GIC_SgiIntInstall(Sgi_Isr_t isr,uint32_t int_id,uint8_t priority);
 * Parameters: Sgi_Isr_t isr     -   SGI Interrup Service Rotune
 *             uint32_t int_id   - interrupt id
 *             uint32_t priority - Priority level from 0(high) to 255(low)
 *
 * Return: int32_t 0 - O.K.
 *                -1 - Error
 *
 * Description: Install SGI Interrupt. Interrupt id 0-15
 *
 *************************************************************************/
int32_t GIC_SgiIntInstall(Sgi_Isr_t isr,uint32_t int_id, uint8_t priority);

/*************************************************************************
 * Function Name: int32_t GIC_PpiIntInstall(Ppi_Isr_t isr,uint32_t int_id,
 *                                          uint8_t priority);
 * Parameters: Sgi_Isr_t isr     - PPI Interrup Service Rotune
 *             uint32_t int_id   - interrupt id
 *             uint32_t priority - Priority level from 0(high) to 255(low)
 *
 * Return: int32_t 0 - O.K.
 *                -1 - Error
 *
 * Description: Install PPI Interrupt. Interrupt id 16-31
 *
 *************************************************************************/
int32_t GIC_PpiIntInstall(Ppi_Isr_t isr,uint32_t int_id, uint8_t priority);

/*************************************************************************
 * Function Name: int32_t GIC_SpiIntInstall(Spi_Isr_t isr,uint32_t int_id,
 *                                          uint8_t priority, uint8_t cfg)
 * Parameters: Sgi_Isr_t isr     - PPI Interrup Service Rotune
 *             uint32_t int_id   - interrupt id
 *             uint32_t priority - Priority level from 0(high) to 255(low)
 *             uint8_t cfg       - interrupt config: 0 - level-sensitive
 *                                 1                   - edge-triggered
 *
 * Return: int32_t 0 - O.K.
 *                -1 - Error
 *
 * Description: Install SPI Interrupt. Interrupt id 32-INT_NUM
 *
 *************************************************************************/
int32_t GIC_SpiIntInstall(Spi_Isr_t isr,uint32_t int_id, uint8_t priority, uint8_t cfg);

/*************************************************************************
 * Function Name: int32_t GIC_IntDeinstall(uint32_t int_id)
 *
 * Parameters: uint32_t int_id   - interrupt id
 *
 * Return: int32_t 0 - O.K.
 *                -1 - Error
 *
 * Description: DeInstall interrupt
 *
 *************************************************************************/
int32_t GIC_IntDeInstall(uint32_t int_id);

/*************************************************************************
 * int32_t GIC_IntEnable(uint32_t int_id, uint8_t enable)
 *
 * Parameters: uint32_t int_id   - interrupt id
 *             uint32_t priority - Priority level from 0(high) to 255(low)
 *             uint8_t enable    -  0 - disable interrup
 *                                  1 - enable interrupt
 *
 * Return: int32_t 0 - O.K.
 *                -1 - Error
 *
 * Description: Install PPI Interrupt. Interrupt id 16-31
 *
 *************************************************************************/
int32_t GIC_IntEnable(uint32_t int_id, uint8_t enable);

#endif // __DRV_GIC_H
