/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : bootloader.c
 *    Description : bootloader module - initialize SDRAM, copy image from
 *    NAND memory to SDRAM memory and then jump to application entry point
 *
 *    History :
 *    1. Date        : May 1, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 5880 $
 *
 **************************************************************************/
#include "arm_comm.h"

#include <NXP/iomcimx27.h>

#pragma segment=".bootloader"


#define FLASH_BASE_ADDR       0xD8000000
#define NAND_SPARE_OFFSET     0x800

#define NAND_MAIN_SIZE        (512)     /*The NFC reads/writes 512 bytes of the main array*/
#define NAND_SPARE_SIZE       (16)      /*The NFC reads/writes 16 bytes of the spare array*/
#define NAND_PG_PER_BLK       (32)
#define NAND_BLK_NUM          (4096)

#define NAND_PAGE_SIZE        (NAND_MAIN_SIZE+NAND_SPARE_SIZE)
#define NAND_BLK_SIZE         (NAND_MAIN_SIZE*NAND_PG_PER_BLK)  /* (only main arrays)*/
#define NAND_PG_NUM           (NAND_BLK_NUM  *NAND_PG_PER_BLK)

#define PERFORM_CMD           0x1
#define PERFORM_ADDR          0x2
#define PERFORM_DATAIN        0x4
#define PERFORM_DATAOUT       0x8
#define PERFORM_IDOUT         0x10
#define PERFORM_STATUS        0x20

#define NAND_STATUS           *((volatile unsigned short *)FLASH_BASE_ADDR)

//flash functions return value
#define FLASH_OK              ( 0 )
#define FLASH_ERROR           ( 1 )

typedef enum _t_DataOutOp
{
  MAIN_SPARE,
  SPARE
} t_DataOutOp;

typedef enum _t_codetype
{
  FLASH_BOOT_CODE,
  FLASH_USER_CODE
} t_codetype;

typedef enum _dev_bat_block_hadling_t
{
  PRESERVE_BAD_BLOCKS = 0,
  CLEAR_BAD_BLOCKS
} dev_bat_block_hadling_t;


static void CmdInOperation(unsigned short cmd);
static void AddrInOperation(unsigned int Page);
static void DataOutOperation(t_DataOutOp PageSize, unsigned int Buff_Index);
static void NandGetStatus(void);
static unsigned int NandReadPage(unsigned int Page,unsigned int Buff_Index);
static unsigned int NandReadSpare(unsigned int Page,unsigned int Buff_Index);
static unsigned int NandCheckBlock(unsigned int Block);
static unsigned int NandFindValidBlock(unsigned int Block);

extern __arm void __iar_program_start (void);
/*************************************************************************
 * Function Name: low_level_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Low level init code
 *
 *************************************************************************/
#pragma location=".bootloader"
__arm void bootloader (void)
{
Int32U * pSrc;
Int32U * pDest;
Int32U block,page;

  // AHB-Lite IP Interface
  AIPI1_PSR0 = 0x00040304;
  AIPI1_PSR1 = 0xFFFBFCFB;
  AIPI2_PSR0 = 0x3FFC0000;
  AIPI2_PSR1 = 0xFFFFFFFF;
  AIPI1_PAR  = 0xFFFFFFFF;
  AIPI2_PAR  = 0xFFFFFFFF;
	// configure peripheral enables for PCCR0
  PCCR0 = 0;
	// configure peripheral enables for PCCR1
  PCCR1 = 0xFF4B6848;
  // Set Peripheral Clock Divider Registers
  PCDR0 = 0x200837C3;
  PCDR1 = 0x03030303;

#ifndef DDRRAM_EXEC

  if(0x00331C23 != MPCTL0)
  {
    // 26MHz OSC adjust
    CSCR_bit.OSC26M_DIV1P5 = 0;
    CSCR_bit.OSC26M_DIS    = 0;
    // wait some time for OSC stabilization
    // depend of OSC type and load capacitors
    for(volatile Int32U i = 100000; i; --i);
    // trim the OSC amplitude
    OSC26MCTL_bit.AGC = 0x3F;
    do
    {
      // delay 30.5 us
      for(volatile Int32U i = 1000; i; --i);
      if(OSC26MCTL_bit.OSC26M_PEAK == 0)
      {
        OSC26MCTL_bit.AGC -= 4;
        break;
      }
    }
    while(--OSC26MCTL_bit.AGC > 4);

    // Set maximum clock divistion
    CSCR_bit.ARM_DIV = 0x3;
    CSCR_bit.AHB_DIV = 0x3;
    CSCR_bit.ARMSRC  = 0x0;   // PLL postdivider 3/2
    CSCR_bit.MCU_SEL = 0x1;   // Ext. 26MHz OSC
    CSCR_bit.SP_SEL  = 0x1;   // Ext. 26MHz OSC

    // configure MPCTL for 399 MHz
    MPCTL0 = 0x00331C23;

    // restart MPCTL
    // after changing the operating
    // frequencies
    CSCR_bit.MPLL_RESTART = 1;		

    // FCLK = 266Hz, HCLK = 133 MHz
    CSCR_bit.ARM_DIV = 0x0;
    CSCR_bit.AHB_DIV = 0x1;

    // Init DDRRAM 0xA0000000 - 0xA7FFFFFF
    // DDR 4x16Mx16 x 2
    // Row Address = 14
    // Col Address = 10
    // ESDMISC: enable DDR mode
    ESDMISC = 0x00000004;
    // ESDCFG0: timing config
    ESDCFG0 = 0x006AC73A;
    // ESDCTL0: Precharge Mode
    ESDCTL0 = 0x92100000;
    // issue Precharge All command
    *(volatile unsigned int *)0xA0000400 = 0;
    // ESDCTL0: AutoRefresh Mode
    ESDCTL0 = 0xA2100000;
    // issue Autorefresh command
    *(volatile unsigned int *)0xA0000000 = 0;
    *(volatile unsigned int *)0xA0000000 = 0;
    // ESDCTL0: Load Mode register Mode
    ESDCTL0 = 0xB2100000;
    // issue Load Mode Register command
    *(volatile unsigned char *)0xA0000033 = 0;
    *(volatile unsigned char *)0xA1000000 = 0;
    // ESDCTL0: Normal Mode
    // Row Address  = 13
    // Col Address  = 10
    // Data width   = 32-bit
    // Refresh Rate = 8192/64ms
    // Burst Length = 8
    // - 2 : 32 bit mode
    // - 1 : 16 bit - D[15:00]
    // - 0 : 16 bit - D[31:16]
    ESDCTL0 = 0x82226080;
    // issue normal access
    *(volatile unsigned int *)0xA0000000 = 0;
    // ESDMISC: Reset Delay Line Measurement
    ESDMISC = 0x0000000C;
	}

#endif // DDRRAM_EXEC

  // MUX
  PCSR = 0x00000003;  // Set high priority masters (0,1)
  // init Slave General Purpose Control Register
  SGPCR0 = SGPCR1 = SGPCR2 = 0x00000100;
  // init Master Priority Register
  MPR0 = MPR1 = MPR2 =  0x00543210;
  // init Master General Purpose Control Register
  MGPCR0 = MGPCR1 = MGPCR2 = 2;

  /*Start from Block 1*/
  /*Reset NAND Flash*/
  CmdInOperation(0xFF);
  /*Unlock RAM Buffer*/
  NFC_CONFIGURATION = 0x2;

  block = NandFindValidBlock(1);
  page = 0;  /*Start from page 1 of the block*/

  while((NAND_BLK_NUM) > (block))
  {
    if(FLASH_ERROR == NandReadPage(block*NAND_PG_PER_BLK + page,0)) while(1); /*Read Error*/
    /*Source address =  NFC RAM buffer address*/
    pSrc = (unsigned int *)(FLASH_BASE_ADDR);
    /*Dest address = Last Word of the Spare Array*/
    pDest = *((unsigned int* *)(FLASH_BASE_ADDR+NAND_SPARE_OFFSET+12));

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
  __iar_program_start();
}

/*************************************************************************
 * Function name: CmdInOperation
 * Parameters:  cmd - the Command to send
 *
 * Return: None
 *
 * Description: Perform  Command Operaion
 *
 *************************************************************************/
#pragma location=".bootloader"
static void CmdInOperation(unsigned short cmd)
{
  /*Load the Command*/
  NAND_FLASH_CMD = cmd;
  /*Start Command Operation*/
  NAND_FC2 = PERFORM_CMD;
  /*Wait operation to complete*/
  while(!NAND_FC2_bit.INT);

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
#pragma location=".bootloader"
static void AddrInOperation(unsigned int Page)
{
  /*Load Column*/
  NAND_FLASH_ADD = 0;
  /*Start Address Operation*/
  NAND_FC2 = PERFORM_ADDR;
  /*Wait operation to complete*/
  while(!NAND_FC2_bit.INT);
  /*Row*/
  for(int i = 0 ; 3 > i ; i++)
  {
    /*Load Row*/
    NAND_FLASH_ADD = Page & 0xFF;
    /*Start Address Operation*/
    NAND_FC2 = PERFORM_ADDR;
    /*Wait operation to complete*/
    while(!NAND_FC2_bit.INT);
    /*Shift Row*/
     Page>>=8;
  }
}

/*************************************************************************
 * Function name: DataOutOperation
 * Parameters:  PageSize - Main + Spare or Spare Only
 *              Buffer_Index - Index to NFC Buffer (0-3)
 * Return: None
 *
 * Description: Perform Data Out Operation(main+spare or spare only)
 *              from NFC Buffer
 *************************************************************************/
#pragma location=".bootloader"
static void DataOutOperation(t_DataOutOp PageSize, unsigned int Buff_Index)
{
  /*Enable ECC*/
  NAND_FC1_bit.ECC_EN = 1;
  /*Set Page Size*/
  if(MAIN_SPARE == PageSize)
  {
    NAND_FC1_bit.SP_EN = 0; /*Main + Spare arry*/
  }
  else
  {
    NAND_FC1_bit.SP_EN = 1; /*Spare arry only*/
  }
  /*Index to the Data Buffer*/
  NFC_RBA = Buff_Index;
  /*Start Data Out Operation*/
  NAND_FC2 = PERFORM_DATAOUT;
  /*Wait operation to complete*/
  while(!NAND_FC2_bit.INT);
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
#pragma location=".bootloader"
static void NandGetStatus(void)
{
  /*Read ID Command*/
  CmdInOperation(0x70);
  /*Load to the firs buffer*/
  NFC_RBA = 0x0;
  /*Start Address Operation*/
  NAND_FC2 = PERFORM_STATUS;
  /*Wait operation to complete*/
  while(!NAND_FC2_bit.INT);
}

/*************************************************************************
 * Function Name: NandReadPage
 * Parameters: Page - NAND Flash Page Number. In is Block*NAND_PG_PER_BLK + Page_in_Block
 *             Buffer_Index - Index to NFC Buffer (0-3)
 * Return: FLASH_OK - No Read Error or 1bit Error
 *         FLASH_ERROR - Non Correcable Read Error
 * Description: Reads one page (512 main+16 spera) from NAND Flash into RBA
 *
 *************************************************************************/
#pragma location=".bootloader"
static unsigned int NandReadPage(unsigned int Page,unsigned int Buff_Index)
{
    /*Read*/
    CmdInOperation(0x00);
    /*Set Address*/
    AddrInOperation(Page);
    /**/
    DataOutOperation(MAIN_SPARE,Buff_Index);
    /*Non Correctable error*/
    if((2 == ECC_SRR_bit.ERM) || (2 == ECC_SRR_bit.ERS)) return FLASH_ERROR;

    return FLASH_OK;
}
/*************************************************************************
 * Function Name: NandReadSpare
 * Parameters: Page - NAND Flash Page Number. In is Block*NAND_PG_PER_BLK + Page_in_Block
 *             Buffer_Index - Index to NFC Buffer (0-3)
 * Return: FLASH_OK - No Read Error or 1bit Error
 *         FLASH_ERROR - Non Correcable Read Error
 * Description: Reads the Spare arrey of the page (16 Bytes) into RBA
 *
 *************************************************************************/
#pragma location=".bootloader"
static unsigned int NandReadSpare(unsigned int Page,unsigned int Buff_Index)
{
    /*Read*/
    CmdInOperation(0x00);
    /*Set Address*/
    AddrInOperation(Page);
    /**/
    DataOutOperation(MAIN_SPARE,Buff_Index);
    /*Non Correctable error*/
    if((2 == ECC_SRR_bit.ERS)) return FLASH_ERROR;

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
#pragma location=".bootloader"
static unsigned int NandCheckBlock(unsigned int Block)
{
unsigned int Page;
  /**/
  for(Page = 0; 2 > Page; Page++)
  {
    NandReadSpare(Block*NAND_PG_PER_BLK+Page,0);
    if(0xFF00 != (*((unsigned int * )(FLASH_BASE_ADDR+NAND_SPARE_OFFSET+4))&0xFF00))
    {
      return FLASH_ERROR;
    }
  }
  return FLASH_OK;
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
#pragma location=".bootloader"
static unsigned int NandFindValidBlock(unsigned int Block)
{
  while(NAND_BLK_NUM > Block)
  {/**/
    if(FLASH_OK == NandCheckBlock(Block)) break;
    Block++;
  }
  return Block;
}
