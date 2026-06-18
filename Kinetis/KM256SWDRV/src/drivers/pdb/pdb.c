/*****************************************************************************
 * (c) Copyright 2010-2015, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      pdb.c
 * @version   1.0.2.0
 * @date      Feb-02-2015
 * @brief     PDB driver source file.
 ******************************************************************************/
#include "common.h"
#include "pdb.h"

#if !defined(MCU_MKM34Z7)
  #error "The pdb.c module is specific to the MKM34Z256VLx7 microcontrollers."
#endif

/******************************************************************************
 * data type definitions                                                      *
 ******************************************************************************/
static PDB_CALLBACK pCallbackPDB;

/******************************************************************************
 * Public functions definitions                                               *
 ******************************************************************************/
void PDB_Init (tPDB pdb, uint8 ip, PDB_CALLBACK pCallback)
{
  if (pCallback) { pCallbackPDB = pCallback; NVIC_SetIsr(INT_PDB0,ip); }
  PDB0_SC    = (PDB_SC_LDMOD(0)|PDB_SC_PDBEN_MASK);
  PDB0_MOD   = pdb.MOD;
  PDB0_IDLY  = pdb.IDLY;
  PDB0_SC   |= PDB_SC_LDOK_MASK;
  PDB0_SC    = pdb.SC;
}

void PDBCH0TRG0_Init (tPDB_TRG trg, uint16 val)
{
  PDB0_CH0DLY0 = val;
  PDB0_CH0C1  |= (((trg.C1&PDB_C1_BB_MASK )&(0x01<<PDB_C1_BB_SHIFT ))|
                  ((trg.C1&PDB_C1_TOS_MASK)&(0x01<<PDB_C1_TOS_SHIFT))|
                  ((trg.C1&PDB_C1_EN_MASK )&(0x01<<PDB_C1_EN_SHIFT)));
}

void PDBCH0TRG1_Init (tPDB_TRG trg, uint16 val)
{
  PDB0_CH0DLY1 = val;
  PDB0_CH0C1  |= (((trg.C1&PDB_C1_BB_MASK )&(0x02<<PDB_C1_BB_SHIFT ))|
                  ((trg.C1&PDB_C1_TOS_MASK)&(0x02<<PDB_C1_TOS_SHIFT))|
                  ((trg.C1&PDB_C1_EN_MASK )&(0x02<<PDB_C1_EN_SHIFT)));
}

void PDBCH0TRG2_Init (tPDB_TRG trg, uint16 val)
{
  PDB0_CH0DLY2 = val;
  PDB0_CH0C1  |= (((trg.C1&PDB_C1_BB_MASK )&(0x04<<PDB_C1_BB_SHIFT ))|
                  ((trg.C1&PDB_C1_TOS_MASK)&(0x04<<PDB_C1_TOS_SHIFT))|
                  ((trg.C1&PDB_C1_EN_MASK )&(0x04<<PDB_C1_EN_SHIFT)));
}

void PDBCH0TRG3_Init (tPDB_TRG trg, uint16 val)
{
  PDB0_CH0DLY3 = val;
  PDB0_CH0C1  |= (((trg.C1&PDB_C1_BB_MASK )&(0x08<<PDB_C1_BB_SHIFT ))|
                  ((trg.C1&PDB_C1_TOS_MASK)&(0x08<<PDB_C1_TOS_SHIFT))|
                  ((trg.C1&PDB_C1_EN_MASK )&(0x08<<PDB_C1_EN_SHIFT)));
}

void PDBPOUT0_Init (tPDB_PO pout, uint16 val1, uint16 val2)
{
  PDB0_PO0DLY = (PDB_PODLY_DLY1(val1))|(PDB_PODLY_DLY2(val2));
  PDB0_POEN   = (pout.POEN&0x1)|(PDB0_POEN&(~0x1));
}

/******************************************************************************
 * interrupt functions definitions                                            *
 ******************************************************************************/
weak void pdb_isr (void)
{
  if (PDB0_SC & PDB_SC_PDBIE_MASK)
  {
    if (PDB0_SC & PDB_SC_PDBIF_MASK)
    {
      ioclrb (PDB0_SC,PDB_SC_PDBIF_MASK);
      pCallbackPDB(PDBDLY_CALLBACK);
    }

    if (PDB0_CH0S & PDB_S_ERR(15))
    {
      iobfiw (PDB0_CH0S,PDB_S_ERR_SHIFT,4,0);
      pCallbackPDB(CH0SEQERR_CALLBACK);
    }
  }
}
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/
