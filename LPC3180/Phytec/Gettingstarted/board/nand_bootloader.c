/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : bootloader.c
 *    Description : bootloader module - initialize SDRAM, copy image from
 *    NAND memory to SDRAM memory and then jump to application entry point
 *
 *    History :
 *    1. Date        : December 15, 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 28 $
 *
 **************************************************************************/
#include "arm_comm.h"

#include <NXP/iolpc3180.h>

#pragma segment=".bootloader"
#pragma segment=".iram_buffer"

/** local definitions **/
#define NAND_MAIN_SIZE              (512)     /*The NFC reads/writes 512 bytes of the main array*/
#define NAND_SPARE_SIZE             (6)       /*The NFC reads/writes 16 bytes of the spare array*/
#define NAND_SPARE_ECC_SIZE         (10)      /*The NFC reads/writes 16 bytes of the spare array*/
#define NAND_PG_PER_BLK             (32)
#define NAND_BLK_NUM                (2048)

#define NAND_PAGE_USR_SIZE          (NAND_MAIN_SIZE+NAND_SPARE_SIZE)
#define NAND_PAGE_SIZE              (NAND_PAGE_USR_SIZE + NAND_SPARE_ECC_SIZE)
#define NAND_BLK_SIZE               (NAND_MAIN_SIZE*NAND_PG_PER_BLK)  /* (only main arrays)*/

#define FLASH_SER_BASE_ADDR         0x200B0000
#define SERIAL_BUF_BASE_ADDR        0x200A8000
#define SERIAL_BUF_BASE_ADDR        0x200A8000
#define SDRAM_BASE_ADDR             0x80000000

// NAND Flash device commands
#define NAND_RST                    0xFF
#define NAND_READ_A                 0x00
#define NAND_READ_B                 0x01
#define NAND_READ_C                 0x50
#define NAND_READ_ID                0x90
#define NAND_READ_STAT              0x70

#define NAND_PGM_PAGE1              0x80
#define NAND_PGM_PAGE2              0x10

#define NAND_BLOCK_ERASE1           0x60
#define NAND_BLOCK_ERASE2           0xD0

// NAND Flash status register
#define NAND_WRITE_PROT             0x80
#define NAND_ACTIVE                 0x40
#define NAND_ERROR                  0x01

//flash functions return value
#define FLASH_OK                    ( 0 )
#define FLASH_ERROR                 ( 1 )

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


static void AddrInOperation(unsigned int Page);
static unsigned int NandReadPage(unsigned int Page);
static unsigned int NandReadSpare(unsigned int Page);
static unsigned int NandFindValidBlock(unsigned int Block);
static unsigned int NandCheckBlock(unsigned int Block);

#pragma location=".iram_buffer"
#pragma data_alignment=4
unsigned char Buffer[NAND_PAGE_USR_SIZE];

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
void Dly_us(Int32U dly)
{
  while(dly--)
  {
    for(volatile int i = 20; i; i --);
  }
}

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
unsigned int * pSrc;
unsigned int * pDest;
unsigned int block,page;
#define LED_D305      (1UL << 2)
#define LED_D306      (1UL << 3)
#define LED_D308      (1UL << 5)
#define LED_D310      (1UL << 6)

  // Enable MLC NAND controller
  FLASHCLK_CTRL = 2;
  // CE controlled by NAND controller
  MLC_CEH = 1;
  // unlocks the access to MLC NAND Timing register
  MLC_LOCK_PR  = 0xA25E;
  MLC_ICR = 0;

  MLC_LOCK_PR  = 0xA25E;
  MLC_TIME_REG =( 3UL << 0 ) |  // Write pulse width (tWP)
                ( 2UL << 4 ) |  // Write high hold time (tWH)
                ( 3UL << 8 ) |  // Read pulse width (tRP)
                ( 2UL << 12) |  // Read high hold time (tREH)
                ( 4UL << 16) |  // Read high to high impedance (tRHZ)
                ( 2UL << 19) |  // Read/Write high to busy (tWB/tRB)
                ( 5UL << 24);   // nCE low to dout valid (tCEA)

  // init clock
  SYSCLK_CTRL = 0x50<<2;
  // Set PER_CLK and HCLK dividers
  HCLKDIV_CTRL = 0x0000003D;
  // init H PLL
  HCLKPLL_CTRL = 0x0001601E;
  // Wait until PLL lock
  while(!HCLKPLL_CTRL_bit.PLL_LOCK);
  // Connect Pll_clk_out
  PWR_CTRL_bit.RUN_MODE = 1;

  // Exiting form the self-refresh mode
  PWR_CTRL &= ~(0x3UL << 8);
  PWR_CTRL_bit.MPMCSREFREQ_UPDATE = 1;
  PWR_CTRL_bit.MPMCSREFREQ_UPDATE = 0;

  MPMCControl_bit.E = 1;  // SDRAM Controller Enable
  MPMCControl_bit.L = 0;  // SDRAM normal mode

  MPMCConfig_bit.N  = 0;  // little-endian mode.

  // disable write buffers
  MPMCAHBControl0_bit.E = 0;
  MPMCAHBControl2_bit.E = 0;
  MPMCAHBControl3_bit.E = 0;
  MPMCAHBControl4_bit.E = 0;

  SDRAMCLK_CTRL      = 0x0001C000;
  MPMCDynamictRP     = 1;             // 19ns
  MPMCDynamictRAS    = 4;             // 48ns
  MPMCDynamictSREX   = 8;             // 80ns
  MPMCDynamictWR     = 1;             // 15ns
  MPMCDynamictRC     = 8;             // 80ns
  MPMCDynamictRFC    = 6;             // 64ns
  MPMCDynamictXSR    = 8;             // 80ns
  MPMCDynamictRRD    = 1;             // 16ns
  MPMCDynamictMRD    = 1;             // 2ckl
  MPMCDynamictCDLR   = 0;             // 1ckl
  MPMCDynamicConfig0 = 0x00005282;    // low power SDR SDRAM 64Mb (4Mx16), 4 banks,
                                      // row length = 12, column length = 8
  MPMCDynamicRasCas0 = 0x00000203;    // CAS = 4 - two clock cycles
                                      // RAS = 2 - 19ns

  // SDRAM controller and device settings
  // Init SDRAM controller and memory
  // SDRAM_D[31:19] are connected to the SDRAM controller.
  PIO_MUX_CLR_bit.GPIO_SDRAM_SEL = 1;

  MPMCDynamicReadConfig = 0x11;

  // JEDEC General SDRAM Initialization Sequence
  MPMCDynamicControl = 0x193;        // NOP
  Dly_us(200);
  MPMCDynamicRefresh = 1;
  MPMCDynamicControl_bit.I = 2;     // PRECHARGE ALL
  Dly_us(100);
  MPMCDynamicControl_bit.I = 0;     // AUTO REFRESH
  Dly_us(100);
  MPMCDynamicRefresh = 101;         // 104*15.625/16
  MPMCDynamicControl_bit.I = 1;     // SDRAM MODE
  // Burst Length, CAS, Burst Type - Sequential
  volatile unsigned long Dummy = *(unsigned long *)(SDRAM_BASE_ADDR | 0x8000);
  MPMCDynamicControl_bit.I = 0;     // OPERATION
  MPMCDynamicControl_bit.CE= 0;
  MPMCDynamicControl_bit.CS= 0;

  BOOT_MAP_bit.MAP = 1;             // iRAM 0x00000000

  // Set LEDs off (LED_D305 - GPO3  LED_D306 - GPO4 LED_D308 GPO5  LED_D310 - GPO6)
  PIO_OUTP_SET = LED_D305 | LED_D306 | LED_D308 | LED_D310;

  block = NandFindValidBlock(1);
  page = 0;  /*Start from page 1 of the block*/


  while((NAND_BLK_NUM) > (block))
  {
    if(FLASH_ERROR == NandReadPage(block*NAND_PG_PER_BLK + page))
    {
      PIO_OUTP_CLR = LED_D305;
      while(1); /*Read Error*/
    }
    /*Source address */
    pSrc = (unsigned int *)Buffer;
    /*Dest address = Last Word of the Spare Array*/
    /*Source address first 4 bytes from page's spare area */
    pDest = (unsigned int *)(*(unsigned int *)(Buffer + NAND_MAIN_SIZE));

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
      if(0 == (block & 0x3F))
      {
        if(block & 1)
        {
          PIO_OUTP_CLR = LED_D310;
        }
        else
        {
          PIO_OUTP_SET = LED_D310;
        }
      }
    }
  }
}

/*************************************************************************
 * Function name: AddrInOperation
 * Parameters:  unsigned int Page
 *
 * Return: None
 *
 * Description: Perform Address Operation
 *
 *************************************************************************/
#pragma location=".bootloader"
static void AddrInOperation(unsigned int Page)
{
  /*Row*/
  MLC_ADDR = 0;
  for(int i = (MLC_ICR_bit.ADD_WORD_4)?(4-1):(3-1) ; i ; --i)
  {
    /*Load Row*/
    MLC_ADDR = Page & 0xFF;
    /*Shift Row*/
    Page>>=8;
  }
}

/*************************************************************************
 * Function Name: NandReadPage
 * Parameters: Page - NAND Flash Page Number.
 *
 * Return: FLASH_OK - No Read Error or 1bit Error
 *         FLASH_ERROR - Non Correctable Read Error
 * Description: Reads one page (512 main+6 spare) from NAND Flash into Buffer
 *
 *************************************************************************/
#pragma location=".bootloader"
static unsigned int NandReadPage(unsigned int Page)
{
unsigned char * pb;
    /*Read*/
    MLC_CMD = NAND_READ_A;
    /*Set Address*/
    AddrInOperation(Page);

    MLC_ECC_AUTO_DEC_REG = 0;
    while(!MLC_ISR_bit.CONT_READY);
    if(MLC_ISR_bit.DECODER_FAULT)
    {
      return(FLASH_ERROR);
    }
    pb = Buffer;
    for (unsigned int Counter = 0; Counter < NAND_PAGE_USR_SIZE; ++Counter)
    {
      *pb++ = *(unsigned char *)SERIAL_BUF_BASE_ADDR;
    }

    return FLASH_OK;
}

/*************************************************************************
 * Function Name: NandReadSpare
 * Parameters: Page - NAND Flash Page Number.
 *
 * Return: FLASH_OK - No Read Error
 *         FLASH_ERROR - Non Correctable Read Error
 * Description: Reads the Spare array of the page (6 Bytes) into Buffer
 *
 *************************************************************************/
#pragma location=".bootloader"
static unsigned int NandReadSpare(unsigned int Page)
{
unsigned char * pb;
  /*Read*/
  MLC_CMD = NAND_READ_C;
  /*Set Address*/
  AddrInOperation(Page);
  /* restart ECC */
  MLC_ECC_AUTO_DEC_REG = 0;
  while(!MLC_ISR_bit.CONT_READY);
  if(MLC_ISR_bit.DECODER_FAULT)
  {
    return(FLASH_ERROR);
  }
  pb = Buffer;
  for (unsigned int Counter = 0; Counter < NAND_SPARE_SIZE; ++Counter)
  {
    *pb++ = *(unsigned char *)SERIAL_BUF_BASE_ADDR;
  }
  return FLASH_OK;
}

/*************************************************************************
 * Function Name: Nanotechnology
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
  for(unsigned int Page = 0; 2 > Page; Page++)
  {
    NandReadSpare(Block*NAND_PG_PER_BLK+Page);
    if(0xFF != *((unsigned char *)(Buffer+5)))
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
