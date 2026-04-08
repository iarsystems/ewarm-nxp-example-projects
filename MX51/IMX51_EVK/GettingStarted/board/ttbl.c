/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2012
 *
 *    File name   : ttbl.c
 *    Description : MMU Translation tables
 *
 *    History :
 *    1. Date        : March, 2012
 *       Author      : Atanas Uzunov
 *       Description : iMX51 EVK translation tables
 *
 *        The physical memory map is:
 *                                                                        Instruction cache
 *                                                                          Data cache
 *                                                                            User Access
 *                                                                              Supervisor Access
 *                                                                                Read Only
 *  Segment                     Description       Size                    I D U S R
 *  0x00000000 - 0x00008FFF     Boot ROM 36Kbyte/Vectors remap 4Kbyte     * - * * *
 *  0x00009000 - 0x1FFDFFFF     Reserved          512 Mbyte–164Kbyte      - - - - -
 *  0x1FFE0000 - 0x1FFFFFFF     SCC RAM           128 Kbyte               * * * * -
 *  0x20000000 - 0x2001FFFF     GPU Memory        128 Kbyte               - - - * -
 *  0x20020000 - 0x2FFFFFFF     L2CC              256 Mbyte-128Kbyte      - - - * -
 *  0x30000000 - 0x3FFFFFFF     GPU               256 Mbyte               - - - * -
 *  0x40000000 - 0x5FFFFFFF     IPUEX             512 Mbyte               - - - * -
 *
 *  0x60000000 - 0x60000FFF     Debug ROM         4 Kbyte                 * - - * *
 *  0x60001000 - 0x60001FFF     ETB               4 Kbyte                 - - - * -
 *  0x60002000 - 0x60002FFF     ETM               4 Kbyte                 - - - * -
 *  0x60003000 - 0x60003FFF     TPIU              4 Kbyte                 - - - * -
 *  0x60004000 - 0x60004FFF     CTI0              4 Kbyte                 - - - * -
 *  0x60005000 - 0x60005FFF     CTI1              4 Kbyte                 - - - * -
 *  0x60006000 - 0x60006FFF     CTI2              4 Kbyte                 - - - * -
 *  0x60007000 - 0x60007FFF     CTI3              4 Kbyte                 - - - * -
 *  0x60008000 - 0x60008FFF     Cortex Debug Unit 4 Kbyte                 - - - * -
 *  0x60009000 - 0x6FFFFFFF     Reserved          256Mbyte-36Kbyte        - - - - -
 *
 *  0x70000000 - 0x70003FFF     Reserved          16Kbyte                 - - - - -
 *  0x70004000 - 0x70007FFF     ESDHC1            16Kbyte                 - - - * -
 *  0x70008000 - 0x7000BFFF     ESDHC2            16Kbyte                 - - - * -
 *  0x7000C000 - 0x7000FFFF     UART3             16Kbyte                 - - - * -
 *  0x70010000 - 0x70013FFF     eCSPI1            16Kbyte                 - - - * -
 *  0x70014000 - 0x70017FFF     SSI2              16Kbyte                 - - - * -
 *  0x70018000 - 0x7001BFFF     Reserved          16Kbyte                 - - - - -
 *  0x7001C000 - 0x7001FFFF     Reserved for SDMA 16Kbyte                 - - - - -
 *  0x70020000 - 0x70023FFF     ESDHC3            16Kbyte                 - - - * -
 *  0x70024000 - 0x70027FFF     ESDHC4            16Kbyte                 - - - * -
 *  0x70028000 - 0x7002BFFF     SPDIF             16Kbyte                 - - - * -
 *  0x7002C000 - 0x7002FFFF     Reserved          16Kbyte                 - - - * -
 *  0x70030000 - 0x70033FFF     PATA(PORT UDMA)   16Kbyte                 - - - * -
 *  0x70034000 - 0x70037FFF     SLIM              16Kbyte                 - - - * -
 *  0x70038000 - 0x7003BFFF     HSI2C             16Kbyte                 - - - * -
 *  0x7003C000 - 0x7003FFFF     SPBA              16Kbyte                 - - - * -
 *
 *  0x70040000 - 0x71FFFFFF     Reserved AIPS_TZ0 32Mbyte-256Kbyte        - - - - -
 *  0x72000000 - 0x73EFFFFF     Reserved AIPS_TZ1 31Mbyte                 - - - - -
 *
 *  0x73F00000 - 0x73F7FFFF     Reserved AIPS_TZ1 512Kbyte                - - - - -
 *
 *  0x73F80000 - 0x73F83FFF     USBOH3(PORT USB)  16 Kbyte                - - - * -
 *  0x73F84000 - 0x73F87FFF     GPIO1             16 Kbyte                - - - * -
 *  0x73F88000 - 0x73F8BFFF     GPIO2             16 Kbyte                - - - * -
 *  0x73F8C000 - 0x73F8FFFF     GPIO3             16 Kbyte                - - - * -
 *  0x73F90000 - 0x73F93FFF     GPIO4             16 Kbyte                - - - * -
 *  0x73F94000 - 0x73F97FFF     KPP               16 Kbyte                - - - * -
 *  0x73F98000 - 0x73F9BFFF     WDOG1             16 Kbyte                - - - * -
 *  0x73F9C000 - 0x73F9FFFF     WDOG2 (TZ)        16 Kbyte                - - - * -
 *  0x73FA0000 - 0x73FA3FFF     GPT               16 Kbyte                - - - * -
 *  0x73FA4000 - 0x73FA7FFF     SRTC              16 Kbyte                - - - * -
 *  0x73FA8000 - 0x73FABFFF     IOMUXC            16 Kbyte                - - - * -
 *  0x73FAC000 - 0x73FAFFFF     EPIT1             16 Kbyte                - - - * -
 *  0x73FB0000 - 0x73FB3FFF     EPIT2             16 Kbyte                - - - * -
 *  0x73FB4000 - 0x73FB7FFF     PWM1              16 Kbyte                - - - * -
 *  0x73FB8000 - 0x73FBBFFF     PWM2              16 Kbyte                - - - * -
 *  0x73FBC000 - 0x73FBFFFF     UART1             16 Kbyte                - - - * -
 *  0x73FC0000 - 0x73FC3FFF     UART2             16 Kbyte                - - - * -
 *  0x73FC4000 - 0x73FC7FFF     USBOH3(PORT PL301)16 Kbyte                - - - * -
 *  0x73FC8000 - 0x73FCBFFF     Reserved          16 Kbyte                - - - - -
 *  0x73FCC000 - 0x73FCFFFF     Reserved          16 Kbyte                - - - - -
 *  0x73FD0000 - 0x73FD3FFF     SRC               16 Kbyte                - - - * -
 *  0x73FD4000 - 0x73FD7FFF     CCM               16 Kbyte                - - - * -
 *  0x73FD8000 - 0x73FDBFFF     GPC               16 Kbyte                - - - * -
 *  0x73FDC000 - 0x73FDFFFF     Reserved          16 Kbyte                - - - - -
 *  0x73FE0000 - 0x73FE3FFF     Reserved          16 Kbyte                - - - - -
 *  0x73FE4000 - 0x73FE7FFF     Reserved          16 Kbyte                - - - - -
 *  0x73FE8000 - 0x73FEBFFF     Reserved          16 Kbyte                - - - - -
 *  0x73FEC000 - 0x73FEFFFF     Reserved          16 Kbyte                - - - - -
 *  0x73FF0000 - 0x73FF3FFF     Reserved          16 Kbyte                - - - - -
 *  0x73FF4000 - 0x73FFFFFF     Reserved AIPS_TZ1 48 Kbyte                - - - - -
 *  0x74000000 - 0x7FFFFFFF     Reserved AIPS_TZ1 448 Kbyte               - - - - -
 *
 *  0x80000000 - 0x81FFFFFF     Reserved AIPS_TZ1 32Mbyte                 - - - - -
 *  0x82000000 - 0x83EFFFFF     Reserved AIPS_TZ1 31Mbyte                 - - - - -
 *  0x83F00000 - 0x83F7FFFF     Reserved AIPS_TZ2 512Kbyte                - - - - -
 *
 *  0x83F80000 - 0x83F83FFF     DPLLIP1           16Kbyte                 - - - * -
 *  0x83F84000 - 0x83F87FFF     DPLLIP2           16Kbyte                 - - - * -
 *  0x83F88000 - 0x83F8BFFF     DPLLIP3           16Kbyte                 - - - * -
 *  0x83F8C000 - 0x83F8FFFF     Reserved          16Kbyte                 - - - - -
 *  0x83F90000 - 0x83F93FFF     Reserved          16Kbyte                 - - - - -
 *  0x83F94000 - 0x83F97FFF     AHBMAX            16Kbyte                 - - - * -
 *  0x83F98000 - 0x83F9BFFF     IIM               16Kbyte                 - - - * -
 *  0x83F9C000 - 0x83F9FFFF     CSU               16Kbyte                 - - - * -
 *  0x83FA0000 - 0x83FA3FFF     TIGERP            16Kbyte                 - - - * -
 *  0x83FA4000 - 0x83FA7FFF     OWIRE             16Kbyte                 - - - * -
 *  0x83FA8000 - 0x83FABFFF     FIRI              16Kbyte                 - - - * -
 *  0x83FAC000 - 0x83FAFFFF     eCSPI2            16Kbyte                 - - - * -
 *  0x83FB0000 - 0x83FB3FFF     SDMA(IPS_HOST)    16Kbyte                 - - - * -
 *  0x83FB4000 - 0x83FB7FFF     SCC               16Kbyte                 - - - * -
 *  0x83FB8000 - 0x83FBBFFF     ROMCP             16Kbyte                 - - - * -
 *  0x83FBC000 - 0x83FBFFFF     RTIC              16Kbyte                 - - - * -
 *  0x83FC0000 - 0x83FC3FFF     CSPI              16Kbyte                 - - - * -
 *  0x83FC4000 - 0x83FC7FFF     I2C2              16Kbyte                 - - - * -
 *  0x83FC8000 - 0x83FCBFFF     I2C1              16Kbyte                 - - - * -
 *  0x83FCC000 - 0x83FCFFFF     SSI1              16Kbyte                 - - - * -
 *  0x83FD0000 - 0x83FD3FFF     AUDMUX            16Kbyte                 - - - * -
 *  0x83FD4000 - 0x83FD7FFF     Reserved          16Kbyte                 - - - - -
 *  0x83FD8000 - 0x83FDBFFF     EMI               16Kbyte                 - - - * -
 *  0x83FDC000 - 0x83FDFFFF     HSC               16Kbyte                 - - - * -
 *  0x83FE0000 - 0x83FE3FFF     PATA(PORT PIO)    16Kbyte                 - - - * -
 *  0x83FE4000 - 0x83FE7FFF     SIM               16Kbyte                 - - - * -
 *  0x83FE8000 - 0x83FEBFFF     SSI3              16Kbyte                 - - - * -
 *  0x83FEC000 - 0x83FEFFFF     FEC               16Kbyte                 - - - * -
 *  0x83FF0000 - 0x83FF3FFF     TVE               16Kbyte                 - - - * -
 *  0x83FF4000 - 0x83FF7FFF     VPU               16Kbyte                 - - - * -
 *  0x83FF8000 - 0x83FFBFFF     SAHARA Lite       16Kbyte                 - - - * -
 *  0x83FFC000 - 0x83FFFFFF     Reserved          16Kbyte                 - - - - -
 *  0x84000000 - 0x8FFFBFFF     Reserved AIPS_TZ2 448MByte-256Kbyte       - - - - -
 *
 *  0x8FFFC000 - 0x8FFFFFFF     Reserved          16Kbyte                 - - - - -
 *
 *  0x90000000 - 0x9FFFFFFF     CS0 DDR           256Mbyte                * * * * -
 *  0xA0000000 - 0xAFFFFFFF     CS1 DDR           256Mbyte                * - - * -
 *  0xB0000000 - 0xB7FFFFFF     CS0 Flash         128Mbyte                * * * * -
 *  0xB8000000 - 0xBFFFFFFF     CS1 Flash         128Mbyte                * * * * -
 *  0xC0000000 - 0xC7FFFFFF     CS2 Flash         128Mbyte                * * * * -
 *  0xC8000000 - 0xCBFFFFFF     CS3 Flash         64Mbyte                 * * * * -
 *  0xCC000000 - 0xCDFFFFFF     CS4 SRAM          32Mbyte                 * * * * -
 *  0xCE000000 - 0xCFFEFFFF     CS5 SRAM          32Mbyte-64Kbyte         * - - * -
 *  0xCFFF0000 - 0xCFFFFFFF     NAND Flash(intbuf)16Kbyte                 - - - * -
 *
 *  0xD0000000 - 0xDFFFFFFF     GPU2D (OpenVG)    256Mbyte                - - - * -
 *  0xE0000000 - 0xE0003FFF     TZIC              16Kbyte                 - - - * -
 *  0xED004000 - 0xFFFFFFFF     Reserved          512Mbyte-16Kbyte        - - - * -
 *
 *  Domain 0 - 01 Client
 *  Domain 1 - 11 Manager
 *
 *    $Revision: 16 $
 **************************************************************************/
#include "ttbl.h"
#pragma segment=".intvec"
#pragma segment="MMU_TT"

#pragma location="MMU_TT"
#pragma data_alignment=16384
__no_init Int32U L1Table[L1_ENTRIES_NUMB];

#pragma location="MMU_TT"
#pragma data_alignment=1024
__no_init Int32U L2PageTable1[L2_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=1024
__no_init Int32U L2PageTable2[L2_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=1024
__no_init Int32U L2PageTable3[L2_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=1024
__no_init Int32U L2PageTable4[L2_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=1024
__no_init Int32U L2PageTable5[L2_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=1024
__no_init Int32U L2PageTable6[L2_ENTRIES_NUMB];
#pragma location="MMU_TT"
#pragma data_alignment=1024
__no_init Int32U L2PageTable7[L2_ENTRIES_NUMB];

extern void __vector(void);

const TtSectionBlock_t TtSB[] =
{
  /*L1 Table*/
  // Vector table Remap L1
  L1_PAGE_TABLE_ENTRY(  1,0x00000000,(Int32U)L2PageTable1,1,0),
  // On-chip memories
  L1_PAGE_TABLE_ENTRY(  1,0x1FFE0000,(Int32U)L2PageTable2,1,0),
  // GPU
  L1_SUPERSECTION_ENTRY(16,0x30000000,0x30000000,0x00,0,0,0,0,0,3,1,0,0,0),
  // IPUEX
  L1_SUPERSECTION_ENTRY(32,0x40000000,0x40000000,0x00,0,0,0,0,0,3,1,0,0,0),
  // AHB IPs - Debug APB
  L1_PAGE_TABLE_ENTRY(  1,0x60000000,(Int32U)L2PageTable3,1,0),
  // AIPS_TZ1 SPBA IPs
  L1_PAGE_TABLE_ENTRY(  1,0x70004000,(Int32U)L2PageTable4,1,0),
  // AIPS_TZ1 Off Platform
  L1_PAGE_TABLE_ENTRY(  1,0x73F80000,(Int32U)L2PageTable5,1,0),
  // AIPS_TZ2 Off Platform
  L1_PAGE_TABLE_ENTRY(  1,0x83F80000,(Int32U)L2PageTable6,1,0),
  // CS0 DDR Normal Memory
  L1_SUPERSECTION_ENTRY(16,0x90000000,0x90000000,0x00,0,0,0,0,0,3,1,0,1,1),
  // CS1 DDR Strongly Ordered
  L1_SUPERSECTION_ENTRY(16,0xA0000000,0xA0000000,0x00,0,0,0,0,0,3,1,0,0,0),
  // CS0 Flash 128M
  L1_SUPERSECTION_ENTRY( 8,0xB0000000,0xB0000000,0x00,0,0,0,0,0,3,1,0,1,0),
  // CS1 Flash 128M
  L1_SUPERSECTION_ENTRY( 8,0xB8000000,0xB8000000,0x00,0,0,0,0,0,3,1,0,1,0),
  // CS2 Flash 128M
  L1_SUPERSECTION_ENTRY( 8,0xC0000000,0xC0000000,0x00,0,0,0,0,0,3,1,0,1,0),
  // CS3 Flash 64M
  L1_SUPERSECTION_ENTRY( 4,0xC8000000,0xC8000000,0x00,0,0,0,0,0,3,1,0,1,0),
  // CS4 SRAM 32M cachable, bufferable
  L1_SUPERSECTION_ENTRY( 2,0xCC000000,0xCC000000,0x00,0,0,0,0,0,3,1,0,1,1),
  // CS5 SRAM 32M
  L1_SUPERSECTION_ENTRY( 2,0xCE000000,0xCE000000,0x00,0,0,0,0,0,3,1,0,0,0),
  // AHB IPs
  L1_SUPERSECTION_ENTRY(16,0xD0000000,0xD0000000,0x00,0,0,0,0,0,3,1,0,0,0),
  // AHB IPs (TZIC)
  L1_PAGE_TABLE_ENTRY(  1,0xE0000000,(Int32U)L2PageTable7,1,0),
  TSB_INVALID,/*L1 Table End*/

  /*L2 Table1*/
  // Vector Table Remap L2 4KB
  L2_SMALL_PAGE_ENTRY( 1,0x00000000,(Int32U)__vector,0,0,0,0,0,3,0,1),
  TSB_INVALID,/*L2 Table1 End*/

  /*L2 Table2*/
  //SCC RAM           128 Kbyte
  L2_LARGE_PAGE_ENTRY(2,0x1FFE0000,0x1FFE0000,0,0,0,0,0,3,1,1),
  //GPU Memory        128 Kbyte
  L2_LARGE_PAGE_ENTRY(2,0x20000000,0x20000000,0,0,0,0,0,3,0,0),
  TSB_INVALID,/*L2 Table2 End*/

  /*L2 Table 3*/
  //Debug ROM         4 Kbyte
  //ETB               4 Kbyte
  //ETM               4 Kbyte
  //TPIU              4 Kbyte
  //CTI0              4 Kbyte
  //CTI1              4 Kbyte
  //CTI2              4 Kbyte
  //CTI3              4 Kbyte
  //Cortex Debug Unit 4 Kbyte
  L2_SMALL_PAGE_ENTRY( 9,0x60000000,0x60000000,0,0,0,0,0,3,0,0),
  TSB_INVALID,/*L2 Table3 End*/

  /*L2 Table4*/
  //ESDHC1            16 Kbyte
  //ESDHC2            16 Kbyte
  //UART3             16 Kbyte
  //eCSPI1            16 Kbyte
  //SSI2              16 Kbyte
  L2_SMALL_PAGE_ENTRY(20,0x70004000,0x70004000,0,0,0,0,0,3,0,0),
  //ESDHC3            16 Kbyte
  //ESDHC4            16 Kbyte
  //SPDIF             16 Kbyte
  L2_SMALL_PAGE_ENTRY(12,0x70020000,0x70020000,0,0,0,0,0,3,0,0),
  //PATA(PORT UDMA)   16 Kbyte
  //SLIM              16 Kbyte
  //HSI2C             16 Kbyte
  //SPBA              16 Kbyte
  L2_SMALL_PAGE_ENTRY(16,0x70030000,0x70030000,0,0,0,0,0,3,0,0),
  TSB_INVALID,/*L2 Table4 End*/

  /*L2 Table5*/
  //USBOH3(PORT USB)  16 Kbyte
  //GPIO1             16 Kbyte
  //GPIO2             16 Kbyte
  //GPIO3             16 Kbyte
  //GPIO4             16 Kbyte
  //KPP               16 Kbyte
  //WDOG1             16 Kbyte
  //WDOG2 (TZ)        16 Kbyte
  //GPT               16 Kbyte
  //SRTC              16 Kbyte
  //IOMUXC            16 Kbyte
  //EPIT1             16 Kbyte
  //EPIT2             16 Kbyte
  //PWM1              16 Kbyte
  //PWM2              16 Kbyte
  //UART1             16 Kbyte
  //UART2             16 Kbyte
  //USBOH3(PORT PL301)16 Kbyte 
  L2_SMALL_PAGE_ENTRY(72,0x73F80000,0x73F80000,0,0,0,0,0,3,0,0),
  //SRC               16 Kbyte
  //CCM               16 Kbyte
  //GPC               16 Kbyte
  L2_SMALL_PAGE_ENTRY(12,0x73FD0000,0x73FD0000,0,0,0,0,0,3,0,0),
  TSB_INVALID,/*L2 Table5 End*/

  /*L2 Table6*/
  //DPLLIP1           16Kbyte
  //DPLLIP2           16Kbyte
  //DPLLIP3           16Kbyte
  L2_SMALL_PAGE_ENTRY(12,0x83F80000,0x83F80000,0,0,0,0,0,3,0,0),
  //AHBMAX            16Kbyte
  //IIM               16Kbyte
  //CSU               16Kbyte
  //TIGERP            16Kbyte
  //OWIRE             16Kbyte
  //FIRI              16Kbyte
  //eCSPI2            16Kbyte
  //SDMA(IPS_HOST)    16Kbyte
  //SCC               16Kbyte
  //ROMCP             16Kbyte
  //RTIC              16Kbyte
  //CSPI              16Kbyte
  //I2C2              16Kbyte
  //I2C1              16Kbyte
  //SSI1              16Kbyte
  //AUDMUX            16Kbyte
  L2_SMALL_PAGE_ENTRY(64,0x83F94000,0x83F94000,0,0,0,0,0,3,0,0),
  //EMI               16Kbyte
  //HSC               16Kbyte
  //PATA(PORT PIO)    16Kbyte
  //SIM               16Kbyte
  //SSI3              16Kbyte
  //FEC               16Kbyte
  //TVE               16Kbyte
  //VPU               16Kbyte
  //SAHARA Lite       16Kbyte
  L2_LARGE_PAGE_ENTRY(1,0x83FD8000,0x83FD8000,0,0,0,0,0,3,0,0),
  L2_LARGE_PAGE_ENTRY(1,0x83FEC000,0x83FEC000,0,0,0,0,0,3,0,0),
  L2_SMALL_PAGE_ENTRY(1,0x83FF8000,0x83FF8000,0,0,0,0,0,3,0,0),
  TSB_INVALID,/*L2 Table6 End*/

  /*L2 Table7*/
  //TZIC              16Kbyte  
  L2_SMALL_PAGE_ENTRY(4,0xE0000000,0xE0000000,0,0,0,0,0,3,0,0),
  TSB_INVALID,/*L2 Table7 End*/
  };

const TtTableBlock_t TtTB[] =
{
  {L1Table,TableL1},
  {L2PageTable1,TableL2_PageTable},
  {L2PageTable2,TableL2_PageTable},
  {L2PageTable3,TableL2_PageTable},
  {L2PageTable4,TableL2_PageTable},
  {L2PageTable5,TableL2_PageTable},
  {L2PageTable6,TableL2_PageTable},
  {L2PageTable7,TableL2_PageTable},
  TTB_INVALID
};
