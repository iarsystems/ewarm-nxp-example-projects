/******************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * pdbpulse_test.c
 ******************************************************************************/
#include "drivers.h"

void main (void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* Route the PDB_Out signal to the PL2 pin where can be monitored.          */
  PORT_Init (PORTL, PORT_MODULE_ALT2_MODE, PIN2);
  XBAR_Init (XBAR_MODULE_NO_EDGE_DETECT_CONFIG,
             XBAR_MODULE_NO_EDGE_DETECT_CONFIG,
             XBAR_MODULE_NO_EDGE_DETECT_CONFIG,
             XBAR_MODULE_NO_EDGE_DETECT_CONFIG,
             0,
             NULL);
  XBAR_Path(XBAR_PDBPOUT0,XBAR_OUT10);


  /* Initiate the PDB module with pulse output function                       */
  PDB_PoutInit(PDB_POUT_ENABLE_CONFIG, 0, 10000);
  PDB_Init(PDB_MODULE_SWTRG_CONT_CONFIG(PDBCLK_DIV1,PDB_MULT1,60000,0),0,NULL);

  /*Trigger the PDB counter                                                   */
  PDB_SWTrg();

  while(1);
}
