/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : drv_intc.h
 *    Description :  lpc3250 Interrupt Controller Driver header file
 *
 *    History :
 *    1. Date        : September, 2010
 *       Author      : Stoyan Choynev
 *       Description : Initial Revison
 *
 *    $Revision: 4206 $
 **************************************************************************/
#ifndef __DRV_INTC_H
#define __DRV_INTC_H

/** include files **/

/** definitions **/
typedef enum IRQ_POLARITY {
  LOW,
  HIGH
} IRQ_POLARITY_t;

typedef enum IRQ_SENS{
  LEVEL,
  EDGE
} IRQ_SENS_t;


typedef enum IRQ_TYPE{
  TYPE_IRQ,
  TYPE_FQI
} IRQ_TYPE_t;

typedef enum IRQ_ENABLE{
  DISABLE_IRQ,
  ENABLE_IRQ
} IRQ_ENABLE_t;
/** default settings **/

/** public data **/

/** public functions **/
/*************************************************************************
 * Function Name: IRQ_Handler
 * Parameters: None  
 *
 * Return: None
 *
 * Description: Nested IRQ handler. 
 *
 *************************************************************************/
__arm __nested __irq void IRQ_Handler(void);
/*************************************************************************
 * Function Name: FIQ_Handler
 * Parameters: None  
 *
 * Return: None
 *
 * Description: FIQ handler. 
 *
 *************************************************************************/
__arm __fiq void FIQ_Handler(void);
/*************************************************************************
 * Function Name: INTC_Init
 * Parameters:None
 *
 * Return: None
 *
 * Description: Initialize LPC3250 Interrupt Controller
 *
 *************************************************************************/
void INTC_Init(void);
/*************************************************************************
 * Function Name: MIC_IRQInstall
 * Parameters:VoidFpnt_t ISR - Interrup Service Rotune
 *            Int32U IRQIndex - IRQ Number
 *            IRQ_POLARITY_t Polarity -  0 - Low level or falling edge,
 *                                       1 - High level or rising edge
 *            IRQ_SENS_T Sens         -  0 - level sensitive, 
 *                                       1 - edge sensitive 
 *            IRQ_TYPE_t Type         -  0 - IRQ,
 *                                       1 - FIQ   
 * Return: None
 *
 * Description: Install IRQ
 *
 *************************************************************************/
void MIC_IRQInstall(VoidFpnt_t ISR, Int32U IRQIndex,
                     IRQ_POLARITY_t Polarity, IRQ_SENS_t Sens,
                     IRQ_TYPE_t Type);
/*************************************************************************
 * Function Name: MIC_IntEnable
 * Parameters:Int32U IRQIndex - IRQ Number
 *            IRQ_ENABLE_t Enable - 0 - Disable, 1- Enable)
 * Return: None
 *
 * Description: Enable or Disable IRQ
 *
 *************************************************************************/
void MIC_IntEnable(Int32U IRQIndex, IRQ_ENABLE_t Enable);
/*************************************************************************
 * Function Name: SIC1_IRQInstall
 * Parameters:VoidFpnt_t ISR - Interrup Service Rotune
 *            Int32U IRQIndex - IRQ Number
 *            IRQ_POLARITY_t Polarity -  0 - Low level or falling edge,
 *                                       1 - High level or rising edge
 *            IRQ_SENS_T Sens         -  0 - level sensitive, 
 *                                       1 - edge sensitive 
 *            IRQ_TYPE_t Type         -  0 - IRQ,
 *                                       1 - FIQ   
 * Return: None
 *
 * Description: Install IRQ
 *
 *************************************************************************/
void SIC1_IRQInstall(VoidFpnt_t ISR, Int32U IRQIndex,
                     IRQ_POLARITY_t Polarity, IRQ_SENS_t Sens,
                     IRQ_TYPE_t Type);
/*************************************************************************
 * Function Name: SIC1_IntEnable
 * Parameters:Int32U IRQIndex - IRQ Number
 *            IRQ_ENABLE_t Enable - 0 - Disable, 1- Enable)
 * Return: None
 *
 * Description: Enable or Disable IRQ
 *
 *************************************************************************/
void SIC1_IntEnable(Int32U IRQIndex, IRQ_ENABLE_t Enable);
/*************************************************************************
 * Function Name: SIC2_IRQInstall
 * Parameters:VoidFpnt_t ISR - Interrup Service Rotune
 *            Int32U IRQIndex - IRQ Number
 *            IRQ_POLARITY_t Polarity -  0 - Low level or falling edge,
 *                                       1 - High level or rising edge
 *            IRQ_SENS_T Sens         -  0 - level sensitive, 
 *                                       1 - edge sensitive 
 *            IRQ_TYPE_t Type         -  0 - IRQ,
 *                                       1 - FIQ   
 * Return: None
 *
 * Description: Install IRQ
 *
 *************************************************************************/
void SIC2_IRQInstall(VoidFpnt_t ISR, Int32U IRQIndex,
                     IRQ_POLARITY_t Polarity, IRQ_SENS_t Sens,
                     IRQ_TYPE_t Type);
/*************************************************************************
 * Function Name: SIC2_IntEnable
 * Parameters:Int32U IRQIndex - IRQ Number
 *            IRQ_ENABLE_t Enable - 0 - Disable, 1- Enable)
 * Return: None
 *
 * Description: Enable or Disable IRQ
 *
 *************************************************************************/
void SIC2_IntEnable(Int32U IRQIndex, IRQ_ENABLE_t Enable);
#endif /* __DRV_INTC_H */
