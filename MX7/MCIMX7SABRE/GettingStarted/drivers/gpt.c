/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2017
 *
 *    File name   : gpt.c
 *    Description : General Purpose Timer driver
 *
 *    History :
 *    1. Date        : January 2017
 *       Author      : Atanas Uzunov
 *       Description : initial revision
 *
 *    $Revision: 7084 $
 **************************************************************************/
#include <stdint.h>
#include "device_registers.h"
#include "gpt.h"

/*GPTimer Prescalers*/
#define GPTIM_PRESCALE      (240)
#define GPTIM_PRESCALE24M   (2)

/*GPTimer Reload value calc macro*/
#define GPTIM_LOAD(ticks_per_sec) ((24000000)         /*Timer Runs @ 24MHz clock*/\
                                 /GPTIM_PRESCALE      /*Prescaler*/\
                                 /GPTIM_PRESCALE24M   /*Prescaler24M*/\
                                 /ticks_per_sec)      /*Ticks per second*/


/*************************************************************************
 * Function Name: GPTimer_Init
 * Parameters: GPT_MemMapPtr GPT_BASE, uint32_t ticks_per_sec
 *
 * Return: none
 *
 * Description: General Purpose Timer Initialization
 *************************************************************************/
void GPTimer_Init(GPT_MemMapPtr GPT_BASE, uint32_t ticks_per_sec)
{
  /*Enable Timer clocks*/
  if(GPT1_BASE_PTR == GPT_BASE)
  {
    CCM_CCGR124_SET = 0x2;
  }
  else
  if(GPT2_BASE_PTR == GPT_BASE)
  {
    CCM_CCGR125_SET = 0x2;
  }
  else
  if(GPT3_BASE_PTR == GPT_BASE)
  {
    CCM_CCGR126_SET = 0x2;
  }
  else
  if(GPT4_BASE_PTR == GPT_BASE)
  {
    CCM_CCGR127_SET = 0x2;
  }
  /*Configure Timer mode*/
  GPT_BASE->CR = (0<<GPT_CR_EN_SHIFT)       /*Timer still disalbed */\
               | (0<<GPT_CR_ENMOD_SHIFT)    /*GPT counter will retain its value when it is disabled*/\
               | (0<<GPT_CR_DBGEN_SHIFT)    /*GPT is disabled in debug mode*/\
               | (0<<GPT_CR_WAITEN_SHIFT)   /*GPT is disabled in wait mode*/\
               | (0<<GPT_CR_DOZEEN_SHIFT)   /*GPT is disabled in doze mode*/\
               | (0<<GPT_CR_STOPEN_SHIFT)   /*GPT is disabled in Stop mode*/\
               | GPT_CR_CLKSRC(5)           /*Clock Source select 24MHz*/\
               | (0<<GPT_CR_FRR_SHIFT)      /*Restart mode*/\
               | (1<<GPT_CR_EN_24M_SHIFT)   /*Enable 24MHz clock input from crystal.*/\
               | (0<<GPT_CR_SWR_SHIFT)      /*GPT is not in reset state*/\
               | GPT_CR_IM1(0)              /*capture disabled*/\
               | GPT_CR_IM2(0)              /*capture disabled*/\
               | GPT_CR_OM1(0)              /*Output disconnected. No response on pin.*/\
               | GPT_CR_OM2(0)              /*Output disconnected. No response on pin.*/\
               | GPT_CR_OM3(0)              /*Output disconnected. No response on pin.*/\
               | (0<<GPT_CR_FO1_SHIFT)      /*Output not forced*/\
               | (0<<GPT_CR_FO2_SHIFT)      /*Output not forced*/\
               | (0<<GPT_CR_FO3_SHIFT);     /*Output not forced*/

  /*Set Timer Prescalers*/
  GPT_BASE->PR = GPT_PR_PRESCALER(GPTIM_PRESCALE-1)        /*Set Timer prescaler*/\
               | GPT_PR_PRESCALER24M(GPTIM_PRESCALE24M-1); /*Divide input*/

  /*Clear Timer Status*/
  GPT_BASE->SR = (1<<GPT_SR_OF1_SHIFT)      /*Clear Compare flag 1*/\
               | (1<<GPT_SR_OF2_SHIFT)      /*Clear Compare flag 2*/\
               | (1<<GPT_SR_OF3_SHIFT)      /*Clear Compare flag 3*/\
               | (1<<GPT_SR_IF1_SHIFT)      /*Clear Capture flag 1*/\
               | (1<<GPT_SR_IF2_SHIFT)      /*Clear Capture flag 1*/\
               | (1<<GPT_SR_ROV_SHIFT);     /*Clear Rollover Flag*/

  /*Enable interrupt*/
  GPT_BASE->IR = (1<<GPT_IR_OF1IE_SHIFT)    /*Enable Compare 1 Interrupt*/\
               | (0<<GPT_IR_OF2IE_SHIFT)    /*Disable Compare 2 Interrupt*/\
               | (0<<GPT_IR_OF3IE_SHIFT)    /*Disable Compare 3 Interrupt*/\
               | (0<<GPT_IR_IF1IE_SHIFT)    /*Disable Capture 1 Interrupt*/\
               | (0<<GPT_IR_IF2IE_SHIFT)    /*Disable Capture 1 Interrupt*/\
               | (0<<GPT_IR_ROVIE_SHIFT);   /*Disable Rollover Interrupt*/

 /*Set Reload period*/
  GPT_BASE->OCR1 = GPT_OCR1_COMP(GPTIM_LOAD(ticks_per_sec));   /*Set reload period*/
}

/*************************************************************************
 * Function Name: GPTimer_Start
 * Parameters: GPT_MemMapPtr GPT_BASE
 *
 * Return: none
 *
 * Description: Starts the General Purpose Timer.
 *************************************************************************/
void GPTimer_Start(GPT_MemMapPtr GPT_BASE)
{
   GPT_BASE->CR |= (1<<GPT_CR_EN_SHIFT);    /*Enable GPTimer*/
}

/*************************************************************************
 * Function Name: GPTimer_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: General Purpose Timer interrupt handler
 *************************************************************************/
void GPTimer_ClearOF1Flag(GPT_MemMapPtr GPT_BASE)
{
  GPT_BASE->SR = (1<<GPT_SR_OF1_SHIFT);     /*Clear Output Compare 1 Flag*/
}
