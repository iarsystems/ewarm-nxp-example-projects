/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2008
 *
 *    File name   : ttbl.c
 *    Description : MMU Translation tables
 *
 *    History :
 *    1. Date        : October 2008
 *       Author      : Stoyan Cnoynev
 *       Description : iMX31 PDK translation tables
 *
 *        The physical memory map is:
 *                                                                        Instruction cache
 *                                                                          Data cache
 *                                                                            User Access
 *                                                                              Supervisor Access
 *                                                                                Read Only
 *  Segment                     Description       Size                    I D U S R
 *  0x00000000 - 0x00003FFF     Secure ROM        16 Kbyte                * - * * *
 *  0x00004000 - 0x00403FFF     Reserved          4 Mbyte–16Kbyte         - - - - -
 *  0x00404000 - 0x00407FFF     ROM               16 Kbyte                * - * * *
 *  0x00408000 - 0x0FFFFFFF     Reserved          252 Mbyte–32 Kbyte      - - - - -
 *  0x10000000 - 0x1FFFBFFF     Reserved for      256 Mbyte–16 Kbyte      - - - - -
 *                              RAM aliasing
 *  0x1FFFC000 - 0x1FFFFFFF     RAM               16 Kbyte                * * * * -
 *  0x20000000 - 0x2FFFFFFF     Reserved          256 Mbyte               - - - - -
 *  0x30000000 - 0x3FFFFFFF     L2CC              256 Mbyte               - - - * -
 *  0x40000000 - 0x41FFFFFF     AIPS A off        32 Mbyte                - - - * -
 *                              platform global
 *                              module enable #0
 *  0x42000000 - 0x43EFFFFF     AIPS A off        31 Mbyte                - - - * -
 *                              platform global
 *                              module enable #1
 *  0x43F00000 - 0x43F03FFF     AIPS A Control    16 Kbyte                - - - * -
 *  0x43F04000 - 0x43F07FFF     MAX               16 Kbyte                - - - * -
 *  0x43F08000 - 0x43F0BFFF     EVTMON            16 Kbyte                - - - * -
 *  0x43F0C000 - 0x43F0FFFF     CLKCTL            16 Kbyte                - - - * -
 *  0x43F10000 - 0x43F13FFF     ETB registers     16 Kbyte                - - - * -
 *  0x43F14000 - 0x43F17FFF     ETB Memory        16 Kbyte                - - - * -
 *  0x43F18000 - 0x43F1BFFF     CTIO              16 Kbyte                - - - * -
 *  0x43F1C000 - 0x43F7FFFF     Reserved          400 Kbyte               - - - - -
 *                              AIPS A on
 *                              platform slots
 *  0x43F80000 - 0x43F83FFF     I2C               16 Kbyte                - - - * -
 *  0x43F84000 - 0x43F87FFF     I2C3              16 Kbyte                - - - * -
 *  0x43F88000 - 0x43F8BFFF     USBOTG            16 Kbyte                - - - * -
 *  0x43F8C000 - 0x43F8FFFF     ATA control port  16 Kbyte                - - - * -
 *  0x43F90000 - 0x43F93FFF     UART1             16 Kbyte                - - - * -
 *  0x43F94000 - 0x43F97FFF     UART2             16 Kbyte                - - - * -
 *  0x43F98000 - 0x43F9BFFF     I2C2              16 Kbyte                - - - * -
 *  0x43F9C000 - 0x43F9FFFF     1-WIRE            16 Kbyte                - - - * -
 *  0x43FA0000 - 0x43FA3FFF     SSI1              16 Kbyte                - - - * -
 *  0x43FA4000 - 0x43FA7FFF     CSPI1             16 Kbyte                - - - * -
 *  0x43FA8000 - 0x43FABFFF     KPP               16 Kbyte                - - - * -
 *  0x43FAC000 - 0x43FAFFFF     IOMUXC            16 Kbyte                - - - * -
 *  0x43FB0000 - 0x43FB3FFF     UART4             16 Kbyte                - - - * -
 *  0x43FB4000 - 0x43FB7FFF     UART5             16 Kbyte                - - - * -
 *  0x43FB8000 - 0x43FBBFFF     ECT (IP BUS 1)    16 Kbyte                - - - * -
 *  0x43FBC000 - 0x43FBFFFF     ECT (IP BUS 2)    16 Kbyte                - - - * -
 *  0x43FC0000 - 0x43FFFFFF     Reserved AIPS A   256 Kbyte               - - - - -
 *  0x44000000 - 0x4FFFFFFF     Reserved          192 Mbyte               - - - - -
 *  0x50000000 - 0x50003FFF     SPBA base address 16 Kbyte                - - - - -
 *  0x50004000 - 0x50007FFF     MMC/SDHC1         16 Kbyte                - - - * -
 *  0x50008000 - 0x5000BFFF     MMC/SDHC2         16 Kbyte                - - - * -
 *  0x5000C000 - 0x5000FFFF     UART3             16 Kbyte                - - - * -
 *  0x50010000 - 0x50013FFF     CSPI2             16 Kbyte                - - - * -
 *  0x50014000 - 0x50017FFF     SSI2              16 Kbyte                - - - * -
 *  0x50018000 - 0x5001BFFF     SIM               16 Kbyte                - - - * -
 *  0x5001C000 - 0x5001FFFF     IIM               16 Kbyte                - - - * -
 *  0x50020000 - 0x50023FFF     ATA (DMA port)    16 Kbyte                - - - * -
 *  0x50024000 - 0x50027FFF     MSHC1             16 Kbyte                - - - * -
 *  0x50028000 - 0x5002BFFF     MSHC2             16 Kbyte                - - - * -
 *  0x5002C000 - 0x5002FFFF     Reserved          16 Kbyte                - - - - -
 *  0x50030000 - 0x50033FFF     Reserved          16 Kbyte                - - - - -
 *  0x50034000 - 0x50037FFF     Reserved          16 Kbyte                - - - - -
 *  0x50038000 - 0x5003BFFF     Reserved          16 Kbyte                - - - - -
 *  0x5003C000 - 0x5003FFFF     SPBA Registers    16 Kbyte                - - - * -
 *  0x50040000 - 0x51FFFFFF     AIPS B off        32 Mbyte-256 Kbyte      - - - - -
 *                              platform global
 *                              module enable #0
 *  0x52000000 - 0x53EFFFFF     AIPS B off        31 Mbyte                - - - - -
 *                              platform global
 *                              module enable #1
 *  0x53F00000 - 0x53F03FFF     AIPS B Control    16 Kbyte                - - - * -
 *  0x53F04000 - 0x53F7FFFF     Reserved AIPS B   496 Kbyte               - - - - -
 *  0x53F80000 - 0x53F83FFF     CCM               16 Kbyte                - - - * -
 *  0x53F84000 - 0x53F87FFF     CSPI3             16 Kbyte                - - - * -
 *  0x53F88000 - 0x53F8BFFF     Reserved          16 Kbyte                - - - * -
 *  0x53F8C000 - 0x53F8FFFF     FIR               16 Kbyte                - - - * -
 *  0x53F90000 - 0x53F93FFF     GPT               16 Kbyte                - - - * -
 *  0x53F94000 - 0x53F97FFF     EPIT1             16 Kbyte                - - - * -
 *  0x53F98000 - 0x53F9BFFF     EPIT2             16 Kbyte                - - - * -
 *  0x53F9C000 - 0x53F9FFFF     Reserved          16 Kbyte                - - - * -
 *  0x53FA0000 - 0x53FA3FFF     Reserved          16 Kbyte                - - - * -
 *  0x53FA4000 - 0x53FA7FFF     GPIO3             16 Kbyte                - - - * -
 *  0x53FA8000 - 0x53FABFFF     Reserved          16 Kbyte                - - - - -
 *  0x53FAC000 - 0x53FAFFFF     SCC               16 Kbyte                - - - * -
 *  0x53FB0000 - 0x53FB3FFF     RNGA              16 Kbyte                - - - * -
 *  0x53FB4000 - 0x53FB7FFF     Reserved          16 Kbyte                - - - * -
 *  0x53FB8000 - 0x53FBBFFF     Reserved          16 Kbyte                - - - * -
 *  0x53FBC000 - 0x53FBFFFF     Reserved          16 Kbyte                - - - * -
 *  0x53FC0000 - 0x53FC3FFF     IPU               16 Kbyte                - - - * -
 *  0x53FC4000 - 0x53FC7FFF     AUDMUX            16 Kbyte                - - - * -
 *  0x53FC8000 - 0x53FCBFFF     MPEG4_Encoder     16 Kbyte                - - - * -
 *  0x53FCC000 - 0x53FCFFFF     GPIO1             16 Kbyte                - - - * -
 *  0x53FD0000 - 0x53FD3FFF     GPIO2             16 Kbyte                - - - * -
 *  0x53FD4000 - 0x53FD7FFF     SDMA              16 Kbyte                - - - * -
 *  0x53FD8000 - 0x53FDBFFF     RTC               16 Kbyte                - - - * -
 *  0x53FDC000 - 0x53FDFFFF     WDOG              16 Kbyte                - - - * -
 *  0x53FE0000 - 0x53FE3FFF     PWM               16 Kbyte                - - - * -
 *  0x53FE4000 - 0x53FE7FFF     Reserved          16 Kbyte                - - - - -
 *  0x53FE8000 - 0x53FEBFFF     Reserved          16 Kbyte                - - - - -
 *  0x53FEC000 - 0x53FEFFFF     RTIC              16 Kbyte                - - - - -
 *  0x53FF0000 - 0x53FF3FFF     Reserved          16 Kbyte                - - - - -
 *  0x53FF4000 - 0x53FF7FFF     Reserved          16 Kbyte                - - - - -
 *  0x53FF8000 - 0x53FFBFFF     Reserved          16 Kbyte                - - - - -
 *  0x53FFC000 - 0x53FFFFFF     Reserved          16 Kbyte                - - - - -
 *  0x54000000 - 0x5FFFFFFF     Reserved          192 Mbyte               - - - - -
 *  0x60000000 - 0x67FFFFFF     ROMPATCH          128 Mbyte               - - - * -
 *  0x68000000 - 0x6BFFFFFF     AVIC              128 Mbyte               - - - * -
 *  0x70000000 - 0x7FFFFFFF     IPU (MAX M2)      256 Mbyte               - - - * -
 *  0x80000000 - 0x8FFFFFFF     CSD0 SDRAM/DDR    256 Mbyte               * * * * -
 *  0x90000000 - 0x9FFFFFFF     CSD1 SDRAM/DDR    256 Mbyte               * * * * -
 *  0xA0000000 - 0xA7FFFFFF     CS0 128MB Flash   128 Mbyte               * * * * *
 *  0xA8000000 - 0xAFFFFFFF     CS1 64MB Flash    128 Mbyte               * * * * -
 *  0xB0000000 - 0xB1FFFFFF     CS2 (SRAM)        32 Mbyte                * * * * -
 *  0xB2000000 - 0xB3FFFFFF     CS3 (Spare)       32 Mbyte                * * * * -
 *  0xB4000000 - 0xB5FFFFFF     CS4 (Spare)       32 Mbyte                * * * * -
 *  0xB6000000 - 0xB7FFFFFF     CS5 (spare)       32 Mbyte                * * * * -
 *  0xB8000000 - 0xB8000FFF     NAND Flash        4 Kbyte                 - - - * -
 *  0xB8001000 - 0xB8001FFF     ESDCTL registers  4 Kbyte                 - - - * -
 *  0xB8002000 - 0xB8002FFF     WEIM registers    4 Kbyte                 - - - * -
 *  0xB8003000 - 0xB8003FFF     M3IF Registers    4 Kbyte                 - - - * -
 *  0xB8004000 - 0xB8004FFF     pcmcia registers  4 Kbyte                 - - - * -
 *  0xB8005000 - 0xBFFFFFFF     Reserved          128 Mbyte–20 Kbyte      - - - - -
 *  0xC0000000 - 0xC3FFFFFF     PCMCIA/CF         64 Mbyte                - - * * -
 *  0xC4000000 - 0xFFFFFFFF     Reserved          960 Mbyte               - - - - -
 *
 *  Domain 0 - 01 Client
 *  Domain 1 - 11 Manager
 *
 *    $Revision: 7859 $
 **************************************************************************/
#include "ttbl.h"
#pragma segment=".intvec"
#pragma segment="MMU_TT"

#pragma location="MMU_TT"
#pragma data_alignment=16384
__no_init Int32U L1Table[L1_ENTRIES_NUMB];

#pragma location="MMU_TT"
#pragma data_alignment=2048
__no_init Int32U L2Coarse1[L2_CP_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=2048
__no_init Int32U L2Coarse2[L2_CP_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=2048
__no_init Int32U L2Coarse3[L2_CP_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=2048
__no_init Int32U L2Coarse4[L2_CP_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=2048
__no_init Int32U L2Coarse5[L2_CP_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=2048
__no_init Int32U L2Coarse6[L2_CP_ENTRIES_NUMB];

extern void __vector(void);

const TtSectionBlock_t TtSB[] =
{
  /*L1 Table*/
  //Vector table Remap L1
  L1_COARSES_PAGE_ENTRY( 1,0x00000000,(Int32U)L2Coarse1,0),
  //Internal RAM L1
  L1_COARSES_PAGE_ENTRY( 1,0x1FF00000,(Int32U)L2Coarse2,0),
  //L2CC Registers 256MB 16x16MB
  L1_SUPERSECTIONS_ENTRY(16,0x30000000,0x30000000,0,3,0,0,1),
  //APIS A Peripherial L1
  L1_COARSES_PAGE_ENTRY( 1,0x43F00000,(Int32U)L2Coarse3,0),
  //AIPS B off platform global module enable #0
  L1_COARSES_PAGE_ENTRY( 1,0x50000000,(Int32U)L2Coarse4,0),
  //APIS B Peripherial
  L1_COARSES_PAGE_ENTRY( 1,0x53F00000,(Int32U)L2Coarse5,0),
  //ROMPACH & AVIC & IPU
  L1_SUPERSECTIONS_ENTRY(32,0x60000000,0x60000000,2,3,0,0,0),
  //CSD0 SDRAM/DDR Normal Memory
  L1_SUPERSECTIONS_ENTRY(4,0x80000000,0x80000000,0,3,0,1,1),
  //CSD0 SDRAM/DDR Strongly Ordered
  L1_SUPERSECTIONS_ENTRY(4,0x84000000,0x84000000,0,3,0,0,0),
  //CS5 CPLD
  L1_SUPERSECTIONS_ENTRY(2,0xB6000000,0xB6000000,0,3,0,0,1),
  //EIM L1
  L1_COARSES_PAGE_ENTRY( 1,0xB8000000,(Int32U)L2Coarse6,0),
  //PCMCIA
  L1_SUPERSECTIONS_ENTRY( 4,0xC0000000,0xC0000000,0,3,0,0,0),
  TSB_INVALID,/*L1 Table End*/
  /*L2 Table 1*/
  // Vector Table Remap L2 16KB 4*4 small
  L2_SMALL_PAGE_ENTRY( 4,0x00000000,(Int32U)__vector,2,2,2,2,1,1),
  TSB_INVALID,/*L2 Table1 End*/
  /*L2 Table2*/
  //Internal RAM L2 16KB 4*4 small
  L2_SMALL_PAGE_ENTRY( 4,0x1FFFC000,0x1FFFC000,3,3,3,3,1,1),
  TSB_INVALID,/*L2 Table2 End*/
  /*L2 Table3*/
  //APIS A Peripherial L2

  //AIPS A Control    16 Kbyte
  //MAX               16 Kbyte
  //EVTMON            16 Kbyte
  //CLKCTL            16 Kbyte
  //ETB registers     16 Kbyte
  //ETB Memory        16 Kbyte
  //CTIO              16 Kbyte
  L2_EXTSMALL_PAGE_ENTRY(28,0x43F00000,0x43F00000,0,2,0,0),
  //I2C               16 Kbyte
  //I2C3              16 Kbyte
  //USBOTG            16 Kbyte
  //ATA control port  16 Kbyte
  //UART1             16 Kbyte
  //UART2             16 Kbyte
  //I2C2              16 Kbyte
  //1-WIRE            16 Kbyte
  //SSI1              16 Kbyte
  //CSPI1             16 Kbyte
  //KPP               16 Kbyte
  //IOMUXC            16 Kbyte
  //UART4             16 Kbyte
  //UART5             16 Kbyte
  //ECT (IP BUS 1)    16 Kbyte
  //ECT (IP BUS 2)    16 Kbyte
  L2_EXTSMALL_PAGE_ENTRY(64,0x43F80000,0x43F80000,0,2,0,0),
  TSB_INVALID,/*L2 Table3 End*/
  /*L2 Table4*/
  //AIPS B off platform global module enable #0 L2
  //
  //MMC/SDHC1         16 Kbyte
  //MMC/SDHC2         16 Kbyte
  //UART3             16 Kbyte
  //CSPI2             16 Kbyte
  //SSI2              16 Kbyte
  //SIM               16 Kbyte
  //IIM               16 Kbyte
  //ATA (DMA port)    16 Kbyte
  //MSHC1             16 Kbyte
  //MSHC2             16 Kbyte
  L2_SMALL_PAGE_ENTRY(40,0x50004000,0x50004000,2,2,2,2,0,0),
  //SPBA Registers    16 Kbyte
  L2_SMALL_PAGE_ENTRY( 4,0x5003C000,0x5003C000,2,2,2,2,0,0),
  TSB_INVALID,/*L2 Table4 End*/

  /*L2 Table5*/
  //AIPS B Control    16 Kbyte
  L2_SMALL_PAGE_ENTRY( 4,0x53F00000,0x53F00000,2,2,2,2,0,0),
  //CCM               16 Kbyte
  //CSPI3             16 Kbyte
  L2_SMALL_PAGE_ENTRY( 8,0x53F80000,0x53F80000,2,2,2,2,0,0),
  //FIR               16 Kbyte
  //GPT               16 Kbyte
  //EPIT1             16 Kbyte
  //EPIT2             16 Kbyte
  L2_SMALL_PAGE_ENTRY(16,0x53F8C000,0x53F8C000,2,2,2,2,0,0),
  //GPIO3             16 Kbyte
  L2_SMALL_PAGE_ENTRY( 4,0x53FA4000,0x53FA4000,2,2,2,2,0,0),
  //SCC               16 Kbyte
  //RNGA              16 Kbyte
  L2_SMALL_PAGE_ENTRY( 8,0x53FAC000,0x53FAC000,2,2,2,2,0,0),
  //IPU               16 Kbyte
  //AUDMUX            16 Kbyte
  //MPEG4_Encoder     16 Kbyte
  //GPIO1             16 Kbyte
  //GPIO2             16 Kbyte
  //SDMA              16 Kbyte
  //RTC               16 Kbyte
  //WDOG              16 Kbyte
  //PWM               16 Kbyte
  L2_SMALL_PAGE_ENTRY(36,0x53FC0000,0x53FC0000,2,2,2,2,0,0),
  //RTIC              16 Kbyte
  L2_SMALL_PAGE_ENTRY( 1,0x53FEC000,0x53FEC000,2,2,2,2,0,0),
  TSB_INVALID,/*L2 Table5 End*/

  /*L2 Table6*/
  //NAND              4 Kbyte
  L2_SMALL_PAGE_ENTRY( 1,0xB8000000,0xB8000000,2,2,2,2,0,0),
  //ESDCTL registers  4 Kbyte
  L2_SMALL_PAGE_ENTRY( 1,0xB8001000,0xB8001000,2,2,2,2,0,0),
  //WEIM registers    4 Kbyte
  L2_SMALL_PAGE_ENTRY( 1,0xB8002000,0xB8002000,2,2,2,2,0,0),
  //M3IF Registers    4 Kbyte
  L2_SMALL_PAGE_ENTRY( 1,0xB8003000,0xB8003000,2,2,2,2,0,0),
  //pcmcia_if registers 4 Kbyte
  L2_SMALL_PAGE_ENTRY( 1,0xB8004000,0xB8004000,2,2,2,2,0,0),
  TSB_INVALID,/*L2 Table6 End*/
};

const TtTableBlock_t TtTB[] =
{
  {L1Table,TableL1},
  {L2Coarse1,TableL2_Coarse},
  {L2Coarse2,TableL2_Coarse},
  {L2Coarse3,TableL2_Coarse},
  {L2Coarse4,TableL2_Coarse},
  {L2Coarse5,TableL2_Coarse},
  {L2Coarse6,TableL2_Coarse},
  TTB_INVALID
};
