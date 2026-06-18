/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : drv_intc.c
 *    Description :  lpc3250 Interrupt Controller Driver
 *
 *    History :
 *    1. Date        : September, 2010
 *       Author      : Stoyan Choynev
 *       Description : Initial Revison
 *
 *    $Revision: 28 $
 **************************************************************************/

/** include files **/
#include <NXP\iolpc3250.h>
#include <assert.h>
#include "arm_comm.h"
#include "drv_intc.h"
/** local definitions **/
#define MIC_APR_USR_MASK     0x00000000
#define MIC_ATR_USR_MASK     0x00000000
#define SIC1_APR_USR_MASK    0x00000050
#define SIC1_ATR_USR_MASK    0x000001D0
#define SIC2_APR_USR_MASK    0x1FC08F3F
#define SIC2_ATR_USR_MASK    0x1FC08F3F
/** default settings **/

/** external functions **/

/** external data **/
extern Int32U  __vector[];
extern Int32U  __vector_end[];

/** internal functions **/
void IRQ_Void(void);
void Sub1IRQ_Handler(void);
void Sub1FIQ_Handler(void);
void Sub2IRQ_Handler(void);
void Sub2FIQ_Handler(void);
/** public data **/

/** private data **/
static VoidFpnt_t MainIntTbl[32];
static VoidFpnt_t Sub1IntTbl[32];
static VoidFpnt_t Sub2IntTbl[32];
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
__arm __nested __irq void IRQ_Handler(void)
{
Int32U mask, i;
Int32U enable, pendirq;
  /*Save current Enabled IRQ*/
  enable = MIC_ER ;
  enable &= ~(MIC_ITR);
  /*find pending interrupt*/
  pendirq = enable & MIC_SR;
  for(mask = (1<<29),i=29;mask; mask>>=1, i--)
  {
    if(mask&pendirq)
    {
      /*mask all low priority interrupts*/
      Int32U disable = (~(mask-1))|MIC_ITR;
      MIC_ER &= disable;
      /*if Int is not from SIC1 of SIC2*/
      if(1<i)
      {
        /*Mask Current interrupt*/
        MIC_ER &= ~mask;
        /*enable interrupts*/
       __enable_irq();
      }
      /*Call Interrupt Service Routine*/
      MainIntTbl[i]();
      /*disable interrupts*/
      __disable_irq();
      /*clear Current interrupt*/
      MIC_RSR = mask;
      /*enable interrupts*/
      MIC_ER |= enable;
      break;
    }
  }
}
/*************************************************************************
 * Function Name: FIQ_Handler
 * Parameters: None  
 *
 * Return: None
 *
 * Description: FIQ handler. 
 *
 *************************************************************************/
__arm __fiq void FIQ_Handler(void)
{
Int32U mask, i;
Int32U pendirq;
  /*find pending interrupt*/
  pendirq = MIC_ER ;
  pendirq &= MIC_ITR;
  pendirq &= MIC_SR ;
 
  for(mask = (1u<<31),i=31;mask; mask>>=1, i--)
  {
    if(mask&pendirq)
    {
      /*Call Interrupt Service Routine*/
      MainIntTbl[i]();
      /*clear Current interrupt*/
      MIC_RSR = mask;
      /**/
      break;
    }
  }
}
/*************************************************************************
 * Function Name: INTC_Init
 * Parameters:None
 *
 * Return: None
 *
 * Description: Initialize LPC3250 Interrupt Controller
 *
 *************************************************************************/
void INTC_Init(void)
{
int i;
  /*Interrupt disable*/
  __disable_fiq();
  __disable_irq();
  /*Init Interrupt Tables*/
  for(i = 0; i<32; i++)
  {
    MainIntTbl[i] = IRQ_Void;
    Sub1IntTbl[i] = IRQ_Void;
    Sub2IntTbl[i] = IRQ_Void;  
  }
  /*Sub interrupt controller 1 init*/
  SIC1_ER = 0x00000000;   /*Disable interrupts*/
  SIC1_APR = 0xFBD27186;  /*Default*/
  SIC1_ATR = 0x00026000;  /*Default*/
  SIC1_ITR = 0x00000000;  /*Select IRQ*/
  SIC1_RSR = 0xFFFFFFFF;  /*Clear pending irq*/
  
  /*Sub interrupt controller 2 init*/
  SIC2_ER = 0x00000000;   /*Disable interrupts*/
  SIC2_APR = 0x801810C0;  /*Default*/
  SIC2_ATR = 0x00000000;  /*Default*/
  SIC2_ITR = 0x00000000;  /*Select IRQ*/
  SIC2_RSR = 0xFFFFFFFF;  /*Clear pending irq*/

  /*Main interrupt controller init*/
  MIC_ER = 0xC0000003;    /*Enable Sub2FIQ, Sub1FIQ, Sub2IRQ and Sub1IRQ*/
  MIC_APR = 0x3FF0EFE0;   /*Default*/
  MIC_ATR = 0x00000000;   /*Default*/
  MIC_ITR = 0xC0000000;   /*Select Sub2FIQ and Sub1FIQ for FIQ*/
  MIC_RSR = 0xFFFFFFFF;   /*Clear pending irq*/
  
  /*Instal Sub2FIQ, Sub1FIQ, Sub2IRQ and Sub1IRQ*/
  MainIntTbl[0] = Sub1IRQ_Handler;
  MainIntTbl[1] = Sub2IRQ_Handler;
  MainIntTbl[30] = Sub1FIQ_Handler;
  MainIntTbl[31] = Sub2FIQ_Handler;
}

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
                     IRQ_TYPE_t Type)
{
Int32U mask;

  if((29 < IRQIndex) || (2 > IRQIndex)) return; /*Do not modify Sub2FIQ, Sub1FIQ
                                                 Sub2IRQ and Sub1IRQ Interrupt
                                                 Handlers*/
  mask = 1<<IRQIndex;
  /*Update Interrupt Table*/
  MainIntTbl[IRQIndex] = ISR;
  /*Set Polarity*/
  if(Polarity)
  {
    MIC_APR |= (mask&MIC_APR_USR_MASK);
  }
  else
  {
    MIC_APR &= ~(mask&MIC_APR_USR_MASK);
  }

  /*Set Sens*/
  if(Sens)
  {
    MIC_ATR |= (mask&MIC_ATR_USR_MASK);
  }
  else
  {
    MIC_ATR &= ~(mask&MIC_ATR_USR_MASK);
  }
  
  /*Set Type*/
  if(Type)
  {
    MIC_ITR |= mask;
  }
  else
  {
    MIC_ITR &= ~mask;
  }
}
/*************************************************************************
 * Function Name: MIC_IntEnable
 * Parameters:Int32U IRQIndex - IRQ Number
 *            IRQ_ENABLE_t Enable - 0 - Disable, 1- Enable)
 * Return: None
 *
 * Description: Enable or Disable IRQ
 *
 *************************************************************************/
void MIC_IntEnable(Int32U IRQIndex, IRQ_ENABLE_t Enable)
{
Int32U mask;

  if((29 < IRQIndex) || (2 > IRQIndex)) return; /*Do not modify Sub2FIQ, Sub1FIQ
                                                 Sub2IRQ and Sub1IRQ Interrupt
                                                 Enable*/
  mask = 1<<IRQIndex;

  if(Enable)
  {
    MIC_ER |= mask;
  }
  else
  {
    MIC_ER &= ~mask;
  }
                                                 
}

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
                     IRQ_TYPE_t Type)
{
Int32U mask;

  mask = 1<<IRQIndex;
  /*Update Interrupt Table*/
  Sub1IntTbl[IRQIndex] = ISR;
  /*Set Polarity*/
  if(Polarity)
  {
    SIC1_APR |= (mask&SIC1_APR_USR_MASK);
  }
  else
  {
    SIC1_APR &= ~(mask&SIC1_APR_USR_MASK);
  }

  /*Set Sens*/
  if(Sens)
  {
    SIC1_ATR |= (mask&SIC1_ATR_USR_MASK);
  }
  else
  {
    SIC1_ATR &= ~(mask&SIC1_ATR_USR_MASK);
  }
  /*Set Type*/
  if(Type)
  {
    SIC1_ITR |= (mask);
  }
  else
  {
    SIC1_ITR &= ~(mask);
  }
}
/*************************************************************************
 * Function Name: SIC1_IntEnable
 * Parameters:Int32U IRQIndex - IRQ Number
 *            IRQ_ENABLE_t Enable - 0 - Disable, 1- Enable)
 * Return: None
 *
 * Description: Enable or Disable IRQ
 *
 *************************************************************************/
void SIC1_IntEnable(Int32U IRQIndex, IRQ_ENABLE_t Enable)
{
Int32U mask;

  mask = 1<<IRQIndex;

  if(Enable)
  {
    SIC1_ER |= mask;
  }
  else
  {
    SIC1_ER &= ~mask;
  }                                                
}

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
                     IRQ_TYPE_t Type)
{
Int32U mask;

  mask = 1<<IRQIndex;
  /*Update Interrupt Table*/
  Sub2IntTbl[IRQIndex] = ISR;
  /*Set Polarity*/
  if(Polarity)
  {
    SIC2_APR |= (mask&SIC2_APR_USR_MASK);
  }
  else
  {
    SIC2_APR &= ~(mask&SIC2_APR_USR_MASK);
  }

  /*Set Sens*/
  if(Sens)
  {
    SIC2_ATR |= (mask&SIC2_ATR_USR_MASK);
  }
  else
  {
    SIC2_ATR &= ~(mask&SIC2_ATR_USR_MASK);
  }
  /*Set Type*/
  if(Type)
  {
    SIC2_ITR |= (mask);
  }
  else
  {
    SIC2_ITR &= ~(mask);
  }
}
/*************************************************************************
 * Function Name: SIC2_IntEnable
 * Parameters:Int32U IRQIndex - IRQ Number
 *            IRQ_ENABLE_t Enable - 0 - Disable, 1- Enable)
 * Return: None
 *
 * Description: Enable or Disable IRQ
 *
 *************************************************************************/
void SIC2_IntEnable(Int32U IRQIndex, IRQ_ENABLE_t Enable)
{
Int32U mask;

  mask = 1<<IRQIndex;

  if(Enable)
  {
    SIC2_ER |= mask;
  }
  else
  {
    SIC2_ER &= ~mask;
  }                                                 
}
/** private functions **/
void IRQ_Void(void)
{
}

/*************************************************************************
 * Function Name: Sub1IRQ_Handler
 * Parameters: None  
 *
 * Return: None
 *
 * Description: Nested SIC1 IRQ handler. 
 *
 *************************************************************************/
void  Sub1IRQ_Handler(void)
{
Int32U mask, i;
Int32U enable, pendirq;
  /*Save current Enabled IRQ*/
  enable = SIC1_ER ;
  enable &= ~(SIC1_ITR);
  /*find pending interrupt*/
  pendirq = enable & SIC1_SR;
  for(mask = (1u<<31),i=31;mask; mask>>=1, i--)
  {
    if(mask&pendirq)
    {
      /*mask current interrupt and all low priority interrupts*/
      Int32U disable  = (~((mask<<1) - 1))|(SIC1_ITR);
      SIC1_ER &= disable;
      /*enable interrupts*/
       __enable_irq();
      /*Call Interrupt Service Routine*/
      Sub1IntTbl[i]();
      /*disable interrupts*/
      __disable_irq();
      /*clear Current interrupt*/
      SIC1_RSR = mask;
      /*enable interrupts*/
      SIC1_ER |= enable;
      break;
    }
  }
}
/*************************************************************************
 * Function Name: Sub1FIQ_Handler
 * Parameters: None  
 *
 * Return: None
 *
 * Description: Nested SIC1 FIQ_Handler. 
 *
 *************************************************************************/
void Sub1FIQ_Handler(void)
{
Int32U mask, i;
Int32U pendirq;
  /*find pending interrupt*/
  pendirq = SIC1_ER;
  pendirq &= SIC1_ITR;
  pendirq &= SIC1_SR;
  
  for(mask = (1u<<31),i=31;mask; mask>>=1, i--)
  {
    if(mask&pendirq)
    {
       /*Call Interrupt Service Routine*/
      Sub1IntTbl[i]();
      /*clear Current interrupt*/
      SIC1_RSR = mask;
      /**/
      break;
    }
  }
}
/*************************************************************************
 * Function Name: Sub2IRQ_Handler
 * Parameters: None  
 *
 * Return: None
 *
 * Description: Nested SIC2 IRQ handler. 
 *
 *************************************************************************/
void Sub2IRQ_Handler(void)
{
Int32U mask, i;
Int32U enable, pendirq;
  /*Save current Enabled IRQ*/
  enable = SIC2_ER;
  enable &= ~(SIC2_ITR);
  /*find pending interrupt*/
  pendirq = enable & SIC2_SR;
  
  for(mask = (1u<<31),i=31;mask; mask>>=1, i--)
  {
    if(mask&pendirq)
    {
      /*mask current interrupt and all low priority interrupts*/
      Int32U disable = (~((mask<<1)-1))|(SIC2_ITR);
      SIC2_ER &= disable;
      /*enable interrupts*/
       __enable_irq();
      /*Call Interrupt Service Routine*/
      Sub2IntTbl[i]();
      /*disable interrupts*/
      __disable_irq();
      /*clear Current interrupt*/
      SIC2_RSR = mask;
      /*enable interrupts*/
      SIC2_ER |= enable;
      break;
    }
  }
}
/*************************************************************************
 * Function Name: Sub2FIQ_Handler
 * Parameters: None  
 *
 * Return: None
 *
 * Description: Nested SIC2 FIQ_Handler. 
 *
 *************************************************************************/
void Sub2FIQ_Handler(void)
{
Int32U mask, i;
Int32U pendirq;
  /*find pending interrupt*/
  pendirq = SIC2_ER;
  pendirq &= SIC2_ITR;
  pendirq &= SIC1_SR;
  
  for(mask = (1u<<31),i=31;mask; mask>>=1, i--)
  {
    if(mask&pendirq)
    {
      /*Call Interrupt Service Routine*/
      Sub2IntTbl[i]();
      /*disable interrupts*/
      __disable_fiq();
      /*clear Current interrupt*/
      SIC2_RSR = mask;
      /**/
      break;
    }
  }
}
