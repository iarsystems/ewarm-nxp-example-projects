/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2010
 *
 *    File name   : drv_adc.c
 *    Description : ADC driver module
 *
 *    History :
 *    1. Date        : February, 2 2010
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 16 $
 **************************************************************************/

#include "drv_adc.h"
#include "drv_asic.h"

typedef union _Fifo_reg_t
{
  Int32U Data;
  struct
  {
    Int32U ITEM_ID    : 4;
    Int32U ADCDATAOUT :12;
    Int32U            :16;
  };
} Fifo_reg_t;

static volatile Boolean VR_NewData;
static volatile Int16U  VR;

/*************************************************************************
 * Function Name: TS_Handler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Touchscreen interrupt handler
 *
 *************************************************************************/
void TS_Handler (void)
{

Int32U count[2];
Int32U acc[2];
Fifo_reg_t reg;

  if (GCQSR_bit.EOQ)
  {
    GCQSR_bit.EOQ = 1;
    count[0] = 0;
    acc[0]   = 0;
    while(!GCQSR_bit.EMPT)
    {
      reg.Data = GCQFIFO;
      switch(reg.ITEM_ID)
      {
      case 0:
        ++count[0];
        acc[0] += reg.ADCDATAOUT;
        break;
      }
    }
    VR = acc[0] / count[0];
    VR_NewData = TRUE;

  }
}

/*************************************************************************
 * Function Name: AdcInit
 * Parameters: none
 *
 * Return: none
 *
 * Description: Init ADC
 *
 *************************************************************************/
void AdcInit (void)
{
  // Init variable
  CGCR2_bit.ipg_clk_tchscrn = 1;  // enable ipclk
  TGCR_bit.IPGCLKEN = 1;          // enable tsc ipgclken bit
  TGCR_bit.TSCRST = 1;            // self-reset tsc
  while(TGCR_bit.TSCRST);         // wait until self-reset done

  TGCR_bit.PDBTIME = 0x7F;            // set debound time
  TGCR_bit.POWERMODE = 1;             // power saving
  TGCR_bit.PDBEN = 1;                 // enable debounce
  TGCR_bit.PDEN = 1;                  // pen down enable

  TCQMR_bit.EOQIRQMSK = 0;            // enable tcq End-of-queue interrupt
  GCQMR_bit.EOQIRQMSK = 0;            // enable gp End-of-queue interrupt

  // Init VR measure
  GCC0_bit.SELREFN = 2;               // NGND_ADC
  GCC0_bit.YNLRSW  = 0;               // YN(LR) off
  GCC0_bit.XNURSW  = 1;               // XN(UR) off
  GCC0_bit.XPULSW  = 0;               // XP(UL) off
  GCC0_bit.SELREFP = 2;               // External reference
  GCC0_bit.WIPERSW = 0;               // WIPER off
  GCC0_bit.SELIN   = 4;               // WIPER input
  GCC0_bit.NOS = 4-1;                 // 4 samples
  GCC0_bit.SETTLING_TIME = 16;

  GCQ_ITEM_7_0_bit.ITEM0 = 0;
  GCQCR_bit.FIFOWATERMARK = 4-1;
  GCQCR_bit.QSM = 2;                  // New conversion queue is started by FQS setting
  GCQCR_bit.LAST_ITEM_ID = 0;         // Set last item

  GCQMR_bit.EOQIRQMSK = 0;            // enable gp End-of-queue interrupt

  // Init TS interrupt
  ASIC_SetupIntr(TS_Handler,FALSE,INT_TSC,2);
  ASIC_EnableIntSource(INT_TSC);

}

/*************************************************************************
 * Function Name: VR_Measure
 * Parameters: none
 *
 * Return: Int16U
 *
 * Description: Return current voltage on WIPER input
 *
 *************************************************************************/
Int16U VR_Measure(void)
{
  VR_NewData = FALSE;
  GCQCR_bit.FQS = 1; // FQS starts the conversion queue
  GCQCR_bit.FQS = 0;
  while(!VR_NewData);
  return(VR);
}
