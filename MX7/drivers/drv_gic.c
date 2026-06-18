/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2016
 *
 *    File name   : drv_gic.c
 *    Description : GICv1/v2 driver. Use only Group0 (Group1 always disabled)
 *
 *    History :
 *    1. Date        : July 2013
 *       Author      : Stoyan Choynev
 *       Description : Initial revision.
 *
 *    $Revision: 5936 $
 *
 **************************************************************************/
#define GIC_GLOBAL

/** include files **/
#include <stddef.h>
#include <stdint.h>
#include <intrinsics.h>
#include <stdlib.h>
#include "gic_v2.h"
#include "drv_gic.h"

/** local definitions **/

/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/
static void Def_Sgi_Isr(uint32_t call_id);

/** public data **/

/** private data **/
static Sgi_Isr_t * Sgi_Table[CPU_NUM];
static Ppi_Isr_t * Ppi_Table[CPU_NUM];
static Spi_Isr_t * Spi_Table;

static uint32_t int_num;

/** public functions **/
/*************************************************************************
 * Function Name: irq_handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: IRQ interrupt handler.
 *
 *************************************************************************/
__arm __nested __irq void IRQ_Handler(void)
{
  /* Read/save active interrupt id */
uint32_t iar_reg = GICC->IAR;
uint16_t int_id = iar_reg & 0x000003FF;
uint8_t  cpu_id;
  /* Check for spurious interrupt */
  if(int_num > int_id)
  {
    /* Enable interrupt nesting */
    __enable_irq();

    if(32 <= int_id)
    {/* Spi Interrupts */
      (*Spi_Table[int_id-32])();
    }
    else
    { /* Read MPIDR and get cpu */
      cpu_id = __MRC(15,0,0,0,5) & 0x3;
      if(16 <= int_id)
      {/* PPI Interrupts */
        (*(Ppi_Table[cpu_id])[int_id-16])();
      }
      else
      {/* SGI Interrups */
        (*(Sgi_Table[cpu_id])[int_id])(iar_reg>>10);
      }
    }

    /* Disable interrupt nesting */
    __disable_irq();

    /* End of interrupt */
    GICC->EOIR = iar_reg;
  }
}

/*************************************************************************
 * Function Name: GIC_Init(uint32_t gicc_bpr);
 * Parameters: uint32_t gicc_bpr - Binary Point value
 *
 * Return: none
 *
 * Description: Interrupt Controller init.
 *
 *************************************************************************/
void GIC_Init(uint32_t gicc_bpr)
{
uint32_t it_lines_num;
uint8_t  cpu_id;
  /* Disable interrupts from the Distributor */
  GICD->CTLR = 0;
  /* CPU Interface Disable interrupt signaling */
  GICC->CTLR = 0;
  /* Check for initialized SPI table */
  if(NULL == Spi_Table)
  {/* GIC_Init called for first time */
    it_lines_num = (GICD->TYPER&0x1F)+1;
    int_num = it_lines_num * 32;
    /* Alloc memory for SPI table */
    Spi_Table = (Spi_Isr_t *)calloc(int_num,sizeof(Spi_Isr_t));
    if(NULL == Spi_Table) while(1); /*Out of memory*/

    /* Disable all SPI interrupts */
    for(uint32_t i = 1; it_lines_num > i ; i++ )
    {
      GICD->ICENABLER[i] = 0xFFFFFFFF;
    }

    /* Clear all Interrupt Processor Targets Registers */
    for(uint32_t i = 32; (it_lines_num+1)*32 > i ; i++ )
    {
      GICD->ITARGETSR[i] = 0x00;
    }
  }

  /* Get CPU ID */
  cpu_id = __MRC(15,0,0,0,5) & 0x3;

  /* Check for initialized PPI table */
  if(NULL == Ppi_Table[cpu_id])
  {
    /* Alloc memory for PPI table */
    Ppi_Table[cpu_id] = (Ppi_Isr_t *)calloc(16,sizeof(Ppi_Isr_t));
    if(NULL == Ppi_Table[cpu_id]) while(1); /*Out of memory*/

    GICD->ICENABLER[0] = 0xFFFF0000;
  }

  /* Check for initialized SGI table */
  if(NULL == Sgi_Table[cpu_id])
  {
    /* Alloc memory for SGI table */
    Sgi_Table[cpu_id] = (Sgi_Isr_t *)calloc(16,sizeof(Sgi_Isr_t));
    if(NULL == Sgi_Table[cpu_id]) while(1); /*Out of memory*/

    Sgi_Isr_t * tbl = Sgi_Table[cpu_id];
    for(uint32_t i = 0; 16 > i ; i++)
    {
      tbl[i] = Def_Sgi_Isr;
    }
  }

  /* Do not filter any interrupts */
  GICC->PMR = 0xFF;
  /* Set Binary point */
  GICC->BPR = gicc_bpr;
  /* CPU Interface Enable interrupt signaling
    Use IRQ signal */
  GICC->CTLR = 1;
  /* Enable interrupts from the Distributor */
  GICD->CTLR = 1;
}

/*************************************************************************
 * Function Name: GIC_SgiIntInstall(Sgi_Isr_t isr,uint32_t int_id,uint8_t priority);
 * Parameters: Sgi_Isr_t isr     - SGI Interrupt Service Routine
 *             uint32_t int_id   - Interrupt id
 *             uint32_t priority - Priority level from 0(high) to 255(low)
 *
 * Return: int32_t 0 - O.K.
 *                -1 - Error
 *
 * Description: Install SGI Interrupt. Interrupt id 0-15.
 *
 *************************************************************************/
int32_t GIC_SgiIntInstall(Sgi_Isr_t isr,uint32_t int_id, uint8_t priority)
{
int32_t ret = -1;
uint8_t  cpu_id;
Sgi_Isr_t * tbl;
  if(16 > int_id)
  {
    /* Get CPU ID */
    cpu_id = __MRC(15,0,0,0,5) & 0x3;

    tbl = Sgi_Table[cpu_id];
    /* Check for initialized SGI table */
    if(NULL != tbl)
    {
      GICC->CTLR = 0;
      /* Install ISR */
      tbl[int_id] = isr;
      /* Set interrupt priority */
      GICD->IPRIORITYR[int_id] = priority;
      GICC->CTLR = 1;
      ret = 0;
    }
  }
  return ret;
}

/*************************************************************************
 * Function Name: int32_t GIC_PpiIntInstall(Ppi_Isr_t isr,uint32_t int_id,
 *                                          uint8_t priority);
 * Parameters: Sgi_Isr_t isr     - PPI Interrupt Service Routine
 *             uint32_t int_id   - Interrupt id
 *             uint32_t priority - Priority level from 0(high) to 255(low)
 *
 * Return: int32_t 0 - O.K.
 *                -1 - Error
 *
 * Description: Install PPI Interrupt. Interrupt id 16-31.
 *
 *************************************************************************/
int32_t GIC_PpiIntInstall(Ppi_Isr_t isr,uint32_t int_id, uint8_t priority)
{
int32_t ret = -1;
uint8_t  cpu_id;
Ppi_Isr_t * tbl;

  if((16 <= int_id) && (32 > int_id))
  {
    /* Get CPU ID */
    cpu_id = __MRC(15,0,0,0,5) & 0x3;

    tbl = Ppi_Table[cpu_id];
    /* Check for initialized SGI table */
    if(NULL != tbl)
    {
      GICC->CTLR = 0;
      /* Install ISR */
      tbl[int_id-16] = isr;
      /* Set interrupt priority */
      GICD->IPRIORITYR[int_id] = priority;
      GICC->CTLR = 1;
      ret = 0;
    }
  }
  return ret;
}

/*************************************************************************
 * Function Name: int32_t GIC_SpiIntInstall(Spi_Isr_t isr,uint32_t int_id,
 *                                          uint8_t priority, uint8_t cfg)
 * Parameters: Sgi_Isr_t isr     - PPI Interrupt Service Routine
 *             uint32_t int_id   - Interrupt id
 *             uint32_t priority - Priority level from 0(high) to 255(low)
 *             uint8_t cfg       - Interrupt config: 0 - level-sensitive
 *                                                   1 - edge-triggered
 *
 * Return: int32_t 0 - O.K.
 *                -1 - Error
 *
 * Description: Install SPI Interrupt. Interrupt id 32-INT_NUM.
 *
 *************************************************************************/
int32_t GIC_SpiIntInstall(Spi_Isr_t isr,uint32_t int_id, uint8_t priority, uint8_t cfg)
{
int32_t ret = -1;
Spi_Isr_t * tbl;
uint8_t  cpu_mask;
uint32_t cfg_reg_idx;
uint32_t cfg_bits_shift;

  if((32 <= int_id) && (int_num > int_id))
  {

    tbl = Spi_Table;
    /* Check for initialized SGI table */
    if(NULL != tbl)
    {
      cpu_mask = 1<<(__MRC(15,0,0,0,5) & 0x3);
      GICC->CTLR = 0;
      /* Check if the interrupt is free */
      if(!((GICD->TYPER>>5)&0x7) || !(GICD->ITARGETSR[int_id]))
      { /* Set target cpu */
        GICD->ITARGETSR[int_id] = cpu_mask;

        /* Install ISR */
        tbl[int_id-32] = isr;
        /* Set interrupt priority */
        GICD->IPRIORITYR[int_id] = priority;
        /* Interrupt config */
        cfg_reg_idx = int_id/16;
        cfg_bits_shift = (int_id%16)*2;
        if(cfg)
        {
          GICD->ICFGR[cfg_reg_idx] |= (0x2UL)<<cfg_bits_shift;
        }
        else
        {
          GICD->ICFGR[cfg_reg_idx] &= ~((0x2UL)<<cfg_bits_shift);
        }
        ret = 0;
      }
      GICC->CTLR = 1;
    }
  }
  return ret;
}

/*************************************************************************
 * Function Name: int32_t GIC_IntDeinstall(uint32_t int_id)
 *
 * Parameters: uint32_t int_id - Interrupt id
 *
 * Return: int32_t 0 - O.K.
 *                -1 - Error
 *
 * Description: DeInstall interrupt.
 *
 *************************************************************************/
int32_t GIC_IntDeInstall(uint32_t int_id)
{
int32_t ret = -1;
uint8_t  cpu_mask;

    cpu_mask = 1<<(__MRC(15,0,0,0,5) & 0x3);
    GICC->CTLR = 0;
    /* Check if the interrupt target is the cpu */
    if(!((GICD->TYPER>>5)&0x7) || (GICD->ITARGETSR[int_id] == cpu_mask))
    { /* Clear target cpu */
      GICD->ITARGETSR[int_id] = 0;
      ret = 0;
    }
    GICC->CTLR = 1;

  return ret;
}

/*************************************************************************
 * int32_t GIC_IntEnable(uint32_t int_id, uint8_t enable)
 *
 * Parameters: uint32_t int_id   - Interrupt id
 *             uint32_t priority - Priority level from 0(high) to 255(low)
 *             uint8_t enable    -  0 - disable interrupt
 *                                  1 - enable interrupt
 *
 * Return: int32_t 0 - O.K.
 *                -1 - Error
 *
 * Description: Install PPI Interrupt. Interrupt id 16-31.
 *
 *************************************************************************/
int32_t GIC_IntEnable(uint32_t int_id, uint8_t enable)
{
int32_t ret = -1;
uint32_t reg_idx;
uint32_t bits_shift;
uint8_t  cpu_mask;

  if(int_num > int_id)
  {
    cpu_mask = 1<<(__MRC(15,0,0,0,5) & 0x3);

    if(!((GICD->TYPER>>5)&0x7) || (GICD->ITARGETSR[int_id] == cpu_mask))
    {
      reg_idx = int_id/32;
      bits_shift = int_id%32;

      if(enable)
      {
        /* Interrupt enable */
        GICD->ICPENDR[reg_idx] |= (0x1UL)<<bits_shift;
        GICD->ISENABLER[reg_idx] |= (0x1UL)<<bits_shift;
      }
      else
      {
        /* Interrupt disable */
        GICD->ICENABLER[reg_idx] |= (0x1UL)<<bits_shift;
      }
      ret = 0;
    }
  }

  return ret;
}

/** private functions **/
/*************************************************************************
 * Function Name: Def_Sgi_Isr
 * Parameters: uint32_t call_id - Caller cpu id
 *
 * Return: none
 *
 * Description: Default SGI ISR. Do nothing just return. Used because
 *              SGIs are always enable.
 *
 *************************************************************************/
static void Def_Sgi_Isr(uint32_t call_id)
{
}
