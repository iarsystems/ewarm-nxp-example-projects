/*
 * File:	start.c
 * Purpose:	Kinetis start up routines. 
 *
 * Notes:		
 */

#include "start.h"
#include "common.h"
#include "wdog.h"
#include "rcm.h"
#include "sysinit.h"

/********************************************************************/
/*!
 * \brief   Kinetis Start
 * \return  None
 *
 * This function calls all of the needed starup routines and then 
 * branches to the main process.
 */
void start(void)
{ 
	/* Disable the watchdog timer */
	wdog_disable();

	/* Copy any vector or data sections that need to be in RAM */
	common_startup();

	/* Perform processor initialization */
	sysinit();
        
        printf("\n\n");
        
	/* Determine the last cause(s) of reset */
        outSRS();

	/* Determine specific Kinetis device and revision */
	cpu_identify();
	
	/* Jump to main process */
	main();

	/* No actions to perform after this so wait forever */
	while(1);
}
/********************************************************************/
/*!
 * \brief   Kinetis Identify
 * \return  None
 *
 * This is primarly a reporting function that displays information
 * about the specific CPU to the default terminal including:
 * - Kinetis K12/K22/K11/K21 family
 * - package
 * - die revision
 * - P-flash size
 * - Ram size
 */
void cpu_identify (void)
{
    /* Determine the Kinetis K12/K22/K11/K21 family */
    switch((SIM_SDID & SIM_SDID_FAMID(0x7))>>SIM_SDID_FAMID_SHIFT) 
    {  
    case 0x0:
      printf("\nK12D-");
      break;
    case 0x1:
      printf("\nK22D-");
      break;
    case 0x2:
      printf("\nK11D-");
      break;
    case 0x3:
      printf("\nK21F-");
      break;
    default:
      printf("\nUnrecognized Kinetis family device.\n");  
      break;  	
    }

    /* Determine the package size */
    switch((SIM_SDID & SIM_SDID_PINID(0xF))>>SIM_SDID_PINID_SHIFT) 
    {  
    case 0x2:
      printf("32 pin       ");
      break;
    case 0x4:      
      printf("48 pin       ");
      break;
    case 0x5:
      printf("64 pin       ");
      break;
    case 0x6:
      printf("80 pin       ");
      break;
    case 0x7:
      printf("81 pin       ");
      break;
    case 0x8:
      printf("100 pin      ");
      break;
    case 0x9:
      printf("121 pin      ");
      break;
    case 0xA:
      printf("144 pin      ");
      break;
    case 0xC:
      printf("196 pin      ");
      break;
    case 0xE:
      printf("256 pin      ");
      break;
    default:
      printf("\nUnrecognized Kinetis package code.      ");  
      break;  	
    }                

    /* Determine the revision ID */

 switch((SIM_SDID & SIM_SDID_REVID(0xF))>>SIM_SDID_REVID_SHIFT) 
    { 
   
    case 0x0:
      printf("Silicon rev 1.0   \n ");
      break;
    default:
      printf("\nThis version of software doesn't recognize the revision code.");  
      break;  
    }
    
    /* Determine the flash revision */
    flash_identify();    
    
    /* Determine the P-flash size */
  switch((SIM_FCFG1 & SIM_FCFG1_PFSIZE(0xF))>>SIM_FCFG1_PFSIZE_SHIFT)
    {
    case 0x7:
      printf("128 kBytes of P-flash	");
      break;
    case 0x9:
      printf("256 kBytes of P-flash	");
      break;
    case 0xB:
      printf("512 kBytes of P-flash     ");
      break;
    case 0xD:
      printf("1024 kBytes of P-flash    ");
      break;      
    case 0xF:
      printf("512 kBytes of P-flash	");
      break;
    default:
      printf("ERR!! Undefined P-flash size\n");  
      break;  	  		
    }

    /* Determine the RAM size */
    switch((SIM_SOPT1 & SIM_SOPT1_RAMSIZE(0xF))>>SIM_SOPT1_RAMSIZE_SHIFT)
    {
    case 0x5:
      printf(" 32 kBytes of RAM\n\n");
      break;
    case 0x7:
      printf(" 64 kBytes of RAM\n\n");
      break;
    case 0x9:
      printf(" 128 kBytes of RAM\n\n");
      break;
    default:
      printf(" ERR!! Undefined RAM size\n\n");  
      break;  		
    }
}
/********************************************************************/
/*!
 * \brief   flash Identify
 * \return  None
 *
 * This is primarly a reporting function that displays information
 * about the specific flash parameters and flash version ID for 
 * the current device. These parameters are obtained using a special
 * flash command call "read resource." The first four bytes returned
 * are the flash parameter revision, and the second four bytes are
 * the flash version ID.
 */
void flash_identify (void)
{
  
#if 0
    /* Get the flash parameter version */

    /* Write the flash FCCOB registers with the values for a read resource command */
    FTFE_FCCOB0 = 0x03;
    FTFE_FCCOB1 = 0x00;
    FTFE_FCCOB2 = 0x00;
    FTFE_FCCOB3 = 0x00;
    FTFE_FCCOB8 = 0x01;

    /* All required FCCOBx registers are written, so launch the command */
    FTFE_FSTAT = FTFE_FSTAT_CCIF_MASK;

    /* Wait for the command to complete */
    while(!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK));
    
    printf("Flash parameter version %d.%d.%d.%d\n",FTFE_FCCOB4,FTFE_FCCOB5,FTFE_FCCOB6,FTFE_FCCOB7);

    /* Get the flash version ID */   

    /* Write the flash FCCOB registers with the values for a read resource command */
    FTFE_FCCOB0 = 0x03;
    FTFE_FCCOB1 = 0x00;
    FTFE_FCCOB2 = 0x00;
    FTFE_FCCOB3 = 0x04;
    FTFE_FCCOB8 = 0x01;

    /* All required FCCOBx registers are written, so launch the command */
    FTFE_FSTAT = FTFE_FSTAT_CCIF_MASK;

    /* Wait for the command to complete */
    while(!(FTFE_FSTAT & FTFE_FSTAT_CCIF_MASK));

    printf("Flash version ID %d.%d.%d.%d\n",FTFE_FCCOB4,FTFE_FCCOB5,FTFE_FCCOB6,FTFE_FCCOB7);  
    
#endif
}
/********************************************************************/

