/****************************************************************************************************/
/**
Copyright (c) 2012 Freescale Semiconductor
Freescale Confidential Proprietary
\file       rnga.c
\brief      Hardware random number generator driver
\author     Freescale Semiconductor
\author     
\version    1.0
\date       Jul 20, 2012
*/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
#include "common.h"
#include "rnga.h"


/*****************************************************************************************************
* Code of project wide FUNCTIONS
*****************************************************************************************************/

/**  
 * \brief   init random number generation module
 * \author   
 * \param    none
 * \return   the 32-bit random number
 * \todo    
 * \warning
 */
void 
rnga_init (unsigned int seed)
{
   RNG_ER = seed;
  
   /*FSL: generate randomness*/
   RNG_CR = RNG_CR_GO_MASK;
}

/**  
 * \brief   Wait for the generation of a random number
 * \author   
 * \param    none
 * \return   the 32-bit random number
 * \todo    
 * \warning
 */
unsigned int 
rnga_getnumber (void)
{   
   /*FSL: check for FIFO level*/
   while(!(RNG_SR & RNG_SR_OREG_LVL_MASK))
   ;
   
   /*FSL: get random number*/
   return RNG_OR;
}

/**  
 * \brief   Stop randomness
 * \author   
 * \param    none
 * \return   the 32-bit random number
 * \todo    
 * \warning
 */
void 
rnga_stop (void)
{
   RNG_CR &= ~RNG_CR_GO_MASK;
}

/********************************************************************/
    
