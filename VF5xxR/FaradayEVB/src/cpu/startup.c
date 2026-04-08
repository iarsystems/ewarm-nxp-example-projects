/*
 * File:    startup.c
 * Purpose: Generic Faraday startup code
 *
 * Notes:
 */

#include "common.h"
#include "sysinit.h"
#include "startup.h"


/********************************************************************/


int __low_level_init (void)
{
  /* Perform system initialization */
  sysinit();

  /* Identify CPU type and configuration */
  cpu_identify();

  return  1;
}


/********************************************************************/
/*!
 * \brief   Vybrid Identify
 * \return  None
 *
 * This is primarly a reporting function that displays information
 * about the specific CPU to the default terminal including:
 */
void cpu_identify(void)
{
  SystemCoreClockUpdate();

  printf("\n\n**********************\n");
  printf("Freescale Vybrid\n");

  if((MSCM->CPxTYPE>>MSCM_CPxTYPE_Personality_SHIFT)==0x434135)
  {
    printf("  A5 Core Running\n");
  }
  else if((MSCM->CPxTYPE>>MSCM_CPxTYPE_Personality_SHIFT)==0x434D34)
  {
    printf("  M4 Core Running\n");
  }

  printf("  Rev #: 1.%d\n",(MSCM->CPxTYPE&MSCM_CPxTYPE_rYpZ_MASK)-1);
  printf("  Core number: %d\n",MSCM->CPxNUM);
  printf("  Number of cores: %d\n",MSCM->CPxCOUNT+1); //Add 1 to get # of cores
  printf("  Sys Clk Freq: %d kHz\n\n", SystemCoreClock);

  printf("\n");
}
