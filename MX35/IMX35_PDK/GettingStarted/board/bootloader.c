/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : bootloader.c
 *    Description : bootloader for iMX35 PDK
 *
 *
 *    History :
 *    1. Date        : 04.2011
 *       Author      : Stoyan Choynev
 *       Description : initial vestion
 *
 *    $Revision: 7859 $
 **************************************************************************/
/** include files **/
#include <NXP/iomcimx35.h>
#include "arm_comm.h"

/** local definitions **/
#define SDRAM_CS0_BASE  0x80000000
#define SDRAM_CS1_BASE  0x90000000

//flash functions return value
#define FLASH_OK            ( 0 )
#define FLASH_ERROR         ( 1 )

#define FLASH_BASE_ADDR     0xBB000000

#define NAND_MAIN_SIZE        (4096)    /*The NFC reads/writes 4096 bytes of the main array*/
#define NAND_SPARE_SIZE       (512)     /*The NFC reads/writes 208 bytes of the spare array*/
#define NAND_BUFF_SIZE        (NAND_MAIN_SIZE+NAND_SPARE_SIZE)
#define NAND_PG_PER_BLK       (128)  /**/
#define NAND_BLK_NUM          (8192)
#define NAND_PG_NUM           (NAND_BLK_NUM*NAND_PG_PER_BLK)

#define NAND_ID               *((volatile unsigned int *)FLASH_BASE_ADDR)
#define NAND_STATUS           *((volatile unsigned int *)FLASH_BASE_ADDR)


#define NAND_SPARE_OFFSET     0x1000     /**/

typedef enum
{
  MAIN_SPARE,
  SPARE
} t_DataOutOp ;

#define PERFORM_CMD     0x1
#define PERFORM_ADDR    0x2
#define PERFORM_DATAIN  0x4
#define PERFORM_DATAOUT 0x8
#define PERFORM_IDOUT   0x10
#define PERFORM_STATUS  0x20

extern __arm void __vector(void);

/** default settings **/

/** external functions **/

/** external data **/

/** internal functions **/
void CmdInOperation(unsigned short cmd);
void AddrInOperation(unsigned int Column, unsigned int Row);
void DataOutOperation(t_DataOutOp PageSiz);
void NandGetStatus(void);
unsigned int NandReadPage(unsigned int Page);
unsigned int NandCheckBlock(unsigned int Block);
unsigned int NandFindValidBlock(unsigned int Block);
/** public data **/

/** private data **/

/** public functions **/
#pragma section =".bootloader"
/*************************************************************************
 * Function Name: _Init_Clock
 * Parameters: None
 *
 * Return: None
 *
 * Description: Clock Controller Module initialization.
 *
 *************************************************************************/
void Init_Clock (void) @ ".bootloader"
{
  /**/
  CCM_CCMR = (0<<30) |    /*WFI = 0*/
             (0<<29) |    /*STBY_EXIT_SRC = 0*/
             (0<<28) |    /*VSTBY = 0*/
             (0<<27) |    /*WBEN = 0*/
            (15<<20) |    /*VOL_RDY_CNT = 15*/
             (3<<18) |    /*ROMW = 3*/
             (0<<16) |    /*RAMW = 0*/
             (0<<14) |    /*LPM = 0*/
              (1<<9) |    /*UPE = 1*/
              (1<<3) ;    /*MPE = 1*/

  /**/
  CCM_PDR0 = (0<<26) |    /*PER_SEL = 0*/
             (1<<23) |    /*IPU_HND_BYP = 1*/
             (0<<20) |    /*HSP_PODF = 0*/
             (0<<16) |    /*CON_MUX_DIV = 0*/
             (0<<15) |    /*CKIL_SEL = 0*/
             (1<<12) ;    /*CCM_PER_AHB = 1*/

  /*Set MCU PLL 532MHz*/
  CCM_MPCTL = (1<<31) |    /*BRMO = 1*/
              (0<<26) |    /*PD = 0*/
             (11<<16) |    /*MFD = 11*/
             (11<<10) |    /*MFI = 11*/
               (1<<0) ;    /*MFN = 1*/

  /*Set Peripheral PLL 300MHz*/
  CCM_PPCTL = (1<<31) |    /*BRMO = 1*/
              (0<<26) |    /*PD = 0*/
              (3<<16) |    /*MFD = 3*/
              (6<<10) |    /*MFI = 11*/
               (1<<0) ;    /*MFN = 1*/
}
/*************************************************************************
 * Function Name: Init_SDRAM
 * Parameters: None
 *
 * Return: None
 *
 * Description: SDRAM Controller Initialization
 *
 *************************************************************************/
void Init_SDRAM (void) @ ".bootloader"
{
volatile Int32U Dummy;
  // DDR RAM initialization
  // DDR 4x8Mx16
  // Row Address = 13
  // Col Address = 10
  // ESDMISC: enable DDR mode
  ESDMISC = 0x00000304;
  // ESDCFG0: timing config
  ESDCFG0 = 0x007FFC3F;
  // ESDCTL0: Precharge Mode
  ESDCTL0 = 0x92226080;
  // issue Precharge All command (MA10 = 1)
  Dummy = *((volatile unsigned int *)(SDRAM_CS0_BASE + 0x400));
  // ESDCTL0: AutoRefresh Mode
  ESDCTL0 = 0xA2226080;
  // issue Autorefresh command
  Dummy = *((volatile unsigned int *)SDRAM_CS0_BASE);
  Dummy = *((volatile unsigned int *)SDRAM_CS0_BASE);
  // ESDCTL0: Load Mode register Mode
  ESDCTL0 = 0xB2226080;
  // issue Load Mode Register command
  Dummy = *((volatile unsigned char *)(SDRAM_CS0_BASE + 0x233));
  Dummy = *((volatile unsigned char *)(SDRAM_CS0_BASE + 0x1000));
  // ESDCTL0: Normal Mode
  // Row Address  = 13
  // Col Address  = 10
  // Data width   = 32-bit
  // Refresh Rate = 7.81 uS
  // Burst Length = 8
  ESDCTL0 = 0x82226080;
  // issue normal access
  *((volatile unsigned int *)SDRAM_CS0_BASE) = 0;
  // ESDCFG1: timing config
  ESDCFG1 = 0x007FFC3F;
  // ESDCTL1: Precharge Mode
  ESDCTL1 = 0x92226080;
  // issue Precharge All command (MA10 = 1)
  Dummy = *((volatile unsigned int *)(SDRAM_CS1_BASE + 0x400));
  // ESDCTL1: AutoRefresh Mode
  ESDCTL1 = 0xA2226080;
  // issue Autorefresh command
  Dummy = *((volatile unsigned int *)SDRAM_CS1_BASE);
  Dummy = *((volatile unsigned int *)SDRAM_CS1_BASE);
  // ESDCTL1: Load Mode register Mode
  ESDCTL1 = 0xB2226080;
  // issue Load Mode Register command
  Dummy = *((volatile unsigned char *)(SDRAM_CS1_BASE + 0x233));
  Dummy = *((volatile unsigned char *)(SDRAM_CS1_BASE + 0x1000));
  // ESDCTL1: Normal Mode
  // Row Address  = 13
  // Col Address  = 10
  // Data width   = 32-bit
  // Refresh Rate = 7.81 uS
  // Burst Length = 8
   ESDCTL1 = 0x82226080;
  // issue normal access
  *((volatile unsigned int *)(SDRAM_CS1_BASE)) = 0;
  // ESDMISC: Reset Delay Line Measurement
  ESDMISC = 0x0000030C;
}


/*************************************************************************
 * Function Name: Init_NAND
 * Parameters: None
 *
 * Return: None
 *
 * Description: Init NFC
 *
 *************************************************************************/
void Init_NAND(void) @ ".bootloader"
{
  /*Selsect CS0*/
  RAM_BUFFER_ADDRESS_bit.ACTIVE_CS = 0;
  /*Unlock RAM Buffer*/
  NFC_CONFIGURATION_bit.BLS = 2;
  /*8-symbol error correction*/
  NAND_FLASH_CONFIG1_bit.ECC_MODE = 0;
  /*ECC operation enable*/
  NAND_FLASH_CONFIG1_bit.ECC_EN = 1;
  /*Spare area size in words*/
  NFC_SPAS_bit.SPAS = 52;
  /*128- pages pre block*/
  NAND_FLASH_CONFIG1_bit.PPB = 2;
  /*Interrupt after complete page operation*/
  NAND_FLASH_CONFIG1_bit.FP_INT = 1;
  /*Select 4K page size 8-bits NAND*/
  CCM_RCSR_bit.NFC_4K = 0;
  CCM_RCSR_bit.NFC_FMS = 1;
  CCM_RCSR_bit.NFC_4K = 1;
  CCM_RCSR_bit.NFC_16bit_SEL0 = 0;
  /*Reset NAND Flash*/
  CmdInOperation(0xFF);
  /*NFC reset*/
  NAND_FLASH_CONFIG1_bit.NFC_RST = 1;
}

/*************************************************************************
 * Function Name: bootload
 * Parameters: none
 *
 * Return: none
 *
 * Description: bootloader for iMX31 PDK. Inits Clock and SDRAM controller
 *              Copies a program from NAND flash to SDRAM
 *
 *************************************************************************/
void bootload(void) @ ".bootloader"
{
Int32U * pSrc;
Int32U * pDest;
Int32U block,page;

  /*Disable Watchdog Power Down Counter*/
  WMCR_bit.PDE = 0;
  /*Init Clock*/
  Init_Clock();
  /*Init SDRAM*/
  Init_SDRAM();
  /*Init NFC*/
  Init_NAND();
  /*Start from Block 1*/
  block = NandFindValidBlock(1);
  page = 0;  /*Start from page 1 of the block*/

  while((NAND_BLK_NUM) > (block))
  {
    if(FLASH_ERROR == NandReadPage(block*NAND_PG_PER_BLK + page)) while(1); /*Read Error*/
    /*Source address =  NFC RAM buffer address*/
    pSrc = (unsigned int *)(FLASH_BASE_ADDR);
    /*Dest address = Last Word of the Spare Array*/
    pDest = *((unsigned int* *)(FLASH_BASE_ADDR+NAND_SPARE_OFFSET));

    if(0xFFFFFFFF == (Int32U)pDest) break;/*No more data*/
    /*Copy Data*/
    for(Int32U cntr = 0;NAND_MAIN_SIZE > cntr; cntr+=sizeof(Int32U))
    {
      *pDest++ = *pSrc++;
    }

    if(NAND_PG_PER_BLK <= ++page)
    {/*It was last page of the block*/
      page = 0;
      block = NandFindValidBlock(block+1);
    }
  }
  /*Jump To pogram Start*/
  __vector();
}
/** private functions **/
/*************************************************************************
 * Function name: CmdInOperation
 * Parameters:  cmd - the Command to send
 *
 * Return: None
 *
 * Description: Perform  Command Operaion
 *
 *************************************************************************/
void CmdInOperation(unsigned short cmd) @ ".bootloader"
{
  /*Load the Command*/
  NAND_FLASH_CMD = cmd;
  /*Start Command Operation*/
  NAND_FLASH_CONFIG2 = PERFORM_CMD;
  /*Wait operation to complete*/
  while(!NAND_FLASH_CONFIG2_bit.INT);
}

/*************************************************************************
 * Function name: AddrInOperation
 * Parameters:  Column - NAND Colum Address
 *              Row    - NAND Row Address
 * Return: None
 *
 * Description: Perform Address Operation
 *
 *************************************************************************/
void AddrInOperation(unsigned int Column, unsigned int Row) @ ".bootloader"
{
  /*Column*/
  for(int i = 0 ; 2 > i ; i++)
  {
    /*Load Column*/
    NAND_FLASH_ADD = Column & 0xFF;
    /*Start Address Operation*/
    NAND_FLASH_CONFIG2 = PERFORM_ADDR;
    /*Wait operation to complete*/
    while(!NAND_FLASH_CONFIG2_bit.INT);
    /*Shift Column*/
     Column>>=8;
  }
  /*Row*/
  for(int i = 0 ; 3 > i ; i++)
  {
    /*Load Row*/
    NAND_FLASH_ADD = Row & 0xFF;
    /*Start Address Operation*/
    NAND_FLASH_CONFIG2 = PERFORM_ADDR;
    /*Wait operation to complete*/
    while(!NAND_FLASH_CONFIG2_bit.INT);
    /*Shift Row*/
     Row>>=8;
  }
}

/*************************************************************************
 * Function name: DataOutOperation
 * Parameters:  PageSize - Main + Spare of Spare Only
 *
 * Return: None
 *
 * Description: Perform Data Out Operation(main+spare or spare only)
 *              from NFC Buffer
 *************************************************************************/
void DataOutOperation(t_DataOutOp PageSize) @ ".bootloader"
{
  /*Enable ECC*/
  NAND_FLASH_CONFIG1_bit.ECC_EN = 1;
  /*Set Page Size*/
  if(MAIN_SPARE == PageSize)
  {
    NAND_FLASH_CONFIG1_bit.SP_EN = 0; /*Main + Spare arry*/
  }
  else
  {
    NAND_FLASH_CONFIG1_bit.SP_EN = 1; /*Spare arry only*/
  }
  /*Index to the Data Buffer*/
  RAM_BUFFER_ADDRESS_bit.RBA = 0;
  /*Start Data Out Operation*/
  NAND_FLASH_CONFIG2 = PERFORM_DATAOUT;
  /*Wait operation to complete*/
  while(!NAND_FLASH_CONFIG2_bit.INT);
}

/*************************************************************************
 * Function name: NandGetStatus
 * Parameters:  None
 *
 * Return: None
 *
 * Description: Reads NAND Flash Status in to the RBA 0
 *
 *************************************************************************/
void NandGetStatus(void) @ ".bootloader"
{
  /*Read ID Command*/
  CmdInOperation(0x70);
  /*Load to the firs buffer*/
  RAM_BUFFER_ADDRESS_bit.RBA = 0x0;
  /*Start Address Operation*/
  NAND_FLASH_CONFIG2  = PERFORM_STATUS;
  /*Wait operation to complete*/
  while(!NAND_FLASH_CONFIG2_bit.INT);
}

/*************************************************************************
 * Function Name: NandReadPage
 * Parameters: Page - NAND Flash Page Number. In is Block*NAND_PG_PER_BLK + Page_in_Block
 *
 * Return: FLASH_OK - No Read Error
 *         FLASH_ERROR - Non Correcable Read Error
 * Description: Reads one page (4096 main+104 spera) from NAND Flash into RBA
 *
 *************************************************************************/
unsigned int NandReadPage(unsigned int Page) @ ".bootloader"
{
    /*Read*/
    CmdInOperation(0x00);
    /*Set Address*/
    AddrInOperation(0,Page);
    /**/
    CmdInOperation(0x30);
    /**/
    DataOutOperation(MAIN_SPARE);
    /*Non Correctable error*/
    if((0xF == ECC_STATUS_RESULT1_bit.NOSER1) || (0xF == ECC_STATUS_RESULT1_bit.NOSER2) ||
       (0xF == ECC_STATUS_RESULT1_bit.NOSER3) || (0xF == ECC_STATUS_RESULT1_bit.NOSER4) ||
       (0xF == ECC_STATUS_RESULT2_bit.NOSER5) || (0xF == ECC_STATUS_RESULT2_bit.NOSER6) ||
       (0xF == ECC_STATUS_RESULT2_bit.NOSER7) || (0xF == ECC_STATUS_RESULT2_bit.NOSER8) )
        return FLASH_ERROR;

    return FLASH_OK;
}

/*************************************************************************
 * Function Name: NandCheckBlock
 * Parameters: Block - NAND Block Number.
 *
 * Return: FLASH_OK -Valid Block
 *         FLASH_ERROR - Invalid Block
 * Description: Checks The Invalid Bytes in the Block
 *
 *************************************************************************/
unsigned int NandCheckBlock(unsigned int Block) @ ".bootloader"
{
  /**/
  return NandReadPage(Block*NAND_PG_PER_BLK+127);
}

/*************************************************************************
 * Function Name: NandGetValidBlock
 * Parameters: Block - NAND Block Number. Start from this block
 *
 * Return: Block < NAND_BLK_NUM NAND -  Valid Block Number.
 *         NAND_BLK_NUM - No Valid Block
 * Description: Finds a vlaid Block.
 *
 *************************************************************************/
unsigned int NandFindValidBlock(unsigned int Block) @ ".bootloader"
{
  while(NAND_BLK_NUM > Block)
  {/**/
    if(FLASH_OK == NandCheckBlock(Block)) break;
    Block++;
  }
  return Block;
}
