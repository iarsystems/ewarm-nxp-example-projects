//////////////////////////////////////////////////////////////////////////////////////
// File: MKM34Z128LL5.h
// Purpose: Define the peripheral registers.
// This file has been automatically generated from the APIF knowledgebase
//
// Revisions:
// - rev. 1.0 (2011-06-01)
//     Initial public version.
// - rev. 1.1 (2012-03-13)
//     Bit/group constants related to 32-bit registers typecasted to uint32_t.
//
//////////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
//
//////////////////////////////////////////////////////////////////////////////////////
// Generic Bit Definitions Used Elsewhere in this File
//
// Peripheral Base Address Defines in Alphabetical Order
//
#define  ADC_BASE_PTR                ((uint8_t *) (0x4002B000u))

#define  AFE_BASE_PTR                ((uint8_t *) (0x40030000u))

#define  AIPS_BASE_PTR               ((uint8_t *) (0x40000000u))

#define  CMP_BASE_PTR                ((uint8_t *) (0x40072000u))

#define  CRC_BASE_PTR                ((uint8_t *) (0x40034000u))

#define  DMAMUX0_BASE_PTR            ((uint8_t *) (0x40021000u))

#define  DMAMUX1_BASE_PTR            ((uint8_t *) (0x40022000u))

#define  DMAMUX2_BASE_PTR            ((uint8_t *) (0x40023000u))

#define  DMAMUX3_BASE_PTR            ((uint8_t *) (0x40024000u))

#define  EWM_BASE_PTR                ((uint8_t *) (0x40061000u))

#define  FTFA_BASE_PTR               ((uint8_t *) (0x40020000u))

#define  GPIO_BASE_PTR               ((uint8_t *) (0x400FF000u))

#define  I2C0_BASE_PTR               ((uint8_t *) (0x40067000u))

#define  I2C1_BASE_PTR               ((uint8_t *) (0x40068000u))

#define  LCD_BASE_PTR                ((uint8_t *) (0x40043000u))

#define  LLWU_BASE_PTR               ((uint8_t *) (0x4007C000u))

#define  LPTMR_BASE_PTR              ((uint32_t *) (0x4003C000u))

#define  MCG_BASE_PTR                ((uint8_t *) (0x40064000u))

#define  MCM_BASE_PTR                ((uint8_t *) (0xF0003000u))

#define  MPU_BASE_PTR                ((uint8_t *) (0x4000A000u))

#define  MTB_BASE_PTR                ((uint8_t *) (0xF0000000u))

#define  MTBDWT_BASE_PTR             ((uint8_t *) (0xF0001000u))

#define  OSC_BASE_PTR                ((uint8_t *) (0x40066000u))

#define  PIT0_BASE_PTR               ((uint32_t *) (0x4002D000u))

#define  PIT1_BASE_PTR               ((uint32_t *) (0x4002E000u))

#define  PMC_BASE_PTR                ((uint8_t *) (0x4007D000u))

#define  RCM_BASE_PTR                ((uint8_t *) (0x4007B000u))

#define  RNG_BASE_PTR                ((uint8_t *) (0x40029000u))

#define  ROM_BASE_PTR                ((uint8_t *) (0xF0002000u))

#define  RTC_BASE_PTR                ((uint8_t *) (0x40050000u))

#define  SIM_BASE_PTR                ((uint8_t *) (0x4003F000u))

#define  SMC_BASE_PTR                ((uint8_t *) (0x4007E000u))

#define  SPI0_BASE_PTR               ((uint8_t *) (0x40075000u))

#define  SPI1_BASE_PTR               ((uint8_t *) (0x40076000u))

#define  TMR0_BASE_PTR               ((uint16_t *) (0x40057000u))

#define  TMR1_BASE_PTR               ((uint16_t *) (0x40058000u))

#define  TMR2_BASE_PTR               ((uint16_t *) (0x40059000u))

#define  TMR3_BASE_PTR               ((uint16_t *) (0x4005A000u))

#define  UART0_BASE_PTR              ((uint8_t *) (0x4006A000u))

#define  UART1_BASE_PTR              ((uint8_t *) (0x4006B000u))

#define  UART2_BASE_PTR              ((uint8_t *) (0x4006C000u))

#define  UART3_BASE_PTR              ((uint8_t *) (0x4006D000u))

#define  VREF_BASE_PTR               ((uint8_t *) (0x4006F000u))

#define  WDOG_BASE_PTR               ((uint8_t *) (0x40053000u))

#define  XBAR_BASE_PTR               ((uint16_t *) (0x40055000u))


//////////////////////////////////////////////////////////////////////////////////////
// The following section defines the IO register locations.
//////////////////////////////////////////////////////////////////////////////////////

#define  ADC_SC1A                    (*(uint32_t *) (0x4002B000))

#define  ADC_SC1B                    (*(uint32_t *) (0x4002B004))

#define  ADC_CFG1                    (*(uint32_t *) (0x4002B008))

#define  ADC_CFG2                    (*(uint32_t *) (0x4002B00C))

#define  ADC_RA                      (*(uint32_t *) (0x4002B010))

#define  ADC_RB                      (*(uint32_t *) (0x4002B014))

#define  ADC_CV1                     (*(uint32_t *) (0x4002B018))

#define  ADC_CV2                     (*(uint32_t *) (0x4002B01C))

#define  ADC_SC2                     (*(uint32_t *) (0x4002B020))

#define  ADC_SC3                     (*(uint32_t *) (0x4002B024))

#define  ADC_OFS                     (*(uint32_t *) (0x4002B028))

#define  ADC_PG                      (*(uint32_t *) (0x4002B02C))

#define  ADC_MG                      (*(uint32_t *) (0x4002B030))

#define  ADC_CLPD                    (*(uint32_t *) (0x4002B034))

#define  ADC_CLPS                    (*(uint32_t *) (0x4002B038))

#define  ADC_CLP4                    (*(uint32_t *) (0x4002B03C))

#define  ADC_CLP3                    (*(uint32_t *) (0x4002B040))

#define  ADC_CLP2                    (*(uint32_t *) (0x4002B044))

#define  ADC_CLP1                    (*(uint32_t *) (0x4002B048))

#define  ADC_CLP0                    (*(uint32_t *) (0x4002B04C))

#define  ADC_PGA                     (*(uint32_t *) (0x4002B050))

#define  ADC_CLMD                    (*(uint32_t *) (0x4002B054))

#define  ADC_CLMS                    (*(uint32_t *) (0x4002B058))

#define  ADC_CLM4                    (*(uint32_t *) (0x4002B05C))

#define  ADC_CLM3                    (*(uint32_t *) (0x4002B060))

#define  ADC_CLM2                    (*(uint32_t *) (0x4002B064))

#define  ADC_CLM1                    (*(uint32_t *) (0x4002B068))

#define  ADC_CLM0                    (*(uint32_t *) (0x4002B06C))

#define  AFE_CH1_CFR                 (*(uint32_t *) (0x40030000))

#define  AFE_CH2_CFR                 (*(uint32_t *) (0x40030004))

#define  AFE_CH3_CFR                 (*(uint32_t *) (0x40030008))

#define  AFE_CH4_CFR                 (*(uint32_t *) (0x4003000C))

#define  AFE_CR                      (*(uint32_t *) (0x40030018))

#define  AFE_CKR                     (*(uint32_t *) (0x4003001C))

#define  AFE_DI                      (*(uint32_t *) (0x40030020))

#define  AFE_CH1_DR                  (*(uint32_t *) (0x4003002C))

#define  AFE_CH2_DR                  (*(uint32_t *) (0x40030030))

#define  AFE_CH3_DR                  (*(uint32_t *) (0x40030034))

#define  AFE_CH4_DR                  (*(uint32_t *) (0x40030038))

#define  AFE_CH1_RR                  (*(uint32_t *) (0x40030044))

#define  AFE_CH2_RR                  (*(uint32_t *) (0x40030048))

#define  AFE_CH3_RR                  (*(uint32_t *) (0x4003004C))

#define  AFE_CH4_RR                  (*(uint32_t *) (0x40030050))

#define  AFE_SR                      (*(uint32_t *) (0x4003005C))

#define  AFE_TR                      (*(uint32_t *) (0x40030060))

#define  AIPS_MPRA                   (*(uint32_t *) (0x40000000))

#define  AIPS_PACRA                  (*(uint32_t *) (0x40000020))

#define  AIPS_PACRB                  (*(uint32_t *) (0x40000024))

#define  AIPS_PACRC                  (*(uint32_t *) (0x40000028))

#define  AIPS_PACRD                  (*(uint32_t *) (0x4000002C))

#define  AIPS_PACRE                  (*(uint32_t *) (0x40000040))

#define  AIPS_PACRF                  (*(uint32_t *) (0x40000044))

#define  AIPS_PACRG                  (*(uint32_t *) (0x40000048))

#define  AIPS_PACRH                  (*(uint32_t *) (0x4000004C))

#define  AIPS_PACRI                  (*(uint32_t *) (0x40000050))

#define  AIPS_PACRJ                  (*(uint32_t *) (0x40000054))

#define  AIPS_PACRK                  (*(uint32_t *) (0x40000058))

#define  AIPS_PACRL                  (*(uint32_t *) (0x4000005C))

#define  AIPS_PACRM                  (*(uint32_t *) (0x40000060))

#define  AIPS_PACRN                  (*(uint32_t *) (0x40000064))

#define  AIPS_PACRO                  (*(uint32_t *) (0x40000068))

#define  AIPS_PACRP                  (*(uint32_t *) (0x4000006C))

#define  AIPS_PACRU                  (*(uint32_t *) (0x40000080))

#define  CMP_CR0                     (*(uint8_t *) (0x40072000))

#define  CMP_CR1                     (*(uint8_t *) (0x40072001))

#define  CMP_FPR                     (*(uint8_t *) (0x40072002))

#define  CMP_SCR                     (*(uint8_t *) (0x40072003))

#define  CMP_DACCR                   (*(uint8_t *) (0x40072004))

#define  CMP_MUXCR                   (*(uint8_t *) (0x40072005))

#define  CRC_CRC                     (*(uint32_t *) (0x40034000))

#define  CRC_GPOLY                   (*(uint32_t *) (0x40034004))

#define  CRC_CTRL                    (*(uint32_t *) (0x40034008))

#define  DMAMUX0_CHCFG0              (*(uint8_t *) (0x40021000))

#define  DMAMUX0_CHCFG1              (*(uint8_t *) (0x40021001))

#define  DMAMUX0_CHCFG2              (*(uint8_t *) (0x40021002))

#define  DMAMUX0_CHCFG3              (*(uint8_t *) (0x40021003))

#define  DMAMUX1_CHCFG0              (*(uint8_t *) (0x40022000))

#define  DMAMUX1_CHCFG1              (*(uint8_t *) (0x40022001))

#define  DMAMUX1_CHCFG2              (*(uint8_t *) (0x40022002))

#define  DMAMUX1_CHCFG3              (*(uint8_t *) (0x40022003))

#define  DMAMUX2_CHCFG0              (*(uint8_t *) (0x40023000))

#define  DMAMUX2_CHCFG1              (*(uint8_t *) (0x40023001))

#define  DMAMUX2_CHCFG2              (*(uint8_t *) (0x40023002))

#define  DMAMUX2_CHCFG3              (*(uint8_t *) (0x40023003))

#define  DMAMUX3_CHCFG0              (*(uint8_t *) (0x40024000))

#define  DMAMUX3_CHCFG1              (*(uint8_t *) (0x40024001))

#define  DMAMUX3_CHCFG2              (*(uint8_t *) (0x40024002))

#define  DMAMUX3_CHCFG3              (*(uint8_t *) (0x40024003))

#define  EWM_CTRL                    (*(uint8_t *) (0x40061000))

#define  EWM_SERV                    (*(uint8_t *) (0x40061001))

#define  EWM_CMPL                    (*(uint8_t *) (0x40061002))

#define  EWM_CMPH                    (*(uint8_t *) (0x40061003))

#define  FTFA_FSTAT                  (*(uint8_t *) (0x40020000))

#define  FTFA_FCNFG                  (*(uint8_t *) (0x40020001))

#define  FTFA_FSEC                   (*(uint8_t *) (0x40020002))

#define  FTFA_FOPT                   (*(uint8_t *) (0x40020003))

#define  FTFA_FCCOB3                 (*(uint8_t *) (0x40020004))

#define  FTFA_FCCOB2                 (*(uint8_t *) (0x40020005))

#define  FTFA_FCCOB1                 (*(uint8_t *) (0x40020006))

#define  FTFA_FCCOB0                 (*(uint8_t *) (0x40020007))

#define  FTFA_FCCOB7                 (*(uint8_t *) (0x40020008))

#define  FTFA_FCCOB6                 (*(uint8_t *) (0x40020009))

#define  FTFA_FCCOB5                 (*(uint8_t *) (0x4002000A))

#define  FTFA_FCCOB4                 (*(uint8_t *) (0x4002000B))

#define  FTFA_FCCOBB                 (*(uint8_t *) (0x4002000C))

#define  FTFA_FCCOBA                 (*(uint8_t *) (0x4002000D))

#define  FTFA_FCCOB9                 (*(uint8_t *) (0x4002000E))

#define  FTFA_FCCOB8                 (*(uint8_t *) (0x4002000F))

#define  FTFA_FPROT3                 (*(uint8_t *) (0x40020010))

#define  FTFA_FPROT2                 (*(uint8_t *) (0x40020011))

#define  FTFA_FPROT1                 (*(uint8_t *) (0x40020012))

#define  FTFA_FPROT0                 (*(uint8_t *) (0x40020013))

#define  GPIO_PDOR                   (*(uint32_t *) (0x400FF000))

#define  GPIO_PSOR                   (*(uint32_t *) (0x400FF004))

#define  GPIO_PCOR                   (*(uint32_t *) (0x400FF008))

#define  GPIO_PTOR                   (*(uint32_t *) (0x400FF00C))

#define  GPIO_PDIR                   (*(uint32_t *) (0x400FF010))

#define  I2C0_A1                     (*(uint8_t *) (0x40067000))

#define  I2C0_F                      (*(uint8_t *) (0x40067001))

#define  I2C0_C1                     (*(uint8_t *) (0x40067002))

#define  I2C0_S                      (*(uint8_t *) (0x40067003))

#define  I2C0_D                      (*(uint8_t *) (0x40067004))

#define  I2C0_C2                     (*(uint8_t *) (0x40067005))

#define  I2C0_FLT                    (*(uint8_t *) (0x40067006))

#define  I2C0_RA                     (*(uint8_t *) (0x40067007))

#define  I2C0_SMB                    (*(uint8_t *) (0x40067008))

#define  I2C0_A2                     (*(uint8_t *) (0x40067009))

#define  I2C0_SLTH                   (*(uint8_t *) (0x4006700A))

#define  I2C0_SLTL                   (*(uint8_t *) (0x4006700B))

#define  I2C1_A1                     (*(uint8_t *) (0x40068000))

#define  I2C1_F                      (*(uint8_t *) (0x40068001))

#define  I2C1_C1                     (*(uint8_t *) (0x40068002))

#define  I2C1_S                      (*(uint8_t *) (0x40068003))

#define  I2C1_D                      (*(uint8_t *) (0x40068004))

#define  I2C1_C2                     (*(uint8_t *) (0x40068005))

#define  I2C1_FLT                    (*(uint8_t *) (0x40068006))

#define  I2C1_RA                     (*(uint8_t *) (0x40068007))

#define  I2C1_SMB                    (*(uint8_t *) (0x40068008))

#define  I2C1_A2                     (*(uint8_t *) (0x40068009))

#define  I2C1_SLTH                   (*(uint8_t *) (0x4006800A))

#define  I2C1_SLTL                   (*(uint8_t *) (0x4006800B))

#define  LCD_GCR                     (*(uint32_t *) (0x40043000))

#define  LCD_AR                      (*(uint32_t *) (0x40043004))

#define  LCD_FDCR                    (*(uint32_t *) (0x40043008))

#define  LCD_FDSR                    (*(uint32_t *) (0x4004300C))

#define  LCD_PENL                    (*(uint32_t *) (0x40043010))

#define  LCD_PENH                    (*(uint32_t *) (0x40043014))

#define  LCD_BPENL                   (*(uint32_t *) (0x40043018))

#define  LCD_BPENH                   (*(uint32_t *) (0x4004301C))

#define  LCD_WF3TO0                  (*(uint32_t *) (0x40043020))

#define  LCD_WF7TO4                  (*(uint32_t *) (0x40043024))

#define  LCD_WF11TO8                 (*(uint32_t *) (0x40043028))

#define  LCD_WF15TO12                (*(uint32_t *) (0x4004302C))

#define  LCD_WF19TO16                (*(uint32_t *) (0x40043030))

#define  LCD_WF23TO20                (*(uint32_t *) (0x40043034))

#define  LCD_WF27TO24                (*(uint32_t *) (0x40043038))

#define  LCD_WF31TO28                (*(uint32_t *) (0x4004303C))

#define  LCD_WF35TO32                (*(uint32_t *) (0x40043040))

#define  LCD_WF39TO36                (*(uint32_t *) (0x40043044))

#define  LCD_WF43TO40                (*(uint32_t *) (0x40043048))

#define  LCD_WF47TO44                (*(uint32_t *) (0x4004304C))

#define  LCD_WF51TO48                (*(uint32_t *) (0x40043050))

#define  LCD_WF55TO52                (*(uint32_t *) (0x40043054))

#define  LCD_WF59TO56                (*(uint32_t *) (0x40043058))

#define  LCD_WF63TO60                (*(uint32_t *) (0x4004305C))

#define  LLWU_PE1                    (*(uint8_t *) (0x4007C000))

#define  LLWU_PE2                    (*(uint8_t *) (0x4007C001))

#define  LLWU_PE3                    (*(uint8_t *) (0x4007C002))

#define  LLWU_PE4                    (*(uint8_t *) (0x4007C003))

#define  LLWU_ME                     (*(uint8_t *) (0x4007C004))

#define  LLWU_F1                     (*(uint8_t *) (0x4007C005))

#define  LLWU_F2                     (*(uint8_t *) (0x4007C006))

#define  LLWU_F3                     (*(uint8_t *) (0x4007C007))

#define  LLWU_FILT1                  (*(uint8_t *) (0x4007C008))

#define  LLWU_FILT2                  (*(uint8_t *) (0x4007C009))

#define  LPTMR_CSR                   (*(uint32_t *) (0x4003C000))

#define  LPTMR_PSR                   (*(uint32_t *) (0x4003C004))

#define  LPTMR_CMR                   (*(uint32_t *) (0x4003C008))

#define  LPTMR_CNR                   (*(uint32_t *) (0x4003C00C))

#define  MCG_C1                      (*(uint8_t *) (0x40064000))

#define  MCG_C2                      (*(uint8_t *) (0x40064001))

#define  MCG_C3                      (*(uint8_t *) (0x40064002))

#define  MCG_C4                      (*(uint8_t *) (0x40064003))

#define  MCG_C5                      (*(uint8_t *) (0x40064004))

#define  MCG_C6                      (*(uint8_t *) (0x40064005))

#define  MCG_S                       (*(uint8_t *) (0x40064006))

#define  MCG_SC                      (*(uint8_t *) (0x40064008))

#define  MCG_ATCVH                   (*(uint8_t *) (0x4006400A))

#define  MCG_ATCVL                   (*(uint8_t *) (0x4006400B))

#define  MCG_C7                      (*(uint8_t *) (0x4006400C))

#define  MCG_C8                      (*(uint8_t *) (0x4006400D))

#define  MCG_C10                     (*(uint8_t *) (0x4006400F))

#define  MCM_CR                      (*(uint32_t *) (0xF000300C))

#define  MCM_PID                     (*(uint32_t *) (0xF0003030))

#define  MPU_CESR                    (*(uint32_t *) (0x4000A000))

#define  MPU_EAR0                    (*(uint32_t *) (0x4000A010))

#define  MPU_EDR0                    (*(uint32_t *) (0x4000A014))

#define  MPU_EAR1                    (*(uint32_t *) (0x4000A018))

#define  MPU_EDR1                    (*(uint32_t *) (0x4000A01C))

#define  MPU_RGD0_WORD0              (*(uint32_t *) (0x4000A400))

#define  MPU_RGD0_WORD1              (*(uint32_t *) (0x4000A404))

#define  MPU_RGD0_WORD2              (*(uint32_t *) (0x4000A408))

#define  MPU_RGD0_WORD3              (*(uint32_t *) (0x4000A40C))

#define  MPU_RGD1_WORD0              (*(uint32_t *) (0x4000A410))

#define  MPU_RGD1_WORD1              (*(uint32_t *) (0x4000A414))

#define  MPU_RGD1_WORD2              (*(uint32_t *) (0x4000A418))

#define  MPU_RGD1_WORD3              (*(uint32_t *) (0x4000A41C))

#define  MPU_RGD2_WORD0              (*(uint32_t *) (0x4000A420))

#define  MPU_RGD2_WORD1              (*(uint32_t *) (0x4000A424))

#define  MPU_RGD2_WORD2              (*(uint32_t *) (0x4000A428))

#define  MPU_RGD2_WORD3              (*(uint32_t *) (0x4000A42C))

#define  MPU_RGD3_WORD0              (*(uint32_t *) (0x4000A430))

#define  MPU_RGD3_WORD1              (*(uint32_t *) (0x4000A434))

#define  MPU_RGD3_WORD2              (*(uint32_t *) (0x4000A438))

#define  MPU_RGD3_WORD3              (*(uint32_t *) (0x4000A43C))

#define  MPU_RGD4_WORD0              (*(uint32_t *) (0x4000A440))

#define  MPU_RGD4_WORD1              (*(uint32_t *) (0x4000A444))

#define  MPU_RGD4_WORD2              (*(uint32_t *) (0x4000A448))

#define  MPU_RGD4_WORD3              (*(uint32_t *) (0x4000A44C))

#define  MPU_RGD5_WORD0              (*(uint32_t *) (0x4000A450))

#define  MPU_RGD5_WORD1              (*(uint32_t *) (0x4000A454))

#define  MPU_RGD5_WORD2              (*(uint32_t *) (0x4000A458))

#define  MPU_RGD5_WORD3              (*(uint32_t *) (0x4000A45C))

#define  MPU_RGD6_WORD0              (*(uint32_t *) (0x4000A460))

#define  MPU_RGD6_WORD1              (*(uint32_t *) (0x4000A464))

#define  MPU_RGD6_WORD2              (*(uint32_t *) (0x4000A468))

#define  MPU_RGD6_WORD3              (*(uint32_t *) (0x4000A46C))

#define  MPU_RGD7_WORD0              (*(uint32_t *) (0x4000A470))

#define  MPU_RGD7_WORD1              (*(uint32_t *) (0x4000A474))

#define  MPU_RGD7_WORD2              (*(uint32_t *) (0x4000A478))

#define  MPU_RGD7_WORD3              (*(uint32_t *) (0x4000A47C))

#define  MPU_RGDAAC0                 (*(uint32_t *) (0x4000A800))

#define  MPU_RGDAAC1                 (*(uint32_t *) (0x4000A804))

#define  MPU_RGDAAC2                 (*(uint32_t *) (0x4000A808))

#define  MPU_RGDAAC3                 (*(uint32_t *) (0x4000A80C))

#define  MPU_RGDAAC4                 (*(uint32_t *) (0x4000A810))

#define  MPU_RGDAAC5                 (*(uint32_t *) (0x4000A814))

#define  MPU_RGDAAC6                 (*(uint32_t *) (0x4000A818))

#define  MPU_RGDAAC7                 (*(uint32_t *) (0x4000A81C))

#define  MTB_POSITION                (*(uint32_t *) (0xF0000000))

#define  MTB_MASTER                  (*(uint32_t *) (0xF0000004))

#define  MTB_FLOW                    (*(uint32_t *) (0xF0000008))

#define  MTB_BASE                    (*(uint32_t *) (0xF000000C))

#define  MTB_MODECTRL                (*(uint32_t *) (0xF0000F00))

#define  MTB_TAGSET                  (*(uint32_t *) (0xF0000FA0))

#define  MTB_TAGCLEAR                (*(uint32_t *) (0xF0000FA4))

#define  MTB_LOCKACCESS              (*(uint32_t *) (0xF0000FB0))

#define  MTB_LOCKSTAT                (*(uint32_t *) (0xF0000FB4))

#define  MTB_AUTHSTAT                (*(uint32_t *) (0xF0000FB8))

#define  MTB_DEVICEARCH              (*(uint32_t *) (0xF0000FBC))

#define  MTB_DEVICECFG               (*(uint32_t *) (0xF0000FC8))

#define  MTB_DEVICETYPID             (*(uint32_t *) (0xF0000FCC))

#define  MTB_PERIPHID4               (*(uint32_t *) (0xF0000FD0))

#define  MTB_PERIPHID5               (*(uint32_t *) (0xF0000FD4))

#define  MTB_PERIPHID6               (*(uint32_t *) (0xF0000FD8))

#define  MTB_PERIPHID7               (*(uint32_t *) (0xF0000FDC))

#define  MTB_PERIPHID0               (*(uint32_t *) (0xF0000FE0))

#define  MTB_PERIPHID1               (*(uint32_t *) (0xF0000FE4))

#define  MTB_PERIPHID2               (*(uint32_t *) (0xF0000FE8))

#define  MTB_PERIPHID3               (*(uint32_t *) (0xF0000FEC))

#define  MTB_COMPID0                 (*(uint32_t *) (0xF0000FF0))

#define  MTB_COMPID1                 (*(uint32_t *) (0xF0000FF4))

#define  MTB_COMPID2                 (*(uint32_t *) (0xF0000FF8))

#define  MTB_COMPID3                 (*(uint32_t *) (0xF0000FFC))

#define  MTBDWT_CTRL                 (*(uint32_t *) (0xF0002000))

#define  MTBDWT_COMP0                (*(uint32_t *) (0xF0002020))

#define  MTBDWT_MASK0                (*(uint32_t *) (0xF0002024))

#define  MTBDWT_COMP1                (*(uint32_t *) (0xF000202A))

#define  MTBDWT_MASK1                (*(uint32_t *) (0xF000202E))

#define  MTBDWT_FCT1                 (*(uint32_t *) (0xF0002038))

#define  MTBDWT_TBCTRL               (*(uint32_t *) (0xF0002200))

#define  MTBDWT_DEVICECFG            (*(uint32_t *) (0xF0002FC8))

#define  MTBDWT_DEVICETYPID          (*(uint32_t *) (0xF0002FCC))

#define  MTBDWT_PERIPHID4            (*(uint32_t *) (0xF0002FD0))

#define  MTBDWT_PERIPHID5            (*(uint32_t *) (0xF0002FD4))

#define  MTBDWT_PERIPHID6            (*(uint32_t *) (0xF0002FD8))

#define  MTBDWT_PERIPHID7            (*(uint32_t *) (0xF0002FDC))

#define  MTBDWT_PERIPHID0            (*(uint32_t *) (0xF0002FE0))

#define  MTBDWT_PERIPHID1            (*(uint32_t *) (0xF0002FE4))

#define  MTBDWT_PERIPHID2            (*(uint32_t *) (0xF0002FE8))

#define  MTBDWT_PERIPHID3            (*(uint32_t *) (0xF0002FEC))

#define  MTBDWT_COMPID0              (*(uint32_t *) (0xF0002FF0))

#define  MTBDWT_COMPID1              (*(uint32_t *) (0xF0002FF4))

#define  MTBDWT_COMPID2              (*(uint32_t *) (0xF0002FF8))

#define  MTBDWT_COMPID3              (*(uint32_t *) (0xF0002FFC))

#define  OSC_CR                      (*(uint8_t *) (0x40066000))

#define  PIT0_MCR                    (*(uint32_t *) (0x4002D000))

#define  PIT0_LDVAL0                 (*(uint32_t *) (0x4002D100))

#define  PIT0_CVAL0                  (*(uint32_t *) (0x4002D104))

#define  PIT0_TCTRL0                 (*(uint32_t *) (0x4002D108))

#define  PIT0_TFLG0                  (*(uint32_t *) (0x4002D10C))

#define  PIT0_LDVAL1                 (*(uint32_t *) (0x4002D110))

#define  PIT0_CVAL1                  (*(uint32_t *) (0x4002D114))

#define  PIT0_TCTRL1                 (*(uint32_t *) (0x4002D118))

#define  PIT0_TFLG1                  (*(uint32_t *) (0x4002D11C))

#define  PIT0_LDVAL2                 (*(uint32_t *) (0x4002D120))

#define  PIT0_CVAL2                  (*(uint32_t *) (0x4002D124))

#define  PIT0_TCTRL2                 (*(uint32_t *) (0x4002D128))

#define  PIT0_TFLG2                  (*(uint32_t *) (0x4002D12C))

#define  PIT0_LDVAL3                 (*(uint32_t *) (0x4002D130))

#define  PIT0_CVAL3                  (*(uint32_t *) (0x4002D134))

#define  PIT0_TCTRL3                 (*(uint32_t *) (0x4002D138))

#define  PIT0_TFLG3                  (*(uint32_t *) (0x4002D13C))

#define  PIT1_MCR                    (*(uint32_t *) (0x4002E000))

#define  PIT1_LDVAL0                 (*(uint32_t *) (0x4002E100))

#define  PIT1_CVAL0                  (*(uint32_t *) (0x4002E104))

#define  PIT1_TCTRL0                 (*(uint32_t *) (0x4002E108))

#define  PIT1_TFLG0                  (*(uint32_t *) (0x4002E10C))

#define  PIT1_LDVAL1                 (*(uint32_t *) (0x4002E110))

#define  PIT1_CVAL1                  (*(uint32_t *) (0x4002E114))

#define  PIT1_TCTRL1                 (*(uint32_t *) (0x4002E118))

#define  PIT1_TFLG1                  (*(uint32_t *) (0x4002E11C))

#define  PIT1_LDVAL2                 (*(uint32_t *) (0x4002E120))

#define  PIT1_CVAL2                  (*(uint32_t *) (0x4002E124))

#define  PIT1_TCTRL2                 (*(uint32_t *) (0x4002E128))

#define  PIT1_TFLG2                  (*(uint32_t *) (0x4002E12C))

#define  PIT1_LDVAL3                 (*(uint32_t *) (0x4002E130))

#define  PIT1_CVAL3                  (*(uint32_t *) (0x4002E134))

#define  PIT1_TCTRL3                 (*(uint32_t *) (0x4002E138))

#define  PIT1_TFLG3                  (*(uint32_t *) (0x4002E13C))

#define  PMC_LVDSC1                  (*(uint8_t *) (0x4007D000))

#define  PMC_LVDSC2                  (*(uint8_t *) (0x4007D001))

#define  PMC_REGSC                   (*(uint8_t *) (0x4007D002))

#define  RCM_SRS0                    (*(uint8_t *) (0x4007B000))

#define  RCM_SRS1                    (*(uint8_t *) (0x4007B001))

#define  RCM_RPFC                    (*(uint8_t *) (0x4007B004))

#define  RCM_RPFW                    (*(uint8_t *) (0x4007B005))

#define  RNG_CR                      (*(uint32_t *) (0x40029000))

#define  RNG_SR                      (*(uint32_t *) (0x40029004))

#define  RNG_ER                      (*(uint32_t *) (0x40029008))

#define  RNG_OR                      (*(uint32_t *) (0x4002900C))

#define  ROM_ENTRY0                  (*(uint32_t *) (0xF0002000))

#define  ROM_ENTRY1                  (*(uint32_t *) (0xF0002004))

#define  ROM_ENTRY2                  (*(uint32_t *) (0xF0002008))

#define  ROM_TABLEMARK               (*(uint32_t *) (0xF000200C))

#define  ROM_PERIPHID4               (*(uint32_t *) (0xF0002FCC))

#define  ROM_SYSACCESS               (*(uint32_t *) (0xF0002FCC))

#define  ROM_PERIPHID5               (*(uint32_t *) (0xF0002FD0))

#define  ROM_PERIPHID6               (*(uint32_t *) (0xF0002FD4))

#define  ROM_PERIPHID7               (*(uint32_t *) (0xF0002FD8))

#define  ROM_PERIPHID0               (*(uint32_t *) (0xF0002FDC))

#define  ROM_PERIPHID1               (*(uint32_t *) (0xF0002FE0))

#define  ROM_PERIPHID2               (*(uint32_t *) (0xF0002FE4))

#define  ROM_PERIPHID3               (*(uint32_t *) (0xF0002FE8))

#define  ROM_COMPID0                 (*(uint32_t *) (0xF0002FF0))

#define  ROM_COMPID1                 (*(uint32_t *) (0xF0002FF4))

#define  ROM_COMPID2                 (*(uint32_t *) (0xF0002FF8))

#define  ROM_COMPID3                 (*(uint32_t *) (0xF0002FFC))

#define  RTC_YEARMON                 (*(uint16_t *) (0x40050000))

#define  RTC_DAYS                    (*(uint16_t *) (0x40050002))

#define  RTC_HOURMIN                 (*(uint16_t *) (0x40050004))

#define  RTC_SECONDS                 (*(uint16_t *) (0x40050006))

#define  RTC_ALM_YEARMON             (*(uint16_t *) (0x40050008))

#define  RTC_ALM_DAYS                (*(uint16_t *) (0x4005000A))

#define  RTC_ALM_HOURMIN             (*(uint16_t *) (0x4005000C))

#define  RTC_ALM_SECONDS             (*(uint16_t *) (0x4005000E))

#define  RTC_CTRL                    (*(uint16_t *) (0x40050010))

#define  RTC_STATUS                  (*(uint16_t *) (0x40050012))

#define  RTC_ISR                     (*(uint16_t *) (0x40050014))

#define  RTC_IER                     (*(uint16_t *) (0x40050016))

#define  RTC_GP_DATA_REG             (*(uint16_t *) (0x40050020))

#define  RTC_DST_HOUR                (*(uint16_t *) (0x40050022))

#define  RTC_DST_MONTH               (*(uint16_t *) (0x40050024))

#define  RTC_DST_DAY                 (*(uint16_t *) (0x40050026))

#define  RTC_COMPEN                  (*(uint16_t *) (0x40050028))

#define  RTC_TAMPER_DIRECTIONAL      (*(uint16_t *) (0x4005002C))

#define  RTC_TAMPER_INT              (*(uint16_t *) (0x4005002E))

#define  RTC_TTSR_YEAR               (*(uint16_t *) (0x40050030))

#define  RTC_FILTER01_CFG            (*(uint16_t *) (0x40050034))

#define  RTC_FILTER23_CFG            (*(uint16_t *) (0x40050036))

#define  RTC_TAMPER_QUEUE            (*(uint16_t *) (0x40050040))

#define  SIM_SOPT1                   (*(uint32_t *) (0x4003F000))

#define  SIM_SOPT1_CFG               (*(uint32_t *) (0x4003F004))

#define  SIM_CTRL_REG                (*(uint32_t *) (0x40040004))

#define  SIM_SOPT3                   (*(uint32_t *) (0x40040008))

#define  SIM_SDID                    (*(uint32_t *) (0x40040024))

#define  SIM_SCGC4                   (*(uint32_t *) (0x40040034))

#define  SIM_SCGC5                   (*(uint32_t *) (0x40040038))

#define  SIM_SCGC6                   (*(uint32_t *) (0x4004003C))

#define  SIM_SCGC7                   (*(uint32_t *) (0x40040040))

#define  SIM_CLKDIV1                 (*(uint32_t *) (0x40040044))

#define  SIM_FCFG1                   (*(uint32_t *) (0x4004004C))

#define  SIM_FCFG2                   (*(uint32_t *) (0x40040050))

#define  SIM_UID0                    (*(uint32_t *) (0x40040054))

#define  SIM_UID1                    (*(uint32_t *) (0x40040058))

#define  SIM_UID2                    (*(uint32_t *) (0x4004005C))

#define  SIM_UID3                    (*(uint32_t *) (0x40040060))

#define  SIM_MISC_CTL                (*(uint32_t *) (0x4004006C))

#define  SIM_ADC_COMP0               (*(uint32_t *) (0x400400C8))

#define  SIM_ADC_COMP1               (*(uint32_t *) (0x400400CC))

#define  SMC_PMPROT                  (*(uint8_t *) (0x4007E000))

#define  SMC_PMCTRL                  (*(uint8_t *) (0x4007E001))

#define  SMC_STOPCTRL                (*(uint8_t *) (0x4007E002))

#define  SMC_PMSTAT                  (*(uint8_t *) (0x4007E003))

#define  SPI0_C1                     (*(uint8_t *) (0x40075000))

#define  SPI0_C2                     (*(uint8_t *) (0x40075001))

#define  SPI0_BR                     (*(uint8_t *) (0x40075002))

#define  SPI0_S                      (*(uint8_t *) (0x40075003))

#define  SPI0_DH                     (*(uint8_t *) (0x40075004))

#define  SPI0_DL                     (*(uint8_t *) (0x40075005))

#define  SPI0_MH                     (*(uint8_t *) (0x40075006))

#define  SPI0_ML                     (*(uint8_t *) (0x40075007))

#define  SPI0_C3                     (*(uint8_t *) (0x40075008))

#define  SPI0_CI                     (*(uint8_t *) (0x40075009))

#define  SPI1_C1                     (*(uint8_t *) (0x40076000))

#define  SPI1_C2                     (*(uint8_t *) (0x40076001))

#define  SPI1_BR                     (*(uint8_t *) (0x40076002))

#define  SPI1_S                      (*(uint8_t *) (0x40076003))

#define  SPI1_DH                     (*(uint8_t *) (0x40076004))

#define  SPI1_DL                     (*(uint8_t *) (0x40076005))

#define  SPI1_MH                     (*(uint8_t *) (0x40076006))

#define  SPI1_ML                     (*(uint8_t *) (0x40076007))

#define  SPI1_C3                     (*(uint8_t *) (0x40076008))

#define  SPI1_CI                     (*(uint8_t *) (0x40076009))

#define  TMR0_0_COMP1                (*(uint16_t *) (0x40057000))

#define  TMR0_0_COMP2                (*(uint16_t *) (0x40057001))

#define  TMR0_0_CAPT                 (*(uint16_t *) (0x40057002))

#define  TMR0_0_LOAD                 (*(uint16_t *) (0x40057003))

#define  TMR0_0_HOLD                 (*(uint16_t *) (0x40057004))

#define  TMR0_0_CNTR                 (*(uint16_t *) (0x40057005))

#define  TMR0_0_CTRL                 (*(uint16_t *) (0x40057006))

#define  TMR0_0_SCTRL                (*(uint16_t *) (0x40057007))

#define  TMR0_0_CMPLD1               (*(uint16_t *) (0x40057008))

#define  TMR0_0_CMPLD2               (*(uint16_t *) (0x40057009))

#define  TMR0_0_CSCTRL               (*(uint16_t *) (0x4005700A))

#define  TMR0_0_FILT                 (*(uint16_t *) (0x4005700B))

#define  TMR0_0_DMA                  (*(uint16_t *) (0x4005700C))

#define  TMR0_0_ENBL                 (*(uint16_t *) (0x4005700F))

#define  TMR0_1_COMP1                (*(uint16_t *) (0x40057010))

#define  TMR0_1_COMP2                (*(uint16_t *) (0x40057011))

#define  TMR0_1_CAPT                 (*(uint16_t *) (0x40057012))

#define  TMR0_1_LOAD                 (*(uint16_t *) (0x40057013))

#define  TMR0_1_HOLD                 (*(uint16_t *) (0x40057014))

#define  TMR0_1_CNTR                 (*(uint16_t *) (0x40057015))

#define  TMR0_1_CTRL                 (*(uint16_t *) (0x40057016))

#define  TMR0_1_SCTRL                (*(uint16_t *) (0x40057017))

#define  TMR0_1_CMPLD1               (*(uint16_t *) (0x40057018))

#define  TMR0_1_CMPLD2               (*(uint16_t *) (0x40057019))

#define  TMR0_1_CSCTRL               (*(uint16_t *) (0x4005701A))

#define  TMR0_1_FILT                 (*(uint16_t *) (0x4005701B))

#define  TMR0_1_DMA                  (*(uint16_t *) (0x4005701C))

#define  TMR0_2_COMP1                (*(uint16_t *) (0x40057020))

#define  TMR0_2_COMP2                (*(uint16_t *) (0x40057021))

#define  TMR0_2_CAPT                 (*(uint16_t *) (0x40057022))

#define  TMR0_2_LOAD                 (*(uint16_t *) (0x40057023))

#define  TMR0_2_HOLD                 (*(uint16_t *) (0x40057024))

#define  TMR0_2_CNTR                 (*(uint16_t *) (0x40057025))

#define  TMR0_2_CTRL                 (*(uint16_t *) (0x40057026))

#define  TMR0_2_SCTRL                (*(uint16_t *) (0x40057027))

#define  TMR0_2_CMPLD1               (*(uint16_t *) (0x40057028))

#define  TMR0_2_CMPLD2               (*(uint16_t *) (0x40057029))

#define  TMR0_2_CSCTRL               (*(uint16_t *) (0x4005702A))

#define  TMR0_2_FILT                 (*(uint16_t *) (0x4005702B))

#define  TMR0_2_DMA                  (*(uint16_t *) (0x4005702C))

#define  TMR0_3_COMP1                (*(uint16_t *) (0x40057030))

#define  TMR0_3_COMP2                (*(uint16_t *) (0x40057031))

#define  TMR0_3_CAPT                 (*(uint16_t *) (0x40057032))

#define  TMR0_3_LOAD                 (*(uint16_t *) (0x40057033))

#define  TMR0_3_HOLD                 (*(uint16_t *) (0x40057034))

#define  TMR0_3_CNTR                 (*(uint16_t *) (0x40057035))

#define  TMR0_3_CTRL                 (*(uint16_t *) (0x40057036))

#define  TMR0_3_SCTRL                (*(uint16_t *) (0x40057037))

#define  TMR0_3_CMPLD1               (*(uint16_t *) (0x40057038))

#define  TMR0_3_CMPLD2               (*(uint16_t *) (0x40057039))

#define  TMR0_3_CSCTRL               (*(uint16_t *) (0x4005703A))

#define  TMR0_3_FILT                 (*(uint16_t *) (0x4005703B))

#define  TMR0_3_DMA                  (*(uint16_t *) (0x4005703C))

#define  TMR1_0_COMP1                (*(uint16_t *) (0x40058000))

#define  TMR1_0_COMP2                (*(uint16_t *) (0x40058001))

#define  TMR1_0_CAPT                 (*(uint16_t *) (0x40058002))

#define  TMR1_0_LOAD                 (*(uint16_t *) (0x40058003))

#define  TMR1_0_HOLD                 (*(uint16_t *) (0x40058004))

#define  TMR1_0_CNTR                 (*(uint16_t *) (0x40058005))

#define  TMR1_0_CTRL                 (*(uint16_t *) (0x40058006))

#define  TMR1_0_SCTRL                (*(uint16_t *) (0x40058007))

#define  TMR1_0_CMPLD1               (*(uint16_t *) (0x40058008))

#define  TMR1_0_CMPLD2               (*(uint16_t *) (0x40058009))

#define  TMR1_0_CSCTRL               (*(uint16_t *) (0x4005800A))

#define  TMR1_0_FILT                 (*(uint16_t *) (0x4005800B))

#define  TMR1_0_DMA                  (*(uint16_t *) (0x4005800C))

#define  TMR1_0_ENBL                 (*(uint16_t *) (0x4005800F))

#define  TMR1_1_COMP1                (*(uint16_t *) (0x40058010))

#define  TMR1_1_COMP2                (*(uint16_t *) (0x40058011))

#define  TMR1_1_CAPT                 (*(uint16_t *) (0x40058012))

#define  TMR1_1_LOAD                 (*(uint16_t *) (0x40058013))

#define  TMR1_1_HOLD                 (*(uint16_t *) (0x40058014))

#define  TMR1_1_CNTR                 (*(uint16_t *) (0x40058015))

#define  TMR1_1_CTRL                 (*(uint16_t *) (0x40058016))

#define  TMR1_1_SCTRL                (*(uint16_t *) (0x40058017))

#define  TMR1_1_CMPLD1               (*(uint16_t *) (0x40058018))

#define  TMR1_1_CMPLD2               (*(uint16_t *) (0x40058019))

#define  TMR1_1_CSCTRL               (*(uint16_t *) (0x4005801A))

#define  TMR1_1_FILT                 (*(uint16_t *) (0x4005801B))

#define  TMR1_1_DMA                  (*(uint16_t *) (0x4005801C))

#define  TMR1_2_COMP1                (*(uint16_t *) (0x40058020))

#define  TMR1_2_COMP2                (*(uint16_t *) (0x40058021))

#define  TMR1_2_CAPT                 (*(uint16_t *) (0x40058022))

#define  TMR1_2_LOAD                 (*(uint16_t *) (0x40058023))

#define  TMR1_2_HOLD                 (*(uint16_t *) (0x40058024))

#define  TMR1_2_CNTR                 (*(uint16_t *) (0x40058025))

#define  TMR1_2_CTRL                 (*(uint16_t *) (0x40058026))

#define  TMR1_2_SCTRL                (*(uint16_t *) (0x40058027))

#define  TMR1_2_CMPLD1               (*(uint16_t *) (0x40058028))

#define  TMR1_2_CMPLD2               (*(uint16_t *) (0x40058029))

#define  TMR1_2_CSCTRL               (*(uint16_t *) (0x4005802A))

#define  TMR1_2_FILT                 (*(uint16_t *) (0x4005802B))

#define  TMR1_2_DMA                  (*(uint16_t *) (0x4005802C))

#define  TMR1_3_COMP1                (*(uint16_t *) (0x40058030))

#define  TMR1_3_COMP2                (*(uint16_t *) (0x40058031))

#define  TMR1_3_CAPT                 (*(uint16_t *) (0x40058032))

#define  TMR1_3_LOAD                 (*(uint16_t *) (0x40058033))

#define  TMR1_3_HOLD                 (*(uint16_t *) (0x40058034))

#define  TMR1_3_CNTR                 (*(uint16_t *) (0x40058035))

#define  TMR1_3_CTRL                 (*(uint16_t *) (0x40058036))

#define  TMR1_3_SCTRL                (*(uint16_t *) (0x40058037))

#define  TMR1_3_CMPLD1               (*(uint16_t *) (0x40058038))

#define  TMR1_3_CMPLD2               (*(uint16_t *) (0x40058039))

#define  TMR1_3_CSCTRL               (*(uint16_t *) (0x4005803A))

#define  TMR1_3_FILT                 (*(uint16_t *) (0x4005803B))

#define  TMR1_3_DMA                  (*(uint16_t *) (0x4005803C))

#define  TMR2_0_COMP1                (*(uint16_t *) (0x40059000))

#define  TMR2_0_COMP2                (*(uint16_t *) (0x40059001))

#define  TMR2_0_CAPT                 (*(uint16_t *) (0x40059002))

#define  TMR2_0_LOAD                 (*(uint16_t *) (0x40059003))

#define  TMR2_0_HOLD                 (*(uint16_t *) (0x40059004))

#define  TMR2_0_CNTR                 (*(uint16_t *) (0x40059005))

#define  TMR2_0_CTRL                 (*(uint16_t *) (0x40059006))

#define  TMR2_0_SCTRL                (*(uint16_t *) (0x40059007))

#define  TMR2_0_CMPLD1               (*(uint16_t *) (0x40059008))

#define  TMR2_0_CMPLD2               (*(uint16_t *) (0x40059009))

#define  TMR2_0_CSCTRL               (*(uint16_t *) (0x4005900A))

#define  TMR2_0_FILT                 (*(uint16_t *) (0x4005900B))

#define  TMR2_0_DMA                  (*(uint16_t *) (0x4005900C))

#define  TMR2_0_ENBL                 (*(uint16_t *) (0x4005900F))

#define  TMR2_1_COMP1                (*(uint16_t *) (0x40059010))

#define  TMR2_1_COMP2                (*(uint16_t *) (0x40059011))

#define  TMR2_1_CAPT                 (*(uint16_t *) (0x40059012))

#define  TMR2_1_LOAD                 (*(uint16_t *) (0x40059013))

#define  TMR2_1_HOLD                 (*(uint16_t *) (0x40059014))

#define  TMR2_1_CNTR                 (*(uint16_t *) (0x40059015))

#define  TMR2_1_CTRL                 (*(uint16_t *) (0x40059016))

#define  TMR2_1_SCTRL                (*(uint16_t *) (0x40059017))

#define  TMR2_1_CMPLD1               (*(uint16_t *) (0x40059018))

#define  TMR2_1_CMPLD2               (*(uint16_t *) (0x40059019))

#define  TMR2_1_CSCTRL               (*(uint16_t *) (0x4005901A))

#define  TMR2_1_FILT                 (*(uint16_t *) (0x4005901B))

#define  TMR2_1_DMA                  (*(uint16_t *) (0x4005901C))

#define  TMR2_2_COMP1                (*(uint16_t *) (0x40059020))

#define  TMR2_2_COMP2                (*(uint16_t *) (0x40059021))

#define  TMR2_2_CAPT                 (*(uint16_t *) (0x40059022))

#define  TMR2_2_LOAD                 (*(uint16_t *) (0x40059023))

#define  TMR2_2_HOLD                 (*(uint16_t *) (0x40059024))

#define  TMR2_2_CNTR                 (*(uint16_t *) (0x40059025))

#define  TMR2_2_CTRL                 (*(uint16_t *) (0x40059026))

#define  TMR2_2_SCTRL                (*(uint16_t *) (0x40059027))

#define  TMR2_2_CMPLD1               (*(uint16_t *) (0x40059028))

#define  TMR2_2_CMPLD2               (*(uint16_t *) (0x40059029))

#define  TMR2_2_CSCTRL               (*(uint16_t *) (0x4005902A))

#define  TMR2_2_FILT                 (*(uint16_t *) (0x4005902B))

#define  TMR2_2_DMA                  (*(uint16_t *) (0x4005902C))

#define  TMR2_3_COMP1                (*(uint16_t *) (0x40059030))

#define  TMR2_3_COMP2                (*(uint16_t *) (0x40059031))

#define  TMR2_3_CAPT                 (*(uint16_t *) (0x40059032))

#define  TMR2_3_LOAD                 (*(uint16_t *) (0x40059033))

#define  TMR2_3_HOLD                 (*(uint16_t *) (0x40059034))

#define  TMR2_3_CNTR                 (*(uint16_t *) (0x40059035))

#define  TMR2_3_CTRL                 (*(uint16_t *) (0x40059036))

#define  TMR2_3_SCTRL                (*(uint16_t *) (0x40059037))

#define  TMR2_3_CMPLD1               (*(uint16_t *) (0x40059038))

#define  TMR2_3_CMPLD2               (*(uint16_t *) (0x40059039))

#define  TMR2_3_CSCTRL               (*(uint16_t *) (0x4005903A))

#define  TMR2_3_FILT                 (*(uint16_t *) (0x4005903B))

#define  TMR2_3_DMA                  (*(uint16_t *) (0x4005903C))

#define  TMR3_0_COMP1                (*(uint16_t *) (0x4005A000))

#define  TMR3_0_COMP2                (*(uint16_t *) (0x4005A001))

#define  TMR3_0_CAPT                 (*(uint16_t *) (0x4005A002))

#define  TMR3_0_LOAD                 (*(uint16_t *) (0x4005A003))

#define  TMR3_0_HOLD                 (*(uint16_t *) (0x4005A004))

#define  TMR3_0_CNTR                 (*(uint16_t *) (0x4005A005))

#define  TMR3_0_CTRL                 (*(uint16_t *) (0x4005A006))

#define  TMR3_0_SCTRL                (*(uint16_t *) (0x4005A007))

#define  TMR3_0_CMPLD1               (*(uint16_t *) (0x4005A008))

#define  TMR3_0_CMPLD2               (*(uint16_t *) (0x4005A009))

#define  TMR3_0_CSCTRL               (*(uint16_t *) (0x4005A00A))

#define  TMR3_0_FILT                 (*(uint16_t *) (0x4005A00B))

#define  TMR3_0_DMA                  (*(uint16_t *) (0x4005A00C))

#define  TMR3_0_ENBL                 (*(uint16_t *) (0x4005A00F))

#define  TMR3_1_COMP1                (*(uint16_t *) (0x4005A010))

#define  TMR3_1_COMP2                (*(uint16_t *) (0x4005A011))

#define  TMR3_1_CAPT                 (*(uint16_t *) (0x4005A012))

#define  TMR3_1_LOAD                 (*(uint16_t *) (0x4005A013))

#define  TMR3_1_HOLD                 (*(uint16_t *) (0x4005A014))

#define  TMR3_1_CNTR                 (*(uint16_t *) (0x4005A015))

#define  TMR3_1_CTRL                 (*(uint16_t *) (0x4005A016))

#define  TMR3_1_SCTRL                (*(uint16_t *) (0x4005A017))

#define  TMR3_1_CMPLD1               (*(uint16_t *) (0x4005A018))

#define  TMR3_1_CMPLD2               (*(uint16_t *) (0x4005A019))

#define  TMR3_1_CSCTRL               (*(uint16_t *) (0x4005A01A))

#define  TMR3_1_FILT                 (*(uint16_t *) (0x4005A01B))

#define  TMR3_1_DMA                  (*(uint16_t *) (0x4005A01C))

#define  TMR3_2_COMP1                (*(uint16_t *) (0x4005A020))

#define  TMR3_2_COMP2                (*(uint16_t *) (0x4005A021))

#define  TMR3_2_CAPT                 (*(uint16_t *) (0x4005A022))

#define  TMR3_2_LOAD                 (*(uint16_t *) (0x4005A023))

#define  TMR3_2_HOLD                 (*(uint16_t *) (0x4005A024))

#define  TMR3_2_CNTR                 (*(uint16_t *) (0x4005A025))

#define  TMR3_2_CTRL                 (*(uint16_t *) (0x4005A026))

#define  TMR3_2_SCTRL                (*(uint16_t *) (0x4005A027))

#define  TMR3_2_CMPLD1               (*(uint16_t *) (0x4005A028))

#define  TMR3_2_CMPLD2               (*(uint16_t *) (0x4005A029))

#define  TMR3_2_CSCTRL               (*(uint16_t *) (0x4005A02A))

#define  TMR3_2_FILT                 (*(uint16_t *) (0x4005A02B))

#define  TMR3_2_DMA                  (*(uint16_t *) (0x4005A02C))

#define  TMR3_3_COMP1                (*(uint16_t *) (0x4005A030))

#define  TMR3_3_COMP2                (*(uint16_t *) (0x4005A031))

#define  TMR3_3_CAPT                 (*(uint16_t *) (0x4005A032))

#define  TMR3_3_LOAD                 (*(uint16_t *) (0x4005A033))

#define  TMR3_3_HOLD                 (*(uint16_t *) (0x4005A034))

#define  TMR3_3_CNTR                 (*(uint16_t *) (0x4005A035))

#define  TMR3_3_CTRL                 (*(uint16_t *) (0x4005A036))

#define  TMR3_3_SCTRL                (*(uint16_t *) (0x4005A037))

#define  TMR3_3_CMPLD1               (*(uint16_t *) (0x4005A038))

#define  TMR3_3_CMPLD2               (*(uint16_t *) (0x4005A039))

#define  TMR3_3_CSCTRL               (*(uint16_t *) (0x4005A03A))

#define  TMR3_3_FILT                 (*(uint16_t *) (0x4005A03B))

#define  TMR3_3_DMA                  (*(uint16_t *) (0x4005A03C))

#define  UART0_BDH                   (*(uint8_t *) (0x4006A000))

#define  UART0_BDL                   (*(uint8_t *) (0x4006A001))

#define  UART0_C1                    (*(uint8_t *) (0x4006A002))

#define  UART0_C2                    (*(uint8_t *) (0x4006A003))

#define  UART0_S1                    (*(uint8_t *) (0x4006A004))

#define  UART0_S2                    (*(uint8_t *) (0x4006A005))

#define  UART0_C3                    (*(uint8_t *) (0x4006A006))

#define  UART0_D                     (*(uint8_t *) (0x4006A007))

#define  UART0_MA1                   (*(uint8_t *) (0x4006A008))

#define  UART0_MA2                   (*(uint8_t *) (0x4006A009))

#define  UART0_C4                    (*(uint8_t *) (0x4006A00A))

#define  UART0_C5                    (*(uint8_t *) (0x4006A00B))

#define  UART0_IR                    (*(uint8_t *) (0x4006A00E))

#define  UART0_C7816                 (*(uint8_t *) (0x4006A018))

#define  UART0_IE7816                (*(uint8_t *) (0x4006A019))

#define  UART0_IS7816                (*(uint8_t *) (0x4006A01A))

#define  UART0_WP7816T0              (*(uint8_t *) (0x4006A01B))

#define  UART0_WP7816T1              (*(uint8_t *) (0x4006A01B))

#define  UART0_WN7816                (*(uint8_t *) (0x4006A01C))

#define  UART0_WF7816                (*(uint8_t *) (0x4006A01D))

#define  UART0_ET7816                (*(uint8_t *) (0x4006A01E))

#define  UART0_TL7816                (*(uint8_t *) (0x4006A01F))

#define  UART0_WB                    (*(uint8_t *) (0x4006A02A))

#define  UART1_BDH                   (*(uint8_t *) (0x4006B000))

#define  UART1_BDL                   (*(uint8_t *) (0x4006B001))

#define  UART1_C1                    (*(uint8_t *) (0x4006B002))

#define  UART1_C2                    (*(uint8_t *) (0x4006B003))

#define  UART1_S1                    (*(uint8_t *) (0x4006B004))

#define  UART1_S2                    (*(uint8_t *) (0x4006B005))

#define  UART1_C3                    (*(uint8_t *) (0x4006B006))

#define  UART1_D                     (*(uint8_t *) (0x4006B007))

#define  UART1_MA1                   (*(uint8_t *) (0x4006B008))

#define  UART1_MA2                   (*(uint8_t *) (0x4006B009))

#define  UART1_C4                    (*(uint8_t *) (0x4006B00A))

#define  UART1_C5                    (*(uint8_t *) (0x4006B00B))

#define  UART1_IR                    (*(uint8_t *) (0x4006B00E))

#define  UART1_C7816                 (*(uint8_t *) (0x4006B018))

#define  UART1_IE7816                (*(uint8_t *) (0x4006B019))

#define  UART1_IS7816                (*(uint8_t *) (0x4006B01A))

#define  UART1_WP7816T0              (*(uint8_t *) (0x4006B01B))

#define  UART1_WP7816T1              (*(uint8_t *) (0x4006B01B))

#define  UART1_WN7816                (*(uint8_t *) (0x4006B01C))

#define  UART1_WF7816                (*(uint8_t *) (0x4006B01D))

#define  UART1_ET7816                (*(uint8_t *) (0x4006B01E))

#define  UART1_TL7816                (*(uint8_t *) (0x4006B01F))

#define  UART1_WB                    (*(uint8_t *) (0x4006B02A))

#define  UART2_BDH                   (*(uint8_t *) (0x4006C000))

#define  UART2_BDL                   (*(uint8_t *) (0x4006C001))

#define  UART2_C1                    (*(uint8_t *) (0x4006C002))

#define  UART2_C2                    (*(uint8_t *) (0x4006C003))

#define  UART2_S1                    (*(uint8_t *) (0x4006C004))

#define  UART2_S2                    (*(uint8_t *) (0x4006C005))

#define  UART2_C3                    (*(uint8_t *) (0x4006C006))

#define  UART2_D                     (*(uint8_t *) (0x4006C007))

#define  UART2_MA1                   (*(uint8_t *) (0x4006C008))

#define  UART2_MA2                   (*(uint8_t *) (0x4006C009))

#define  UART2_C4                    (*(uint8_t *) (0x4006C00A))

#define  UART2_C5                    (*(uint8_t *) (0x4006C00B))

#define  UART2_IR                    (*(uint8_t *) (0x4006C00E))

#define  UART2_C7816                 (*(uint8_t *) (0x4006C018))

#define  UART2_IE7816                (*(uint8_t *) (0x4006C019))

#define  UART2_IS7816                (*(uint8_t *) (0x4006C01A))

#define  UART2_WP7816T0              (*(uint8_t *) (0x4006C01B))

#define  UART2_WP7816T1              (*(uint8_t *) (0x4006C01B))

#define  UART2_WN7816                (*(uint8_t *) (0x4006C01C))

#define  UART2_WF7816                (*(uint8_t *) (0x4006C01D))

#define  UART2_ET7816                (*(uint8_t *) (0x4006C01E))

#define  UART2_TL7816                (*(uint8_t *) (0x4006C01F))

#define  UART2_WB                    (*(uint8_t *) (0x4006C02A))

#define  UART3_BDH                   (*(uint8_t *) (0x4006D000))

#define  UART3_BDL                   (*(uint8_t *) (0x4006D001))

#define  UART3_C1                    (*(uint8_t *) (0x4006D002))

#define  UART3_C2                    (*(uint8_t *) (0x4006D003))

#define  UART3_S1                    (*(uint8_t *) (0x4006D004))

#define  UART3_S2                    (*(uint8_t *) (0x4006D005))

#define  UART3_C3                    (*(uint8_t *) (0x4006D006))

#define  UART3_D                     (*(uint8_t *) (0x4006D007))

#define  UART3_MA1                   (*(uint8_t *) (0x4006D008))

#define  UART3_MA2                   (*(uint8_t *) (0x4006D009))

#define  UART3_C4                    (*(uint8_t *) (0x4006D00A))

#define  UART3_C5                    (*(uint8_t *) (0x4006D00B))

#define  UART3_IR                    (*(uint8_t *) (0x4006D00E))

#define  UART3_C7816                 (*(uint8_t *) (0x4006D018))

#define  UART3_IE7816                (*(uint8_t *) (0x4006D019))

#define  UART3_IS7816                (*(uint8_t *) (0x4006D01A))

#define  UART3_WP7816T0              (*(uint8_t *) (0x4006D01B))

#define  UART3_WP7816T1              (*(uint8_t *) (0x4006D01B))

#define  UART3_WN7816                (*(uint8_t *) (0x4006D01C))

#define  UART3_WF7816                (*(uint8_t *) (0x4006D01D))

#define  UART3_ET7816                (*(uint8_t *) (0x4006D01E))

#define  UART3_TL7816                (*(uint8_t *) (0x4006D01F))

#define  UART3_WB                    (*(uint8_t *) (0x4006D02A))

#define  VREF_VREFH_TRM              (*(uint8_t *) (0x4006F000))

#define  VREF_VREFH_SC               (*(uint8_t *) (0x4006F001))

#define  VREF_VREFL_TRM              (*(uint8_t *) (0x4006F005))

#define  WDOG_STCTRLH                (*(uint16_t *) (0x40053000))

#define  WDOG_STCTRLL                (*(uint16_t *) (0x40053002))

#define  WDOG_TOVALH                 (*(uint16_t *) (0x40053004))

#define  WDOG_WINH                   (*(uint16_t *) (0x40053008))

#define  WDOG_WINL                   (*(uint16_t *) (0x4005300A))

#define  WDOG_REFRESH                (*(uint16_t *) (0x4005300C))

#define  WDOG_UNLOCK                 (*(uint16_t *) (0x4005300E))

#define  WDOG_TMROUTH                (*(uint16_t *) (0x40053010))

#define  WDOG_TMROUTL                (*(uint16_t *) (0x40053012))

#define  WDOG_RSTCNT                 (*(uint16_t *) (0x40053014))

#define  WDOG_PRESC                  (*(uint16_t *) (0x40053016))

#define  XBAR_SEL0                   (*(uint16_t *) (0x40055000))

#define  XBAR_SEL1                   (*(uint16_t *) (0x40055001))

#define  XBAR_SEL2                   (*(uint16_t *) (0x40055002))

#define  XBAR_SEL3                   (*(uint16_t *) (0x40055003))

#define  XBAR_SEL4                   (*(uint16_t *) (0x40055004))

#define  XBAR_SEL5                   (*(uint16_t *) (0x40055005))

#define  XBAR_SEL6                   (*(uint16_t *) (0x40055006))

#define  XBAR_SEL7                   (*(uint16_t *) (0x40055007))

#define  XBAR_SEL8                   (*(uint16_t *) (0x40055008))

#define  XBAR_SEL9                   (*(uint16_t *) (0x40055009))

#define  XBAR_SEL10                  (*(uint16_t *) (0x4005500A))

#define  XBAR_SEL11                  (*(uint16_t *) (0x4005500B))

#define  XBAR_SEL12                  (*(uint16_t *) (0x4005500C))

#define  XBAR_CTRL0                  (*(uint16_t *) (0x4005500D))

//////////////////////////////////////////////////////////////////////////////////////
// The following section defines the IO register bitfield locations and includes some commentary on the meaning of some settings.
//////////////////////////////////////////////////////////////////////////////////////

#define  ADC_SC1A_ADCH_MASK          0x5u
#define  ADC_SC1A_ADCH_SHIFT         0
#define  ADC_SC1A_ADCH(x)            (((uint32_t)(((uint32_t)(x))<<ADC_SC1A_ADCH_SHIFT))&ADC_SC1A_ADCH_MASK)
//   Input channel select
//     0b00000 ==> When DIFF=0, DADP0 is selected as input; when DIFF=1, DAD0 is selected as input.
//     0b00001 ==> When DIFF=0, DADP1 is selected as input; when DIFF=1, DAD1 is selected as input.
//     0b00010 ==> When DIFF=0, DADP2 is selected as input; when DIFF=1, DAD2 is selected as input.
//     0b00011 ==> When DIFF=0, DADP3 is selected as input; when DIFF=1, DAD3 is selected as input.
//     0b00100 ==> When DIFF=0, AD4 is selected as input; when DIFF=1, it is reserved.
//     0b00101 ==> When DIFF=0, AD5 is selected as input; when DIFF=1, it is reserved.
//     0b00110 ==> When DIFF=0, AD6 is selected as input; when DIFF=1, it is reserved.
//     0b00111 ==> When DIFF=0, AD7 is selected as input; when DIFF=1, it is reserved.
//     0b01000 ==> When DIFF=0, AD8 is selected as input; when DIFF=1, it is reserved.
//     0b01001 ==> When DIFF=0, AD9 is selected as input; when DIFF=1, it is reserved.
//     0b01010 ==> When DIFF=0, AD10 is selected as input; when DIFF=1, it is reserved.
//     0b01011 ==> When DIFF=0, AD11 is selected as input; when DIFF=1, it is reserved.
//     0b01100 ==> When DIFF=0, AD12 is selected as input; when DIFF=1, it is reserved.
//     0b01101 ==> When DIFF=0, AD13 is selected as input; when DIFF=1, it is reserved.
//     0b01110 ==> When DIFF=0, AD14 is selected as input; when DIFF=1, it is reserved.
//     0b01111 ==> When DIFF=0, AD15 is selected as input; when DIFF=1, it is reserved.
//     0b10000 ==> When DIFF=0, AD16 is selected as input; when DIFF=1, it is reserved.
//     0b10001 ==> When DIFF=0, AD17 is selected as input; when DIFF=1, it is reserved.
//     0b10010 ==> When DIFF=0, AD18 is selected as input; when DIFF=1, it is reserved.
//     0b10011 ==> When DIFF=0, AD19 is selected as input; when DIFF=1, it is reserved.
//     0b10100 ==> When DIFF=0, AD20 is selected as input; when DIFF=1, it is reserved.
//     0b10101 ==> When DIFF=0, AD21 is selected as input; when DIFF=1, it is reserved.
//     0b10110 ==> When DIFF=0, AD22 is selected as input; when DIFF=1, it is reserved.
//     0b10111 ==> When DIFF=0, AD23 is selected as input; when DIFF=1, it is reserved.
//     0b11000 ==> Reserved.
//     0b11001 ==> Reserved.
//     0b11010 ==> When DIFF=0, Temp Sensor (single-ended) is selected as input; when DIFF=1, Temp Sensor (differential) is selected as input.
//     0b11011 ==> When DIFF=0, (single-ended) is selected as input; when DIFF=1, (differential) is selected as input.
//     0b11100 ==> Reserved.
//     0b11101 ==> When DIFF=0,VREFSH is selected as input; when DIFF=1, -VREFSH (differential) is selected as input. Voltage reference selected is determined by SC2[REFSEL].
//     0b11110 ==> When DIFF=0,VREFSL is selected as input; when DIFF=1, it is reserved. Voltage reference selected is determined by SC2[REFSEL].
//     0b11111 ==> Module is disabled.

#define  ADC_SC1A_DIFF_MASK          0x20u
#define  ADC_SC1A_DIFF_SHIFT         5
//   Differential Mode Enable
//     0b0 ==> Single-ended conversions and input channels are selected.
//     0b1 ==> Differential conversions and input channels are selected.

#define  ADC_SC1A_AIEN_MASK          0x40u
#define  ADC_SC1A_AIEN_SHIFT         6
//   Interrupt Enable
//     0b0 ==> Conversion complete interrupt is disabled.
//     0b1 ==> Conversion complete interrupt is enabled.

#define  ADC_SC1A_COCO_MASK          0x80u
#define  ADC_SC1A_COCO_SHIFT         7
//   Conversion Complete Flag
//     0b0 ==> Conversion is not completed.
//     0b1 ==> Conversion is completed.

#define  ADC_SC1B_ADCH_MASK          0x5u
#define  ADC_SC1B_ADCH_SHIFT         0
#define  ADC_SC1B_ADCH(x)            (((uint32_t)(((uint32_t)(x))<<ADC_SC1B_ADCH_SHIFT))&ADC_SC1B_ADCH_MASK)
//   Input channel select
//     0b00000 ==> When DIFF=0, DADP0 is selected as input; when DIFF=1, DAD0 is selected as input.
//     0b00001 ==> When DIFF=0, DADP1 is selected as input; when DIFF=1, DAD1 is selected as input.
//     0b00010 ==> When DIFF=0, DADP2 is selected as input; when DIFF=1, DAD2 is selected as input.
//     0b00011 ==> When DIFF=0, DADP3 is selected as input; when DIFF=1, DAD3 is selected as input.
//     0b00100 ==> When DIFF=0, AD4 is selected as input; when DIFF=1, it is reserved.
//     0b00101 ==> When DIFF=0, AD5 is selected as input; when DIFF=1, it is reserved.
//     0b00110 ==> When DIFF=0, AD6 is selected as input; when DIFF=1, it is reserved.
//     0b00111 ==> When DIFF=0, AD7 is selected as input; when DIFF=1, it is reserved.
//     0b01000 ==> When DIFF=0, AD8 is selected as input; when DIFF=1, it is reserved.
//     0b01001 ==> When DIFF=0, AD9 is selected as input; when DIFF=1, it is reserved.
//     0b01010 ==> When DIFF=0, AD10 is selected as input; when DIFF=1, it is reserved.
//     0b01011 ==> When DIFF=0, AD11 is selected as input; when DIFF=1, it is reserved.
//     0b01100 ==> When DIFF=0, AD12 is selected as input; when DIFF=1, it is reserved.
//     0b01101 ==> When DIFF=0, AD13 is selected as input; when DIFF=1, it is reserved.
//     0b01110 ==> When DIFF=0, AD14 is selected as input; when DIFF=1, it is reserved.
//     0b01111 ==> When DIFF=0, AD15 is selected as input; when DIFF=1, it is reserved.
//     0b10000 ==> When DIFF=0, AD16 is selected as input; when DIFF=1, it is reserved.
//     0b10001 ==> When DIFF=0, AD17 is selected as input; when DIFF=1, it is reserved.
//     0b10010 ==> When DIFF=0, AD18 is selected as input; when DIFF=1, it is reserved.
//     0b10011 ==> When DIFF=0, AD19 is selected as input; when DIFF=1, it is reserved.
//     0b10100 ==> When DIFF=0, AD20 is selected as input; when DIFF=1, it is reserved.
//     0b10101 ==> When DIFF=0, AD21 is selected as input; when DIFF=1, it is reserved.
//     0b10110 ==> When DIFF=0, AD22 is selected as input; when DIFF=1, it is reserved.
//     0b10111 ==> When DIFF=0, AD23 is selected as input; when DIFF=1, it is reserved.
//     0b11000 ==> Reserved.
//     0b11001 ==> Reserved.
//     0b11010 ==> When DIFF=0, Temp Sensor (single-ended) is selected as input; when DIFF=1, Temp Sensor (differential) is selected as input.
//     0b11011 ==> When DIFF=0, (single-ended) is selected as input; when DIFF=1, (differential) is selected as input.
//     0b11100 ==> Reserved.
//     0b11101 ==> When DIFF=0,VREFSH is selected as input; when DIFF=1, -VREFSH (differential) is selected as input. Voltage reference selected is determined by SC2[REFSEL].
//     0b11110 ==> When DIFF=0,VREFSL is selected as input; when DIFF=1, it is reserved. Voltage reference selected is determined by SC2[REFSEL].
//     0b11111 ==> Module is disabled.

#define  ADC_SC1B_DIFF_MASK          0x20u
#define  ADC_SC1B_DIFF_SHIFT         5
//   Differential Mode Enable
//     0b0 ==> Single-ended conversions and input channels are selected.
//     0b1 ==> Differential conversions and input channels are selected.

#define  ADC_SC1B_AIEN_MASK          0x40u
#define  ADC_SC1B_AIEN_SHIFT         6
//   Interrupt Enable
//     0b0 ==> Conversion complete interrupt is disabled.
//     0b1 ==> Conversion complete interrupt is enabled.

#define  ADC_SC1B_COCO_MASK          0x80u
#define  ADC_SC1B_COCO_SHIFT         7
//   Conversion Complete Flag
//     0b0 ==> Conversion is not completed.
//     0b1 ==> Conversion is completed.

#define  ADC_CFG1_ADICLK_MASK        0x2u
#define  ADC_CFG1_ADICLK_SHIFT       0
#define  ADC_CFG1_ADICLK(x)          (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_ADICLK_SHIFT))&ADC_CFG1_ADICLK_MASK)
//   Input Clock Select
//     0b00 ==> Bus clock
//     0b01 ==> (Bus clock)/2
//     0b10 ==> Alternate clock (ALTCLK)
//     0b11 ==> Asynchronous clock (ADACK)

#define  ADC_CFG1_MODE_MASK          0x8u
#define  ADC_CFG1_MODE_SHIFT         2
#define  ADC_CFG1_MODE(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_MODE_SHIFT))&ADC_CFG1_MODE_MASK)
//   Conversion mode selection
//     0b00 ==> When DIFF=0:It is single-ended 8-bit conversion; when DIFF=1, it is differential 9-bit conversion with 2's complement output.
//     0b01 ==> When DIFF=0:It is single-ended 12-bit conversion ; when DIFF=1, it is differential 13-bit conversion with 2's complement output.
//     0b10 ==> When DIFF=0:It is single-ended 10-bit conversion ; when DIFF=1, it is differential 11-bit conversion with 2's complement output.
//     0b11 ==> When DIFF=0:It is single-ended 16-bit conversion; when DIFF=1, it is differential 16-bit conversion with 2's complement output.

#define  ADC_CFG1_ADLSMP_MASK        0x10u
#define  ADC_CFG1_ADLSMP_SHIFT       4
//   Sample time configuration
//     0b0 ==> Short sample time.
//     0b1 ==> Long sample time.

#define  ADC_CFG1_ADIV_MASK          0x40u
#define  ADC_CFG1_ADIV_SHIFT         5
#define  ADC_CFG1_ADIV(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_ADIV_SHIFT))&ADC_CFG1_ADIV_MASK)
//   Clock Divide Select
//     0b00 ==> The divide ratio is 1 and the clock rate is input clock.
//     0b01 ==> The divide ratio is 2 and the clock rate is (input clock)/2.
//     0b10 ==> The divide ratio is 4 and the clock rate is (input clock)/4.
//     0b11 ==> The divide ratio is 8 and the clock rate is (input clock)/8.

#define  ADC_CFG1_ADLPC_MASK         0x80u
#define  ADC_CFG1_ADLPC_SHIFT        7
//   Low-Power Configuration
//     0b0 ==> Normal power configuration.
//     0b1 ==> Low-power configuration. The power is reduced at the expense of maximum clock speed.

#define  ADC_CFG2_ADLSTS_MASK        0x2u
#define  ADC_CFG2_ADLSTS_SHIFT       0
#define  ADC_CFG2_ADLSTS(x)          (((uint32_t)(((uint32_t)(x))<<ADC_CFG2_ADLSTS_SHIFT))&ADC_CFG2_ADLSTS_MASK)
//   Long Sample Time Select
//     0b00 ==> Default longest sample time; 20 extra ADCK cycles; 24 ADCK cycles total.
//     0b01 ==> 12 extra ADCK cycles; 16 ADCK cycles total sample time.
//     0b10 ==> 6 extra ADCK cycles; 10 ADCK cycles total sample time.
//     0b11 ==> 2 extra ADCK cycles; 6 ADCK cycles total sample time.

#define  ADC_CFG2_ADHSC_MASK         0x4u
#define  ADC_CFG2_ADHSC_SHIFT        2
//   High-Speed Configuration
//     0b0 ==> Normal conversion sequence selected.
//     0b1 ==> High-speed conversion sequence selected with 2 additional ADCK cycles to total conversion time.

#define  ADC_CFG2_ADACKEN_MASK       0x8u
#define  ADC_CFG2_ADACKEN_SHIFT      3
//   Asynchronous Clock Output Enable
//     0b0 ==> Asynchronous clock output disabled; Asynchronous clock is enabled only if selected by ADICLK and a conversion is active.
//     0b1 ==> Asynchronous clock and clock output is enabled regardless of the state of the ADC.

#define  ADC_CFG2_MUXSEL_MASK        0x10u
#define  ADC_CFG2_MUXSEL_SHIFT       4
//   ADC Mux Select
//     0b0 ==> ADxxa channels are selected.
//     0b1 ==> ADxxb channels are selected.

#define  ADC_RA_D_MASK               0x10u
#define  ADC_RA_D_SHIFT              0
#define  ADC_RA_D(x)                 (((uint32_t)(((uint32_t)(x))<<ADC_RA_D_SHIFT))&ADC_RA_D_MASK)
//   Data result.

#define  ADC_RB_D_MASK               0x10u
#define  ADC_RB_D_SHIFT              0
#define  ADC_RB_D(x)                 (((uint32_t)(((uint32_t)(x))<<ADC_RB_D_SHIFT))&ADC_RB_D_MASK)
//   Data result.

#define  ADC_CV1_CV_MASK             0x10u
#define  ADC_CV1_CV_SHIFT            0
#define  ADC_CV1_CV(x)               (((uint32_t)(((uint32_t)(x))<<ADC_CV1_CV_SHIFT))&ADC_CV1_CV_MASK)
//   Compare Value.

#define  ADC_CV2_CV_MASK             0x10u
#define  ADC_CV2_CV_SHIFT            0
#define  ADC_CV2_CV(x)               (((uint32_t)(((uint32_t)(x))<<ADC_CV2_CV_SHIFT))&ADC_CV2_CV_MASK)
//   Compare Value.

#define  ADC_SC2_REFSEL_MASK         0x2u
#define  ADC_SC2_REFSEL_SHIFT        0
#define  ADC_SC2_REFSEL(x)           (((uint32_t)(((uint32_t)(x))<<ADC_SC2_REFSEL_SHIFT))&ADC_SC2_REFSEL_MASK)
//   Voltage Reference Selection
//     0b00 ==> Default voltage reference pin pair, that is, external pins V and V REFH REFL
//     0b01 ==> Alternate reference pair, that is, V and V . This pair may be additional external pins or internal sources depending on the MCU configuration. See the chip configuration information for details specific to this MCU. ALTH ALTL
//     0b10 ==> Internal bandgap reference and associated ground reference (V BGH and V BGL ). Consult the Chip Configuration information for details specific to this MCU.
//     0b11 ==> Reserved

#define  ADC_SC2_DMAEN_MASK          0x4u
#define  ADC_SC2_DMAEN_SHIFT         2
//   DMA Enable
//     0b0 ==> DMA is disabled.
//     0b1 ==> DMA is enabled and will assert the ADC DMA request during an ADC conversion complete event noted when any of the SC1n[COCO] flags is asserted.

#define  ADC_SC2_ACREN_MASK          0x8u
#define  ADC_SC2_ACREN_SHIFT         3
//   Compare Function Range Enable
//     0b0 ==> Range function disabled. Only CV1 is compared.
//     0b1 ==> Range function enabled. Both CV1 and CV2 are compared.

#define  ADC_SC2_ACFGT_MASK          0x10u
#define  ADC_SC2_ACFGT_SHIFT         4
//   Compare Function Greater Than Enable
//     0b0 ==> Configures less than threshold, outside range not inclusive and inside range not inclusive; functionality based on the values placed in CV1 and CV2.
//     0b1 ==> Configures greater than or equal to threshold, outside and inside ranges inclusive; functionality based on the values placed in CV1 and CV2.

#define  ADC_SC2_ACFE_MASK           0x20u
#define  ADC_SC2_ACFE_SHIFT          5
//   Compare Function Enable
//     0b0 ==> Compare function disabled.
//     0b1 ==> Compare function enabled.

#define  ADC_SC2_ADTRG_MASK          0x40u
#define  ADC_SC2_ADTRG_SHIFT         6
//   Conversion Trigger Select
//     0b0 ==> Software trigger selected.
//     0b1 ==> Hardware trigger selected.

#define  ADC_SC2_ADACT_MASK          0x80u
#define  ADC_SC2_ADACT_SHIFT         7
//   Conversion Active
//     0b0 ==> Conversion not in progress.
//     0b1 ==> Conversion in progress.

#define  ADC_SC3_AVGS_MASK           0x2u
#define  ADC_SC3_AVGS_SHIFT          0
#define  ADC_SC3_AVGS(x)             (((uint32_t)(((uint32_t)(x))<<ADC_SC3_AVGS_SHIFT))&ADC_SC3_AVGS_MASK)
//   Hardware Average Select
//     0b00 ==> 4 samples averaged.
//     0b01 ==> 8 samples averaged.
//     0b10 ==> 16 samples averaged.
//     0b11 ==> 32 samples averaged.

#define  ADC_SC3_AVGE_MASK           0x4u
#define  ADC_SC3_AVGE_SHIFT          2
//   Hardware Average Enable
//     0b0 ==> Hardware average function disabled.
//     0b1 ==> Hardware average function enabled.

#define  ADC_SC3_ADCO_MASK           0x8u
#define  ADC_SC3_ADCO_SHIFT          3
//   Continuous Conversion Enable
//     0b0 ==> One conversion or one set of conversions if the hardware average function is enabled, that is, AVGE=1, after initiating a conversion.
//     0b1 ==> Continuous conversions or sets of conversions if the hardware average function is enabled, that is, AVGE=1, after initiating a conversion.

#define  ADC_SC3_CALF_MASK           0x40u
#define  ADC_SC3_CALF_SHIFT          6
//   Calibration Failed Flag
//     0b0 ==> Calibration completed normally.
//     0b1 ==> Calibration failed. ADC accuracy specifications are not guaranteed.

#define  ADC_SC3_CAL_MASK            0x80u
#define  ADC_SC3_CAL_SHIFT           7
//   Calibration

#define  ADC_OFS_OFS_MASK            0x10u
#define  ADC_OFS_OFS_SHIFT           0
#define  ADC_OFS_OFS(x)              (((uint32_t)(((uint32_t)(x))<<ADC_OFS_OFS_SHIFT))&ADC_OFS_OFS_MASK)
//   Offset Error Correction Value

#define  ADC_PG_PG_MASK              0x10u
#define  ADC_PG_PG_SHIFT             0
#define  ADC_PG_PG(x)                (((uint32_t)(((uint32_t)(x))<<ADC_PG_PG_SHIFT))&ADC_PG_PG_MASK)
//   Plus-Side Gain

#define  ADC_MG_MG_MASK              0x10u
#define  ADC_MG_MG_SHIFT             0
#define  ADC_MG_MG(x)                (((uint32_t)(((uint32_t)(x))<<ADC_MG_MG_SHIFT))&ADC_MG_MG_MASK)
//   Minus-Side Gain

#define  ADC_CLPD_CLPD_MASK          0x6u
#define  ADC_CLPD_CLPD_SHIFT         0
#define  ADC_CLPD_CLPD(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CLPD_CLPD_SHIFT))&ADC_CLPD_CLPD_MASK)

#define  ADC_CLPS_CLPS_MASK          0x6u
#define  ADC_CLPS_CLPS_SHIFT         0
#define  ADC_CLPS_CLPS(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CLPS_CLPS_SHIFT))&ADC_CLPS_CLPS_MASK)

#define  ADC_CLP4_CLP4_MASK          0xAu
#define  ADC_CLP4_CLP4_SHIFT         0
#define  ADC_CLP4_CLP4(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CLP4_CLP4_SHIFT))&ADC_CLP4_CLP4_MASK)

#define  ADC_CLP3_CLP3_MASK          0x9u
#define  ADC_CLP3_CLP3_SHIFT         0
#define  ADC_CLP3_CLP3(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CLP3_CLP3_SHIFT))&ADC_CLP3_CLP3_MASK)

#define  ADC_CLP2_CLP2_MASK          0x8u
#define  ADC_CLP2_CLP2_SHIFT         0
#define  ADC_CLP2_CLP2(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CLP2_CLP2_SHIFT))&ADC_CLP2_CLP2_MASK)

#define  ADC_CLP1_CLP1_MASK          0x7u
#define  ADC_CLP1_CLP1_SHIFT         0
#define  ADC_CLP1_CLP1(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CLP1_CLP1_SHIFT))&ADC_CLP1_CLP1_MASK)

#define  ADC_CLP0_CLP0_MASK          0x6u
#define  ADC_CLP0_CLP0_SHIFT         0
#define  ADC_CLP0_CLP0(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CLP0_CLP0_SHIFT))&ADC_CLP0_CLP0_MASK)

#define  ADC_PGA_PGAG_MASK           0x40000u
#define  ADC_PGA_PGAG_SHIFT          16
#define  ADC_PGA_PGAG(x)             (((uint32_t)(((uint32_t)(x))<<ADC_PGA_PGAG_SHIFT))&ADC_PGA_PGAG_MASK)
//   PGA Gain Setting
//     0b0000 ==> 1
//     0b0001 ==> 2
//     0b0010 ==> 4
//     0b0011 ==> 8
//     0b0100 ==> 16
//     0b0101 ==> 32
//     0b0110 ==> 64
//     0b0111 ==> Reserved
//     0b1000 ==> Reserved
//     0b1001 ==> Reserved
//     0b1010 ==> Reserved
//     0b1011 ==> Reserved
//     0b1100 ==> Reserved
//     0b1101 ==> Reserved
//     0b1110 ==> Reserved
//     0b1111 ==> Reserved

#define  ADC_PGA_PGALPb_MASK         0x100000u
#define  ADC_PGA_PGALPb_SHIFT        20
//   PGA Low-Power Mode Control
//     0b0 ==> PGA runs in Low-Power mode.
//     0b1 ==> PGA runs in Normal Power mode.

#define  ADC_PGA_PGAEN_MASK          0x800000u
#define  ADC_PGA_PGAEN_SHIFT         23
//   PGA Enable
//     0b0 ==> PGA disabled.
//     0b1 ==> PGA enabled.

#define  ADC_CLMD_CLMD_MASK          0x6u
#define  ADC_CLMD_CLMD_SHIFT         0
#define  ADC_CLMD_CLMD(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CLMD_CLMD_SHIFT))&ADC_CLMD_CLMD_MASK)

#define  ADC_CLMS_CLMS_MASK          0x6u
#define  ADC_CLMS_CLMS_SHIFT         0
#define  ADC_CLMS_CLMS(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CLMS_CLMS_SHIFT))&ADC_CLMS_CLMS_MASK)

#define  ADC_CLM4_CLM4_MASK          0xAu
#define  ADC_CLM4_CLM4_SHIFT         0
#define  ADC_CLM4_CLM4(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CLM4_CLM4_SHIFT))&ADC_CLM4_CLM4_MASK)

#define  ADC_CLM3_CLM3_MASK          0x9u
#define  ADC_CLM3_CLM3_SHIFT         0
#define  ADC_CLM3_CLM3(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CLM3_CLM3_SHIFT))&ADC_CLM3_CLM3_MASK)

#define  ADC_CLM2_CLM2_MASK          0x8u
#define  ADC_CLM2_CLM2_SHIFT         0
#define  ADC_CLM2_CLM2(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CLM2_CLM2_SHIFT))&ADC_CLM2_CLM2_MASK)

#define  ADC_CLM1_CLM1_MASK          0x7u
#define  ADC_CLM1_CLM1_SHIFT         0
#define  ADC_CLM1_CLM1(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CLM1_CLM1_SHIFT))&ADC_CLM1_CLM1_MASK)

#define  ADC_CLM0_CLM0_MASK          0x6u
#define  ADC_CLM0_CLM0_SHIFT         0
#define  ADC_CLM0_CLM0(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CLM0_CLM0_SHIFT))&ADC_CLM0_CLM0_MASK)

#define  AFE_CH1_CFR_HW_TRG_MASK     0x200u
#define  AFE_CH1_CFR_HW_TRG_SHIFT    9
//   Hardware Trigger Select
//     0b0 ==> = Software trigger select
//     0b1 ==> = Hardware trigger select

#define  AFE_CH1_CFR_DEC_CLK_INP_SEL_MASK 0x400u
#define  AFE_CH1_CFR_DEC_CLK_INP_SEL_SHIFT 10
//   Decimator Clock Input Select
//     0b0 ==> = On the chip modulator clock will be used
//     0b1 ==> = External clock will be used.

#define  AFE_CH1_CFR_DEC_CLK_EDGE_SEL_MASK 0x800u
#define  AFE_CH1_CFR_DEC_CLK_EDGE_SEL_SHIFT 11
//   Decimator Clock Edge Select
//     0b0 ==> = Posedge will be used.
//     0b1 ==> = Negedge will be used.

#define  AFE_CH1_CFR_CC_MASK         0x1000u
#define  AFE_CH1_CFR_CC_SHIFT        12
//   Continuous Conversion/Single Conversion Mode Select
//     0b0 ==> = One conversion following a triggering event
//     0b1 ==> = Continuous conversions following a triggering event.

#define  AFE_CH1_CFR_DEC_EN_MASK     0x2000u
#define  AFE_CH1_CFR_DEC_EN_SHIFT    13
//   Decimation Filter enable
//     0b0 ==> = Decimation filter is disabled.
//     0b1 ==> = Decimation filter is enabled.

#define  AFE_CH1_CFR_SD_MOD_EN_MASK  0x4000u
#define  AFE_CH1_CFR_SD_MOD_EN_SHIFT 14
//   Sigma Delta Modulator enable
//     0b0 ==> = SD ADC1 is disabled
//     0b1 ==> = SD ADC1 is enabled

#define  AFE_CH1_CFR_BYP_MODE_MASK   0x20000u
#define  AFE_CH1_CFR_BYP_MODE_SHIFT  17
//   AFE Channel1 bypass mode
//     0b0 ==> = Normal mode
//     0b1 ==> = Bypass mode where ADC and PGA of channel1 are disabled.

#define  AFE_CH1_CFR_PGA_GAIN_SEL_MASK 0x180000u
#define  AFE_CH1_CFR_PGA_GAIN_SEL_SHIFT 19
#define  AFE_CH1_CFR_PGA_GAIN_SEL(x) (((uint32_t)(((uint32_t)(x))<<AFE_CH1_CFR_PGA_GAIN_SEL_SHIFT))&AFE_CH1_CFR_PGA_GAIN_SEL_MASK)
//   PGA Gain Select
//     0b000 ==> = reserved
//     0b001 ==> = 1x (default)
//     0b010 ==> = 2x
//     0b011 ==> = 4x
//     0b100 ==> = 8x
//     0b101 ==> = 16x
//     0b110 ==> = 32x
//     0b111 ==> = reserved

#define  AFE_CH1_CFR_PGA_EN_MASK     0x1000000u
#define  AFE_CH1_CFR_PGA_EN_SHIFT    24
//   PGA enable
//     0b0 ==> = PGA disabled
//     0b1 ==> = PGA enabled

#define  AFE_CH1_CFR_DEC_OSR_MASK    0x60000000u
#define  AFE_CH1_CFR_DEC_OSR_SHIFT   29
#define  AFE_CH1_CFR_DEC_OSR(x)      (((uint32_t)(((uint32_t)(x))<<AFE_CH1_CFR_DEC_OSR_SHIFT))&AFE_CH1_CFR_DEC_OSR_MASK)
//   Decimator OverSampling Ratio select
//     0b000 ==> = 64
//     0b001 ==> = 128
//     0b010 ==> = 256
//     0b011 ==> = 512
//     0b100 ==> = 1024
//     0b101 ==> = 2048

#define  AFE_CH2_CFR_HW_TRG_MASK     0x200u
#define  AFE_CH2_CFR_HW_TRG_SHIFT    9
//   Hardware Trigger Select
//     0b0 ==> = Software trigger select
//     0b1 ==> = Hardware trigger select

#define  AFE_CH2_CFR_DEC_CLK_INP_SEL_MASK 0x400u
#define  AFE_CH2_CFR_DEC_CLK_INP_SEL_SHIFT 10
//   Decimator Clock Input Select
//     0b0 ==> = On the chip modulator clock will be used
//     0b1 ==> = External clock will be used.

#define  AFE_CH2_CFR_DEC_CLK_EDGE_SEL_MASK 0x800u
#define  AFE_CH2_CFR_DEC_CLK_EDGE_SEL_SHIFT 11
//   Decimator Clock Edge Select
//     0b0 ==> = Posedge will be used.
//     0b1 ==> = Negedge will be used.

#define  AFE_CH2_CFR_CC_MASK         0x1000u
#define  AFE_CH2_CFR_CC_SHIFT        12
//   Continuous Conversion/Single Conversion Mode Select
//     0b0 ==> = One conversion following a triggering event
//     0b1 ==> = Continuous conversions following a triggering event.

#define  AFE_CH2_CFR_DEC_EN_MASK     0x2000u
#define  AFE_CH2_CFR_DEC_EN_SHIFT    13
//   Decimation Filter enable
//     0b0 ==> = Decimation filter is disabled.
//     0b1 ==> = Decimation filter is enabled.

#define  AFE_CH2_CFR_SD_MOD_EN_MASK  0x4000u
#define  AFE_CH2_CFR_SD_MOD_EN_SHIFT 14
//   Sigma Delta Modulator enable
//     0b0 ==> = SD ADC1 is disabled
//     0b1 ==> = SD ADC1 is enabled

#define  AFE_CH2_CFR_BYP_MODE_MASK   0x20000u
#define  AFE_CH2_CFR_BYP_MODE_SHIFT  17
//   AFE Channel1 bypass mode
//     0b0 ==> = Normal mode
//     0b1 ==> = Bypass mode where ADC and PGA of channel1 are disabled.

#define  AFE_CH2_CFR_PGA_GAIN_SEL_MASK 0x180000u
#define  AFE_CH2_CFR_PGA_GAIN_SEL_SHIFT 19
#define  AFE_CH2_CFR_PGA_GAIN_SEL(x) (((uint32_t)(((uint32_t)(x))<<AFE_CH2_CFR_PGA_GAIN_SEL_SHIFT))&AFE_CH2_CFR_PGA_GAIN_SEL_MASK)
//   PGA Gain Select
//     0b000 ==> = reserved
//     0b001 ==> = 1x (default)
//     0b010 ==> = 2x
//     0b011 ==> = 4x
//     0b100 ==> = 8x
//     0b101 ==> = 16x
//     0b110 ==> = 32x
//     0b111 ==> = reserved

#define  AFE_CH2_CFR_PGA_EN_MASK     0x1000000u
#define  AFE_CH2_CFR_PGA_EN_SHIFT    24
//   PGA enable
//     0b0 ==> = PGA disabled
//     0b1 ==> = PGA enabled

#define  AFE_CH2_CFR_DEC_OSR_MASK    0x60000000u
#define  AFE_CH2_CFR_DEC_OSR_SHIFT   29
#define  AFE_CH2_CFR_DEC_OSR(x)      (((uint32_t)(((uint32_t)(x))<<AFE_CH2_CFR_DEC_OSR_SHIFT))&AFE_CH2_CFR_DEC_OSR_MASK)
//   Decimator OverSampling Ratio select
//     0b000 ==> = 64
//     0b001 ==> = 128
//     0b010 ==> = 256
//     0b011 ==> = 512
//     0b100 ==> = 1024
//     0b101 ==> = 2048

#define  AFE_CH3_CFR_HW_TRG_MASK     0x200u
#define  AFE_CH3_CFR_HW_TRG_SHIFT    9
//   Hardware Trigger Select
//     0b0 ==> = Software trigger select
//     0b1 ==> = Hardware trigger select

#define  AFE_CH3_CFR_DEC_CLK_INP_SEL_MASK 0x400u
#define  AFE_CH3_CFR_DEC_CLK_INP_SEL_SHIFT 10
//   Decimator Clock Input Select
//     0b0 ==> = On the chip modulator clock will be used.
//     0b1 ==> = External clock will be used.

#define  AFE_CH3_CFR_DEC_CLK_EDGE_SEL_MASK 0x800u
#define  AFE_CH3_CFR_DEC_CLK_EDGE_SEL_SHIFT 11
//   Decimator Clock Edge Select
//     0b0 ==> = Posedge will be used.
//     0b1 ==> = Negedge will be used.

#define  AFE_CH3_CFR_CC_MASK         0x1000u
#define  AFE_CH3_CFR_CC_SHIFT        12
//   Continuous Conversion/Single Conversion Mode Select
//     0b0 ==> = One conversion following a triggering event
//     0b1 ==> = Continuous conversions following a triggering event.

#define  AFE_CH3_CFR_DEC_EN_MASK     0x2000u
#define  AFE_CH3_CFR_DEC_EN_SHIFT    13
//   Decimation Filter enable
//     0b0 ==> = Decimation filter is disabled.
//     0b1 ==> = Decimation filter is enabled.

#define  AFE_CH3_CFR_SD_MOD_EN_MASK  0x4000u
#define  AFE_CH3_CFR_SD_MOD_EN_SHIFT 14
//   Sigma Delta Modulator enable
//     0b0 ==> = SD ADC3 is disabled
//     0b1 ==> = SD ADC3 is enabled

#define  AFE_CH3_CFR_BYP_MODE_MASK   0x20000u
#define  AFE_CH3_CFR_BYP_MODE_SHIFT  17
//   AFE Channel3 bypass mode
//     0b0 ==> = Normal mode
//     0b1 ==> = Bypass mode where ADC and PGA of channel3 are disabled.

#define  AFE_CH3_CFR_DEC_OSR_MASK    0x60000000u
#define  AFE_CH3_CFR_DEC_OSR_SHIFT   29
#define  AFE_CH3_CFR_DEC_OSR(x)      (((uint32_t)(((uint32_t)(x))<<AFE_CH3_CFR_DEC_OSR_SHIFT))&AFE_CH3_CFR_DEC_OSR_MASK)
//   Decimator OverSampling Ratio select
//     0b000 ==> =64
//     0b001 ==> =128
//     0b010 ==> =256
//     0b011 ==> =512
//     0b100 ==> =1024
//     0b101 ==> =2048

#define  AFE_CH4_CFR_HW_TRG_MASK     0x200u
#define  AFE_CH4_CFR_HW_TRG_SHIFT    9
//   Hardware Trigger Select
//     0b0 ==> = Software trigger select
//     0b1 ==> = Hardware trigger select

#define  AFE_CH4_CFR_DEC_CLK_INP_SEL_MASK 0x400u
#define  AFE_CH4_CFR_DEC_CLK_INP_SEL_SHIFT 10
//   Decimator Clock Input Select
//     0b0 ==> = On the chip modulator clock will be used.
//     0b1 ==> = External clock will be used.

#define  AFE_CH4_CFR_DEC_CLK_EDGE_SEL_MASK 0x800u
#define  AFE_CH4_CFR_DEC_CLK_EDGE_SEL_SHIFT 11
//   Decimator Clock Edge Select
//     0b0 ==> = Posedge will be used.
//     0b1 ==> = Negedge will be used.

#define  AFE_CH4_CFR_CC_MASK         0x1000u
#define  AFE_CH4_CFR_CC_SHIFT        12
//   Continuous Conversion/Single Conversion Mode Select
//     0b0 ==> = One conversion following a triggering event
//     0b1 ==> = Continuous conversions following a triggering event.

#define  AFE_CH4_CFR_DEC_EN_MASK     0x2000u
#define  AFE_CH4_CFR_DEC_EN_SHIFT    13
//   Decimation Filter enable
//     0b0 ==> = Decimation filter is disabled.
//     0b1 ==> = Decimation filter is enabled.

#define  AFE_CH4_CFR_SD_MOD_EN_MASK  0x4000u
#define  AFE_CH4_CFR_SD_MOD_EN_SHIFT 14
//   Sigma Delta Modulator enable
//     0b0 ==> = SD ADC3 is disabled
//     0b1 ==> = SD ADC3 is enabled

#define  AFE_CH4_CFR_BYP_MODE_MASK   0x20000u
#define  AFE_CH4_CFR_BYP_MODE_SHIFT  17
//   AFE Channel3 bypass mode
//     0b0 ==> = Normal mode
//     0b1 ==> = Bypass mode where ADC and PGA of channel3 are disabled.

#define  AFE_CH4_CFR_DEC_OSR_MASK    0x60000000u
#define  AFE_CH4_CFR_DEC_OSR_SHIFT   29
#define  AFE_CH4_CFR_DEC_OSR(x)      (((uint32_t)(((uint32_t)(x))<<AFE_CH4_CFR_DEC_OSR_SHIFT))&AFE_CH4_CFR_DEC_OSR_MASK)
//   Decimator OverSampling Ratio select
//     0b000 ==> =64
//     0b001 ==> =128
//     0b010 ==> =256
//     0b011 ==> =512
//     0b100 ==> =1024
//     0b101 ==> =2048

#define  AFE_CR_STRTUP_CNT_MASK      0xE00u
#define  AFE_CR_STRTUP_CNT_SHIFT     9
#define  AFE_CR_STRTUP_CNT(x)        (((uint32_t)(((uint32_t)(x))<<AFE_CR_STRTUP_CNT_SHIFT))&AFE_CR_STRTUP_CNT_MASK)
//   Start up count

#define  AFE_CR_RESULT_FORMAT_MASK   0x40000u
#define  AFE_CR_RESULT_FORMAT_SHIFT  18
//   Result Format
//     0b0 ==> = Left justified 2's complement 32-bit : SVVVVVVVVVVVVVVVVVVVVVVV00000000 where (S= sign bit , V=valid result value, 0=zero)
//     0b1 ==> = Right justified 2's complement 32-bit : SSSSSSSSSVVVVVVVVVVVVVVVVVVVVVVV where (S= sign bit , V= valid result value, 0= zero)

#define  AFE_CR_DLY_OK_MASK          0x200000u
#define  AFE_CR_DLY_OK_SHIFT         21
//   Delay OK

#define  AFE_CR_RST_B_MASK           0x400000u
#define  AFE_CR_RST_B_SHIFT          22
//   Software Reset
//     0b0 ==> = All ADCs, PGAs and Decimation filters are disabled. Clock Configuration bits will be reset.
//     0b1 ==> .= All ADCs, PGAs and Decimation filters are enabled.

#define  AFE_CR_LPM_EN_MASK          0x2000000u
#define  AFE_CR_LPM_EN_SHIFT         25
//   Low power Mode enable
//     0b0 ==> = AFE will be in normal mode
//     0b1 ==> = AFE will be in low power mode. Setting this bit reduce the current consumption of ADC and Buffer Amplifier , the max modulator clock frequency is below 1Mhz.

#define  AFE_CR_SOFT_TRG4_MASK       0x8000000u
#define  AFE_CR_SOFT_TRG4_SHIFT      27
//   Software Trigger4

#define  AFE_CR_SOFT_TRG3_MASK       0x10000000u
#define  AFE_CR_SOFT_TRG3_SHIFT      28
//   Software Trigger3

#define  AFE_CR_SOFT_TRG2_MASK       0x20000000u
#define  AFE_CR_SOFT_TRG2_SHIFT      29
//   Software Trigger2

#define  AFE_CR_SOFT_TRG1_MASK       0x40000000u
#define  AFE_CR_SOFT_TRG1_SHIFT      30
//   Software Trigger1

#define  AFE_CR_MSTR_EN_MASK         0x80000000u
#define  AFE_CR_MSTR_EN_SHIFT        31
//   AFE Master Enable
//     0b0 ==> = All ADCs are disabled.
//     0b1 ==> = All ADCs and filters will get simultaneously enabled .

#define  AFE_CKR_CLS_MASK            0x400000u
#define  AFE_CKR_CLS_SHIFT           21
#define  AFE_CKR_CLS(x)              (((uint32_t)(((uint32_t)(x))<<AFE_CKR_CLS_SHIFT))&AFE_CKR_CLS_MASK)
//   Clock Source Select
//     0b00 ==> = mod_clk0
//     0b01 ==> = mod_clk1
//     0b10 ==> = mod_clk2
//     0b11 ==> = mod_clk3

#define  AFE_CKR_DIV_MASK            0x40000000u
#define  AFE_CKR_DIV_SHIFT           28
#define  AFE_CKR_DIV(x)              (((uint32_t)(((uint32_t)(x))<<AFE_CKR_DIV_SHIFT))&AFE_CKR_DIV_MASK)
//   Clock Divider Select
//     0b0000 ==> = divide by 1
//     0b0001 ==> = divide by 2 (default)
//     0b0010 ==> = divide by 4
//     0b0011 ==> = divide by 8
//     0b0100 ==> = divide by 16
//     0b0101 ==> = divide by 32
//     0b0110 ==> = divide by 64
//     0b0111 ==> = divide by 128
//     0b1xxx ==> = divide by 256

#define  AFE_DI_INTEN4_MASK          0x800000u
#define  AFE_DI_INTEN4_SHIFT         23
//   Interrupt Enable 4

#define  AFE_DI_INTEN3_MASK          0x1000000u
#define  AFE_DI_INTEN3_SHIFT         24
//   Interrupt Enable 3

#define  AFE_DI_INTEN2_MASK          0x2000000u
#define  AFE_DI_INTEN2_SHIFT         25
//   Interrupt Enable 2

#define  AFE_DI_INTEN1_MASK          0x4000000u
#define  AFE_DI_INTEN1_SHIFT         26
//   Interrupt Enable 1

#define  AFE_DI_DMA_EN4_MASK         0x10000000u
#define  AFE_DI_DMA_EN4_SHIFT        28
//   DMA Enable4

#define  AFE_DI_DMA_EN3_MASK         0x20000000u
#define  AFE_DI_DMA_EN3_SHIFT        29
//   DMA Enable3

#define  AFE_DI_DMA_EN2_MASK         0x40000000u
#define  AFE_DI_DMA_EN2_SHIFT        30
//   DMA Enable2

#define  AFE_DI_DMA_EN1_MASK         0x80000000u
#define  AFE_DI_DMA_EN1_SHIFT        31
//   DMA Enable1

#define  AFE_CH1_DR_DLY_MASK         0xBu
#define  AFE_CH1_DR_DLY_SHIFT        0
#define  AFE_CH1_DR_DLY(x)           (((uint32_t)(((uint32_t)(x))<<AFE_CH1_DR_DLY_SHIFT))&AFE_CH1_DR_DLY_MASK)
//   Delay

#define  AFE_CH2_DR_DLY_MASK         0xBu
#define  AFE_CH2_DR_DLY_SHIFT        0
#define  AFE_CH2_DR_DLY(x)           (((uint32_t)(((uint32_t)(x))<<AFE_CH2_DR_DLY_SHIFT))&AFE_CH2_DR_DLY_MASK)
//   Delay

#define  AFE_CH3_DR_DLY_MASK         0xBu
#define  AFE_CH3_DR_DLY_SHIFT        0
#define  AFE_CH3_DR_DLY(x)           (((uint32_t)(((uint32_t)(x))<<AFE_CH3_DR_DLY_SHIFT))&AFE_CH3_DR_DLY_MASK)
//   Delay

#define  AFE_CH4_DR_DLY_MASK         0xBu
#define  AFE_CH4_DR_DLY_SHIFT        0
#define  AFE_CH4_DR_DLY(x)           (((uint32_t)(((uint32_t)(x))<<AFE_CH4_DR_DLY_SHIFT))&AFE_CH4_DR_DLY_MASK)
//   Delay

#define  AFE_CH1_RR_SDR_MASK         0x17u
#define  AFE_CH1_RR_SDR_SHIFT        0
#define  AFE_CH1_RR_SDR(x)           (((uint32_t)(((uint32_t)(x))<<AFE_CH1_RR_SDR_SHIFT))&AFE_CH1_RR_SDR_MASK)
//   Sample Data Result

#define  AFE_CH1_RR_SIGN_BITS_MASK   0x4800000u
#define  AFE_CH1_RR_SIGN_BITS_SHIFT  23
#define  AFE_CH1_RR_SIGN_BITS(x)     (((uint32_t)(((uint32_t)(x))<<AFE_CH1_RR_SIGN_BITS_SHIFT))&AFE_CH1_RR_SIGN_BITS_MASK)
//   Sign Bits

#define  AFE_CH2_RR_SDR_MASK         0x17u
#define  AFE_CH2_RR_SDR_SHIFT        0
#define  AFE_CH2_RR_SDR(x)           (((uint32_t)(((uint32_t)(x))<<AFE_CH2_RR_SDR_SHIFT))&AFE_CH2_RR_SDR_MASK)
//   Sample Data Result

#define  AFE_CH2_RR_SIGN_BITS_MASK   0x4800000u
#define  AFE_CH2_RR_SIGN_BITS_SHIFT  23
#define  AFE_CH2_RR_SIGN_BITS(x)     (((uint32_t)(((uint32_t)(x))<<AFE_CH2_RR_SIGN_BITS_SHIFT))&AFE_CH2_RR_SIGN_BITS_MASK)
//   Sign Bits

#define  AFE_CH3_RR_SDR_MASK         0x17u
#define  AFE_CH3_RR_SDR_SHIFT        0
#define  AFE_CH3_RR_SDR(x)           (((uint32_t)(((uint32_t)(x))<<AFE_CH3_RR_SDR_SHIFT))&AFE_CH3_RR_SDR_MASK)
//   Sample Data result

#define  AFE_CH3_RR_SIGN_BITS_MASK   0x4800000u
#define  AFE_CH3_RR_SIGN_BITS_SHIFT  23
#define  AFE_CH3_RR_SIGN_BITS(x)     (((uint32_t)(((uint32_t)(x))<<AFE_CH3_RR_SIGN_BITS_SHIFT))&AFE_CH3_RR_SIGN_BITS_MASK)
//   Sign Bits

#define  AFE_CH4_RR_SDR_MASK         0x17u
#define  AFE_CH4_RR_SDR_SHIFT        0
#define  AFE_CH4_RR_SDR(x)           (((uint32_t)(((uint32_t)(x))<<AFE_CH4_RR_SDR_SHIFT))&AFE_CH4_RR_SDR_MASK)
//   Sample Data result

#define  AFE_CH4_RR_SIGN_BITS_MASK   0x4800000u
#define  AFE_CH4_RR_SIGN_BITS_SHIFT  23
#define  AFE_CH4_RR_SIGN_BITS(x)     (((uint32_t)(((uint32_t)(x))<<AFE_CH4_RR_SIGN_BITS_SHIFT))&AFE_CH4_RR_SIGN_BITS_MASK)
//   Sign Bits

#define  AFE_SR_RDY4_MASK            0x4000u
#define  AFE_SR_RDY4_SHIFT           14
//   AFE Ready4
//     0b0 ==> AFE Channel 4 is disabled or has not completed its start up period
//     0b1 ==> AFE Channel 4 is ready to initiate conversions.

#define  AFE_SR_RDY3_MASK            0x8000u
#define  AFE_SR_RDY3_SHIFT           15
//   AFE Ready3
//     0b0 ==> AFE Channel 3 is disabled or has not completed its start up period
//     0b1 ==> AFE Channel 3 is ready to initiate conversions.

#define  AFE_SR_RDY2_MASK            0x10000u
#define  AFE_SR_RDY2_SHIFT           16
//   AFE Ready2
//     0b0 ==> AFE Channel 2 is disabled or has not completed its start up period
//     0b1 ==> AFE Channel 2 is ready to initiate conversions.

#define  AFE_SR_RDY1_MASK            0x20000u
#define  AFE_SR_RDY1_SHIFT           17
//   AFE Ready1
//     0b0 ==> AFE Channel 1 is disabled or has not completed its start up period
//     0b1 ==> AFE Channel 1 is ready to initiate conversions.

#define  AFE_SR_OVR4_MASK            0x200000u
#define  AFE_SR_OVR4_SHIFT           21
//   Overflow Flag

#define  AFE_SR_OVR3_MASK            0x400000u
#define  AFE_SR_OVR3_SHIFT           22
//   Overflow Flag

#define  AFE_SR_OVR2_MASK            0x800000u
#define  AFE_SR_OVR2_SHIFT           23
//   Overflow Flag

#define  AFE_SR_OVR1_MASK            0x1000000u
#define  AFE_SR_OVR1_SHIFT           24
//   Overflow Flag

#define  AFE_SR_COC4_MASK            0x10000000u
#define  AFE_SR_COC4_SHIFT           28
//   Conversion Complete

#define  AFE_SR_COC3_MASK            0x20000000u
#define  AFE_SR_COC3_SHIFT           29
//   Conversion Complete

#define  AFE_SR_COC2_MASK            0x40000000u
#define  AFE_SR_COC2_SHIFT           30
//   Conversion Complete

#define  AFE_SR_COC1_MASK            0x80000000u
#define  AFE_SR_COC1_SHIFT           31
//   Conversion Complete

#define  AFE_TR_TESTMODE_MASK        0x20000000u
#define  AFE_TR_TESTMODE_SHIFT       27
#define  AFE_TR_TESTMODE(x)          (((uint32_t)(((uint32_t)(x))<<AFE_TR_TESTMODE_SHIFT))&AFE_TR_TESTMODE_MASK)
//   Test Mode Control Word

#define  AFE_TR_TEST_EN_MASK         0x80000000u
#define  AFE_TR_TEST_EN_SHIFT        31
//   Test Mode enable
//     0b0 ==> = Analog test mode disabled
//     0b1 ==> = Analog test mode enabled.

#define  AIPS_MPRA_MPL0_MASK         0x10000000u
#define  AIPS_MPRA_MPL0_SHIFT        28
//   Master Privilege Level
//     0b0 ==> Accesses from this master are forced to user-mode.
//     0b1 ==> Accesses from this master are not forced to user-mode.

#define  AIPS_MPRA_MTW0_MASK         0x20000000u
#define  AIPS_MPRA_MTW0_SHIFT        29
//   Master Trusted For Writes
//     0b0 ==> This master is not trusted for write accesses.
//     0b1 ==> This master is trusted for write accesses.

#define  AIPS_MPRA_MTR0_MASK         0x40000000u
#define  AIPS_MPRA_MTR0_SHIFT        30
//   Master Trusted For Read
//     0b0 ==> This master is not trusted for read accesses.
//     0b1 ==> This master is trusted for read accesses.

#define  AIPS_PACRA_TP7_MASK         0x1u
#define  AIPS_PACRA_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRA_WP7_MASK         0x2u
#define  AIPS_PACRA_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRA_SP7_MASK         0x4u
#define  AIPS_PACRA_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRA_TP6_MASK         0x10u
#define  AIPS_PACRA_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRA_WP6_MASK         0x20u
#define  AIPS_PACRA_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRA_SP6_MASK         0x40u
#define  AIPS_PACRA_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRA_TP5_MASK         0x100u
#define  AIPS_PACRA_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRA_WP5_MASK         0x200u
#define  AIPS_PACRA_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRA_SP5_MASK         0x400u
#define  AIPS_PACRA_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRA_TP4_MASK         0x1000u
#define  AIPS_PACRA_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRA_WP4_MASK         0x2000u
#define  AIPS_PACRA_WP4_SHIFT        13
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRA_SP4_MASK         0x4000u
#define  AIPS_PACRA_SP4_SHIFT        14
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRA_TP3_MASK         0x10000u
#define  AIPS_PACRA_TP3_SHIFT        16
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRA_WP3_MASK         0x20000u
#define  AIPS_PACRA_WP3_SHIFT        17
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRA_SP3_MASK         0x40000u
#define  AIPS_PACRA_SP3_SHIFT        18
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRA_TP2_MASK         0x100000u
#define  AIPS_PACRA_TP2_SHIFT        20
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRA_WP2_MASK         0x200000u
#define  AIPS_PACRA_WP2_SHIFT        21
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRA_SP2_MASK         0x400000u
#define  AIPS_PACRA_SP2_SHIFT        22
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRA_TP1_MASK         0x1000000u
#define  AIPS_PACRA_TP1_SHIFT        24
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRA_WP1_MASK         0x2000000u
#define  AIPS_PACRA_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRA_SP1_MASK         0x4000000u
#define  AIPS_PACRA_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRA_TP0_MASK         0x10000000u
#define  AIPS_PACRA_TP0_SHIFT        28
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRA_WP0_MASK         0x20000000u
#define  AIPS_PACRA_WP0_SHIFT        29
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRA_SP0_MASK         0x40000000u
#define  AIPS_PACRA_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRB_TP7_MASK         0x1u
#define  AIPS_PACRB_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRB_WP7_MASK         0x2u
#define  AIPS_PACRB_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRB_SP7_MASK         0x4u
#define  AIPS_PACRB_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRB_TP6_MASK         0x10u
#define  AIPS_PACRB_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRB_WP6_MASK         0x20u
#define  AIPS_PACRB_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRB_SP6_MASK         0x40u
#define  AIPS_PACRB_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRB_TP5_MASK         0x100u
#define  AIPS_PACRB_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRB_WP5_MASK         0x200u
#define  AIPS_PACRB_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRB_SP5_MASK         0x400u
#define  AIPS_PACRB_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRB_TP4_MASK         0x1000u
#define  AIPS_PACRB_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRB_WP4_MASK         0x2000u
#define  AIPS_PACRB_WP4_SHIFT        13
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRB_SP4_MASK         0x4000u
#define  AIPS_PACRB_SP4_SHIFT        14
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRB_TP3_MASK         0x10000u
#define  AIPS_PACRB_TP3_SHIFT        16
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRB_WP3_MASK         0x20000u
#define  AIPS_PACRB_WP3_SHIFT        17
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRB_SP3_MASK         0x40000u
#define  AIPS_PACRB_SP3_SHIFT        18
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRB_TP2_MASK         0x100000u
#define  AIPS_PACRB_TP2_SHIFT        20
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRB_WP2_MASK         0x200000u
#define  AIPS_PACRB_WP2_SHIFT        21
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRB_SP2_MASK         0x400000u
#define  AIPS_PACRB_SP2_SHIFT        22
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRB_TP1_MASK         0x1000000u
#define  AIPS_PACRB_TP1_SHIFT        24
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRB_WP1_MASK         0x2000000u
#define  AIPS_PACRB_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRB_SP1_MASK         0x4000000u
#define  AIPS_PACRB_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRB_TP0_MASK         0x10000000u
#define  AIPS_PACRB_TP0_SHIFT        28
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRB_WP0_MASK         0x20000000u
#define  AIPS_PACRB_WP0_SHIFT        29
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRB_SP0_MASK         0x40000000u
#define  AIPS_PACRB_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRC_TP7_MASK         0x1u
#define  AIPS_PACRC_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRC_WP7_MASK         0x2u
#define  AIPS_PACRC_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRC_SP7_MASK         0x4u
#define  AIPS_PACRC_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRC_TP6_MASK         0x10u
#define  AIPS_PACRC_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRC_WP6_MASK         0x20u
#define  AIPS_PACRC_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRC_SP6_MASK         0x40u
#define  AIPS_PACRC_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRC_TP5_MASK         0x100u
#define  AIPS_PACRC_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRC_WP5_MASK         0x200u
#define  AIPS_PACRC_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRC_SP5_MASK         0x400u
#define  AIPS_PACRC_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRC_TP4_MASK         0x1000u
#define  AIPS_PACRC_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRC_WP4_MASK         0x2000u
#define  AIPS_PACRC_WP4_SHIFT        13
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRC_SP4_MASK         0x4000u
#define  AIPS_PACRC_SP4_SHIFT        14
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRC_TP3_MASK         0x10000u
#define  AIPS_PACRC_TP3_SHIFT        16
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRC_WP3_MASK         0x20000u
#define  AIPS_PACRC_WP3_SHIFT        17
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRC_SP3_MASK         0x40000u
#define  AIPS_PACRC_SP3_SHIFT        18
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRC_TP2_MASK         0x100000u
#define  AIPS_PACRC_TP2_SHIFT        20
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRC_WP2_MASK         0x200000u
#define  AIPS_PACRC_WP2_SHIFT        21
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRC_SP2_MASK         0x400000u
#define  AIPS_PACRC_SP2_SHIFT        22
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRC_TP1_MASK         0x1000000u
#define  AIPS_PACRC_TP1_SHIFT        24
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRC_WP1_MASK         0x2000000u
#define  AIPS_PACRC_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRC_SP1_MASK         0x4000000u
#define  AIPS_PACRC_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRC_TP0_MASK         0x10000000u
#define  AIPS_PACRC_TP0_SHIFT        28
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRC_WP0_MASK         0x20000000u
#define  AIPS_PACRC_WP0_SHIFT        29
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRC_SP0_MASK         0x40000000u
#define  AIPS_PACRC_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRD_TP7_MASK         0x1u
#define  AIPS_PACRD_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRD_WP7_MASK         0x2u
#define  AIPS_PACRD_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRD_SP7_MASK         0x4u
#define  AIPS_PACRD_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRD_TP6_MASK         0x10u
#define  AIPS_PACRD_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRD_WP6_MASK         0x20u
#define  AIPS_PACRD_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRD_SP6_MASK         0x40u
#define  AIPS_PACRD_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRD_TP5_MASK         0x100u
#define  AIPS_PACRD_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRD_WP5_MASK         0x200u
#define  AIPS_PACRD_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRD_SP5_MASK         0x400u
#define  AIPS_PACRD_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRD_TP4_MASK         0x1000u
#define  AIPS_PACRD_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRD_WP4_MASK         0x2000u
#define  AIPS_PACRD_WP4_SHIFT        13
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRD_SP4_MASK         0x4000u
#define  AIPS_PACRD_SP4_SHIFT        14
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRD_TP3_MASK         0x10000u
#define  AIPS_PACRD_TP3_SHIFT        16
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRD_WP3_MASK         0x20000u
#define  AIPS_PACRD_WP3_SHIFT        17
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRD_SP3_MASK         0x40000u
#define  AIPS_PACRD_SP3_SHIFT        18
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRD_TP2_MASK         0x100000u
#define  AIPS_PACRD_TP2_SHIFT        20
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRD_WP2_MASK         0x200000u
#define  AIPS_PACRD_WP2_SHIFT        21
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRD_SP2_MASK         0x400000u
#define  AIPS_PACRD_SP2_SHIFT        22
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRD_TP1_MASK         0x1000000u
#define  AIPS_PACRD_TP1_SHIFT        24
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRD_WP1_MASK         0x2000000u
#define  AIPS_PACRD_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRD_SP1_MASK         0x4000000u
#define  AIPS_PACRD_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRD_TP0_MASK         0x10000000u
#define  AIPS_PACRD_TP0_SHIFT        28
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRD_WP0_MASK         0x20000000u
#define  AIPS_PACRD_WP0_SHIFT        29
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRD_SP0_MASK         0x40000000u
#define  AIPS_PACRD_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRE_TP7_MASK         0x1u
#define  AIPS_PACRE_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRE_WP7_MASK         0x2u
#define  AIPS_PACRE_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRE_SP7_MASK         0x4u
#define  AIPS_PACRE_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRE_TP6_MASK         0x10u
#define  AIPS_PACRE_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRE_WP6_MASK         0x20u
#define  AIPS_PACRE_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRE_SP6_MASK         0x40u
#define  AIPS_PACRE_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRE_TP5_MASK         0x100u
#define  AIPS_PACRE_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRE_WP5_MASK         0x200u
#define  AIPS_PACRE_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRE_SP5_MASK         0x400u
#define  AIPS_PACRE_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRE_TP4_MASK         0x1000u
#define  AIPS_PACRE_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRE_WP4_MASK         0x2000u
#define  AIPS_PACRE_WP4_SHIFT        13
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRE_SP4_MASK         0x4000u
#define  AIPS_PACRE_SP4_SHIFT        14
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRE_TP3_MASK         0x10000u
#define  AIPS_PACRE_TP3_SHIFT        16
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRE_WP3_MASK         0x20000u
#define  AIPS_PACRE_WP3_SHIFT        17
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRE_SP3_MASK         0x40000u
#define  AIPS_PACRE_SP3_SHIFT        18
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRE_TP2_MASK         0x100000u
#define  AIPS_PACRE_TP2_SHIFT        20
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRE_WP2_MASK         0x200000u
#define  AIPS_PACRE_WP2_SHIFT        21
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRE_SP2_MASK         0x400000u
#define  AIPS_PACRE_SP2_SHIFT        22
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRE_TP1_MASK         0x1000000u
#define  AIPS_PACRE_TP1_SHIFT        24
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRE_WP1_MASK         0x2000000u
#define  AIPS_PACRE_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRE_SP1_MASK         0x4000000u
#define  AIPS_PACRE_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRE_TP0_MASK         0x10000000u
#define  AIPS_PACRE_TP0_SHIFT        28
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRE_WP0_MASK         0x20000000u
#define  AIPS_PACRE_WP0_SHIFT        29
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRE_SP0_MASK         0x40000000u
#define  AIPS_PACRE_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRF_TP7_MASK         0x1u
#define  AIPS_PACRF_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRF_WP7_MASK         0x2u
#define  AIPS_PACRF_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRF_SP7_MASK         0x4u
#define  AIPS_PACRF_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRF_TP6_MASK         0x10u
#define  AIPS_PACRF_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRF_WP6_MASK         0x20u
#define  AIPS_PACRF_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRF_SP6_MASK         0x40u
#define  AIPS_PACRF_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRF_TP5_MASK         0x100u
#define  AIPS_PACRF_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRF_WP5_MASK         0x200u
#define  AIPS_PACRF_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRF_SP5_MASK         0x400u
#define  AIPS_PACRF_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRF_TP4_MASK         0x1000u
#define  AIPS_PACRF_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRF_WP4_MASK         0x2000u
#define  AIPS_PACRF_WP4_SHIFT        13
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRF_SP4_MASK         0x4000u
#define  AIPS_PACRF_SP4_SHIFT        14
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRF_TP3_MASK         0x10000u
#define  AIPS_PACRF_TP3_SHIFT        16
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRF_WP3_MASK         0x20000u
#define  AIPS_PACRF_WP3_SHIFT        17
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRF_SP3_MASK         0x40000u
#define  AIPS_PACRF_SP3_SHIFT        18
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRF_TP2_MASK         0x100000u
#define  AIPS_PACRF_TP2_SHIFT        20
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRF_WP2_MASK         0x200000u
#define  AIPS_PACRF_WP2_SHIFT        21
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRF_SP2_MASK         0x400000u
#define  AIPS_PACRF_SP2_SHIFT        22
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRF_TP1_MASK         0x1000000u
#define  AIPS_PACRF_TP1_SHIFT        24
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRF_WP1_MASK         0x2000000u
#define  AIPS_PACRF_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRF_SP1_MASK         0x4000000u
#define  AIPS_PACRF_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRF_TP0_MASK         0x10000000u
#define  AIPS_PACRF_TP0_SHIFT        28
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRF_WP0_MASK         0x20000000u
#define  AIPS_PACRF_WP0_SHIFT        29
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRF_SP0_MASK         0x40000000u
#define  AIPS_PACRF_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRG_TP7_MASK         0x1u
#define  AIPS_PACRG_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRG_WP7_MASK         0x2u
#define  AIPS_PACRG_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRG_SP7_MASK         0x4u
#define  AIPS_PACRG_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRG_TP6_MASK         0x10u
#define  AIPS_PACRG_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRG_WP6_MASK         0x20u
#define  AIPS_PACRG_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRG_SP6_MASK         0x40u
#define  AIPS_PACRG_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRG_TP5_MASK         0x100u
#define  AIPS_PACRG_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRG_WP5_MASK         0x200u
#define  AIPS_PACRG_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRG_SP5_MASK         0x400u
#define  AIPS_PACRG_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRG_TP4_MASK         0x1000u
#define  AIPS_PACRG_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRG_WP4_MASK         0x2000u
#define  AIPS_PACRG_WP4_SHIFT        13
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRG_SP4_MASK         0x4000u
#define  AIPS_PACRG_SP4_SHIFT        14
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRG_TP3_MASK         0x10000u
#define  AIPS_PACRG_TP3_SHIFT        16
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRG_WP3_MASK         0x20000u
#define  AIPS_PACRG_WP3_SHIFT        17
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRG_SP3_MASK         0x40000u
#define  AIPS_PACRG_SP3_SHIFT        18
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRG_TP2_MASK         0x100000u
#define  AIPS_PACRG_TP2_SHIFT        20
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRG_WP2_MASK         0x200000u
#define  AIPS_PACRG_WP2_SHIFT        21
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRG_SP2_MASK         0x400000u
#define  AIPS_PACRG_SP2_SHIFT        22
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRG_TP1_MASK         0x1000000u
#define  AIPS_PACRG_TP1_SHIFT        24
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRG_WP1_MASK         0x2000000u
#define  AIPS_PACRG_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRG_SP1_MASK         0x4000000u
#define  AIPS_PACRG_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRG_TP0_MASK         0x10000000u
#define  AIPS_PACRG_TP0_SHIFT        28
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRG_WP0_MASK         0x20000000u
#define  AIPS_PACRG_WP0_SHIFT        29
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRG_SP0_MASK         0x40000000u
#define  AIPS_PACRG_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRH_TP7_MASK         0x1u
#define  AIPS_PACRH_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRH_WP7_MASK         0x2u
#define  AIPS_PACRH_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRH_SP7_MASK         0x4u
#define  AIPS_PACRH_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRH_TP6_MASK         0x10u
#define  AIPS_PACRH_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRH_WP6_MASK         0x20u
#define  AIPS_PACRH_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRH_SP6_MASK         0x40u
#define  AIPS_PACRH_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRH_TP5_MASK         0x100u
#define  AIPS_PACRH_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRH_WP5_MASK         0x200u
#define  AIPS_PACRH_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRH_SP5_MASK         0x400u
#define  AIPS_PACRH_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRH_TP4_MASK         0x1000u
#define  AIPS_PACRH_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRH_WP4_MASK         0x2000u
#define  AIPS_PACRH_WP4_SHIFT        13
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRH_SP4_MASK         0x4000u
#define  AIPS_PACRH_SP4_SHIFT        14
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRH_TP3_MASK         0x10000u
#define  AIPS_PACRH_TP3_SHIFT        16
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRH_WP3_MASK         0x20000u
#define  AIPS_PACRH_WP3_SHIFT        17
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRH_SP3_MASK         0x40000u
#define  AIPS_PACRH_SP3_SHIFT        18
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRH_TP2_MASK         0x100000u
#define  AIPS_PACRH_TP2_SHIFT        20
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRH_WP2_MASK         0x200000u
#define  AIPS_PACRH_WP2_SHIFT        21
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRH_SP2_MASK         0x400000u
#define  AIPS_PACRH_SP2_SHIFT        22
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRH_TP1_MASK         0x1000000u
#define  AIPS_PACRH_TP1_SHIFT        24
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRH_WP1_MASK         0x2000000u
#define  AIPS_PACRH_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRH_SP1_MASK         0x4000000u
#define  AIPS_PACRH_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRH_TP0_MASK         0x10000000u
#define  AIPS_PACRH_TP0_SHIFT        28
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRH_WP0_MASK         0x20000000u
#define  AIPS_PACRH_WP0_SHIFT        29
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRH_SP0_MASK         0x40000000u
#define  AIPS_PACRH_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRI_TP7_MASK         0x1u
#define  AIPS_PACRI_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRI_WP7_MASK         0x2u
#define  AIPS_PACRI_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRI_SP7_MASK         0x4u
#define  AIPS_PACRI_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRI_TP6_MASK         0x10u
#define  AIPS_PACRI_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRI_WP6_MASK         0x20u
#define  AIPS_PACRI_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRI_SP6_MASK         0x40u
#define  AIPS_PACRI_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRI_TP5_MASK         0x100u
#define  AIPS_PACRI_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRI_WP5_MASK         0x200u
#define  AIPS_PACRI_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRI_SP5_MASK         0x400u
#define  AIPS_PACRI_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRI_TP4_MASK         0x1000u
#define  AIPS_PACRI_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRI_WP4_MASK         0x2000u
#define  AIPS_PACRI_WP4_SHIFT        13
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRI_SP4_MASK         0x4000u
#define  AIPS_PACRI_SP4_SHIFT        14
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRI_TP3_MASK         0x10000u
#define  AIPS_PACRI_TP3_SHIFT        16
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRI_WP3_MASK         0x20000u
#define  AIPS_PACRI_WP3_SHIFT        17
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRI_SP3_MASK         0x40000u
#define  AIPS_PACRI_SP3_SHIFT        18
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRI_TP2_MASK         0x100000u
#define  AIPS_PACRI_TP2_SHIFT        20
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRI_WP2_MASK         0x200000u
#define  AIPS_PACRI_WP2_SHIFT        21
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRI_SP2_MASK         0x400000u
#define  AIPS_PACRI_SP2_SHIFT        22
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRI_TP1_MASK         0x1000000u
#define  AIPS_PACRI_TP1_SHIFT        24
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRI_WP1_MASK         0x2000000u
#define  AIPS_PACRI_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRI_SP1_MASK         0x4000000u
#define  AIPS_PACRI_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRI_TP0_MASK         0x10000000u
#define  AIPS_PACRI_TP0_SHIFT        28
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRI_WP0_MASK         0x20000000u
#define  AIPS_PACRI_WP0_SHIFT        29
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRI_SP0_MASK         0x40000000u
#define  AIPS_PACRI_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRJ_TP7_MASK         0x1u
#define  AIPS_PACRJ_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRJ_WP7_MASK         0x2u
#define  AIPS_PACRJ_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRJ_SP7_MASK         0x4u
#define  AIPS_PACRJ_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRJ_TP6_MASK         0x10u
#define  AIPS_PACRJ_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRJ_WP6_MASK         0x20u
#define  AIPS_PACRJ_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRJ_SP6_MASK         0x40u
#define  AIPS_PACRJ_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRJ_TP5_MASK         0x100u
#define  AIPS_PACRJ_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRJ_WP5_MASK         0x200u
#define  AIPS_PACRJ_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRJ_SP5_MASK         0x400u
#define  AIPS_PACRJ_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRJ_TP4_MASK         0x1000u
#define  AIPS_PACRJ_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRJ_WP4_MASK         0x2000u
#define  AIPS_PACRJ_WP4_SHIFT        13
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRJ_SP4_MASK         0x4000u
#define  AIPS_PACRJ_SP4_SHIFT        14
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRJ_TP3_MASK         0x10000u
#define  AIPS_PACRJ_TP3_SHIFT        16
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRJ_WP3_MASK         0x20000u
#define  AIPS_PACRJ_WP3_SHIFT        17
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRJ_SP3_MASK         0x40000u
#define  AIPS_PACRJ_SP3_SHIFT        18
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRJ_TP2_MASK         0x100000u
#define  AIPS_PACRJ_TP2_SHIFT        20
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRJ_WP2_MASK         0x200000u
#define  AIPS_PACRJ_WP2_SHIFT        21
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRJ_SP2_MASK         0x400000u
#define  AIPS_PACRJ_SP2_SHIFT        22
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRJ_TP1_MASK         0x1000000u
#define  AIPS_PACRJ_TP1_SHIFT        24
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRJ_WP1_MASK         0x2000000u
#define  AIPS_PACRJ_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRJ_SP1_MASK         0x4000000u
#define  AIPS_PACRJ_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRJ_TP0_MASK         0x10000000u
#define  AIPS_PACRJ_TP0_SHIFT        28
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRJ_WP0_MASK         0x20000000u
#define  AIPS_PACRJ_WP0_SHIFT        29
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRJ_SP0_MASK         0x40000000u
#define  AIPS_PACRJ_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRK_TP7_MASK         0x1u
#define  AIPS_PACRK_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRK_WP7_MASK         0x2u
#define  AIPS_PACRK_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRK_SP7_MASK         0x4u
#define  AIPS_PACRK_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRK_TP6_MASK         0x10u
#define  AIPS_PACRK_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRK_WP6_MASK         0x20u
#define  AIPS_PACRK_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRK_SP6_MASK         0x40u
#define  AIPS_PACRK_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRK_TP5_MASK         0x100u
#define  AIPS_PACRK_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRK_WP5_MASK         0x200u
#define  AIPS_PACRK_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRK_SP5_MASK         0x400u
#define  AIPS_PACRK_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRK_TP4_MASK         0x1000u
#define  AIPS_PACRK_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRK_WP4_MASK         0x2000u
#define  AIPS_PACRK_WP4_SHIFT        13
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRK_SP4_MASK         0x4000u
#define  AIPS_PACRK_SP4_SHIFT        14
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRK_TP3_MASK         0x10000u
#define  AIPS_PACRK_TP3_SHIFT        16
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRK_WP3_MASK         0x20000u
#define  AIPS_PACRK_WP3_SHIFT        17
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRK_SP3_MASK         0x40000u
#define  AIPS_PACRK_SP3_SHIFT        18
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRK_TP2_MASK         0x100000u
#define  AIPS_PACRK_TP2_SHIFT        20
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRK_WP2_MASK         0x200000u
#define  AIPS_PACRK_WP2_SHIFT        21
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRK_SP2_MASK         0x400000u
#define  AIPS_PACRK_SP2_SHIFT        22
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRK_TP1_MASK         0x1000000u
#define  AIPS_PACRK_TP1_SHIFT        24
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRK_WP1_MASK         0x2000000u
#define  AIPS_PACRK_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRK_SP1_MASK         0x4000000u
#define  AIPS_PACRK_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRK_TP0_MASK         0x10000000u
#define  AIPS_PACRK_TP0_SHIFT        28
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRK_WP0_MASK         0x20000000u
#define  AIPS_PACRK_WP0_SHIFT        29
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRK_SP0_MASK         0x40000000u
#define  AIPS_PACRK_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRL_TP7_MASK         0x1u
#define  AIPS_PACRL_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRL_WP7_MASK         0x2u
#define  AIPS_PACRL_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRL_SP7_MASK         0x4u
#define  AIPS_PACRL_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRL_TP6_MASK         0x10u
#define  AIPS_PACRL_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRL_WP6_MASK         0x20u
#define  AIPS_PACRL_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRL_SP6_MASK         0x40u
#define  AIPS_PACRL_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRL_TP5_MASK         0x100u
#define  AIPS_PACRL_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRL_WP5_MASK         0x200u
#define  AIPS_PACRL_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRL_SP5_MASK         0x400u
#define  AIPS_PACRL_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRL_TP4_MASK         0x1000u
#define  AIPS_PACRL_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRL_WP4_MASK         0x2000u
#define  AIPS_PACRL_WP4_SHIFT        13
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRL_SP4_MASK         0x4000u
#define  AIPS_PACRL_SP4_SHIFT        14
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRL_TP3_MASK         0x10000u
#define  AIPS_PACRL_TP3_SHIFT        16
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRL_WP3_MASK         0x20000u
#define  AIPS_PACRL_WP3_SHIFT        17
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRL_SP3_MASK         0x40000u
#define  AIPS_PACRL_SP3_SHIFT        18
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRL_TP2_MASK         0x100000u
#define  AIPS_PACRL_TP2_SHIFT        20
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRL_WP2_MASK         0x200000u
#define  AIPS_PACRL_WP2_SHIFT        21
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRL_SP2_MASK         0x400000u
#define  AIPS_PACRL_SP2_SHIFT        22
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRL_TP1_MASK         0x1000000u
#define  AIPS_PACRL_TP1_SHIFT        24
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRL_WP1_MASK         0x2000000u
#define  AIPS_PACRL_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRL_SP1_MASK         0x4000000u
#define  AIPS_PACRL_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRL_TP0_MASK         0x10000000u
#define  AIPS_PACRL_TP0_SHIFT        28
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRL_WP0_MASK         0x20000000u
#define  AIPS_PACRL_WP0_SHIFT        29
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRL_SP0_MASK         0x40000000u
#define  AIPS_PACRL_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRM_TP7_MASK         0x1u
#define  AIPS_PACRM_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRM_WP7_MASK         0x2u
#define  AIPS_PACRM_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRM_SP7_MASK         0x4u
#define  AIPS_PACRM_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRM_TP6_MASK         0x10u
#define  AIPS_PACRM_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRM_WP6_MASK         0x20u
#define  AIPS_PACRM_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRM_SP6_MASK         0x40u
#define  AIPS_PACRM_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRM_TP5_MASK         0x100u
#define  AIPS_PACRM_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRM_WP5_MASK         0x200u
#define  AIPS_PACRM_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRM_SP5_MASK         0x400u
#define  AIPS_PACRM_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRM_TP4_MASK         0x1000u
#define  AIPS_PACRM_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRM_WP4_MASK         0x2000u
#define  AIPS_PACRM_WP4_SHIFT        13
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRM_SP4_MASK         0x4000u
#define  AIPS_PACRM_SP4_SHIFT        14
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRM_TP3_MASK         0x10000u
#define  AIPS_PACRM_TP3_SHIFT        16
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRM_WP3_MASK         0x20000u
#define  AIPS_PACRM_WP3_SHIFT        17
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRM_SP3_MASK         0x40000u
#define  AIPS_PACRM_SP3_SHIFT        18
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRM_TP2_MASK         0x100000u
#define  AIPS_PACRM_TP2_SHIFT        20
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRM_WP2_MASK         0x200000u
#define  AIPS_PACRM_WP2_SHIFT        21
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRM_SP2_MASK         0x400000u
#define  AIPS_PACRM_SP2_SHIFT        22
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRM_TP1_MASK         0x1000000u
#define  AIPS_PACRM_TP1_SHIFT        24
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRM_WP1_MASK         0x2000000u
#define  AIPS_PACRM_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRM_SP1_MASK         0x4000000u
#define  AIPS_PACRM_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRM_TP0_MASK         0x10000000u
#define  AIPS_PACRM_TP0_SHIFT        28
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRM_WP0_MASK         0x20000000u
#define  AIPS_PACRM_WP0_SHIFT        29
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRM_SP0_MASK         0x40000000u
#define  AIPS_PACRM_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRN_TP7_MASK         0x1u
#define  AIPS_PACRN_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRN_WP7_MASK         0x2u
#define  AIPS_PACRN_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRN_SP7_MASK         0x4u
#define  AIPS_PACRN_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRN_TP6_MASK         0x10u
#define  AIPS_PACRN_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRN_WP6_MASK         0x20u
#define  AIPS_PACRN_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRN_SP6_MASK         0x40u
#define  AIPS_PACRN_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRN_TP5_MASK         0x100u
#define  AIPS_PACRN_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRN_WP5_MASK         0x200u
#define  AIPS_PACRN_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRN_SP5_MASK         0x400u
#define  AIPS_PACRN_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRN_TP4_MASK         0x1000u
#define  AIPS_PACRN_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRN_WP4_MASK         0x2000u
#define  AIPS_PACRN_WP4_SHIFT        13
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRN_SP4_MASK         0x4000u
#define  AIPS_PACRN_SP4_SHIFT        14
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRN_TP3_MASK         0x10000u
#define  AIPS_PACRN_TP3_SHIFT        16
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRN_WP3_MASK         0x20000u
#define  AIPS_PACRN_WP3_SHIFT        17
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRN_SP3_MASK         0x40000u
#define  AIPS_PACRN_SP3_SHIFT        18
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRN_TP2_MASK         0x100000u
#define  AIPS_PACRN_TP2_SHIFT        20
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRN_WP2_MASK         0x200000u
#define  AIPS_PACRN_WP2_SHIFT        21
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRN_SP2_MASK         0x400000u
#define  AIPS_PACRN_SP2_SHIFT        22
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRN_TP1_MASK         0x1000000u
#define  AIPS_PACRN_TP1_SHIFT        24
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRN_WP1_MASK         0x2000000u
#define  AIPS_PACRN_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRN_SP1_MASK         0x4000000u
#define  AIPS_PACRN_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRN_TP0_MASK         0x10000000u
#define  AIPS_PACRN_TP0_SHIFT        28
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRN_WP0_MASK         0x20000000u
#define  AIPS_PACRN_WP0_SHIFT        29
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRN_SP0_MASK         0x40000000u
#define  AIPS_PACRN_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRO_TP7_MASK         0x1u
#define  AIPS_PACRO_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRO_WP7_MASK         0x2u
#define  AIPS_PACRO_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRO_SP7_MASK         0x4u
#define  AIPS_PACRO_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRO_TP6_MASK         0x10u
#define  AIPS_PACRO_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRO_WP6_MASK         0x20u
#define  AIPS_PACRO_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRO_SP6_MASK         0x40u
#define  AIPS_PACRO_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRO_TP5_MASK         0x100u
#define  AIPS_PACRO_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRO_WP5_MASK         0x200u
#define  AIPS_PACRO_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRO_SP5_MASK         0x400u
#define  AIPS_PACRO_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRO_TP4_MASK         0x1000u
#define  AIPS_PACRO_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRO_WP4_MASK         0x2000u
#define  AIPS_PACRO_WP4_SHIFT        13
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRO_SP4_MASK         0x4000u
#define  AIPS_PACRO_SP4_SHIFT        14
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRO_TP3_MASK         0x10000u
#define  AIPS_PACRO_TP3_SHIFT        16
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRO_WP3_MASK         0x20000u
#define  AIPS_PACRO_WP3_SHIFT        17
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRO_SP3_MASK         0x40000u
#define  AIPS_PACRO_SP3_SHIFT        18
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRO_TP2_MASK         0x100000u
#define  AIPS_PACRO_TP2_SHIFT        20
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRO_WP2_MASK         0x200000u
#define  AIPS_PACRO_WP2_SHIFT        21
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRO_SP2_MASK         0x400000u
#define  AIPS_PACRO_SP2_SHIFT        22
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRO_TP1_MASK         0x1000000u
#define  AIPS_PACRO_TP1_SHIFT        24
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRO_WP1_MASK         0x2000000u
#define  AIPS_PACRO_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRO_SP1_MASK         0x4000000u
#define  AIPS_PACRO_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRO_TP0_MASK         0x10000000u
#define  AIPS_PACRO_TP0_SHIFT        28
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRO_WP0_MASK         0x20000000u
#define  AIPS_PACRO_WP0_SHIFT        29
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRO_SP0_MASK         0x40000000u
#define  AIPS_PACRO_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRP_TP7_MASK         0x1u
#define  AIPS_PACRP_TP7_SHIFT        0
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRP_WP7_MASK         0x2u
#define  AIPS_PACRP_WP7_SHIFT        1
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRP_SP7_MASK         0x4u
#define  AIPS_PACRP_SP7_SHIFT        2
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRP_TP6_MASK         0x10u
#define  AIPS_PACRP_TP6_SHIFT        4
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRP_WP6_MASK         0x20u
#define  AIPS_PACRP_WP6_SHIFT        5
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRP_SP6_MASK         0x40u
#define  AIPS_PACRP_SP6_SHIFT        6
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRP_TP5_MASK         0x100u
#define  AIPS_PACRP_TP5_SHIFT        8
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRP_WP5_MASK         0x200u
#define  AIPS_PACRP_WP5_SHIFT        9
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRP_SP5_MASK         0x400u
#define  AIPS_PACRP_SP5_SHIFT        10
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRP_TP4_MASK         0x1000u
#define  AIPS_PACRP_TP4_SHIFT        12
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRP_WP4_MASK         0x2000u
#define  AIPS_PACRP_WP4_SHIFT        13
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRP_SP4_MASK         0x4000u
#define  AIPS_PACRP_SP4_SHIFT        14
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRP_TP3_MASK         0x10000u
#define  AIPS_PACRP_TP3_SHIFT        16
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRP_WP3_MASK         0x20000u
#define  AIPS_PACRP_WP3_SHIFT        17
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRP_SP3_MASK         0x40000u
#define  AIPS_PACRP_SP3_SHIFT        18
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRP_TP2_MASK         0x100000u
#define  AIPS_PACRP_TP2_SHIFT        20
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRP_WP2_MASK         0x200000u
#define  AIPS_PACRP_WP2_SHIFT        21
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRP_SP2_MASK         0x400000u
#define  AIPS_PACRP_SP2_SHIFT        22
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRP_TP1_MASK         0x1000000u
#define  AIPS_PACRP_TP1_SHIFT        24
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRP_WP1_MASK         0x2000000u
#define  AIPS_PACRP_WP1_SHIFT        25
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRP_SP1_MASK         0x4000000u
#define  AIPS_PACRP_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRP_TP0_MASK         0x10000000u
#define  AIPS_PACRP_TP0_SHIFT        28
//   Trusted protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRP_WP0_MASK         0x20000000u
#define  AIPS_PACRP_WP0_SHIFT        29
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRP_SP0_MASK         0x40000000u
#define  AIPS_PACRP_SP0_SHIFT        30
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRU_TP1_MASK         0x1000000u
#define  AIPS_PACRU_TP1_SHIFT        24
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRU_WP1_MASK         0x2000000u
#define  AIPS_PACRU_WP1_SHIFT        25
//   Write protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRU_SP1_MASK         0x4000000u
#define  AIPS_PACRU_SP1_SHIFT        26
//   Supervisor Protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  AIPS_PACRU_TP0_MASK         0x10000000u
#define  AIPS_PACRU_TP0_SHIFT        28
//   Trusted Protect
//     0b0 ==> Accesses from an untrusted master are allowed.
//     0b1 ==> Accesses from an untrusted master are not allowed.

#define  AIPS_PACRU_WP0_MASK         0x20000000u
#define  AIPS_PACRU_WP0_SHIFT        29
//   Write Protect
//     0b0 ==> This peripheral allows write accesses.
//     0b1 ==> This peripheral is write protected.

#define  AIPS_PACRU_SP0_MASK         0x40000000u
#define  AIPS_PACRU_SP0_SHIFT        30
//   Supervisor protect
//     0b0 ==> This peripheral does not require supervisor privilege level for accesses.
//     0b1 ==> This peripheral requires supervisor privilege level for accesses.

#define  CMP_CR0_HYSTCTR_MASK        0x2u
#define  CMP_CR0_HYSTCTR_SHIFT       0
#define  CMP_CR0_HYSTCTR(x)          (((uint8_t)(((uint8_t)(x))<<CMP_CR0_HYSTCTR_SHIFT))&CMP_CR0_HYSTCTR_MASK)
//   Comparator hard block hysteresis control
//     0b00 ==> Level 0
//     0b01 ==> Level 1
//     0b10 ==> Level 2
//     0b11 ==> Level 3

#define  CMP_CR0_FILTER_CNT_MASK     0x30u
#define  CMP_CR0_FILTER_CNT_SHIFT    4
#define  CMP_CR0_FILTER_CNT(x)       (((uint8_t)(((uint8_t)(x))<<CMP_CR0_FILTER_CNT_SHIFT))&CMP_CR0_FILTER_CNT_MASK)
//   Filter Sample Count
//     0b000 ==> Filter is disabled. If SE = 1, then COUT is a logic 0. This is not a legal state, and is not recommended. If SE = 0, COUT = COUTA.
//     0b001 ==> One sample must agree. The comparator output is simply sampled.
//     0b010 ==> 2 consecutive samples must agree.
//     0b011 ==> 3 consecutive samples must agree.
//     0b100 ==> 4 consecutive samples must agree.
//     0b101 ==> 5 consecutive samples must agree.
//     0b110 ==> 6 consecutive samples must agree.
//     0b111 ==> 7 consecutive samples must agree.

#define  CMP_CR1_EN_MASK             0x1u
#define  CMP_CR1_EN_SHIFT            0
//   Comparator Module Enable
//     0b0 ==> Analog Comparator is disabled.
//     0b1 ==> Analog Comparator is enabled.

#define  CMP_CR1_OPE_MASK            0x2u
#define  CMP_CR1_OPE_SHIFT           1
//   Comparator Output Pin Enable
//     0b0 ==> CMPO is not available on the associated CMPO output pin.
//     0b1 ==> CMPO is available on the associated CMPO output pin.

#define  CMP_CR1_COS_MASK            0x4u
#define  CMP_CR1_COS_SHIFT           2
//   Comparator Output Select
//     0b0 ==> Set the filtered comparator output (CMPO) to equal COUT.
//     0b1 ==> Set the unfiltered comparator output (CMPO) to equal COUTA.

#define  CMP_CR1_INV_MASK            0x8u
#define  CMP_CR1_INV_SHIFT           3
//   Comparator INVERT
//     0b0 ==> Does not invert the comparator output.
//     0b1 ==> Inverts the comparator output.

#define  CMP_CR1_PMODE_MASK          0x10u
#define  CMP_CR1_PMODE_SHIFT         4
//   Power Mode Select
//     0b0 ==> Low-Speed (LS) Comparison mode selected. In this mode, CMP has slower output propagation delay and lower current consumption.
//     0b1 ==> High-Speed (HS) Comparison mode selected. In this mode, CMP has faster output propagation delay and higher current consumption.

#define  CMP_CR1_TRIGM_MASK          0x20u
#define  CMP_CR1_TRIGM_SHIFT         5
//   Trigger Mode Enable
//     0b0 ==> Trigger mode is disabled.
//     0b1 ==> Trigger mode is enabled.

#define  CMP_CR1_WE_MASK             0x40u
#define  CMP_CR1_WE_SHIFT            6
//   Windowing Enable
//     0b0 ==> Windowing mode is not selected.
//     0b1 ==> Windowing mode is selected.

#define  CMP_CR1_SE_MASK             0x80u
#define  CMP_CR1_SE_SHIFT            7
//   Sample Enable
//     0b0 ==> Sampling mode is not selected.
//     0b1 ==> Sampling mode is selected.

#define  CMP_FPR_FILT_PER_MASK       0x8u
#define  CMP_FPR_FILT_PER_SHIFT      0
#define  CMP_FPR_FILT_PER(x)         (((uint8_t)(((uint8_t)(x))<<CMP_FPR_FILT_PER_SHIFT))&CMP_FPR_FILT_PER_MASK)
//   Filter Sample Period

#define  CMP_SCR_COUT_MASK           0x1u
#define  CMP_SCR_COUT_SHIFT          0
//   Analog Comparator Output

#define  CMP_SCR_CFF_MASK            0x2u
#define  CMP_SCR_CFF_SHIFT           1
//   Analog Comparator Flag Falling
//     0b0 ==> Falling-edge on COUT has not been detected.
//     0b1 ==> Falling-edge on COUT has occurred.

#define  CMP_SCR_CFR_MASK            0x4u
#define  CMP_SCR_CFR_SHIFT           2
//   Analog Comparator Flag Rising
//     0b0 ==> Rising-edge on COUT has not been detected.
//     0b1 ==> Rising-edge on COUT has occurred.

#define  CMP_SCR_IEF_MASK            0x8u
#define  CMP_SCR_IEF_SHIFT           3
//   Comparator Interrupt Enable Falling
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  CMP_SCR_IER_MASK            0x10u
#define  CMP_SCR_IER_SHIFT           4
//   Comparator Interrupt Enable Rising
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  CMP_SCR_DMAEN_MASK          0x40u
#define  CMP_SCR_DMAEN_SHIFT         6
//   DMA Enable Control
//     0b0 ==> DMA is disabled.
//     0b1 ==> DMA is enabled.

#define  CMP_DACCR_VOSEL_MASK        0x6u
#define  CMP_DACCR_VOSEL_SHIFT       0
#define  CMP_DACCR_VOSEL(x)          (((uint8_t)(((uint8_t)(x))<<CMP_DACCR_VOSEL_SHIFT))&CMP_DACCR_VOSEL_MASK)
//   DAC Output Voltage Select

#define  CMP_DACCR_VRSEL_MASK        0x40u
#define  CMP_DACCR_VRSEL_SHIFT       6
//   Supply Voltage Reference Source Select
//     0b0 ==> V is selected as resistor ladder network supply reference V. in1 in
//     0b1 ==> V is selected as resistor ladder network supply reference V. in2 in

#define  CMP_DACCR_DACEN_MASK        0x80u
#define  CMP_DACCR_DACEN_SHIFT       7
//   DAC Enable
//     0b0 ==> DAC is disabled.
//     0b1 ==> DAC is enabled.

#define  CMP_MUXCR_MSEL_MASK         0x3u
#define  CMP_MUXCR_MSEL_SHIFT        0
#define  CMP_MUXCR_MSEL(x)           (((uint8_t)(((uint8_t)(x))<<CMP_MUXCR_MSEL_SHIFT))&CMP_MUXCR_MSEL_MASK)
//   Minus Input Mux Control
//     0b000 ==> IN0
//     0b001 ==> IN1
//     0b010 ==> IN2
//     0b011 ==> IN3
//     0b100 ==> IN4
//     0b101 ==> IN5
//     0b110 ==> IN6
//     0b111 ==> IN7

#define  CMP_MUXCR_PSEL_MASK         0x18u
#define  CMP_MUXCR_PSEL_SHIFT        3
#define  CMP_MUXCR_PSEL(x)           (((uint8_t)(((uint8_t)(x))<<CMP_MUXCR_PSEL_SHIFT))&CMP_MUXCR_PSEL_MASK)
//   Plus Input Mux Control
//     0b000 ==> IN0
//     0b001 ==> IN1
//     0b010 ==> IN2
//     0b011 ==> IN3
//     0b100 ==> IN4
//     0b101 ==> IN5
//     0b110 ==> IN6
//     0b111 ==> IN7

#define  CMP_MUXCR_PSTM_MASK         0x40u
#define  CMP_MUXCR_PSTM_SHIFT        6
//   Pass Through Mode Enable
//     0b0 ==> Pass Through Mode is disabled.
//     0b1 ==> Pass Through Mode is enabled.

#define  CRC_CRC_LL_MASK             0x8u
#define  CRC_CRC_LL_SHIFT            0
#define  CRC_CRC_LL(x)               (((uint32_t)(((uint32_t)(x))<<CRC_CRC_LL_SHIFT))&CRC_CRC_LL_MASK)
//   CRC Low Lower Byte

#define  CRC_CRC_LU_MASK             0x800u
#define  CRC_CRC_LU_SHIFT            8
#define  CRC_CRC_LU(x)               (((uint32_t)(((uint32_t)(x))<<CRC_CRC_LU_SHIFT))&CRC_CRC_LU_MASK)
//   CRC Low Upper Byte

#define  CRC_CRC_HL_MASK             0x80000u
#define  CRC_CRC_HL_SHIFT            16
#define  CRC_CRC_HL(x)               (((uint32_t)(((uint32_t)(x))<<CRC_CRC_HL_SHIFT))&CRC_CRC_HL_MASK)
//   CRC High Lower Byte

#define  CRC_CRC_HU_MASK             0x8000000u
#define  CRC_CRC_HU_SHIFT            24
#define  CRC_CRC_HU(x)               (((uint32_t)(((uint32_t)(x))<<CRC_CRC_HU_SHIFT))&CRC_CRC_HU_MASK)
//   CRC High Upper Byte

#define  CRC_GPOLY_LOW_MASK          0x10u
#define  CRC_GPOLY_LOW_SHIFT         0
#define  CRC_GPOLY_LOW(x)            (((uint32_t)(((uint32_t)(x))<<CRC_GPOLY_LOW_SHIFT))&CRC_GPOLY_LOW_MASK)
//   Low Polynominal Half-word

#define  CRC_GPOLY_HIGH_MASK         0x100000u
#define  CRC_GPOLY_HIGH_SHIFT        16
#define  CRC_GPOLY_HIGH(x)           (((uint32_t)(((uint32_t)(x))<<CRC_GPOLY_HIGH_SHIFT))&CRC_GPOLY_HIGH_MASK)
//   High Polynominal Half-word

#define  CRC_CTRL_TCRC_MASK          0x1000000u
#define  CRC_CTRL_TCRC_SHIFT         24
//     0b0 ==> 16-bit CRC protocol.
//     0b1 ==> 32-bit CRC protocol.

#define  CRC_CTRL_WAS_MASK           0x2000000u
#define  CRC_CTRL_WAS_SHIFT          25
//   Write CRC Data Register As Seed
//     0b0 ==> Writes to the CRC data register are data values.
//     0b1 ==> Writes to the CRC data register are seed values.

#define  CRC_CTRL_FXOR_MASK          0x4000000u
#define  CRC_CTRL_FXOR_SHIFT         26
//   Complement Read Of CRC Data Register
//     0b0 ==> No XOR on reading.
//     0b1 ==> Invert or complement the read value of the CRC Data register.

#define  CRC_CTRL_TOTR_MASK          0x20000000u
#define  CRC_CTRL_TOTR_SHIFT         28
#define  CRC_CTRL_TOTR(x)            (((uint32_t)(((uint32_t)(x))<<CRC_CTRL_TOTR_SHIFT))&CRC_CTRL_TOTR_MASK)
//   Type Of Transpose For Read
//     0b00 ==> No transposition.
//     0b01 ==> Bits in bytes are transposed; bytes are not transposed.
//     0b10 ==> Both bits in bytes and bytes are transposed.
//     0b11 ==> Only bytes are transposed; no bits in a byte are transposed.

#define  CRC_CTRL_TOT_MASK           0x80000000u
#define  CRC_CTRL_TOT_SHIFT          30
#define  CRC_CTRL_TOT(x)             (((uint32_t)(((uint32_t)(x))<<CRC_CTRL_TOT_SHIFT))&CRC_CTRL_TOT_MASK)
//   Type Of Transpose For Writes
//     0b00 ==> No transposition.
//     0b01 ==> Bits in bytes are transposed; bytes are not transposed.
//     0b10 ==> Both bits in bytes and bytes are transposed.
//     0b11 ==> Only bytes are transposed; no bits in a byte are transposed.

#define  DMAMUX0_CHCFG0_SOURCE_MASK  0x6u
#define  DMAMUX0_CHCFG0_SOURCE_SHIFT 0
#define  DMAMUX0_CHCFG0_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX0_CHCFG0_SOURCE_SHIFT))&DMAMUX0_CHCFG0_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX0_CHCFG0_TRIG_MASK    0x40u
#define  DMAMUX0_CHCFG0_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX0_CHCFG0_ENBL_MASK    0x80u
#define  DMAMUX0_CHCFG0_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  DMAMUX0_CHCFG1_SOURCE_MASK  0x6u
#define  DMAMUX0_CHCFG1_SOURCE_SHIFT 0
#define  DMAMUX0_CHCFG1_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX0_CHCFG1_SOURCE_SHIFT))&DMAMUX0_CHCFG1_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX0_CHCFG1_TRIG_MASK    0x40u
#define  DMAMUX0_CHCFG1_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX0_CHCFG1_ENBL_MASK    0x80u
#define  DMAMUX0_CHCFG1_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  DMAMUX0_CHCFG2_SOURCE_MASK  0x6u
#define  DMAMUX0_CHCFG2_SOURCE_SHIFT 0
#define  DMAMUX0_CHCFG2_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX0_CHCFG2_SOURCE_SHIFT))&DMAMUX0_CHCFG2_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX0_CHCFG2_TRIG_MASK    0x40u
#define  DMAMUX0_CHCFG2_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX0_CHCFG2_ENBL_MASK    0x80u
#define  DMAMUX0_CHCFG2_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  DMAMUX0_CHCFG3_SOURCE_MASK  0x6u
#define  DMAMUX0_CHCFG3_SOURCE_SHIFT 0
#define  DMAMUX0_CHCFG3_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX0_CHCFG3_SOURCE_SHIFT))&DMAMUX0_CHCFG3_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX0_CHCFG3_TRIG_MASK    0x40u
#define  DMAMUX0_CHCFG3_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX0_CHCFG3_ENBL_MASK    0x80u
#define  DMAMUX0_CHCFG3_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  DMAMUX1_CHCFG0_SOURCE_MASK  0x6u
#define  DMAMUX1_CHCFG0_SOURCE_SHIFT 0
#define  DMAMUX1_CHCFG0_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX1_CHCFG0_SOURCE_SHIFT))&DMAMUX1_CHCFG0_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX1_CHCFG0_TRIG_MASK    0x40u
#define  DMAMUX1_CHCFG0_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX1_CHCFG0_ENBL_MASK    0x80u
#define  DMAMUX1_CHCFG0_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  DMAMUX1_CHCFG1_SOURCE_MASK  0x6u
#define  DMAMUX1_CHCFG1_SOURCE_SHIFT 0
#define  DMAMUX1_CHCFG1_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX1_CHCFG1_SOURCE_SHIFT))&DMAMUX1_CHCFG1_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX1_CHCFG1_TRIG_MASK    0x40u
#define  DMAMUX1_CHCFG1_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX1_CHCFG1_ENBL_MASK    0x80u
#define  DMAMUX1_CHCFG1_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  DMAMUX1_CHCFG2_SOURCE_MASK  0x6u
#define  DMAMUX1_CHCFG2_SOURCE_SHIFT 0
#define  DMAMUX1_CHCFG2_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX1_CHCFG2_SOURCE_SHIFT))&DMAMUX1_CHCFG2_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX1_CHCFG2_TRIG_MASK    0x40u
#define  DMAMUX1_CHCFG2_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX1_CHCFG2_ENBL_MASK    0x80u
#define  DMAMUX1_CHCFG2_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  DMAMUX1_CHCFG3_SOURCE_MASK  0x6u
#define  DMAMUX1_CHCFG3_SOURCE_SHIFT 0
#define  DMAMUX1_CHCFG3_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX1_CHCFG3_SOURCE_SHIFT))&DMAMUX1_CHCFG3_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX1_CHCFG3_TRIG_MASK    0x40u
#define  DMAMUX1_CHCFG3_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX1_CHCFG3_ENBL_MASK    0x80u
#define  DMAMUX1_CHCFG3_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  DMAMUX2_CHCFG0_SOURCE_MASK  0x6u
#define  DMAMUX2_CHCFG0_SOURCE_SHIFT 0
#define  DMAMUX2_CHCFG0_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX2_CHCFG0_SOURCE_SHIFT))&DMAMUX2_CHCFG0_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX2_CHCFG0_TRIG_MASK    0x40u
#define  DMAMUX2_CHCFG0_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX2_CHCFG0_ENBL_MASK    0x80u
#define  DMAMUX2_CHCFG0_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  DMAMUX2_CHCFG1_SOURCE_MASK  0x6u
#define  DMAMUX2_CHCFG1_SOURCE_SHIFT 0
#define  DMAMUX2_CHCFG1_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX2_CHCFG1_SOURCE_SHIFT))&DMAMUX2_CHCFG1_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX2_CHCFG1_TRIG_MASK    0x40u
#define  DMAMUX2_CHCFG1_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX2_CHCFG1_ENBL_MASK    0x80u
#define  DMAMUX2_CHCFG1_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  DMAMUX2_CHCFG2_SOURCE_MASK  0x6u
#define  DMAMUX2_CHCFG2_SOURCE_SHIFT 0
#define  DMAMUX2_CHCFG2_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX2_CHCFG2_SOURCE_SHIFT))&DMAMUX2_CHCFG2_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX2_CHCFG2_TRIG_MASK    0x40u
#define  DMAMUX2_CHCFG2_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX2_CHCFG2_ENBL_MASK    0x80u
#define  DMAMUX2_CHCFG2_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  DMAMUX2_CHCFG3_SOURCE_MASK  0x6u
#define  DMAMUX2_CHCFG3_SOURCE_SHIFT 0
#define  DMAMUX2_CHCFG3_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX2_CHCFG3_SOURCE_SHIFT))&DMAMUX2_CHCFG3_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX2_CHCFG3_TRIG_MASK    0x40u
#define  DMAMUX2_CHCFG3_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX2_CHCFG3_ENBL_MASK    0x80u
#define  DMAMUX2_CHCFG3_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  DMAMUX3_CHCFG0_SOURCE_MASK  0x6u
#define  DMAMUX3_CHCFG0_SOURCE_SHIFT 0
#define  DMAMUX3_CHCFG0_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX3_CHCFG0_SOURCE_SHIFT))&DMAMUX3_CHCFG0_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX3_CHCFG0_TRIG_MASK    0x40u
#define  DMAMUX3_CHCFG0_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX3_CHCFG0_ENBL_MASK    0x80u
#define  DMAMUX3_CHCFG0_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  DMAMUX3_CHCFG1_SOURCE_MASK  0x6u
#define  DMAMUX3_CHCFG1_SOURCE_SHIFT 0
#define  DMAMUX3_CHCFG1_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX3_CHCFG1_SOURCE_SHIFT))&DMAMUX3_CHCFG1_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX3_CHCFG1_TRIG_MASK    0x40u
#define  DMAMUX3_CHCFG1_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX3_CHCFG1_ENBL_MASK    0x80u
#define  DMAMUX3_CHCFG1_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  DMAMUX3_CHCFG2_SOURCE_MASK  0x6u
#define  DMAMUX3_CHCFG2_SOURCE_SHIFT 0
#define  DMAMUX3_CHCFG2_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX3_CHCFG2_SOURCE_SHIFT))&DMAMUX3_CHCFG2_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX3_CHCFG2_TRIG_MASK    0x40u
#define  DMAMUX3_CHCFG2_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX3_CHCFG2_ENBL_MASK    0x80u
#define  DMAMUX3_CHCFG2_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  DMAMUX3_CHCFG3_SOURCE_MASK  0x6u
#define  DMAMUX3_CHCFG3_SOURCE_SHIFT 0
#define  DMAMUX3_CHCFG3_SOURCE(x)    (((uint8_t)(((uint8_t)(x))<<DMAMUX3_CHCFG3_SOURCE_SHIFT))&DMAMUX3_CHCFG3_SOURCE_MASK)
//   DMA Channel Source (Slot)

#define  DMAMUX3_CHCFG3_TRIG_MASK    0x40u
#define  DMAMUX3_CHCFG3_TRIG_SHIFT   6
//   DMA Channel Trigger Enable
//     0b0 ==> Triggering is disabled. If triggering is disabled, and the ENBL bit is set, the DMA Channel will simply route the specified source to the DMA channel. (Normal mode)
//     0b1 ==> Triggering is enabled. If triggering is enabled, and the ENBL bit is set, the DMAMUX is in Periodic Trigger mode.

#define  DMAMUX3_CHCFG3_ENBL_MASK    0x80u
#define  DMAMUX3_CHCFG3_ENBL_SHIFT   7
//   DMA Channel Enable
//     0b0 ==> DMA channel is disabled. This mode is primarily used during configuration of the DMA Mux. The DMA has separate channel enables/disables, which should be used to disable or re-configure a DMA channel.
//     0b1 ==> DMA channel is enabled

#define  EWM_CTRL_EWMEN_MASK         0x1u
#define  EWM_CTRL_EWMEN_SHIFT        0
//   EWM enable.

#define  EWM_CTRL_ASSIN_MASK         0x2u
#define  EWM_CTRL_ASSIN_SHIFT        1
//   EWM_in's Assertion State Select.

#define  EWM_CTRL_INEN_MASK          0x4u
#define  EWM_CTRL_INEN_SHIFT         2
//   Input Enable.

#define  EWM_CTRL_INTEN_MASK         0x8u
#define  EWM_CTRL_INTEN_SHIFT        3
//   Interrupt Enable.

#define  EWM_SERV_SERVICE_MASK       0x8u
#define  EWM_SERV_SERVICE_SHIFT      0
#define  EWM_SERV_SERVICE(x)         (((uint8_t)(((uint8_t)(x))<<EWM_SERV_SERVICE_SHIFT))&EWM_SERV_SERVICE_MASK)

#define  EWM_CMPL_COMPAREL_MASK      0x8u
#define  EWM_CMPL_COMPAREL_SHIFT     0
#define  EWM_CMPL_COMPAREL(x)        (((uint8_t)(((uint8_t)(x))<<EWM_CMPL_COMPAREL_SHIFT))&EWM_CMPL_COMPAREL_MASK)

#define  EWM_CMPH_COMPAREH_MASK      0x8u
#define  EWM_CMPH_COMPAREH_SHIFT     0
#define  EWM_CMPH_COMPAREH(x)        (((uint8_t)(((uint8_t)(x))<<EWM_CMPH_COMPAREH_SHIFT))&EWM_CMPH_COMPAREH_MASK)

#define  FTFA_FSTAT_MGSTAT0_MASK     0x1u
#define  FTFA_FSTAT_MGSTAT0_SHIFT    0
//   Memory Controller Command Completion Status Flag

#define  FTFA_FSTAT_FPVIOL_MASK      0x10u
#define  FTFA_FSTAT_FPVIOL_SHIFT     4
//   Flash Protection Violation Flag
//     0b0 ==> No protection violation detected
//     0b1 ==> Protection violation detected

#define  FTFA_FSTAT_ACCERR_MASK      0x20u
#define  FTFA_FSTAT_ACCERR_SHIFT     5
//   Flash Access Error Flag
//     0b0 ==> No access error detected
//     0b1 ==> Access error detected

#define  FTFA_FSTAT_RDCOLERR_MASK    0x40u
#define  FTFA_FSTAT_RDCOLERR_SHIFT   6
//   Flash Read Collision Error Flag
//     0b0 ==> No collision error detected
//     0b1 ==> Collision error detected

#define  FTFA_FSTAT_CCIF_MASK        0x80u
#define  FTFA_FSTAT_CCIF_SHIFT       7
//   Command Complete Interrupt Flag
//     0b0 ==> Flash command in progress
//     0b1 ==> Flash command has completed

#define  FTFA_FCNFG_ERSSUSP_MASK     0x10u
#define  FTFA_FCNFG_ERSSUSP_SHIFT    4
//   Erase Suspend
//     0b0 ==> No suspend requested
//     0b1 ==> Suspend the current Erase Flash Sector command execution.

#define  FTFA_FCNFG_ERSAREQ_MASK     0x20u
#define  FTFA_FCNFG_ERSAREQ_SHIFT    5
//   Erase All Request
//     0b0 ==> No request or request complete
//     0b1 ==> Request to: run the Erase All Blocks command, verify the erased state, program the security byte in the Flash Configuration Field to the unsecure state, and release MCU security by setting the FSEC[SEC] field to the unsecure state.

#define  FTFA_FCNFG_RDCOLLIE_MASK    0x40u
#define  FTFA_FCNFG_RDCOLLIE_SHIFT   6
//   Read Collision Error Interrupt Enable
//     0b0 ==> Read collision error interrupt disabled
//     0b1 ==> Read collision error interrupt enabled. An interrupt request is generated whenever a flash memory read collision error is detected (see the description of FSTAT[RDCOLERR]).

#define  FTFA_FCNFG_CCIE_MASK        0x80u
#define  FTFA_FCNFG_CCIE_SHIFT       7
//   Command Complete Interrupt Enable
//     0b0 ==> Command complete interrupt disabled
//     0b1 ==> Command complete interrupt enabled. An interrupt request is generated whenever the FSTAT[CCIF] flag is set.

#define  FTFA_FSEC_SEC_MASK          0x2u
#define  FTFA_FSEC_SEC_SHIFT         0
#define  FTFA_FSEC_SEC(x)            (((uint8_t)(((uint8_t)(x))<<FTFA_FSEC_SEC_SHIFT))&FTFA_FSEC_SEC_MASK)
//   Flash Security
//     0b00 ==> MCU security status is secure
//     0b01 ==> MCU security status is secure
//     0b10 ==> MCU security status is unsecure (The standard shipping condition of the flash memory module is unsecure.)
//     0b11 ==> MCU security status is secure

#define  FTFA_FSEC_FSLACC_MASK       0x8u
#define  FTFA_FSEC_FSLACC_SHIFT      2
#define  FTFA_FSEC_FSLACC(x)         (((uint8_t)(((uint8_t)(x))<<FTFA_FSEC_FSLACC_SHIFT))&FTFA_FSEC_FSLACC_MASK)
//   Freescale Failure Analysis Access Code
//     0b00 ==> Freescale factory access granted
//     0b01 ==> Freescale factory access denied
//     0b10 ==> Freescale factory access denied
//     0b11 ==> Freescale factory access granted

#define  FTFA_FSEC_MEEN_MASK         0x20u
#define  FTFA_FSEC_MEEN_SHIFT        4
#define  FTFA_FSEC_MEEN(x)           (((uint8_t)(((uint8_t)(x))<<FTFA_FSEC_MEEN_SHIFT))&FTFA_FSEC_MEEN_MASK)
//   Mass Erase Enable Bits
//     0b00 ==> Mass erase is enabled
//     0b01 ==> Mass erase is enabled
//     0b10 ==> Mass erase is disabled
//     0b11 ==> Mass erase is enabled

#define  FTFA_FSEC_KEYEN_MASK        0x80u
#define  FTFA_FSEC_KEYEN_SHIFT       6
#define  FTFA_FSEC_KEYEN(x)          (((uint8_t)(((uint8_t)(x))<<FTFA_FSEC_KEYEN_SHIFT))&FTFA_FSEC_KEYEN_MASK)
//   Backdoor Key Security Enable
//     0b00 ==> Backdoor key access disabled
//     0b01 ==> Backdoor key access disabled (preferred KEYEN state to disable backdoor key access)
//     0b10 ==> Backdoor key access enabled
//     0b11 ==> Backdoor key access disabled

#define  FTFA_FOPT_OPT_MASK          0x8u
#define  FTFA_FOPT_OPT_SHIFT         0
#define  FTFA_FOPT_OPT(x)            (((uint8_t)(((uint8_t)(x))<<FTFA_FOPT_OPT_SHIFT))&FTFA_FOPT_OPT_MASK)
//   Nonvolatile Option

#define  FTFA_FCCOB3_CCOBn_MASK      0x8u
#define  FTFA_FCCOB3_CCOBn_SHIFT     0
#define  FTFA_FCCOB3_CCOBn(x)        (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB3_CCOBn_SHIFT))&FTFA_FCCOB3_CCOBn_MASK)

#define  FTFA_FCCOB2_CCOBn_MASK      0x8u
#define  FTFA_FCCOB2_CCOBn_SHIFT     0
#define  FTFA_FCCOB2_CCOBn(x)        (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB2_CCOBn_SHIFT))&FTFA_FCCOB2_CCOBn_MASK)

#define  FTFA_FCCOB1_CCOBn_MASK      0x8u
#define  FTFA_FCCOB1_CCOBn_SHIFT     0
#define  FTFA_FCCOB1_CCOBn(x)        (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB1_CCOBn_SHIFT))&FTFA_FCCOB1_CCOBn_MASK)

#define  FTFA_FCCOB0_CCOBn_MASK      0x8u
#define  FTFA_FCCOB0_CCOBn_SHIFT     0
#define  FTFA_FCCOB0_CCOBn(x)        (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB0_CCOBn_SHIFT))&FTFA_FCCOB0_CCOBn_MASK)

#define  FTFA_FCCOB7_CCOBn_MASK      0x8u
#define  FTFA_FCCOB7_CCOBn_SHIFT     0
#define  FTFA_FCCOB7_CCOBn(x)        (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB7_CCOBn_SHIFT))&FTFA_FCCOB7_CCOBn_MASK)

#define  FTFA_FCCOB6_CCOBn_MASK      0x8u
#define  FTFA_FCCOB6_CCOBn_SHIFT     0
#define  FTFA_FCCOB6_CCOBn(x)        (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB6_CCOBn_SHIFT))&FTFA_FCCOB6_CCOBn_MASK)

#define  FTFA_FCCOB5_CCOBn_MASK      0x8u
#define  FTFA_FCCOB5_CCOBn_SHIFT     0
#define  FTFA_FCCOB5_CCOBn(x)        (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB5_CCOBn_SHIFT))&FTFA_FCCOB5_CCOBn_MASK)

#define  FTFA_FCCOB4_CCOBn_MASK      0x8u
#define  FTFA_FCCOB4_CCOBn_SHIFT     0
#define  FTFA_FCCOB4_CCOBn(x)        (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB4_CCOBn_SHIFT))&FTFA_FCCOB4_CCOBn_MASK)

#define  FTFA_FCCOBB_CCOBn_MASK      0x8u
#define  FTFA_FCCOBB_CCOBn_SHIFT     0
#define  FTFA_FCCOBB_CCOBn(x)        (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOBB_CCOBn_SHIFT))&FTFA_FCCOBB_CCOBn_MASK)

#define  FTFA_FCCOBA_CCOBn_MASK      0x8u
#define  FTFA_FCCOBA_CCOBn_SHIFT     0
#define  FTFA_FCCOBA_CCOBn(x)        (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOBA_CCOBn_SHIFT))&FTFA_FCCOBA_CCOBn_MASK)

#define  FTFA_FCCOB9_CCOBn_MASK      0x8u
#define  FTFA_FCCOB9_CCOBn_SHIFT     0
#define  FTFA_FCCOB9_CCOBn(x)        (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB9_CCOBn_SHIFT))&FTFA_FCCOB9_CCOBn_MASK)

#define  FTFA_FCCOB8_CCOBn_MASK      0x8u
#define  FTFA_FCCOB8_CCOBn_SHIFT     0
#define  FTFA_FCCOB8_CCOBn(x)        (((uint8_t)(((uint8_t)(x))<<FTFA_FCCOB8_CCOBn_SHIFT))&FTFA_FCCOB8_CCOBn_MASK)

#define  FTFA_FPROT3_PROT_MASK       0x8u
#define  FTFA_FPROT3_PROT_SHIFT      0
#define  FTFA_FPROT3_PROT(x)         (((uint8_t)(((uint8_t)(x))<<FTFA_FPROT3_PROT_SHIFT))&FTFA_FPROT3_PROT_MASK)
//   Program Flash Region Protect
//     0b0 ==> Program flash region is protected.
//     0b1 ==> Program flash region is not protected

#define  FTFA_FPROT2_PROT_MASK       0x8u
#define  FTFA_FPROT2_PROT_SHIFT      0
#define  FTFA_FPROT2_PROT(x)         (((uint8_t)(((uint8_t)(x))<<FTFA_FPROT2_PROT_SHIFT))&FTFA_FPROT2_PROT_MASK)
//   Program Flash Region Protect
//     0b0 ==> Program flash region is protected.
//     0b1 ==> Program flash region is not protected

#define  FTFA_FPROT1_PROT_MASK       0x8u
#define  FTFA_FPROT1_PROT_SHIFT      0
#define  FTFA_FPROT1_PROT(x)         (((uint8_t)(((uint8_t)(x))<<FTFA_FPROT1_PROT_SHIFT))&FTFA_FPROT1_PROT_MASK)
//   Program Flash Region Protect
//     0b0 ==> Program flash region is protected.
//     0b1 ==> Program flash region is not protected

#define  FTFA_FPROT0_PROT_MASK       0x8u
#define  FTFA_FPROT0_PROT_SHIFT      0
#define  FTFA_FPROT0_PROT(x)         (((uint8_t)(((uint8_t)(x))<<FTFA_FPROT0_PROT_SHIFT))&FTFA_FPROT0_PROT_MASK)
//   Program Flash Region Protect
//     0b0 ==> Program flash region is protected.
//     0b1 ==> Program flash region is not protected

#define  GPIO_PDOR_PDO_MASK          0x20u
#define  GPIO_PDOR_PDO_SHIFT         0
#define  GPIO_PDOR_PDO(x)            (((uint32_t)(((uint32_t)(x))<<GPIO_PDOR_PDO_SHIFT))&GPIO_PDOR_PDO_MASK)
//   Port Data Output
//     0b0 ==> Logic level 0 is driven on pin, provided pin is configured for general-purpose output.
//     0b1 ==> Logic level 1 is driven on pin, provided pin is configured for general-purpose output.

#define  GPIO_PSOR_PTSO_MASK         0x20u
#define  GPIO_PSOR_PTSO_SHIFT        0
#define  GPIO_PSOR_PTSO(x)           (((uint32_t)(((uint32_t)(x))<<GPIO_PSOR_PTSO_SHIFT))&GPIO_PSOR_PTSO_MASK)
//   Port Set Output
//     0b0 ==> Corresponding bit in PDORn does not change.
//     0b1 ==> Corresponding bit in PDORn is set to logic 1.

#define  GPIO_PCOR_PTCO_MASK         0x20u
#define  GPIO_PCOR_PTCO_SHIFT        0
#define  GPIO_PCOR_PTCO(x)           (((uint32_t)(((uint32_t)(x))<<GPIO_PCOR_PTCO_SHIFT))&GPIO_PCOR_PTCO_MASK)
//   Port Clear Output
//     0b0 ==> Corresponding bit in PDORn does not change.
//     0b1 ==> Corresponding bit in PDORn is cleared to logic 0.

#define  GPIO_PTOR_PTTO_MASK         0x20u
#define  GPIO_PTOR_PTTO_SHIFT        0
#define  GPIO_PTOR_PTTO(x)           (((uint32_t)(((uint32_t)(x))<<GPIO_PTOR_PTTO_SHIFT))&GPIO_PTOR_PTTO_MASK)
//   Port Toggle Output
//     0b0 ==> Corresponding bit in PDORn does not change.
//     0b1 ==> Corresponding bit in PDORn is set to the inverse of its existing logic state.

#define  GPIO_PDIR_PDI_MASK          0x20u
#define  GPIO_PDIR_PDI_SHIFT         0
#define  GPIO_PDIR_PDI(x)            (((uint32_t)(((uint32_t)(x))<<GPIO_PDIR_PDI_SHIFT))&GPIO_PDIR_PDI_MASK)
//   Port Data Input
//     0b0 ==> Pin logic level is logic 0, or is not configured for use by digital function.
//     0b1 ==> Pin logic level is logic 1.

#define  I2C0_A1_AD_MASK             0xEu
#define  I2C0_A1_AD_SHIFT            1
#define  I2C0_A1_AD(x)               (((uint8_t)(((uint8_t)(x))<<I2C0_A1_AD_SHIFT))&I2C0_A1_AD_MASK)
//   Address

#define  I2C0_F_ICR_MASK             0x6u
#define  I2C0_F_ICR_SHIFT            0
#define  I2C0_F_ICR(x)               (((uint8_t)(((uint8_t)(x))<<I2C0_F_ICR_SHIFT))&I2C0_F_ICR_MASK)
//   ClockRate

#define  I2C0_F_MULT_MASK            0x80u
#define  I2C0_F_MULT_SHIFT           6
#define  I2C0_F_MULT(x)              (((uint8_t)(((uint8_t)(x))<<I2C0_F_MULT_SHIFT))&I2C0_F_MULT_MASK)
//     0b00 ==> mul = 1
//     0b01 ==> mul = 2
//     0b10 ==> mul = 4
//     0b11 ==> Reserved

#define  I2C0_C1_DMAEN_MASK          0x1u
#define  I2C0_C1_DMAEN_SHIFT         0
//   DMA Enable
//     0b0 ==> All DMA signalling disabled.
//     0b1 ==> DMA transfer is enabled and the following conditions trigger the DMA request: While FACK = 0, a data byte is received, either address or data is transmitted. (ACK/NACK automatic) While FACK = 0, the first byte received matches the A1 register or is general call address. If any address matching occurs, IAAS and TCF are set. If the direction of transfer is known from master to slave, then it is not required to check the SRW. With this assumption, DMA can also be used in this
//             case. In other cases, if the master reads data from the slave, then it is required to rewrite the C1 register operation. With this assumption, DMA cannot be used. When FACK = 1, an address or a data byte is transmitted.

#define  I2C0_C1_WUEN_MASK           0x2u
#define  I2C0_C1_WUEN_SHIFT          1
//   Wakeup Enable
//     0b0 ==> Normal operation. No interrupt generated when address matching in low power mode.
//     0b1 ==> Enables the wakeup function in low power mode.

#define  I2C0_C1_RSTA_MASK           0x4u
#define  I2C0_C1_RSTA_SHIFT          2
//   Repeat START

#define  I2C0_C1_TXAK_MASK           0x8u
#define  I2C0_C1_TXAK_SHIFT          3
//   Transmit Acknowledge Enable
//     0b0 ==> An acknowledge signal is sent to the bus on the following receiving byte (if FACK is cleared) or the current receiving byte (if FACK is set).
//     0b1 ==> No acknowledge signal is sent to the bus on the following receiving data byte (if FACK is cleared) or the current receiving data byte (if FACK is set).

#define  I2C0_C1_TX_MASK             0x10u
#define  I2C0_C1_TX_SHIFT            4
//   Transmit Mode Select
//     0b0 ==> Receive
//     0b1 ==> Transmit

#define  I2C0_C1_MST_MASK            0x20u
#define  I2C0_C1_MST_SHIFT           5
//   Master Mode Select
//     0b0 ==> Slave mode
//     0b1 ==> Master mode

#define  I2C0_C1_IICIE_MASK          0x40u
#define  I2C0_C1_IICIE_SHIFT         6
//   I2C Interrupt Enable
//     0b0 ==> Disabled
//     0b1 ==> Enabled

#define  I2C0_C1_IICEN_MASK          0x80u
#define  I2C0_C1_IICEN_SHIFT         7
//   I2C Enable
//     0b0 ==> Disabled
//     0b1 ==> Enabled

#define  I2C0_S_RXAK_MASK            0x1u
#define  I2C0_S_RXAK_SHIFT           0
//   Receive Acknowledge
//     0b0 ==> Acknowledge signal was received after the completion of one byte of data transmission on the bus
//     0b1 ==> No acknowledge signal detected

#define  I2C0_S_IICIF_MASK           0x2u
#define  I2C0_S_IICIF_SHIFT          1
//   Interrupt Flag
//     0b0 ==> No interrupt pending
//     0b1 ==> Interrupt pending

#define  I2C0_S_SRW_MASK             0x4u
#define  I2C0_S_SRW_SHIFT            2
//   Slave Read/Write
//     0b0 ==> Slave receive, master writing to slave
//     0b1 ==> Slave transmit, master reading from slave

#define  I2C0_S_RAM_MASK             0x8u
#define  I2C0_S_RAM_SHIFT            3
//   Range Address Match
//     0b0 ==> Not addressed
//     0b1 ==> Addressed as a slave

#define  I2C0_S_ARBL_MASK            0x10u
#define  I2C0_S_ARBL_SHIFT           4
//   Arbitration Lost
//     0b0 ==> Standard bus operation.
//     0b1 ==> Loss of arbitration.

#define  I2C0_S_BUSY_MASK            0x20u
#define  I2C0_S_BUSY_SHIFT           5
//   Bus Busy
//     0b0 ==> Bus is idle
//     0b1 ==> Bus is busy

#define  I2C0_S_IAAS_MASK            0x40u
#define  I2C0_S_IAAS_SHIFT           6
//   Addressed As A Slave
//     0b0 ==> Not addressed
//     0b1 ==> Addressed as a slave

#define  I2C0_S_TCF_MASK             0x80u
#define  I2C0_S_TCF_SHIFT            7
//   Transfer Complete Flag
//     0b0 ==> Transfer in progress
//     0b1 ==> Transfer complete

#define  I2C0_D_DATA_MASK            0x8u
#define  I2C0_D_DATA_SHIFT           0
#define  I2C0_D_DATA(x)              (((uint8_t)(((uint8_t)(x))<<I2C0_D_DATA_SHIFT))&I2C0_D_DATA_MASK)
//   Data

#define  I2C0_C2_AD_MASK             0x3u
#define  I2C0_C2_AD_SHIFT            0
#define  I2C0_C2_AD(x)               (((uint8_t)(((uint8_t)(x))<<I2C0_C2_AD_SHIFT))&I2C0_C2_AD_MASK)
//   Slave Address

#define  I2C0_C2_RMEN_MASK           0x8u
#define  I2C0_C2_RMEN_SHIFT          3
//   Range Address Matching Enable
//     0b0 ==> Range mode disabled. No address match occurs for an address within the range of values of the A1 and RA registers.
//     0b1 ==> Range mode enabled. Address matching occurs when a slave receives an address within the range of values of the A1 and RA registers.

#define  I2C0_C2_SBRC_MASK           0x10u
#define  I2C0_C2_SBRC_SHIFT          4
//   Slave Baud Rate Control
//     0b0 ==> The slave baud rate follows the master baud rate and clock stretching may occur
//     0b1 ==> Slave baud rate is independent of the master baud rate

#define  I2C0_C2_HDRS_MASK           0x20u
#define  I2C0_C2_HDRS_SHIFT          5
//   High Drive Select
//     0b0 ==> Normal drive mode
//     0b1 ==> High drive mode

#define  I2C0_C2_ADEXT_MASK          0x40u
#define  I2C0_C2_ADEXT_SHIFT         6
//   Address Extension
//     0b0 ==> 7-bit address scheme
//     0b1 ==> 10-bit address scheme

#define  I2C0_C2_GCAEN_MASK          0x80u
#define  I2C0_C2_GCAEN_SHIFT         7
//   General Call Address Enable
//     0b0 ==> Disabled
//     0b1 ==> Enabled

#define  I2C0_FLT_FLT_MASK           0x5u
#define  I2C0_FLT_FLT_SHIFT          0
#define  I2C0_FLT_FLT(x)             (((uint8_t)(((uint8_t)(x))<<I2C0_FLT_FLT_SHIFT))&I2C0_FLT_FLT_MASK)
//   I2C Programmable Filter Factor
//     0b0 ==> No filter/bypass

#define  I2C0_RA_RAD_MASK            0xEu
#define  I2C0_RA_RAD_SHIFT           1
#define  I2C0_RA_RAD(x)              (((uint8_t)(((uint8_t)(x))<<I2C0_RA_RAD_SHIFT))&I2C0_RA_RAD_MASK)
//   Range Slave Address

#define  I2C0_SMB_SHTF2IE_MASK       0x1u
#define  I2C0_SMB_SHTF2IE_SHIFT      0
//   SHTF2 Interrupt Enable
//     0b0 ==> SHTF2 interrupt is disabled
//     0b1 ==> SHTF2 interrupt is enabled

#define  I2C0_SMB_SHTF2_MASK         0x2u
#define  I2C0_SMB_SHTF2_SHIFT        1
//   SCL High Timeout Flag 2
//     0b0 ==> No SCL high and SDA low timeout occurs
//     0b1 ==> SCL high and SDA low timeout occurs

#define  I2C0_SMB_SHTF1_MASK         0x4u
#define  I2C0_SMB_SHTF1_SHIFT        2
//   SCL High Timeout Flag 1
//     0b0 ==> No SCL high and SDA high timeout occurs
//     0b1 ==> SCL high and SDA high timeout occurs

#define  I2C0_SMB_SLTF_MASK          0x8u
#define  I2C0_SMB_SLTF_SHIFT         3
//   SCL Low Timeout Flag
//     0b0 ==> No low timeout occurs
//     0b1 ==> Low timeout occurs

#define  I2C0_SMB_TCKSEL_MASK        0x10u
#define  I2C0_SMB_TCKSEL_SHIFT       4
//   Timeout Counter Clock Select
//     0b0 ==> Timeout counter counts at the frequency of the bus clock / 64
//     0b1 ==> Timeout counter counts at the frequency of the bus clock

#define  I2C0_SMB_SIICAEN_MASK       0x20u
#define  I2C0_SMB_SIICAEN_SHIFT      5
//   Second I2C Address Enable
//     0b0 ==> I2C address register 2 matching is disabled
//     0b1 ==> I2C address register 2 matching is enabled

#define  I2C0_SMB_ALERTEN_MASK       0x40u
#define  I2C0_SMB_ALERTEN_SHIFT      6
//   SMBus Alert Response Address Enable
//     0b0 ==> SMBus alert response address matching is disabled
//     0b1 ==> SMBus alert response address matching is enabled

#define  I2C0_SMB_FACK_MASK          0x80u
#define  I2C0_SMB_FACK_SHIFT         7
//   Fast NACK/ACK Enable
//     0b0 ==> An ACK or NACK is sent on the following receiving data byte
//     0b1 ==> Writing 0 to TXAK after receiving a data byte generates an ACK. Writing 1 to TXAK after receiving a data byte generates a NACK.

#define  I2C0_A2_SAD_MASK            0xEu
#define  I2C0_A2_SAD_SHIFT           1
#define  I2C0_A2_SAD(x)              (((uint8_t)(((uint8_t)(x))<<I2C0_A2_SAD_SHIFT))&I2C0_A2_SAD_MASK)
//   SMBus Address

#define  I2C0_SLTH_SSLT_MASK         0x8u
#define  I2C0_SLTH_SSLT_SHIFT        0
#define  I2C0_SLTH_SSLT(x)           (((uint8_t)(((uint8_t)(x))<<I2C0_SLTH_SSLT_SHIFT))&I2C0_SLTH_SSLT_MASK)

#define  I2C0_SLTL_SSLT_MASK         0x8u
#define  I2C0_SLTL_SSLT_SHIFT        0
#define  I2C0_SLTL_SSLT(x)           (((uint8_t)(((uint8_t)(x))<<I2C0_SLTL_SSLT_SHIFT))&I2C0_SLTL_SSLT_MASK)

#define  I2C1_A1_AD_MASK             0xEu
#define  I2C1_A1_AD_SHIFT            1
#define  I2C1_A1_AD(x)               (((uint8_t)(((uint8_t)(x))<<I2C1_A1_AD_SHIFT))&I2C1_A1_AD_MASK)
//   Address

#define  I2C1_F_ICR_MASK             0x6u
#define  I2C1_F_ICR_SHIFT            0
#define  I2C1_F_ICR(x)               (((uint8_t)(((uint8_t)(x))<<I2C1_F_ICR_SHIFT))&I2C1_F_ICR_MASK)
//   ClockRate

#define  I2C1_F_MULT_MASK            0x80u
#define  I2C1_F_MULT_SHIFT           6
#define  I2C1_F_MULT(x)              (((uint8_t)(((uint8_t)(x))<<I2C1_F_MULT_SHIFT))&I2C1_F_MULT_MASK)
//     0b00 ==> mul = 1
//     0b01 ==> mul = 2
//     0b10 ==> mul = 4
//     0b11 ==> Reserved

#define  I2C1_C1_DMAEN_MASK          0x1u
#define  I2C1_C1_DMAEN_SHIFT         0
//   DMA Enable
//     0b0 ==> All DMA signalling disabled.
//     0b1 ==> DMA transfer is enabled and the following conditions trigger the DMA request: While FACK = 0, a data byte is received, either address or data is transmitted. (ACK/NACK automatic) While FACK = 0, the first byte received matches the A1 register or is general call address. If any address matching occurs, IAAS and TCF are set. If the direction of transfer is known from master to slave, then it is not required to check the SRW. With this assumption, DMA can also be used in this
//             case. In other cases, if the master reads data from the slave, then it is required to rewrite the C1 register operation. With this assumption, DMA cannot be used. When FACK = 1, an address or a data byte is transmitted.

#define  I2C1_C1_WUEN_MASK           0x2u
#define  I2C1_C1_WUEN_SHIFT          1
//   Wakeup Enable
//     0b0 ==> Normal operation. No interrupt generated when address matching in low power mode.
//     0b1 ==> Enables the wakeup function in low power mode.

#define  I2C1_C1_RSTA_MASK           0x4u
#define  I2C1_C1_RSTA_SHIFT          2
//   Repeat START

#define  I2C1_C1_TXAK_MASK           0x8u
#define  I2C1_C1_TXAK_SHIFT          3
//   Transmit Acknowledge Enable
//     0b0 ==> An acknowledge signal is sent to the bus on the following receiving byte (if FACK is cleared) or the current receiving byte (if FACK is set).
//     0b1 ==> No acknowledge signal is sent to the bus on the following receiving data byte (if FACK is cleared) or the current receiving data byte (if FACK is set).

#define  I2C1_C1_TX_MASK             0x10u
#define  I2C1_C1_TX_SHIFT            4
//   Transmit Mode Select
//     0b0 ==> Receive
//     0b1 ==> Transmit

#define  I2C1_C1_MST_MASK            0x20u
#define  I2C1_C1_MST_SHIFT           5
//   Master Mode Select
//     0b0 ==> Slave mode
//     0b1 ==> Master mode

#define  I2C1_C1_IICIE_MASK          0x40u
#define  I2C1_C1_IICIE_SHIFT         6
//   I2C Interrupt Enable
//     0b0 ==> Disabled
//     0b1 ==> Enabled

#define  I2C1_C1_IICEN_MASK          0x80u
#define  I2C1_C1_IICEN_SHIFT         7
//   I2C Enable
//     0b0 ==> Disabled
//     0b1 ==> Enabled

#define  I2C1_S_RXAK_MASK            0x1u
#define  I2C1_S_RXAK_SHIFT           0
//   Receive Acknowledge
//     0b0 ==> Acknowledge signal was received after the completion of one byte of data transmission on the bus
//     0b1 ==> No acknowledge signal detected

#define  I2C1_S_IICIF_MASK           0x2u
#define  I2C1_S_IICIF_SHIFT          1
//   Interrupt Flag
//     0b0 ==> No interrupt pending
//     0b1 ==> Interrupt pending

#define  I2C1_S_SRW_MASK             0x4u
#define  I2C1_S_SRW_SHIFT            2
//   Slave Read/Write
//     0b0 ==> Slave receive, master writing to slave
//     0b1 ==> Slave transmit, master reading from slave

#define  I2C1_S_RAM_MASK             0x8u
#define  I2C1_S_RAM_SHIFT            3
//   Range Address Match
//     0b0 ==> Not addressed
//     0b1 ==> Addressed as a slave

#define  I2C1_S_ARBL_MASK            0x10u
#define  I2C1_S_ARBL_SHIFT           4
//   Arbitration Lost
//     0b0 ==> Standard bus operation.
//     0b1 ==> Loss of arbitration.

#define  I2C1_S_BUSY_MASK            0x20u
#define  I2C1_S_BUSY_SHIFT           5
//   Bus Busy
//     0b0 ==> Bus is idle
//     0b1 ==> Bus is busy

#define  I2C1_S_IAAS_MASK            0x40u
#define  I2C1_S_IAAS_SHIFT           6
//   Addressed As A Slave
//     0b0 ==> Not addressed
//     0b1 ==> Addressed as a slave

#define  I2C1_S_TCF_MASK             0x80u
#define  I2C1_S_TCF_SHIFT            7
//   Transfer Complete Flag
//     0b0 ==> Transfer in progress
//     0b1 ==> Transfer complete

#define  I2C1_D_DATA_MASK            0x8u
#define  I2C1_D_DATA_SHIFT           0
#define  I2C1_D_DATA(x)              (((uint8_t)(((uint8_t)(x))<<I2C1_D_DATA_SHIFT))&I2C1_D_DATA_MASK)
//   Data

#define  I2C1_C2_AD_MASK             0x3u
#define  I2C1_C2_AD_SHIFT            0
#define  I2C1_C2_AD(x)               (((uint8_t)(((uint8_t)(x))<<I2C1_C2_AD_SHIFT))&I2C1_C2_AD_MASK)
//   Slave Address

#define  I2C1_C2_RMEN_MASK           0x8u
#define  I2C1_C2_RMEN_SHIFT          3
//   Range Address Matching Enable
//     0b0 ==> Range mode disabled. No address match occurs for an address within the range of values of the A1 and RA registers.
//     0b1 ==> Range mode enabled. Address matching occurs when a slave receives an address within the range of values of the A1 and RA registers.

#define  I2C1_C2_SBRC_MASK           0x10u
#define  I2C1_C2_SBRC_SHIFT          4
//   Slave Baud Rate Control
//     0b0 ==> The slave baud rate follows the master baud rate and clock stretching may occur
//     0b1 ==> Slave baud rate is independent of the master baud rate

#define  I2C1_C2_HDRS_MASK           0x20u
#define  I2C1_C2_HDRS_SHIFT          5
//   High Drive Select
//     0b0 ==> Normal drive mode
//     0b1 ==> High drive mode

#define  I2C1_C2_ADEXT_MASK          0x40u
#define  I2C1_C2_ADEXT_SHIFT         6
//   Address Extension
//     0b0 ==> 7-bit address scheme
//     0b1 ==> 10-bit address scheme

#define  I2C1_C2_GCAEN_MASK          0x80u
#define  I2C1_C2_GCAEN_SHIFT         7
//   General Call Address Enable
//     0b0 ==> Disabled
//     0b1 ==> Enabled

#define  I2C1_FLT_FLT_MASK           0x5u
#define  I2C1_FLT_FLT_SHIFT          0
#define  I2C1_FLT_FLT(x)             (((uint8_t)(((uint8_t)(x))<<I2C1_FLT_FLT_SHIFT))&I2C1_FLT_FLT_MASK)
//   I2C Programmable Filter Factor
//     0b0 ==> No filter/bypass

#define  I2C1_RA_RAD_MASK            0xEu
#define  I2C1_RA_RAD_SHIFT           1
#define  I2C1_RA_RAD(x)              (((uint8_t)(((uint8_t)(x))<<I2C1_RA_RAD_SHIFT))&I2C1_RA_RAD_MASK)
//   Range Slave Address

#define  I2C1_SMB_SHTF2IE_MASK       0x1u
#define  I2C1_SMB_SHTF2IE_SHIFT      0
//   SHTF2 Interrupt Enable
//     0b0 ==> SHTF2 interrupt is disabled
//     0b1 ==> SHTF2 interrupt is enabled

#define  I2C1_SMB_SHTF2_MASK         0x2u
#define  I2C1_SMB_SHTF2_SHIFT        1
//   SCL High Timeout Flag 2
//     0b0 ==> No SCL high and SDA low timeout occurs
//     0b1 ==> SCL high and SDA low timeout occurs

#define  I2C1_SMB_SHTF1_MASK         0x4u
#define  I2C1_SMB_SHTF1_SHIFT        2
//   SCL High Timeout Flag 1
//     0b0 ==> No SCL high and SDA high timeout occurs
//     0b1 ==> SCL high and SDA high timeout occurs

#define  I2C1_SMB_SLTF_MASK          0x8u
#define  I2C1_SMB_SLTF_SHIFT         3
//   SCL Low Timeout Flag
//     0b0 ==> No low timeout occurs
//     0b1 ==> Low timeout occurs

#define  I2C1_SMB_TCKSEL_MASK        0x10u
#define  I2C1_SMB_TCKSEL_SHIFT       4
//   Timeout Counter Clock Select
//     0b0 ==> Timeout counter counts at the frequency of the bus clock / 64
//     0b1 ==> Timeout counter counts at the frequency of the bus clock

#define  I2C1_SMB_SIICAEN_MASK       0x20u
#define  I2C1_SMB_SIICAEN_SHIFT      5
//   Second I2C Address Enable
//     0b0 ==> I2C address register 2 matching is disabled
//     0b1 ==> I2C address register 2 matching is enabled

#define  I2C1_SMB_ALERTEN_MASK       0x40u
#define  I2C1_SMB_ALERTEN_SHIFT      6
//   SMBus Alert Response Address Enable
//     0b0 ==> SMBus alert response address matching is disabled
//     0b1 ==> SMBus alert response address matching is enabled

#define  I2C1_SMB_FACK_MASK          0x80u
#define  I2C1_SMB_FACK_SHIFT         7
//   Fast NACK/ACK Enable
//     0b0 ==> An ACK or NACK is sent on the following receiving data byte
//     0b1 ==> Writing 0 to TXAK after receiving a data byte generates an ACK. Writing 1 to TXAK after receiving a data byte generates a NACK.

#define  I2C1_A2_SAD_MASK            0xEu
#define  I2C1_A2_SAD_SHIFT           1
#define  I2C1_A2_SAD(x)              (((uint8_t)(((uint8_t)(x))<<I2C1_A2_SAD_SHIFT))&I2C1_A2_SAD_MASK)
//   SMBus Address

#define  I2C1_SLTH_SSLT_MASK         0x8u
#define  I2C1_SLTH_SSLT_SHIFT        0
#define  I2C1_SLTH_SSLT(x)           (((uint8_t)(((uint8_t)(x))<<I2C1_SLTH_SSLT_SHIFT))&I2C1_SLTH_SSLT_MASK)

#define  I2C1_SLTL_SSLT_MASK         0x8u
#define  I2C1_SLTL_SSLT_SHIFT        0
#define  I2C1_SLTL_SSLT(x)           (((uint8_t)(((uint8_t)(x))<<I2C1_SLTL_SSLT_SHIFT))&I2C1_SLTL_SSLT_MASK)

#define  LCD_GCR_DUTY_MASK           0x3u
#define  LCD_GCR_DUTY_SHIFT          0
#define  LCD_GCR_DUTY(x)             (((uint32_t)(((uint32_t)(x))<<LCD_GCR_DUTY_SHIFT))&LCD_GCR_DUTY_MASK)
//   LCD duty select
//     0b000 ==> Use 1 BP (1/1 duty cycle).
//     0b001 ==> Use 2 BP (1/2 duty cycle).
//     0b010 ==> Use 3 BP (1/3 duty cycle).
//     0b011 ==> Use 4 BP (1/4 duty cycle). (Default)
//     0b100 ==> Use 5 BP (1/5 duty cycle).
//     0b101 ==> Use 6 BP (1/6 duty cycle).
//     0b110 ==> Use 7 BP (1/7 duty cycle).
//     0b111 ==> Use 8 BP (1/8 duty cycle).

#define  LCD_GCR_LCLK_MASK           0x18u
#define  LCD_GCR_LCLK_SHIFT          3
#define  LCD_GCR_LCLK(x)             (((uint32_t)(((uint32_t)(x))<<LCD_GCR_LCLK_SHIFT))&LCD_GCR_LCLK_MASK)
//   LCD Clock Prescaler

#define  LCD_GCR_SOURCE_MASK         0x40u
#define  LCD_GCR_SOURCE_SHIFT        6
//   LCD Clock Source Select
//     0b0 ==> Selects the default clock as the LCD clock source.
//     0b1 ==> Selects the alternate clock as the LCD clock source.

#define  LCD_GCR_LCDEN_MASK          0x80u
#define  LCD_GCR_LCDEN_SHIFT         7
//   LCD Driver Enable
//     0b0 ==> All front plane and back plane pins are disabled. The LCD controller system is also disabled, and all LCD waveform generation clocks are stopped. V LL3 is connected to V DD internally.
//     0b1 ==> LCD controller driver system is enabled, and front plane and back plane waveforms are generated. All LCD pins, LCD_Pn, enabled using the LCD Pin Enable register, output an LCD driver waveform. The back plane pins output an LCD driver back plane waveform based on the settings of DUTY[2:0]. Charge pump or resistor bias is enabled.

#define  LCD_GCR_LCDSTP_MASK         0x100u
#define  LCD_GCR_LCDSTP_SHIFT        8
//   LCD Stop
//     0b0 ==> Allows the LCD driver, charge pump, resistor bias network, and voltage regulator to continue running during Stop mode.
//     0b1 ==> Disables the LCD driver, charge pump, resistor bias network, and voltage regulator when MCU enters Stop mode.

#define  LCD_GCR_LCDWAIT_MASK        0x200u
#define  LCD_GCR_LCDWAIT_SHIFT       9
//   LCD Wait
//     0b0 ==> Allows the LCD driver, charge pump, resistor bias network, and voltage regulator to continue running during Wait mode.
//     0b1 ==> Disables the LCD driver, charge pump, resistor bias network, and voltage regulator when MCU enters Wait mode.

#define  LCD_GCR_ALTDIV_MASK         0x2000u
#define  LCD_GCR_ALTDIV_SHIFT        12
#define  LCD_GCR_ALTDIV(x)           (((uint32_t)(((uint32_t)(x))<<LCD_GCR_ALTDIV_SHIFT))&LCD_GCR_ALTDIV_MASK)
//   LCD AlternateClock Divider
//     0b0 ==> Divide factor = 1 (No divide)
//     0b1 ==> Divide factor = 8

#define  LCD_GCR_FDCIEN_MASK         0x4000u
#define  LCD_GCR_FDCIEN_SHIFT        14
//   LCD Fault Detection Complete Interrupt Enable
//     0b0 ==> No interrupt request is generated by this event.
//     0b1 ==> When a fault is detected and FDCF bit is set, this event causes an interrupt request.

#define  LCD_GCR_LCDIEN_MASK         0x8000u
#define  LCD_GCR_LCDIEN_SHIFT        15
//   LCD Frame Frequency Interrupt Enable
//     0b0 ==> No interrupt request is generated by this event.
//     0b1 ==> When LCDIF bit is set, this event causes an interrupt request.

#define  LCD_GCR_VSUPPLY_MASK        0x20000u
#define  LCD_GCR_VSUPPLY_SHIFT       16
#define  LCD_GCR_VSUPPLY(x)          (((uint32_t)(((uint32_t)(x))<<LCD_GCR_VSUPPLY_SHIFT))&LCD_GCR_VSUPPLY_MASK)
//   Voltage Supply Control
//     0b00 ==> Drive V LL2 internally from V DD .
//     0b01 ==> Drive V LL3 internally from V DD .
//     0b10 ==> Reserved
//     0b11 ==> Drive V LL3 externally from V DD or drive V LL1 internally from V IREG .

#define  LCD_GCR_LADJ_MASK           0x200000u
#define  LCD_GCR_LADJ_SHIFT          20
#define  LCD_GCR_LADJ(x)             (((uint32_t)(((uint32_t)(x))<<LCD_GCR_LADJ_SHIFT))&LCD_GCR_LADJ_MASK)
//   Load Adjust

#define  LCD_GCR_HREFSEL_MASK        0x400000u
#define  LCD_GCR_HREFSEL_SHIFT       22
//   High Reference Select
//     0b0 ==> Divide input, V IREG = 1.0 V for 3 V glass.
//     0b1 ==> Do not divide the input, V IREG = 1.67 V for 5 V glass.

#define  LCD_GCR_CPSEL_MASK          0x800000u
#define  LCD_GCR_CPSEL_SHIFT         23
//   Charge Pump or Resistor Bias Select
//     0b0 ==> LCD charge pump is disabled. Resistor network selected. (The internal 1/3-bias is forced.)
//     0b1 ==> LCD charge pump is selected. Resistor network disabled. (The internal 1/3-bias is forced.)

#define  LCD_GCR_RVTRIM_MASK         0x4000000u
#define  LCD_GCR_RVTRIM_SHIFT        24
#define  LCD_GCR_RVTRIM(x)           (((uint32_t)(((uint32_t)(x))<<LCD_GCR_RVTRIM_SHIFT))&LCD_GCR_RVTRIM_MASK)
//   Regulated Voltage Trim

#define  LCD_GCR_RVEN_MASK           0x80000000u
#define  LCD_GCR_RVEN_SHIFT          31
//   Regulated Voltage Enable
//     0b0 ==> Regulated voltage disabled.
//     0b1 ==> Regulated voltage enabled.

#define  LCD_AR_BRATE_MASK           0x3u
#define  LCD_AR_BRATE_SHIFT          0
#define  LCD_AR_BRATE(x)             (((uint32_t)(((uint32_t)(x))<<LCD_AR_BRATE_SHIFT))&LCD_AR_BRATE_MASK)
//   Blink-rate configuration

#define  LCD_AR_BMODE_MASK           0x8u
#define  LCD_AR_BMODE_SHIFT          3
//   Blink mode
//     0b0 ==> Display blank during the blink period.
//     0b1 ==> Display alternate display during blink period (Ignored if duty is 5 or greater).

#define  LCD_AR_BLANK_MASK           0x20u
#define  LCD_AR_BLANK_SHIFT          5
//   Blank display mode
//     0b0 ==> Normal or alternate display mode.
//     0b1 ==> Blank display mode.

#define  LCD_AR_ALT_MASK             0x40u
#define  LCD_AR_ALT_SHIFT            6
//   Alternate display mode
//     0b0 ==> Normal display mode.
//     0b1 ==> Alternate display mode.

#define  LCD_AR_BLINK_MASK           0x80u
#define  LCD_AR_BLINK_SHIFT          7
//   Blink command
//     0b0 ==> Disables blinking.
//     0b1 ==> Starts blinking at blinking frequency specified by LCD blink rate calculation.

#define  LCD_AR_LCDIF_MASK           0x8000u
#define  LCD_AR_LCDIF_SHIFT          15
//   LCD Frame Frequency Interrupt flag
//     0b0 ==> Frame frequency interrupt condition has not occurred.
//     0b1 ==> Start of SLCD frame has occurred.

#define  LCD_FDCR_FDPINID_MASK       0x6u
#define  LCD_FDCR_FDPINID_SHIFT      0
#define  LCD_FDCR_FDPINID(x)         (((uint32_t)(((uint32_t)(x))<<LCD_FDCR_FDPINID_SHIFT))&LCD_FDCR_FDPINID_MASK)
//   Fault Detect Pin ID
//     0b0 ==> Fault detection for LCD_P0 pin.
//     0b1 ==> Fault detection for LCD_P1 pin.

#define  LCD_FDCR_FDBPEN_MASK        0x40u
#define  LCD_FDCR_FDBPEN_SHIFT       6
//   Fault Detect Back Plane Enable
//     0b0 ==> Type of the selected pin under fault detect test is front plane.
//     0b1 ==> Type of the selected pin under fault detect test is back plane.

#define  LCD_FDCR_FDEN_MASK          0x80u
#define  LCD_FDCR_FDEN_SHIFT         7
//   Fault Detect Enable
//     0b0 ==> Disable fault detection.
//     0b1 ==> Enable fault detection.

#define  LCD_FDCR_FDSWW_MASK         0x600u
#define  LCD_FDCR_FDSWW_SHIFT        9
#define  LCD_FDCR_FDSWW(x)           (((uint32_t)(((uint32_t)(x))<<LCD_FDCR_FDSWW_SHIFT))&LCD_FDCR_FDSWW_MASK)
//   Fault Detect Sample Window Width
//     0b0 ==> Sample window width is 4 sample clock cycles.
//     0b1 ==> Sample window width is 8 sample clock cycles.

#define  LCD_FDCR_FDPRS_MASK         0x3000u
#define  LCD_FDCR_FDPRS_SHIFT        12
#define  LCD_FDCR_FDPRS(x)           (((uint32_t)(((uint32_t)(x))<<LCD_FDCR_FDPRS_SHIFT))&LCD_FDCR_FDPRS_MASK)
//   Fault Detect Clock Prescaler
//     0b0 ==> 1/1 bus clock.
//     0b1 ==> 1/2 bus clock.

#define  LCD_FDSR_FDCNT_MASK         0x8u
#define  LCD_FDSR_FDCNT_SHIFT        0
#define  LCD_FDSR_FDCNT(x)           (((uint32_t)(((uint32_t)(x))<<LCD_FDSR_FDCNT_SHIFT))&LCD_FDSR_FDCNT_MASK)
//   Fault Detect Counter
//     0b0 ==> No "one" samples.
//     0b1 ==> 1 "one" samples.

#define  LCD_FDSR_FDCF_MASK          0x8000u
#define  LCD_FDSR_FDCF_SHIFT         15
//   Fault Detection Complete Flag
//     0b0 ==> Fault detection is not completed.
//     0b1 ==> Fault detection is completed.

#define  LCD_PENL_PEN_MASK           0x20u
#define  LCD_PENL_PEN_SHIFT          0
#define  LCD_PENL_PEN(x)             (((uint32_t)(((uint32_t)(x))<<LCD_PENL_PEN_SHIFT))&LCD_PENL_PEN_MASK)
//   LCD Pin Enable
//     0b0 ==> LCD operation disabled on LCD_Pn.
//     0b1 ==> LCD operation enabled on LCD_Pn.

#define  LCD_PENH_PEN_MASK           0x20u
#define  LCD_PENH_PEN_SHIFT          0
#define  LCD_PENH_PEN(x)             (((uint32_t)(((uint32_t)(x))<<LCD_PENH_PEN_SHIFT))&LCD_PENH_PEN_MASK)
//   LCD Pin Enable
//     0b0 ==> LCD operation disabled on LCD_Pn.
//     0b1 ==> LCD operation enabled on LCD_Pn.

#define  LCD_BPENL_BPEN_MASK         0x20u
#define  LCD_BPENL_BPEN_SHIFT        0
#define  LCD_BPENL_BPEN(x)           (((uint32_t)(((uint32_t)(x))<<LCD_BPENL_BPEN_SHIFT))&LCD_BPENL_BPEN_MASK)
//   Back Plane Enable
//     0b0 ==> Front plane operation enabled on LCD_Pn.
//     0b1 ==> Back plane operation enabled on LCD_Pn.

#define  LCD_BPENH_BPEN_MASK         0x20u
#define  LCD_BPENH_BPEN_SHIFT        0
#define  LCD_BPENH_BPEN(x)           (((uint32_t)(((uint32_t)(x))<<LCD_BPENH_BPEN_SHIFT))&LCD_BPENH_BPEN_MASK)
//   Back Plane Enable
//     0b0 ==> Front plane operation enabled on LCD_Pn.
//     0b1 ==> Back plane operation enabled on LCD_Pn.

#define  LCD_WF3TO0_WF0_MASK         0x8u
#define  LCD_WF3TO0_WF0_SHIFT        0
#define  LCD_WF3TO0_WF0(x)           (((uint32_t)(((uint32_t)(x))<<LCD_WF3TO0_WF0_SHIFT))&LCD_WF3TO0_WF0_MASK)

#define  LCD_WF3TO0_WF1_MASK         0x800u
#define  LCD_WF3TO0_WF1_SHIFT        8
#define  LCD_WF3TO0_WF1(x)           (((uint32_t)(((uint32_t)(x))<<LCD_WF3TO0_WF1_SHIFT))&LCD_WF3TO0_WF1_MASK)

#define  LCD_WF3TO0_WF2_MASK         0x80000u
#define  LCD_WF3TO0_WF2_SHIFT        16
#define  LCD_WF3TO0_WF2(x)           (((uint32_t)(((uint32_t)(x))<<LCD_WF3TO0_WF2_SHIFT))&LCD_WF3TO0_WF2_MASK)

#define  LCD_WF3TO0_WF3_MASK         0x8000000u
#define  LCD_WF3TO0_WF3_SHIFT        24
#define  LCD_WF3TO0_WF3(x)           (((uint32_t)(((uint32_t)(x))<<LCD_WF3TO0_WF3_SHIFT))&LCD_WF3TO0_WF3_MASK)

#define  LCD_WF7TO4_WF4_MASK         0x8u
#define  LCD_WF7TO4_WF4_SHIFT        0
#define  LCD_WF7TO4_WF4(x)           (((uint32_t)(((uint32_t)(x))<<LCD_WF7TO4_WF4_SHIFT))&LCD_WF7TO4_WF4_MASK)

#define  LCD_WF7TO4_WF5_MASK         0x800u
#define  LCD_WF7TO4_WF5_SHIFT        8
#define  LCD_WF7TO4_WF5(x)           (((uint32_t)(((uint32_t)(x))<<LCD_WF7TO4_WF5_SHIFT))&LCD_WF7TO4_WF5_MASK)

#define  LCD_WF7TO4_WF6_MASK         0x80000u
#define  LCD_WF7TO4_WF6_SHIFT        16
#define  LCD_WF7TO4_WF6(x)           (((uint32_t)(((uint32_t)(x))<<LCD_WF7TO4_WF6_SHIFT))&LCD_WF7TO4_WF6_MASK)

#define  LCD_WF7TO4_WF7_MASK         0x8000000u
#define  LCD_WF7TO4_WF7_SHIFT        24
#define  LCD_WF7TO4_WF7(x)           (((uint32_t)(((uint32_t)(x))<<LCD_WF7TO4_WF7_SHIFT))&LCD_WF7TO4_WF7_MASK)

#define  LCD_WF11TO8_WF8_MASK        0x8u
#define  LCD_WF11TO8_WF8_SHIFT       0
#define  LCD_WF11TO8_WF8(x)          (((uint32_t)(((uint32_t)(x))<<LCD_WF11TO8_WF8_SHIFT))&LCD_WF11TO8_WF8_MASK)

#define  LCD_WF11TO8_WF9_MASK        0x800u
#define  LCD_WF11TO8_WF9_SHIFT       8
#define  LCD_WF11TO8_WF9(x)          (((uint32_t)(((uint32_t)(x))<<LCD_WF11TO8_WF9_SHIFT))&LCD_WF11TO8_WF9_MASK)

#define  LCD_WF11TO8_WF10_MASK       0x80000u
#define  LCD_WF11TO8_WF10_SHIFT      16
#define  LCD_WF11TO8_WF10(x)         (((uint32_t)(((uint32_t)(x))<<LCD_WF11TO8_WF10_SHIFT))&LCD_WF11TO8_WF10_MASK)

#define  LCD_WF11TO8_WF11_MASK       0x8000000u
#define  LCD_WF11TO8_WF11_SHIFT      24
#define  LCD_WF11TO8_WF11(x)         (((uint32_t)(((uint32_t)(x))<<LCD_WF11TO8_WF11_SHIFT))&LCD_WF11TO8_WF11_MASK)

#define  LCD_WF15TO12_WF12_MASK      0x8u
#define  LCD_WF15TO12_WF12_SHIFT     0
#define  LCD_WF15TO12_WF12(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF15TO12_WF12_SHIFT))&LCD_WF15TO12_WF12_MASK)

#define  LCD_WF15TO12_WF13_MASK      0x800u
#define  LCD_WF15TO12_WF13_SHIFT     8
#define  LCD_WF15TO12_WF13(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF15TO12_WF13_SHIFT))&LCD_WF15TO12_WF13_MASK)

#define  LCD_WF15TO12_WF14_MASK      0x80000u
#define  LCD_WF15TO12_WF14_SHIFT     16
#define  LCD_WF15TO12_WF14(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF15TO12_WF14_SHIFT))&LCD_WF15TO12_WF14_MASK)

#define  LCD_WF15TO12_WF15_MASK      0x8000000u
#define  LCD_WF15TO12_WF15_SHIFT     24
#define  LCD_WF15TO12_WF15(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF15TO12_WF15_SHIFT))&LCD_WF15TO12_WF15_MASK)

#define  LCD_WF19TO16_WF16_MASK      0x8u
#define  LCD_WF19TO16_WF16_SHIFT     0
#define  LCD_WF19TO16_WF16(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF19TO16_WF16_SHIFT))&LCD_WF19TO16_WF16_MASK)

#define  LCD_WF19TO16_WF17_MASK      0x800u
#define  LCD_WF19TO16_WF17_SHIFT     8
#define  LCD_WF19TO16_WF17(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF19TO16_WF17_SHIFT))&LCD_WF19TO16_WF17_MASK)

#define  LCD_WF19TO16_WF18_MASK      0x80000u
#define  LCD_WF19TO16_WF18_SHIFT     16
#define  LCD_WF19TO16_WF18(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF19TO16_WF18_SHIFT))&LCD_WF19TO16_WF18_MASK)

#define  LCD_WF19TO16_WF19_MASK      0x8000000u
#define  LCD_WF19TO16_WF19_SHIFT     24
#define  LCD_WF19TO16_WF19(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF19TO16_WF19_SHIFT))&LCD_WF19TO16_WF19_MASK)

#define  LCD_WF23TO20_WF20_MASK      0x8u
#define  LCD_WF23TO20_WF20_SHIFT     0
#define  LCD_WF23TO20_WF20(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF23TO20_WF20_SHIFT))&LCD_WF23TO20_WF20_MASK)

#define  LCD_WF23TO20_WF21_MASK      0x800u
#define  LCD_WF23TO20_WF21_SHIFT     8
#define  LCD_WF23TO20_WF21(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF23TO20_WF21_SHIFT))&LCD_WF23TO20_WF21_MASK)

#define  LCD_WF23TO20_WF22_MASK      0x80000u
#define  LCD_WF23TO20_WF22_SHIFT     16
#define  LCD_WF23TO20_WF22(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF23TO20_WF22_SHIFT))&LCD_WF23TO20_WF22_MASK)

#define  LCD_WF23TO20_WF23_MASK      0x8000000u
#define  LCD_WF23TO20_WF23_SHIFT     24
#define  LCD_WF23TO20_WF23(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF23TO20_WF23_SHIFT))&LCD_WF23TO20_WF23_MASK)

#define  LCD_WF27TO24_WF24_MASK      0x8u
#define  LCD_WF27TO24_WF24_SHIFT     0
#define  LCD_WF27TO24_WF24(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF27TO24_WF24_SHIFT))&LCD_WF27TO24_WF24_MASK)

#define  LCD_WF27TO24_WF25_MASK      0x800u
#define  LCD_WF27TO24_WF25_SHIFT     8
#define  LCD_WF27TO24_WF25(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF27TO24_WF25_SHIFT))&LCD_WF27TO24_WF25_MASK)

#define  LCD_WF27TO24_WF26_MASK      0x80000u
#define  LCD_WF27TO24_WF26_SHIFT     16
#define  LCD_WF27TO24_WF26(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF27TO24_WF26_SHIFT))&LCD_WF27TO24_WF26_MASK)

#define  LCD_WF27TO24_WF27_MASK      0x8000000u
#define  LCD_WF27TO24_WF27_SHIFT     24
#define  LCD_WF27TO24_WF27(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF27TO24_WF27_SHIFT))&LCD_WF27TO24_WF27_MASK)

#define  LCD_WF31TO28_WF28_MASK      0x8u
#define  LCD_WF31TO28_WF28_SHIFT     0
#define  LCD_WF31TO28_WF28(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF31TO28_WF28_SHIFT))&LCD_WF31TO28_WF28_MASK)

#define  LCD_WF31TO28_WF29_MASK      0x800u
#define  LCD_WF31TO28_WF29_SHIFT     8
#define  LCD_WF31TO28_WF29(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF31TO28_WF29_SHIFT))&LCD_WF31TO28_WF29_MASK)

#define  LCD_WF31TO28_WF30_MASK      0x80000u
#define  LCD_WF31TO28_WF30_SHIFT     16
#define  LCD_WF31TO28_WF30(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF31TO28_WF30_SHIFT))&LCD_WF31TO28_WF30_MASK)

#define  LCD_WF31TO28_WF31_MASK      0x8000000u
#define  LCD_WF31TO28_WF31_SHIFT     24
#define  LCD_WF31TO28_WF31(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF31TO28_WF31_SHIFT))&LCD_WF31TO28_WF31_MASK)

#define  LCD_WF35TO32_WF32_MASK      0x8u
#define  LCD_WF35TO32_WF32_SHIFT     0
#define  LCD_WF35TO32_WF32(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF35TO32_WF32_SHIFT))&LCD_WF35TO32_WF32_MASK)

#define  LCD_WF35TO32_WF33_MASK      0x800u
#define  LCD_WF35TO32_WF33_SHIFT     8
#define  LCD_WF35TO32_WF33(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF35TO32_WF33_SHIFT))&LCD_WF35TO32_WF33_MASK)

#define  LCD_WF35TO32_WF34_MASK      0x80000u
#define  LCD_WF35TO32_WF34_SHIFT     16
#define  LCD_WF35TO32_WF34(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF35TO32_WF34_SHIFT))&LCD_WF35TO32_WF34_MASK)

#define  LCD_WF35TO32_WF35_MASK      0x8000000u
#define  LCD_WF35TO32_WF35_SHIFT     24
#define  LCD_WF35TO32_WF35(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF35TO32_WF35_SHIFT))&LCD_WF35TO32_WF35_MASK)

#define  LCD_WF39TO36_WF36_MASK      0x8u
#define  LCD_WF39TO36_WF36_SHIFT     0
#define  LCD_WF39TO36_WF36(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF39TO36_WF36_SHIFT))&LCD_WF39TO36_WF36_MASK)

#define  LCD_WF39TO36_WF37_MASK      0x800u
#define  LCD_WF39TO36_WF37_SHIFT     8
#define  LCD_WF39TO36_WF37(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF39TO36_WF37_SHIFT))&LCD_WF39TO36_WF37_MASK)

#define  LCD_WF39TO36_WF38_MASK      0x80000u
#define  LCD_WF39TO36_WF38_SHIFT     16
#define  LCD_WF39TO36_WF38(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF39TO36_WF38_SHIFT))&LCD_WF39TO36_WF38_MASK)

#define  LCD_WF39TO36_WF39_MASK      0x8000000u
#define  LCD_WF39TO36_WF39_SHIFT     24
#define  LCD_WF39TO36_WF39(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF39TO36_WF39_SHIFT))&LCD_WF39TO36_WF39_MASK)

#define  LCD_WF43TO40_WF40_MASK      0x8u
#define  LCD_WF43TO40_WF40_SHIFT     0
#define  LCD_WF43TO40_WF40(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF43TO40_WF40_SHIFT))&LCD_WF43TO40_WF40_MASK)

#define  LCD_WF43TO40_WF41_MASK      0x800u
#define  LCD_WF43TO40_WF41_SHIFT     8
#define  LCD_WF43TO40_WF41(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF43TO40_WF41_SHIFT))&LCD_WF43TO40_WF41_MASK)

#define  LCD_WF43TO40_WF42_MASK      0x80000u
#define  LCD_WF43TO40_WF42_SHIFT     16
#define  LCD_WF43TO40_WF42(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF43TO40_WF42_SHIFT))&LCD_WF43TO40_WF42_MASK)

#define  LCD_WF43TO40_WF43_MASK      0x8000000u
#define  LCD_WF43TO40_WF43_SHIFT     24
#define  LCD_WF43TO40_WF43(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF43TO40_WF43_SHIFT))&LCD_WF43TO40_WF43_MASK)

#define  LCD_WF47TO44_WF44_MASK      0x8u
#define  LCD_WF47TO44_WF44_SHIFT     0
#define  LCD_WF47TO44_WF44(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF47TO44_WF44_SHIFT))&LCD_WF47TO44_WF44_MASK)

#define  LCD_WF47TO44_WF45_MASK      0x800u
#define  LCD_WF47TO44_WF45_SHIFT     8
#define  LCD_WF47TO44_WF45(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF47TO44_WF45_SHIFT))&LCD_WF47TO44_WF45_MASK)

#define  LCD_WF47TO44_WF46_MASK      0x80000u
#define  LCD_WF47TO44_WF46_SHIFT     16
#define  LCD_WF47TO44_WF46(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF47TO44_WF46_SHIFT))&LCD_WF47TO44_WF46_MASK)

#define  LCD_WF47TO44_WF47_MASK      0x8000000u
#define  LCD_WF47TO44_WF47_SHIFT     24
#define  LCD_WF47TO44_WF47(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF47TO44_WF47_SHIFT))&LCD_WF47TO44_WF47_MASK)

#define  LCD_WF51TO48_WF48_MASK      0x8u
#define  LCD_WF51TO48_WF48_SHIFT     0
#define  LCD_WF51TO48_WF48(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF51TO48_WF48_SHIFT))&LCD_WF51TO48_WF48_MASK)

#define  LCD_WF51TO48_WF49_MASK      0x800u
#define  LCD_WF51TO48_WF49_SHIFT     8
#define  LCD_WF51TO48_WF49(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF51TO48_WF49_SHIFT))&LCD_WF51TO48_WF49_MASK)

#define  LCD_WF51TO48_WF50_MASK      0x80000u
#define  LCD_WF51TO48_WF50_SHIFT     16
#define  LCD_WF51TO48_WF50(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF51TO48_WF50_SHIFT))&LCD_WF51TO48_WF50_MASK)

#define  LCD_WF51TO48_WF51_MASK      0x8000000u
#define  LCD_WF51TO48_WF51_SHIFT     24
#define  LCD_WF51TO48_WF51(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF51TO48_WF51_SHIFT))&LCD_WF51TO48_WF51_MASK)

#define  LCD_WF55TO52_WF52_MASK      0x8u
#define  LCD_WF55TO52_WF52_SHIFT     0
#define  LCD_WF55TO52_WF52(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF55TO52_WF52_SHIFT))&LCD_WF55TO52_WF52_MASK)

#define  LCD_WF55TO52_WF53_MASK      0x800u
#define  LCD_WF55TO52_WF53_SHIFT     8
#define  LCD_WF55TO52_WF53(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF55TO52_WF53_SHIFT))&LCD_WF55TO52_WF53_MASK)

#define  LCD_WF55TO52_WF54_MASK      0x80000u
#define  LCD_WF55TO52_WF54_SHIFT     16
#define  LCD_WF55TO52_WF54(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF55TO52_WF54_SHIFT))&LCD_WF55TO52_WF54_MASK)

#define  LCD_WF55TO52_WF55_MASK      0x8000000u
#define  LCD_WF55TO52_WF55_SHIFT     24
#define  LCD_WF55TO52_WF55(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF55TO52_WF55_SHIFT))&LCD_WF55TO52_WF55_MASK)

#define  LCD_WF59TO56_WF56_MASK      0x8u
#define  LCD_WF59TO56_WF56_SHIFT     0
#define  LCD_WF59TO56_WF56(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF59TO56_WF56_SHIFT))&LCD_WF59TO56_WF56_MASK)

#define  LCD_WF59TO56_WF57_MASK      0x800u
#define  LCD_WF59TO56_WF57_SHIFT     8
#define  LCD_WF59TO56_WF57(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF59TO56_WF57_SHIFT))&LCD_WF59TO56_WF57_MASK)

#define  LCD_WF59TO56_WF58_MASK      0x80000u
#define  LCD_WF59TO56_WF58_SHIFT     16
#define  LCD_WF59TO56_WF58(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF59TO56_WF58_SHIFT))&LCD_WF59TO56_WF58_MASK)

#define  LCD_WF59TO56_WF59_MASK      0x8000000u
#define  LCD_WF59TO56_WF59_SHIFT     24
#define  LCD_WF59TO56_WF59(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF59TO56_WF59_SHIFT))&LCD_WF59TO56_WF59_MASK)

#define  LCD_WF63TO60_WF60_MASK      0x8u
#define  LCD_WF63TO60_WF60_SHIFT     0
#define  LCD_WF63TO60_WF60(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF63TO60_WF60_SHIFT))&LCD_WF63TO60_WF60_MASK)

#define  LCD_WF63TO60_WF61_MASK      0x800u
#define  LCD_WF63TO60_WF61_SHIFT     8
#define  LCD_WF63TO60_WF61(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF63TO60_WF61_SHIFT))&LCD_WF63TO60_WF61_MASK)

#define  LCD_WF63TO60_WF62_MASK      0x80000u
#define  LCD_WF63TO60_WF62_SHIFT     16
#define  LCD_WF63TO60_WF62(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF63TO60_WF62_SHIFT))&LCD_WF63TO60_WF62_MASK)

#define  LCD_WF63TO60_WF63_MASK      0x8000000u
#define  LCD_WF63TO60_WF63_SHIFT     24
#define  LCD_WF63TO60_WF63(x)        (((uint32_t)(((uint32_t)(x))<<LCD_WF63TO60_WF63_SHIFT))&LCD_WF63TO60_WF63_MASK)

#define  LLWU_PE1_WUPE0_MASK         0x2u
#define  LLWU_PE1_WUPE0_SHIFT        0
#define  LLWU_PE1_WUPE0(x)           (((uint8_t)(((uint8_t)(x))<<LLWU_PE1_WUPE0_SHIFT))&LLWU_PE1_WUPE0_MASK)
//   Wakeup Pin Enable For LLWU_P0
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_PE1_WUPE1_MASK         0x8u
#define  LLWU_PE1_WUPE1_SHIFT        2
#define  LLWU_PE1_WUPE1(x)           (((uint8_t)(((uint8_t)(x))<<LLWU_PE1_WUPE1_SHIFT))&LLWU_PE1_WUPE1_MASK)
//   Wakeup Pin Enable For LLWU_P1
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_PE1_WUPE2_MASK         0x20u
#define  LLWU_PE1_WUPE2_SHIFT        4
#define  LLWU_PE1_WUPE2(x)           (((uint8_t)(((uint8_t)(x))<<LLWU_PE1_WUPE2_SHIFT))&LLWU_PE1_WUPE2_MASK)
//   Wakeup Pin Enable For LLWU_P2
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_PE1_WUPE3_MASK         0x80u
#define  LLWU_PE1_WUPE3_SHIFT        6
#define  LLWU_PE1_WUPE3(x)           (((uint8_t)(((uint8_t)(x))<<LLWU_PE1_WUPE3_SHIFT))&LLWU_PE1_WUPE3_MASK)
//   Wakeup Pin Enable For LLWU_P3
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_PE2_WUPE4_MASK         0x2u
#define  LLWU_PE2_WUPE4_SHIFT        0
#define  LLWU_PE2_WUPE4(x)           (((uint8_t)(((uint8_t)(x))<<LLWU_PE2_WUPE4_SHIFT))&LLWU_PE2_WUPE4_MASK)
//   Wakeup Pin Enable For LLWU_P4
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_PE2_WUPE5_MASK         0x8u
#define  LLWU_PE2_WUPE5_SHIFT        2
#define  LLWU_PE2_WUPE5(x)           (((uint8_t)(((uint8_t)(x))<<LLWU_PE2_WUPE5_SHIFT))&LLWU_PE2_WUPE5_MASK)
//   Wakeup Pin Enable For LLWU_P5
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_PE2_WUPE6_MASK         0x20u
#define  LLWU_PE2_WUPE6_SHIFT        4
#define  LLWU_PE2_WUPE6(x)           (((uint8_t)(((uint8_t)(x))<<LLWU_PE2_WUPE6_SHIFT))&LLWU_PE2_WUPE6_MASK)
//   Wakeup Pin Enable For LLWU_P6
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_PE2_WUPE7_MASK         0x80u
#define  LLWU_PE2_WUPE7_SHIFT        6
#define  LLWU_PE2_WUPE7(x)           (((uint8_t)(((uint8_t)(x))<<LLWU_PE2_WUPE7_SHIFT))&LLWU_PE2_WUPE7_MASK)
//   Wakeup Pin Enable For LLWU_P7
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_PE3_WUPE8_MASK         0x2u
#define  LLWU_PE3_WUPE8_SHIFT        0
#define  LLWU_PE3_WUPE8(x)           (((uint8_t)(((uint8_t)(x))<<LLWU_PE3_WUPE8_SHIFT))&LLWU_PE3_WUPE8_MASK)
//   Wakeup Pin Enable For LLWU_P8
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_PE3_WUPE9_MASK         0x8u
#define  LLWU_PE3_WUPE9_SHIFT        2
#define  LLWU_PE3_WUPE9(x)           (((uint8_t)(((uint8_t)(x))<<LLWU_PE3_WUPE9_SHIFT))&LLWU_PE3_WUPE9_MASK)
//   Wakeup Pin Enable For LLWU_P9
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_PE3_WUPE10_MASK        0x20u
#define  LLWU_PE3_WUPE10_SHIFT       4
#define  LLWU_PE3_WUPE10(x)          (((uint8_t)(((uint8_t)(x))<<LLWU_PE3_WUPE10_SHIFT))&LLWU_PE3_WUPE10_MASK)
//   Wakeup Pin Enable For LLWU_P10
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_PE3_WUPE11_MASK        0x80u
#define  LLWU_PE3_WUPE11_SHIFT       6
#define  LLWU_PE3_WUPE11(x)          (((uint8_t)(((uint8_t)(x))<<LLWU_PE3_WUPE11_SHIFT))&LLWU_PE3_WUPE11_MASK)
//   Wakeup Pin Enable For LLWU_P11
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_PE4_WUPE12_MASK        0x2u
#define  LLWU_PE4_WUPE12_SHIFT       0
#define  LLWU_PE4_WUPE12(x)          (((uint8_t)(((uint8_t)(x))<<LLWU_PE4_WUPE12_SHIFT))&LLWU_PE4_WUPE12_MASK)
//   Wakeup Pin Enable For LLWU_P12
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_PE4_WUPE13_MASK        0x8u
#define  LLWU_PE4_WUPE13_SHIFT       2
#define  LLWU_PE4_WUPE13(x)          (((uint8_t)(((uint8_t)(x))<<LLWU_PE4_WUPE13_SHIFT))&LLWU_PE4_WUPE13_MASK)
//   Wakeup Pin Enable For LLWU_P13
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_PE4_WUPE14_MASK        0x20u
#define  LLWU_PE4_WUPE14_SHIFT       4
#define  LLWU_PE4_WUPE14(x)          (((uint8_t)(((uint8_t)(x))<<LLWU_PE4_WUPE14_SHIFT))&LLWU_PE4_WUPE14_MASK)
//   Wakeup Pin Enable For LLWU_P14
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_PE4_WUPE15_MASK        0x80u
#define  LLWU_PE4_WUPE15_SHIFT       6
#define  LLWU_PE4_WUPE15(x)          (((uint8_t)(((uint8_t)(x))<<LLWU_PE4_WUPE15_SHIFT))&LLWU_PE4_WUPE15_MASK)
//   Wakeup Pin Enable For LLWU_P15
//     0b00 ==> External input pin disabled as wakeup input
//     0b01 ==> External input pin enabled with rising edge detection
//     0b10 ==> External input pin enabled with falling edge detection
//     0b11 ==> External input pin enabled with any change detection

#define  LLWU_ME_WUME0_MASK          0x1u
#define  LLWU_ME_WUME0_SHIFT         0
//   Wakeup Module Enable For Module 0
//     0b0 ==> Internal module flag not used as wakeup source
//     0b1 ==> Internal module flag used as wakeup source

#define  LLWU_ME_WUME1_MASK          0x2u
#define  LLWU_ME_WUME1_SHIFT         1
//   Wakeup Module Enable for Module 1
//     0b0 ==> Internal module flag not used as wakeup source
//     0b1 ==> Internal module flag used as wakeup source

#define  LLWU_ME_WUME2_MASK          0x4u
#define  LLWU_ME_WUME2_SHIFT         2
//   Wakeup Module Enable For Module 2
//     0b0 ==> Internal module flag not used as wakeup source
//     0b1 ==> Internal module flag used as wakeup source

#define  LLWU_ME_WUME3_MASK          0x8u
#define  LLWU_ME_WUME3_SHIFT         3
//   Wakeup Module Enable For Module 3
//     0b0 ==> Internal module flag not used as wakeup source
//     0b1 ==> Internal module flag used as wakeup source

#define  LLWU_ME_WUME4_MASK          0x10u
#define  LLWU_ME_WUME4_SHIFT         4
//   Wakeup Module Enable For Module 4
//     0b0 ==> Internal module flag not used as wakeup source
//     0b1 ==> Internal module flag used as wakeup source

#define  LLWU_ME_WUME5_MASK          0x20u
#define  LLWU_ME_WUME5_SHIFT         5
//   Wakeup Module Enable For Module 5
//     0b0 ==> Internal module flag not used as wakeup source
//     0b1 ==> Internal module flag used as wakeup source

#define  LLWU_ME_WUME6_MASK          0x40u
#define  LLWU_ME_WUME6_SHIFT         6
//   Wakeup Module Enable For Module 6
//     0b0 ==> Internal module flag not used as wakeup source
//     0b1 ==> Internal module flag used as wakeup source

#define  LLWU_ME_WUME7_MASK          0x80u
#define  LLWU_ME_WUME7_SHIFT         7
//   Wakeup Module Enable For Module 7
//     0b0 ==> Internal module flag not used as wakeup source
//     0b1 ==> Internal module flag used as wakeup source

#define  LLWU_F1_WUF0_MASK           0x1u
#define  LLWU_F1_WUF0_SHIFT          0
//   Wakeup Flag For LLWU_P0
//     0b0 ==> LLWU_P0 input was not a wakeup source
//     0b1 ==> LLWU_P0 input was a wakeup source

#define  LLWU_F1_WUF1_MASK           0x2u
#define  LLWU_F1_WUF1_SHIFT          1
//   Wakeup Flag For LLWU_P1
//     0b0 ==> LLWU_P1 input was not a wakeup source
//     0b1 ==> LLWU_P1 input was a wakeup source

#define  LLWU_F1_WUF2_MASK           0x4u
#define  LLWU_F1_WUF2_SHIFT          2
//   Wakeup Flag For LLWU_P2
//     0b0 ==> LLWU_P2 input was not a wakeup source
//     0b1 ==> LLWU_P2 input was a wakeup source

#define  LLWU_F1_WUF3_MASK           0x8u
#define  LLWU_F1_WUF3_SHIFT          3
//   Wakeup Flag For LLWU_P3
//     0b0 ==> LLWU_P3 input was not a wakeup source
//     0b1 ==> LLWU_P3 input was a wakeup source

#define  LLWU_F1_WUF4_MASK           0x10u
#define  LLWU_F1_WUF4_SHIFT          4
//   Wakeup Flag For LLWU_P4
//     0b0 ==> LLWU_P4 input was not a wakeup source
//     0b1 ==> LLWU_P4 input was a wakeup source

#define  LLWU_F1_WUF5_MASK           0x20u
#define  LLWU_F1_WUF5_SHIFT          5
//   Wakeup Flag For LLWU_P5
//     0b0 ==> LLWU_P5 input was not a wakeup source
//     0b1 ==> LLWU_P5 input was a wakeup source

#define  LLWU_F1_WUF6_MASK           0x40u
#define  LLWU_F1_WUF6_SHIFT          6
//   Wakeup Flag For LLWU_P6
//     0b0 ==> LLWU_P6 input was not a wakeup source
//     0b1 ==> LLWU_P6 input was a wakeup source

#define  LLWU_F1_WUF7_MASK           0x80u
#define  LLWU_F1_WUF7_SHIFT          7
//   Wakeup Flag For LLWU_P7
//     0b0 ==> LLWU_P7 input was not a wakeup source
//     0b1 ==> LLWU_P7 input was a wakeup source

#define  LLWU_F2_WUF8_MASK           0x1u
#define  LLWU_F2_WUF8_SHIFT          0
//   Wakeup Flag For LLWU_P8
//     0b0 ==> LLWU_P8 input was not a wakeup source
//     0b1 ==> LLWU_P8 input was a wakeup source

#define  LLWU_F2_WUF9_MASK           0x2u
#define  LLWU_F2_WUF9_SHIFT          1
//   Wakeup Flag For LLWU_P9
//     0b0 ==> LLWU_P9 input was not a wakeup source
//     0b1 ==> LLWU_P9 input was a wakeup source

#define  LLWU_F2_WUF10_MASK          0x4u
#define  LLWU_F2_WUF10_SHIFT         2
//   Wakeup Flag For LLWU_P10
//     0b0 ==> LLWU_P10 input was not a wakeup source
//     0b1 ==> LLWU_P10 input was a wakeup source

#define  LLWU_F2_WUF11_MASK          0x8u
#define  LLWU_F2_WUF11_SHIFT         3
//   Wakeup Flag For LLWU_P11
//     0b0 ==> LLWU_P11 input was not a wakeup source
//     0b1 ==> LLWU_P11 input was a wakeup source

#define  LLWU_F2_WUF12_MASK          0x10u
#define  LLWU_F2_WUF12_SHIFT         4
//   Wakeup Flag For LLWU_P12
//     0b0 ==> LLWU_P12 input was not a wakeup source
//     0b1 ==> LLWU_P12 input was a wakeup source

#define  LLWU_F2_WUF13_MASK          0x20u
#define  LLWU_F2_WUF13_SHIFT         5
//   Wakeup Flag For LLWU_P13
//     0b0 ==> LLWU_P13 input was not a wakeup source
//     0b1 ==> LLWU_P13 input was a wakeup source

#define  LLWU_F2_WUF14_MASK          0x40u
#define  LLWU_F2_WUF14_SHIFT         6
//   Wakeup Flag For LLWU_P14
//     0b0 ==> LLWU_P14 input was not a wakeup source
//     0b1 ==> LLWU_P14 input was a wakeup source

#define  LLWU_F2_WUF15_MASK          0x80u
#define  LLWU_F2_WUF15_SHIFT         7
//   Wakeup Flag For LLWU_P15
//     0b0 ==> LLWU_P15 input was not a wakeup source
//     0b1 ==> LLWU_P15 input was a wakeup source

#define  LLWU_F3_MWUF0_MASK          0x1u
#define  LLWU_F3_MWUF0_SHIFT         0
//   Wakeup flag For module 0
//     0b0 ==> Module 0 input was not a wakeup source
//     0b1 ==> Module 0 input was a wakeup source

#define  LLWU_F3_MWUF1_MASK          0x2u
#define  LLWU_F3_MWUF1_SHIFT         1
//   Wakeup flag For module 1
//     0b0 ==> Module 1 input was not a wakeup source
//     0b1 ==> Module 1 input was a wakeup source

#define  LLWU_F3_MWUF2_MASK          0x4u
#define  LLWU_F3_MWUF2_SHIFT         2
//   Wakeup flag For module 2
//     0b0 ==> Module 2 input was not a wakeup source
//     0b1 ==> Module 2 input was a wakeup source

#define  LLWU_F3_MWUF3_MASK          0x8u
#define  LLWU_F3_MWUF3_SHIFT         3
//   Wakeup flag For module 3
//     0b0 ==> Module 3 input was not a wakeup source
//     0b1 ==> Module 3 input was a wakeup source

#define  LLWU_F3_MWUF4_MASK          0x10u
#define  LLWU_F3_MWUF4_SHIFT         4
//   Wakeup flag For module 4
//     0b0 ==> Module 4 input was not a wakeup source
//     0b1 ==> Module 4 input was a wakeup source

#define  LLWU_F3_MWUF5_MASK          0x20u
#define  LLWU_F3_MWUF5_SHIFT         5
//   Wakeup flag For module 5
//     0b0 ==> Module 5 input was not a wakeup source
//     0b1 ==> Module 5 input was a wakeup source

#define  LLWU_F3_MWUF6_MASK          0x40u
#define  LLWU_F3_MWUF6_SHIFT         6
//   Wakeup flag For module 6
//     0b0 ==> Module 6 input was not a wakeup source
//     0b1 ==> Module 6 input was a wakeup source

#define  LLWU_F3_MWUF7_MASK          0x80u
#define  LLWU_F3_MWUF7_SHIFT         7
//   Wakeup flag For module 7
//     0b0 ==> Module 7 input was not a wakeup source
//     0b1 ==> Module 7 input was a wakeup source

#define  LLWU_FILT1_FILTSEL_MASK     0x4u
#define  LLWU_FILT1_FILTSEL_SHIFT    0
#define  LLWU_FILT1_FILTSEL(x)       (((uint8_t)(((uint8_t)(x))<<LLWU_FILT1_FILTSEL_SHIFT))&LLWU_FILT1_FILTSEL_MASK)
//   Filter Pin Select
//     0b0000 ==> Select LLWU_P0 for filter
//     0b1111 ==> Select LLWU_P15 for filter

#define  LLWU_FILT1_FILTE_MASK       0x40u
#define  LLWU_FILT1_FILTE_SHIFT      5
#define  LLWU_FILT1_FILTE(x)         (((uint8_t)(((uint8_t)(x))<<LLWU_FILT1_FILTE_SHIFT))&LLWU_FILT1_FILTE_MASK)
//   Digital Filter On External Pin
//     0b00 ==> Filter disabled
//     0b01 ==> Filter posedge detect enabled
//     0b10 ==> Filter negedge detect enabled
//     0b11 ==> Filter any edge detect enabled

#define  LLWU_FILT1_FILTF_MASK       0x80u
#define  LLWU_FILT1_FILTF_SHIFT      7
//   Filter Detect Flag
//     0b0 ==> Pin Filter 1 was not a wakeup source
//     0b1 ==> Pin Filter 1 was a wakeup source

#define  LLWU_FILT2_FILTSEL_MASK     0x4u
#define  LLWU_FILT2_FILTSEL_SHIFT    0
#define  LLWU_FILT2_FILTSEL(x)       (((uint8_t)(((uint8_t)(x))<<LLWU_FILT2_FILTSEL_SHIFT))&LLWU_FILT2_FILTSEL_MASK)
//   Filter Pin Select
//     0b0000 ==> Select LLWU_P0 for filter
//     0b1111 ==> Select LLWU_P15 for filter

#define  LLWU_FILT2_FILTE_MASK       0x40u
#define  LLWU_FILT2_FILTE_SHIFT      5
#define  LLWU_FILT2_FILTE(x)         (((uint8_t)(((uint8_t)(x))<<LLWU_FILT2_FILTE_SHIFT))&LLWU_FILT2_FILTE_MASK)
//   Digital Filter On External Pin
//     0b00 ==> Filter disabled
//     0b01 ==> Filter posedge detect enabled
//     0b10 ==> Filter negedge detect enabled
//     0b11 ==> Filter any edge detect enabled

#define  LLWU_FILT2_FILTF_MASK       0x80u
#define  LLWU_FILT2_FILTF_SHIFT      7
//   Filter Detect Flag
//     0b0 ==> Pin Filter 2 was not a wakeup source
//     0b1 ==> Pin Filter 2 was a wakeup source

#define  LPTMR_CSR_TEN_MASK          0x1u
#define  LPTMR_CSR_TEN_SHIFT         0
//   Timer Enable
//     0b0 ==> LPTMR is disabled and internal logic is reset.
//     0b1 ==> LPTMR is enabled.

#define  LPTMR_CSR_TMS_MASK          0x2u
#define  LPTMR_CSR_TMS_SHIFT         1
//   Timer Mode Select
//     0b0 ==> Time Counter mode.
//     0b1 ==> Pulse Counter mode.

#define  LPTMR_CSR_TFC_MASK          0x4u
#define  LPTMR_CSR_TFC_SHIFT         2
//   Timer Free-Running Counter
//     0b0 ==> CNR is reset whenever TCF is set.
//     0b1 ==> CNR is reset on overflow.

#define  LPTMR_CSR_TPP_MASK          0x8u
#define  LPTMR_CSR_TPP_SHIFT         3
//   Timer Pin Polarity
//     0b0 ==> Pulse Counter input source is active-high, and the CNR will increment on the rising-edge.
//     0b1 ==> Pulse Counter input source is active-low, and the CNR will increment on the falling-edge.

#define  LPTMR_CSR_TPS_MASK          0x20u
#define  LPTMR_CSR_TPS_SHIFT         4
#define  LPTMR_CSR_TPS(x)            (((uint32_t)(((uint32_t)(x))<<LPTMR_CSR_TPS_SHIFT))&LPTMR_CSR_TPS_MASK)
//   Timer Pin Select
//     0b00 ==> Pulse counter input 0 is selected.
//     0b01 ==> Pulse counter input 1 is selected.
//     0b10 ==> Pulse counter input 2 is selected.
//     0b11 ==> Pulse counter input 3 is selected.

#define  LPTMR_CSR_TIE_MASK          0x40u
#define  LPTMR_CSR_TIE_SHIFT         6
//   Timer Interrupt Enable
//     0b0 ==> Timer interrupt disabled.
//     0b1 ==> Timer interrupt enabled.

#define  LPTMR_CSR_TCF_MASK          0x80u
#define  LPTMR_CSR_TCF_SHIFT         7
//   Timer Compare Flag
//     0b0 ==> The value of CNR is not equal to CMR and increments.
//     0b1 ==> The value of CNR is equal to CMR and increments.

#define  LPTMR_PSR_PCS_MASK          0x2u
#define  LPTMR_PSR_PCS_SHIFT         0
#define  LPTMR_PSR_PCS(x)            (((uint32_t)(((uint32_t)(x))<<LPTMR_PSR_PCS_SHIFT))&LPTMR_PSR_PCS_MASK)
//   Prescaler Clock Select
//     0b00 ==> Prescaler/glitch filter clock 0 selected.
//     0b01 ==> Prescaler/glitch filter clock 1 selected.
//     0b10 ==> Prescaler/glitch filter clock 2 selected.
//     0b11 ==> Prescaler/glitch filter clock 3 selected.

#define  LPTMR_PSR_PBYP_MASK         0x4u
#define  LPTMR_PSR_PBYP_SHIFT        2
//   Prescaler Bypass
//     0b0 ==> Prescaler/glitch filter is enabled.
//     0b1 ==> Prescaler/glitch filter is bypassed.

#define  LPTMR_PSR_PRESCALE_MASK     0x20u
#define  LPTMR_PSR_PRESCALE_SHIFT    3
#define  LPTMR_PSR_PRESCALE(x)       (((uint32_t)(((uint32_t)(x))<<LPTMR_PSR_PRESCALE_SHIFT))&LPTMR_PSR_PRESCALE_MASK)
//   Prescale Value
//     0b0000 ==> Prescaler divides the prescaler clock by 2; glitch filter does not support this configuration.
//     0b0001 ==> Prescaler divides the prescaler clock by 4; glitch filter recognizes change on input pin after 2 rising clock edges.
//     0b0010 ==> Prescaler divides the prescaler clock by 8; glitch filter recognizes change on input pin after 4 rising clock edges.
//     0b0011 ==> Prescaler divides the prescaler clock by 16; glitch filter recognizes change on input pin after 8 rising clock edges.
//     0b0100 ==> Prescaler divides the prescaler clock by 32; glitch filter recognizes change on input pin after 16 rising clock edges.
//     0b0101 ==> Prescaler divides the prescaler clock by 64; glitch filter recognizes change on input pin after 32 rising clock edges.
//     0b0110 ==> Prescaler divides the prescaler clock by 128; glitch filter recognizes change on input pin after 64 rising clock edges.
//     0b0111 ==> Prescaler divides the prescaler clock by 256; glitch filter recognizes change on input pin after 128 rising clock edges.
//     0b1000 ==> Prescaler divides the prescaler clock by 512; glitch filter recognizes change on input pin after 256 rising clock edges.
//     0b1001 ==> Prescaler divides the prescaler clock by 1024; glitch filter recognizes change on input pin after 512 rising clock edges.
//     0b1010 ==> Prescaler divides the prescaler clock by 2048; glitch filter recognizes change on input pin after 1024 rising clock edges.
//     0b1011 ==> Prescaler divides the prescaler clock by 4096; glitch filter recognizes change on input pin after 2048 rising clock edges.
//     0b1100 ==> Prescaler divides the prescaler clock by 8192; glitch filter recognizes change on input pin after 4096 rising clock edges.
//     0b1101 ==> Prescaler divides the prescaler clock by 16,384; glitch filter recognizes change on input pin after 8192 rising clock edges.
//     0b1110 ==> Prescaler divides the prescaler clock by 32,768; glitch filter recognizes change on input pin after 16,384 rising clock edges.
//     0b1111 ==> Prescaler divides the prescaler clock by 65,536; glitch filter recognizes change on input pin after 32,768 rising clock edges.

#define  LPTMR_CMR_COMPARE_MASK      0x10u
#define  LPTMR_CMR_COMPARE_SHIFT     0
#define  LPTMR_CMR_COMPARE(x)        (((uint32_t)(((uint32_t)(x))<<LPTMR_CMR_COMPARE_SHIFT))&LPTMR_CMR_COMPARE_MASK)
//   Compare Value

#define  LPTMR_CNR_COUNTER_MASK      0x10u
#define  LPTMR_CNR_COUNTER_SHIFT     0
#define  LPTMR_CNR_COUNTER(x)        (((uint32_t)(((uint32_t)(x))<<LPTMR_CNR_COUNTER_SHIFT))&LPTMR_CNR_COUNTER_MASK)
//   Counter Value

#define  MCG_C1_IREFSTEN_MASK        0x1u
#define  MCG_C1_IREFSTEN_SHIFT       0
//   Internal Reference Stop Enable
//     0b0 ==> Internal reference clock is disabled in Stop mode.
//     0b1 ==> Internal reference clock is enabled in Stop mode if IRCLKEN is set or if MCG is in FEI, FBI, or BLPI modes before entering Stop mode.

#define  MCG_C1_IRCLKEN_MASK         0x2u
#define  MCG_C1_IRCLKEN_SHIFT        1
//   Internal Reference Clock Enable
//     0b0 ==> MCGIRCLK inactive.
//     0b1 ==> MCGIRCLK active.

#define  MCG_C1_IREFS_MASK           0x4u
#define  MCG_C1_IREFS_SHIFT          2
//   Internal Reference Select
//     0b0 ==> External reference clock is selected.
//     0b1 ==> The slow internal reference clock is selected.

#define  MCG_C1_FRDIV_MASK           0x18u
#define  MCG_C1_FRDIV_SHIFT          3
#define  MCG_C1_FRDIV(x)             (((uint8_t)(((uint8_t)(x))<<MCG_C1_FRDIV_SHIFT))&MCG_C1_FRDIV_MASK)
//   FLL External Reference Divider
//     0b000 ==> If RANGE 0 = 0 or OSCSEL=1 , Divide Factor is 1; for all other RANGE 0 values, Divide Factor is 32.
//     0b001 ==> If RANGE 0 = 0 or OSCSEL=1 , Divide Factor is 2; for all other RANGE 0 values, Divide Factor is 64.
//     0b010 ==> If RANGE 0 = 0 or OSCSEL=1 , Divide Factor is 4; for all other RANGE 0 values, Divide Factor is 128.
//     0b011 ==> If RANGE 0 = 0 or OSCSEL=1 , Divide Factor is 8; for all other RANGE 0 values, Divide Factor is 256.
//     0b100 ==> If RANGE 0 = 0 or OSCSEL=1 , Divide Factor is 16; for all other RANGE 0 values, Divide Factor is 512.
//     0b101 ==> If RANGE 0 = 0 or OSCSEL=1 , Divide Factor is 32; for all other RANGE 0 values, Divide Factor is 1024.
//     0b110 ==> If RANGE 0 = 0 or OSCSEL=1 , Divide Factor is 64; for all other RANGE 0 values, Divide Factor is 1280 .
//     0b111 ==> If RANGE 0 = 0 or OSCSEL=1 , Divide Factor is 128; for all other RANGE 0 values, Divide Factor is 1536 .

#define  MCG_C1_CLKS_MASK            0x80u
#define  MCG_C1_CLKS_SHIFT           6
#define  MCG_C1_CLKS(x)              (((uint8_t)(((uint8_t)(x))<<MCG_C1_CLKS_SHIFT))&MCG_C1_CLKS_MASK)
//   Clock Source Select
//     0b00 ==> Encoding 0 - Output of FLL or PLL is selected (depends on PLLS control bit).
//     0b01 ==> Encoding 1 - Internal reference clock is selected.
//     0b10 ==> Encoding 2 - External reference clock is selected.
//     0b11 ==> Encoding 3 - Reserved.

#define  MCG_C2_IRCS_MASK            0x1u
#define  MCG_C2_IRCS_SHIFT           0
//   Internal Reference Clock Select
//     0b0 ==> Slow internal reference clock selected.
//     0b1 ==> Fast internal reference clock selected.

#define  MCG_C2_LP_MASK              0x2u
#define  MCG_C2_LP_SHIFT             1
//   Low Power Select
//     0b0 ==> FLL or PLL is not disabled in bypass modes.
//     0b1 ==> FLL or PLL is disabled in bypass modes (lower power)

#define  MCG_C2_EREFS0_MASK          0x4u
#define  MCG_C2_EREFS0_SHIFT         2
//   External Reference Select
//     0b0 ==> External reference clock requested.
//     0b1 ==> Oscillator requested.

#define  MCG_C2_HGO0_MASK            0x8u
#define  MCG_C2_HGO0_SHIFT           3
//   High Gain Oscillator Select
//     0b0 ==> Configure crystal oscillator for low-power operation.
//     0b1 ==> Configure crystal oscillator for high-gain operation.

#define  MCG_C2_RANGE0_MASK          0x20u
#define  MCG_C2_RANGE0_SHIFT         4
#define  MCG_C2_RANGE0(x)            (((uint8_t)(((uint8_t)(x))<<MCG_C2_RANGE0_SHIFT))&MCG_C2_RANGE0_MASK)
//   Frequency Range Select
//     0b00 ==> Encoding 0 - Low frequency range selected for the crystal oscillator .
//     0b01 ==> Encoding 1 - High frequency range selected for the crystal oscillator .

#define  MCG_C2_LOCRE0_MASK          0x80u
#define  MCG_C2_LOCRE0_SHIFT         7
//   Loss of Clock Reset Enable
//     0b0 ==> Interrupt request is generated on a loss of OSC0 external reference clock.
//     0b1 ==> Generate a reset request on a loss of OSC0 external reference clock.

#define  MCG_C3_SCTRIM_MASK          0x8u
#define  MCG_C3_SCTRIM_SHIFT         0
#define  MCG_C3_SCTRIM(x)            (((uint8_t)(((uint8_t)(x))<<MCG_C3_SCTRIM_SHIFT))&MCG_C3_SCTRIM_MASK)
//   Slow Internal Reference Clock Trim Setting

#define  MCG_C4_SCFTRIM_MASK         0x1u
#define  MCG_C4_SCFTRIM_SHIFT        0
//   Slow Internal Reference Clock Fine Trim

#define  MCG_C4_FCTRIM_MASK          0x8u
#define  MCG_C4_FCTRIM_SHIFT         1
#define  MCG_C4_FCTRIM(x)            (((uint8_t)(((uint8_t)(x))<<MCG_C4_FCTRIM_SHIFT))&MCG_C4_FCTRIM_MASK)
//   Fast Internal Reference Clock Trim Setting

#define  MCG_C4_DRST_DRS_MASK        0x40u
#define  MCG_C4_DRST_DRS_SHIFT       5
#define  MCG_C4_DRST_DRS(x)          (((uint8_t)(((uint8_t)(x))<<MCG_C4_DRST_DRS_SHIFT))&MCG_C4_DRST_DRS_MASK)
//   DCO Range Select
//     0b00 ==> Encoding 0 - Low range (reset default).
//     0b01 ==> Encoding 1 - Mid range.
//     0b10 ==> Encoding 2 - Mid-high range.
//     0b11 ==> Encoding 3 - High range.

#define  MCG_C4_DMX32_MASK           0x80u
#define  MCG_C4_DMX32_SHIFT          7
//   DCO Maximum Frequency with 32.768 kHz Reference
//     0b0 ==> DCO has a default range of 25%.
//     0b1 ==> DCO is fine-tuned for maximum frequency with 32.768 kHz reference.

#define  MCG_C5_PLLSTEN0_MASK        0x20u
#define  MCG_C5_PLLSTEN0_SHIFT       5
//   PLL Stop Enable
//     0b0 ==> MCGPLLCLK is disabled in any of the Stop modes.
//     0b1 ==> MCGPLLCLK is enabled if system is in Normal Stop mode.

#define  MCG_C5_PLLCLKEN0_MASK       0x40u
#define  MCG_C5_PLLCLKEN0_SHIFT      6
//   PLL Clock Enable
//     0b0 ==> MCGPLLCLK is inactive.
//     0b1 ==> MCGPLLCLK is active.

#define  MCG_C6_CME0_MASK            0x20u
#define  MCG_C6_CME0_SHIFT           5
//   Clock Monitor Enable
//     0b0 ==> External clock monitor is disabled for OSC0.
//     0b1 ==> External clock monitor is enabled for OSC0.

#define  MCG_C6_PLLS_MASK            0x40u
#define  MCG_C6_PLLS_SHIFT           6
//   PLL Select
//     0b0 ==> FLL is selected.
//     0b1 ==> PLL is selected (PRDIV 0 need to be programmed to the correct divider to generate a PLL reference clock in the range of 2-4 MHz prior to setting the PLLS bit).

#define  MCG_C6_LOLIE0_MASK          0x80u
#define  MCG_C6_LOLIE0_SHIFT         7
//   Loss of Lock Interrrupt Enable
//     0b0 ==> No interrupt request is generated on loss of lock.
//     0b1 ==> Generate an interrupt request on loss of lock.

#define  MCG_S_IRCST_MASK            0x1u
#define  MCG_S_IRCST_SHIFT           0
//   Internal Reference Clock Status
//     0b0 ==> Source of internal reference clock is the slow clock (32 kHz IRC).
//     0b1 ==> Source of internal reference clock is the fast clock (2 MHz IRC).

#define  MCG_S_OSCINIT0_MASK         0x2u
#define  MCG_S_OSCINIT0_SHIFT        1
//   OSC Initialization

#define  MCG_S_CLKST_MASK            0x8u
#define  MCG_S_CLKST_SHIFT           2
#define  MCG_S_CLKST(x)              (((uint8_t)(((uint8_t)(x))<<MCG_S_CLKST_SHIFT))&MCG_S_CLKST_MASK)
//   Clock Mode Status
//     0b00 ==> Encoding 0 - Output of the FLL is selected (reset default).
//     0b01 ==> Encoding 1 - Internal reference clock is selected.
//     0b10 ==> Encoding 2 - External reference clock is selected.
//     0b11 ==> Encoding 3 - Output of the PLL is selected.

#define  MCG_S_IREFST_MASK           0x10u
#define  MCG_S_IREFST_SHIFT          4
//   Internal Reference Status
//     0b0 ==> Source of FLL reference clock is the external reference clock.
//     0b1 ==> Source of FLL reference clock is the internal reference clock.

#define  MCG_S_PLLST_MASK            0x20u
#define  MCG_S_PLLST_SHIFT           5
//   PLL Select Status
//     0b0 ==> Source of PLLS clock is FLL clock.
//     0b1 ==> Source of PLLS clock is PLL clock.

#define  MCG_S_LOCK0_MASK            0x40u
#define  MCG_S_LOCK0_SHIFT           6
//   Lock Status
//     0b0 ==> PLL is currently unlocked.
//     0b1 ==> PLL is currently locked.

#define  MCG_S_LOLS0_MASK            0x80u
#define  MCG_S_LOLS0_SHIFT           7
//   Loss of Lock Status
//     0b0 ==> PLL has not lost lock since LOLS 0 was last cleared.
//     0b1 ==> PLL has lost lock since LOLS 0 was last cleared.

#define  MCG_SC_LOCS0_MASK           0x1u
#define  MCG_SC_LOCS0_SHIFT          0
//   OSC0 Loss of Clock Status
//     0b0 ==> Loss of OSC0 has not occurred.
//     0b1 ==> Loss of OSC0 has occurred.

#define  MCG_SC_FCRDIV_MASK          0x6u
#define  MCG_SC_FCRDIV_SHIFT         1
#define  MCG_SC_FCRDIV(x)            (((uint8_t)(((uint8_t)(x))<<MCG_SC_FCRDIV_SHIFT))&MCG_SC_FCRDIV_MASK)
//   Fast Clock Internal Reference Divider
//     0b000 ==> Divide Factor is 1
//     0b001 ==> Divide Factor is 2.
//     0b010 ==> Divide Factor is 4.
//     0b011 ==> Divide Factor is 8.
//     0b100 ==> Divide Factor is 16
//     0b101 ==> Divide Factor is 32
//     0b110 ==> Divide Factor is 64
//     0b111 ==> Divide Factor is 128.

#define  MCG_SC_FLTPRSRV_MASK        0x10u
#define  MCG_SC_FLTPRSRV_SHIFT       4
//   FLL Filter Preserve Enable
//     0b0 ==> FLL filter and FLL frequency will reset on changes to currect clock mode.
//     0b1 ==> Fll filter and FLL frequency retain their previous values during new clock mode change.

#define  MCG_SC_ATMF_MASK            0x20u
#define  MCG_SC_ATMF_SHIFT           5
//   Automatic Trim Machine Fail Flag
//     0b0 ==> Automatic Trim Machine completed normally.
//     0b1 ==> Automatic Trim Machine failed.

#define  MCG_SC_ATMS_MASK            0x40u
#define  MCG_SC_ATMS_SHIFT           6
//   Automatic Trim Machine Select
//     0b0 ==> 32 kHz Internal Reference Clock selected.
//     0b1 ==> 4 MHz Internal Reference Clock selected.

#define  MCG_SC_ATME_MASK            0x80u
#define  MCG_SC_ATME_SHIFT           7
//   Automatic Trim Machine Enable
//     0b0 ==> Auto Trim Machine disabled.
//     0b1 ==> Auto Trim Machine enabled.

#define  MCG_ATCVH_ATCVH_MASK        0x8u
#define  MCG_ATCVH_ATCVH_SHIFT       0
#define  MCG_ATCVH_ATCVH(x)          (((uint8_t)(((uint8_t)(x))<<MCG_ATCVH_ATCVH_SHIFT))&MCG_ATCVH_ATCVH_MASK)
//   ATM Compare Value High

#define  MCG_ATCVL_ATCVL_MASK        0x8u
#define  MCG_ATCVL_ATCVL_SHIFT       0
#define  MCG_ATCVL_ATCVL(x)          (((uint8_t)(((uint8_t)(x))<<MCG_ATCVL_ATCVL_SHIFT))&MCG_ATCVL_ATCVL_MASK)
//   ATM Compare Value Low

#define  MCG_C7_OSCSEL_MASK          0x1u
#define  MCG_C7_OSCSEL_SHIFT         0
//   MCG OSC Clock Select
//     0b0 ==> Selects System Oscillator (OSCCLK).
//     0b1 ==> Selects 32 kHz RTC Oscillator.

#define  MCG_C7_PLL32KREFSEL_MASK    0x80u
#define  MCG_C7_PLL32KREFSEL_SHIFT   6
#define  MCG_C7_PLL32KREFSEL(x)      (((uint8_t)(((uint8_t)(x))<<MCG_C7_PLL32KREFSEL_SHIFT))&MCG_C7_PLL32KREFSEL_MASK)
//   MCG PLL 32Khz Reference Clock Select
//     0b00 ==> Selects 32 kHz RTC Oscillator.
//     0b01 ==> Selects 32 kHz IRC.
//     0b10 ==> Selects FLL FRDIV clock.
//     0b11 ==> Reserved.

#define  MCG_C8_LOCS1_MASK           0x1u
#define  MCG_C8_LOCS1_SHIFT          0
//   RTC Loss of Clock Status
//     0b0 ==> Loss of RTC has not occur.
//     0b1 ==> Loss of RTC has occur

#define  MCG_C8_CME1_MASK            0x20u
#define  MCG_C8_CME1_SHIFT           5
//   Clock Monitor Enable1
//     0b0 ==> External clock monitor is disabled for RTC clock.
//     0b1 ==> External clock monitor is enabled for RTC clock.

#define  MCG_C8_LOLRE_MASK           0x40u
#define  MCG_C8_LOLRE_SHIFT          6
//     0b0 ==> Interrupt request is generated on a PLL loss of lock indication. The PLL loss of lock interrupt enable bit must also be set to generate the interrupt request.
//     0b1 ==> Generate a reset request on a PLL loss of lock indication.

#define  MCG_C8_LOCRE1_MASK          0x80u
#define  MCG_C8_LOCRE1_SHIFT         7
//   Loss of Clock Reset Enable
//     0b0 ==> Interrupt request is generated on a loss of RTC external reference clock.
//     0b1 ==> Generate a reset request on a loss of RTC external reference clock

#define  MCM_CR_SRAMUAP_MASK         0x2000000u
#define  MCM_CR_SRAMUAP_SHIFT        24
#define  MCM_CR_SRAMUAP(x)           (((uint32_t)(((uint32_t)(x))<<MCM_CR_SRAMUAP_SHIFT))&MCM_CR_SRAMUAP_MASK)
//   SRAM_U arbitration priority
//     0b00 ==> Round robin
//     0b01 ==> Special round robin (favors SRAM backoor accesses over the processor)
//     0b10 ==> Fixed priority. Processor has highest, backdoor has lowest
//     0b11 ==> Fixed priority. Backdoor has highest, processor has lowest

#define  MCM_CR_SRAMUWP_MASK         0x4000000u
#define  MCM_CR_SRAMUWP_SHIFT        26
//   SRAM_U write protect

#define  MCM_CR_SRAMLAP_MASK         0x20000000u
#define  MCM_CR_SRAMLAP_SHIFT        28
#define  MCM_CR_SRAMLAP(x)           (((uint32_t)(((uint32_t)(x))<<MCM_CR_SRAMLAP_SHIFT))&MCM_CR_SRAMLAP_MASK)
//   SRAM_L arbitration priority
//     0b00 ==> Round robin
//     0b01 ==> Special round robin (favors SRAM backoor accesses over the processor)
//     0b10 ==> Fixed priority. Processor has highest, backdoor has lowest
//     0b11 ==> Fixed priority. Backdoor has highest, processor has lowest

#define  MCM_CR_SRAMLWP_MASK         0x40000000u
#define  MCM_CR_SRAMLWP_SHIFT        30
//   SRAM_L Write Protect

#define  MCM_PID_PID_MASK            0x8u
#define  MCM_PID_PID_SHIFT           0
#define  MCM_PID_PID(x)              (((uint32_t)(((uint32_t)(x))<<MCM_PID_PID_SHIFT))&MCM_PID_PID_MASK)
//   M0_PID And M1_PID For MPU

#define  MPU_CESR_VLD_MASK           0x1u
#define  MPU_CESR_VLD_SHIFT          0
//   Valid
//     0b0 ==> MPU is disabled. All accesses from all bus masters are allowed.
//     0b1 ==> MPU is enabled

#define  MPU_CESR_NRGD_MASK          0x400u
#define  MPU_CESR_NRGD_SHIFT         8
#define  MPU_CESR_NRGD(x)            (((uint32_t)(((uint32_t)(x))<<MPU_CESR_NRGD_SHIFT))&MPU_CESR_NRGD_MASK)
//   Number Of Region Descriptors
//     0b0000 ==> 8 region descriptors
//     0b0001 ==> 12 region descriptors
//     0b0010 ==> 16 region descriptors

#define  MPU_CESR_NSP_MASK           0x4000u
#define  MPU_CESR_NSP_SHIFT          12
#define  MPU_CESR_NSP(x)             (((uint32_t)(((uint32_t)(x))<<MPU_CESR_NSP_SHIFT))&MPU_CESR_NSP_MASK)
//   Number Of Slave Ports

#define  MPU_CESR_HRL_MASK           0x40000u
#define  MPU_CESR_HRL_SHIFT          16
#define  MPU_CESR_HRL(x)             (((uint32_t)(((uint32_t)(x))<<MPU_CESR_HRL_SHIFT))&MPU_CESR_HRL_MASK)
//   Hardware Revision Level

#define  MPU_CESR_SPERR_MASK         0x80000000u
#define  MPU_CESR_SPERR_SHIFT        30
#define  MPU_CESR_SPERR(x)           (((uint32_t)(((uint32_t)(x))<<MPU_CESR_SPERR_SHIFT))&MPU_CESR_SPERR_MASK)
//   Slave Port n Error
//     0b0 ==> No error has occurred for slave port n.
//     0b1 ==> An error has occurred for slave port n.

#define  MPU_EAR0_EADDR_MASK         0x20u
#define  MPU_EAR0_EADDR_SHIFT        0
#define  MPU_EAR0_EADDR(x)           (((uint32_t)(((uint32_t)(x))<<MPU_EAR0_EADDR_SHIFT))&MPU_EAR0_EADDR_MASK)
//   Error Address

#define  MPU_EDR0_ERW_MASK           0x1u
#define  MPU_EDR0_ERW_SHIFT          0
//   Error Read/Write
//     0b0 ==> Read
//     0b1 ==> Write

#define  MPU_EDR0_EATTR_MASK         0x6u
#define  MPU_EDR0_EATTR_SHIFT        1
#define  MPU_EDR0_EATTR(x)           (((uint32_t)(((uint32_t)(x))<<MPU_EDR0_EATTR_SHIFT))&MPU_EDR0_EATTR_MASK)
//   Error Attributes
//     0b000 ==> User mode, instruction access
//     0b001 ==> User mode, data access
//     0b010 ==> Supervisor mode, instruction access
//     0b011 ==> Supervisor mode, data access

#define  MPU_EDR0_EMN_MASK           0x40u
#define  MPU_EDR0_EMN_SHIFT          4
#define  MPU_EDR0_EMN(x)             (((uint32_t)(((uint32_t)(x))<<MPU_EDR0_EMN_SHIFT))&MPU_EDR0_EMN_MASK)
//   Error Master Number

#define  MPU_EDR0_EACD_MASK          0x100000u
#define  MPU_EDR0_EACD_SHIFT         16
#define  MPU_EDR0_EACD(x)            (((uint32_t)(((uint32_t)(x))<<MPU_EDR0_EACD_SHIFT))&MPU_EDR0_EACD_MASK)
//   Error Access Control Detail

#define  MPU_EAR1_EADDR_MASK         0x20u
#define  MPU_EAR1_EADDR_SHIFT        0
#define  MPU_EAR1_EADDR(x)           (((uint32_t)(((uint32_t)(x))<<MPU_EAR1_EADDR_SHIFT))&MPU_EAR1_EADDR_MASK)
//   Error Address

#define  MPU_EDR1_ERW_MASK           0x1u
#define  MPU_EDR1_ERW_SHIFT          0
//   Error Read/Write
//     0b0 ==> Read
//     0b1 ==> Write

#define  MPU_EDR1_EATTR_MASK         0x6u
#define  MPU_EDR1_EATTR_SHIFT        1
#define  MPU_EDR1_EATTR(x)           (((uint32_t)(((uint32_t)(x))<<MPU_EDR1_EATTR_SHIFT))&MPU_EDR1_EATTR_MASK)
//   Error Attributes
//     0b000 ==> User mode, instruction access
//     0b001 ==> User mode, data access
//     0b010 ==> Supervisor mode, instruction access
//     0b011 ==> Supervisor mode, data access

#define  MPU_EDR1_EMN_MASK           0x40u
#define  MPU_EDR1_EMN_SHIFT          4
#define  MPU_EDR1_EMN(x)             (((uint32_t)(((uint32_t)(x))<<MPU_EDR1_EMN_SHIFT))&MPU_EDR1_EMN_MASK)
//   Error Master Number

#define  MPU_EDR1_EACD_MASK          0x100000u
#define  MPU_EDR1_EACD_SHIFT         16
#define  MPU_EDR1_EACD(x)            (((uint32_t)(((uint32_t)(x))<<MPU_EDR1_EACD_SHIFT))&MPU_EDR1_EACD_MASK)
//   Error Access Control Detail

#define  MPU_RGD0_WORD0_SRTADDR_MASK 0x360u
#define  MPU_RGD0_WORD0_SRTADDR_SHIFT 5
#define  MPU_RGD0_WORD0_SRTADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD0_WORD0_SRTADDR_SHIFT))&MPU_RGD0_WORD0_SRTADDR_MASK)
//   Start Address

#define  MPU_RGD0_WORD1_ENDADDR_MASK 0x360u
#define  MPU_RGD0_WORD1_ENDADDR_SHIFT 5
#define  MPU_RGD0_WORD1_ENDADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD0_WORD1_ENDADDR_SHIFT))&MPU_RGD0_WORD1_ENDADDR_MASK)
//   End Address

#define  MPU_RGD0_WORD2_M0UM_MASK    0x3u
#define  MPU_RGD0_WORD2_M0UM_SHIFT   0
#define  MPU_RGD0_WORD2_M0UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD0_WORD2_M0UM_SHIFT))&MPU_RGD0_WORD2_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGD0_WORD2_M0SM_MASK    0x10u
#define  MPU_RGD0_WORD2_M0SM_SHIFT   3
#define  MPU_RGD0_WORD2_M0SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD0_WORD2_M0SM_SHIFT))&MPU_RGD0_WORD2_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGD0_WORD2_M1UM_MASK    0xC0u
#define  MPU_RGD0_WORD2_M1UM_SHIFT   6
#define  MPU_RGD0_WORD2_M1UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD0_WORD2_M1UM_SHIFT))&MPU_RGD0_WORD2_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGD0_WORD2_M1SM_MASK    0x400u
#define  MPU_RGD0_WORD2_M1SM_SHIFT   9
#define  MPU_RGD0_WORD2_M1SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD0_WORD2_M1SM_SHIFT))&MPU_RGD0_WORD2_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGD0_WORD2_M2UM_MASK    0x3000u
#define  MPU_RGD0_WORD2_M2UM_SHIFT   12
#define  MPU_RGD0_WORD2_M2UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD0_WORD2_M2UM_SHIFT))&MPU_RGD0_WORD2_M2UM_MASK)
//   Bus Master 2 User Mode Access control

#define  MPU_RGD0_WORD2_M2SM_MASK    0x10000u
#define  MPU_RGD0_WORD2_M2SM_SHIFT   15
#define  MPU_RGD0_WORD2_M2SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD0_WORD2_M2SM_SHIFT))&MPU_RGD0_WORD2_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGD0_WORD2_M3UM_MASK    0xC0000u
#define  MPU_RGD0_WORD2_M3UM_SHIFT   18
#define  MPU_RGD0_WORD2_M3UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD0_WORD2_M3UM_SHIFT))&MPU_RGD0_WORD2_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGD0_WORD2_M3SM_MASK    0x400000u
#define  MPU_RGD0_WORD2_M3SM_SHIFT   21
#define  MPU_RGD0_WORD2_M3SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD0_WORD2_M3SM_SHIFT))&MPU_RGD0_WORD2_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGD0_WORD2_M4WE_MASK    0x1000000u
#define  MPU_RGD0_WORD2_M4WE_SHIFT   24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGD0_WORD2_M4RE_MASK    0x2000000u
#define  MPU_RGD0_WORD2_M4RE_SHIFT   25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGD0_WORD2_M5WE_MASK    0x4000000u
#define  MPU_RGD0_WORD2_M5WE_SHIFT   26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGD0_WORD2_M5RE_MASK    0x8000000u
#define  MPU_RGD0_WORD2_M5RE_SHIFT   27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGD0_WORD2_M6WE_MASK    0x10000000u
#define  MPU_RGD0_WORD2_M6WE_SHIFT   28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGD0_WORD2_M6RE_MASK    0x20000000u
#define  MPU_RGD0_WORD2_M6RE_SHIFT   29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGD0_WORD2_M7WE_MASK    0x40000000u
#define  MPU_RGD0_WORD2_M7WE_SHIFT   30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGD0_WORD2_M7RE_MASK    0x80000000u
#define  MPU_RGD0_WORD2_M7RE_SHIFT   31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MPU_RGD0_WORD3_VLD_MASK     0x1u
#define  MPU_RGD0_WORD3_VLD_SHIFT    0
//   Valid
//     0b0 ==> Region descriptor is invalid
//     0b1 ==> Region descriptor is valid

#define  MPU_RGD1_WORD0_SRTADDR_MASK 0x360u
#define  MPU_RGD1_WORD0_SRTADDR_SHIFT 5
#define  MPU_RGD1_WORD0_SRTADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD1_WORD0_SRTADDR_SHIFT))&MPU_RGD1_WORD0_SRTADDR_MASK)
//   Start Address

#define  MPU_RGD1_WORD1_ENDADDR_MASK 0x360u
#define  MPU_RGD1_WORD1_ENDADDR_SHIFT 5
#define  MPU_RGD1_WORD1_ENDADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD1_WORD1_ENDADDR_SHIFT))&MPU_RGD1_WORD1_ENDADDR_MASK)
//   End Address

#define  MPU_RGD1_WORD2_M0UM_MASK    0x3u
#define  MPU_RGD1_WORD2_M0UM_SHIFT   0
#define  MPU_RGD1_WORD2_M0UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD1_WORD2_M0UM_SHIFT))&MPU_RGD1_WORD2_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGD1_WORD2_M0SM_MASK    0x10u
#define  MPU_RGD1_WORD2_M0SM_SHIFT   3
#define  MPU_RGD1_WORD2_M0SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD1_WORD2_M0SM_SHIFT))&MPU_RGD1_WORD2_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGD1_WORD2_M1UM_MASK    0xC0u
#define  MPU_RGD1_WORD2_M1UM_SHIFT   6
#define  MPU_RGD1_WORD2_M1UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD1_WORD2_M1UM_SHIFT))&MPU_RGD1_WORD2_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGD1_WORD2_M1SM_MASK    0x400u
#define  MPU_RGD1_WORD2_M1SM_SHIFT   9
#define  MPU_RGD1_WORD2_M1SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD1_WORD2_M1SM_SHIFT))&MPU_RGD1_WORD2_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGD1_WORD2_M2UM_MASK    0x3000u
#define  MPU_RGD1_WORD2_M2UM_SHIFT   12
#define  MPU_RGD1_WORD2_M2UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD1_WORD2_M2UM_SHIFT))&MPU_RGD1_WORD2_M2UM_MASK)
//   Bus Master 2 User Mode Access control

#define  MPU_RGD1_WORD2_M2SM_MASK    0x10000u
#define  MPU_RGD1_WORD2_M2SM_SHIFT   15
#define  MPU_RGD1_WORD2_M2SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD1_WORD2_M2SM_SHIFT))&MPU_RGD1_WORD2_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGD1_WORD2_M3UM_MASK    0xC0000u
#define  MPU_RGD1_WORD2_M3UM_SHIFT   18
#define  MPU_RGD1_WORD2_M3UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD1_WORD2_M3UM_SHIFT))&MPU_RGD1_WORD2_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGD1_WORD2_M3SM_MASK    0x400000u
#define  MPU_RGD1_WORD2_M3SM_SHIFT   21
#define  MPU_RGD1_WORD2_M3SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD1_WORD2_M3SM_SHIFT))&MPU_RGD1_WORD2_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGD1_WORD2_M4WE_MASK    0x1000000u
#define  MPU_RGD1_WORD2_M4WE_SHIFT   24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGD1_WORD2_M4RE_MASK    0x2000000u
#define  MPU_RGD1_WORD2_M4RE_SHIFT   25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGD1_WORD2_M5WE_MASK    0x4000000u
#define  MPU_RGD1_WORD2_M5WE_SHIFT   26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGD1_WORD2_M5RE_MASK    0x8000000u
#define  MPU_RGD1_WORD2_M5RE_SHIFT   27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGD1_WORD2_M6WE_MASK    0x10000000u
#define  MPU_RGD1_WORD2_M6WE_SHIFT   28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGD1_WORD2_M6RE_MASK    0x20000000u
#define  MPU_RGD1_WORD2_M6RE_SHIFT   29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGD1_WORD2_M7WE_MASK    0x40000000u
#define  MPU_RGD1_WORD2_M7WE_SHIFT   30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGD1_WORD2_M7RE_MASK    0x80000000u
#define  MPU_RGD1_WORD2_M7RE_SHIFT   31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MPU_RGD1_WORD3_VLD_MASK     0x1u
#define  MPU_RGD1_WORD3_VLD_SHIFT    0
//   Valid
//     0b0 ==> Region descriptor is invalid
//     0b1 ==> Region descriptor is valid

#define  MPU_RGD2_WORD0_SRTADDR_MASK 0x360u
#define  MPU_RGD2_WORD0_SRTADDR_SHIFT 5
#define  MPU_RGD2_WORD0_SRTADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD2_WORD0_SRTADDR_SHIFT))&MPU_RGD2_WORD0_SRTADDR_MASK)
//   Start Address

#define  MPU_RGD2_WORD1_ENDADDR_MASK 0x360u
#define  MPU_RGD2_WORD1_ENDADDR_SHIFT 5
#define  MPU_RGD2_WORD1_ENDADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD2_WORD1_ENDADDR_SHIFT))&MPU_RGD2_WORD1_ENDADDR_MASK)
//   End Address

#define  MPU_RGD2_WORD2_M0UM_MASK    0x3u
#define  MPU_RGD2_WORD2_M0UM_SHIFT   0
#define  MPU_RGD2_WORD2_M0UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD2_WORD2_M0UM_SHIFT))&MPU_RGD2_WORD2_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGD2_WORD2_M0SM_MASK    0x10u
#define  MPU_RGD2_WORD2_M0SM_SHIFT   3
#define  MPU_RGD2_WORD2_M0SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD2_WORD2_M0SM_SHIFT))&MPU_RGD2_WORD2_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGD2_WORD2_M1UM_MASK    0xC0u
#define  MPU_RGD2_WORD2_M1UM_SHIFT   6
#define  MPU_RGD2_WORD2_M1UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD2_WORD2_M1UM_SHIFT))&MPU_RGD2_WORD2_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGD2_WORD2_M1SM_MASK    0x400u
#define  MPU_RGD2_WORD2_M1SM_SHIFT   9
#define  MPU_RGD2_WORD2_M1SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD2_WORD2_M1SM_SHIFT))&MPU_RGD2_WORD2_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGD2_WORD2_M2UM_MASK    0x3000u
#define  MPU_RGD2_WORD2_M2UM_SHIFT   12
#define  MPU_RGD2_WORD2_M2UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD2_WORD2_M2UM_SHIFT))&MPU_RGD2_WORD2_M2UM_MASK)
//   Bus Master 2 User Mode Access control

#define  MPU_RGD2_WORD2_M2SM_MASK    0x10000u
#define  MPU_RGD2_WORD2_M2SM_SHIFT   15
#define  MPU_RGD2_WORD2_M2SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD2_WORD2_M2SM_SHIFT))&MPU_RGD2_WORD2_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGD2_WORD2_M3UM_MASK    0xC0000u
#define  MPU_RGD2_WORD2_M3UM_SHIFT   18
#define  MPU_RGD2_WORD2_M3UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD2_WORD2_M3UM_SHIFT))&MPU_RGD2_WORD2_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGD2_WORD2_M3SM_MASK    0x400000u
#define  MPU_RGD2_WORD2_M3SM_SHIFT   21
#define  MPU_RGD2_WORD2_M3SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD2_WORD2_M3SM_SHIFT))&MPU_RGD2_WORD2_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGD2_WORD2_M4WE_MASK    0x1000000u
#define  MPU_RGD2_WORD2_M4WE_SHIFT   24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGD2_WORD2_M4RE_MASK    0x2000000u
#define  MPU_RGD2_WORD2_M4RE_SHIFT   25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGD2_WORD2_M5WE_MASK    0x4000000u
#define  MPU_RGD2_WORD2_M5WE_SHIFT   26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGD2_WORD2_M5RE_MASK    0x8000000u
#define  MPU_RGD2_WORD2_M5RE_SHIFT   27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGD2_WORD2_M6WE_MASK    0x10000000u
#define  MPU_RGD2_WORD2_M6WE_SHIFT   28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGD2_WORD2_M6RE_MASK    0x20000000u
#define  MPU_RGD2_WORD2_M6RE_SHIFT   29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGD2_WORD2_M7WE_MASK    0x40000000u
#define  MPU_RGD2_WORD2_M7WE_SHIFT   30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGD2_WORD2_M7RE_MASK    0x80000000u
#define  MPU_RGD2_WORD2_M7RE_SHIFT   31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MPU_RGD2_WORD3_VLD_MASK     0x1u
#define  MPU_RGD2_WORD3_VLD_SHIFT    0
//   Valid
//     0b0 ==> Region descriptor is invalid
//     0b1 ==> Region descriptor is valid

#define  MPU_RGD3_WORD0_SRTADDR_MASK 0x360u
#define  MPU_RGD3_WORD0_SRTADDR_SHIFT 5
#define  MPU_RGD3_WORD0_SRTADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD3_WORD0_SRTADDR_SHIFT))&MPU_RGD3_WORD0_SRTADDR_MASK)
//   Start Address

#define  MPU_RGD3_WORD1_ENDADDR_MASK 0x360u
#define  MPU_RGD3_WORD1_ENDADDR_SHIFT 5
#define  MPU_RGD3_WORD1_ENDADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD3_WORD1_ENDADDR_SHIFT))&MPU_RGD3_WORD1_ENDADDR_MASK)
//   End Address

#define  MPU_RGD3_WORD2_M0UM_MASK    0x3u
#define  MPU_RGD3_WORD2_M0UM_SHIFT   0
#define  MPU_RGD3_WORD2_M0UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD3_WORD2_M0UM_SHIFT))&MPU_RGD3_WORD2_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGD3_WORD2_M0SM_MASK    0x10u
#define  MPU_RGD3_WORD2_M0SM_SHIFT   3
#define  MPU_RGD3_WORD2_M0SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD3_WORD2_M0SM_SHIFT))&MPU_RGD3_WORD2_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGD3_WORD2_M1UM_MASK    0xC0u
#define  MPU_RGD3_WORD2_M1UM_SHIFT   6
#define  MPU_RGD3_WORD2_M1UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD3_WORD2_M1UM_SHIFT))&MPU_RGD3_WORD2_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGD3_WORD2_M1SM_MASK    0x400u
#define  MPU_RGD3_WORD2_M1SM_SHIFT   9
#define  MPU_RGD3_WORD2_M1SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD3_WORD2_M1SM_SHIFT))&MPU_RGD3_WORD2_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGD3_WORD2_M2UM_MASK    0x3000u
#define  MPU_RGD3_WORD2_M2UM_SHIFT   12
#define  MPU_RGD3_WORD2_M2UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD3_WORD2_M2UM_SHIFT))&MPU_RGD3_WORD2_M2UM_MASK)
//   Bus Master 2 User Mode Access control

#define  MPU_RGD3_WORD2_M2SM_MASK    0x10000u
#define  MPU_RGD3_WORD2_M2SM_SHIFT   15
#define  MPU_RGD3_WORD2_M2SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD3_WORD2_M2SM_SHIFT))&MPU_RGD3_WORD2_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGD3_WORD2_M3UM_MASK    0xC0000u
#define  MPU_RGD3_WORD2_M3UM_SHIFT   18
#define  MPU_RGD3_WORD2_M3UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD3_WORD2_M3UM_SHIFT))&MPU_RGD3_WORD2_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGD3_WORD2_M3SM_MASK    0x400000u
#define  MPU_RGD3_WORD2_M3SM_SHIFT   21
#define  MPU_RGD3_WORD2_M3SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD3_WORD2_M3SM_SHIFT))&MPU_RGD3_WORD2_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGD3_WORD2_M4WE_MASK    0x1000000u
#define  MPU_RGD3_WORD2_M4WE_SHIFT   24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGD3_WORD2_M4RE_MASK    0x2000000u
#define  MPU_RGD3_WORD2_M4RE_SHIFT   25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGD3_WORD2_M5WE_MASK    0x4000000u
#define  MPU_RGD3_WORD2_M5WE_SHIFT   26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGD3_WORD2_M5RE_MASK    0x8000000u
#define  MPU_RGD3_WORD2_M5RE_SHIFT   27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGD3_WORD2_M6WE_MASK    0x10000000u
#define  MPU_RGD3_WORD2_M6WE_SHIFT   28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGD3_WORD2_M6RE_MASK    0x20000000u
#define  MPU_RGD3_WORD2_M6RE_SHIFT   29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGD3_WORD2_M7WE_MASK    0x40000000u
#define  MPU_RGD3_WORD2_M7WE_SHIFT   30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGD3_WORD2_M7RE_MASK    0x80000000u
#define  MPU_RGD3_WORD2_M7RE_SHIFT   31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MPU_RGD3_WORD3_VLD_MASK     0x1u
#define  MPU_RGD3_WORD3_VLD_SHIFT    0
//   Valid
//     0b0 ==> Region descriptor is invalid
//     0b1 ==> Region descriptor is valid

#define  MPU_RGD4_WORD0_SRTADDR_MASK 0x360u
#define  MPU_RGD4_WORD0_SRTADDR_SHIFT 5
#define  MPU_RGD4_WORD0_SRTADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD4_WORD0_SRTADDR_SHIFT))&MPU_RGD4_WORD0_SRTADDR_MASK)
//   Start Address

#define  MPU_RGD4_WORD1_ENDADDR_MASK 0x360u
#define  MPU_RGD4_WORD1_ENDADDR_SHIFT 5
#define  MPU_RGD4_WORD1_ENDADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD4_WORD1_ENDADDR_SHIFT))&MPU_RGD4_WORD1_ENDADDR_MASK)
//   End Address

#define  MPU_RGD4_WORD2_M0UM_MASK    0x3u
#define  MPU_RGD4_WORD2_M0UM_SHIFT   0
#define  MPU_RGD4_WORD2_M0UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD4_WORD2_M0UM_SHIFT))&MPU_RGD4_WORD2_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGD4_WORD2_M0SM_MASK    0x10u
#define  MPU_RGD4_WORD2_M0SM_SHIFT   3
#define  MPU_RGD4_WORD2_M0SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD4_WORD2_M0SM_SHIFT))&MPU_RGD4_WORD2_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGD4_WORD2_M1UM_MASK    0xC0u
#define  MPU_RGD4_WORD2_M1UM_SHIFT   6
#define  MPU_RGD4_WORD2_M1UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD4_WORD2_M1UM_SHIFT))&MPU_RGD4_WORD2_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGD4_WORD2_M1SM_MASK    0x400u
#define  MPU_RGD4_WORD2_M1SM_SHIFT   9
#define  MPU_RGD4_WORD2_M1SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD4_WORD2_M1SM_SHIFT))&MPU_RGD4_WORD2_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGD4_WORD2_M2UM_MASK    0x3000u
#define  MPU_RGD4_WORD2_M2UM_SHIFT   12
#define  MPU_RGD4_WORD2_M2UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD4_WORD2_M2UM_SHIFT))&MPU_RGD4_WORD2_M2UM_MASK)
//   Bus Master 2 User Mode Access control

#define  MPU_RGD4_WORD2_M2SM_MASK    0x10000u
#define  MPU_RGD4_WORD2_M2SM_SHIFT   15
#define  MPU_RGD4_WORD2_M2SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD4_WORD2_M2SM_SHIFT))&MPU_RGD4_WORD2_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGD4_WORD2_M3UM_MASK    0xC0000u
#define  MPU_RGD4_WORD2_M3UM_SHIFT   18
#define  MPU_RGD4_WORD2_M3UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD4_WORD2_M3UM_SHIFT))&MPU_RGD4_WORD2_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGD4_WORD2_M3SM_MASK    0x400000u
#define  MPU_RGD4_WORD2_M3SM_SHIFT   21
#define  MPU_RGD4_WORD2_M3SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD4_WORD2_M3SM_SHIFT))&MPU_RGD4_WORD2_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGD4_WORD2_M4WE_MASK    0x1000000u
#define  MPU_RGD4_WORD2_M4WE_SHIFT   24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGD4_WORD2_M4RE_MASK    0x2000000u
#define  MPU_RGD4_WORD2_M4RE_SHIFT   25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGD4_WORD2_M5WE_MASK    0x4000000u
#define  MPU_RGD4_WORD2_M5WE_SHIFT   26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGD4_WORD2_M5RE_MASK    0x8000000u
#define  MPU_RGD4_WORD2_M5RE_SHIFT   27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGD4_WORD2_M6WE_MASK    0x10000000u
#define  MPU_RGD4_WORD2_M6WE_SHIFT   28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGD4_WORD2_M6RE_MASK    0x20000000u
#define  MPU_RGD4_WORD2_M6RE_SHIFT   29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGD4_WORD2_M7WE_MASK    0x40000000u
#define  MPU_RGD4_WORD2_M7WE_SHIFT   30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGD4_WORD2_M7RE_MASK    0x80000000u
#define  MPU_RGD4_WORD2_M7RE_SHIFT   31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MPU_RGD4_WORD3_VLD_MASK     0x1u
#define  MPU_RGD4_WORD3_VLD_SHIFT    0
//   Valid
//     0b0 ==> Region descriptor is invalid
//     0b1 ==> Region descriptor is valid

#define  MPU_RGD5_WORD0_SRTADDR_MASK 0x360u
#define  MPU_RGD5_WORD0_SRTADDR_SHIFT 5
#define  MPU_RGD5_WORD0_SRTADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD5_WORD0_SRTADDR_SHIFT))&MPU_RGD5_WORD0_SRTADDR_MASK)
//   Start Address

#define  MPU_RGD5_WORD1_ENDADDR_MASK 0x360u
#define  MPU_RGD5_WORD1_ENDADDR_SHIFT 5
#define  MPU_RGD5_WORD1_ENDADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD5_WORD1_ENDADDR_SHIFT))&MPU_RGD5_WORD1_ENDADDR_MASK)
//   End Address

#define  MPU_RGD5_WORD2_M0UM_MASK    0x3u
#define  MPU_RGD5_WORD2_M0UM_SHIFT   0
#define  MPU_RGD5_WORD2_M0UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD5_WORD2_M0UM_SHIFT))&MPU_RGD5_WORD2_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGD5_WORD2_M0SM_MASK    0x10u
#define  MPU_RGD5_WORD2_M0SM_SHIFT   3
#define  MPU_RGD5_WORD2_M0SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD5_WORD2_M0SM_SHIFT))&MPU_RGD5_WORD2_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGD5_WORD2_M1UM_MASK    0xC0u
#define  MPU_RGD5_WORD2_M1UM_SHIFT   6
#define  MPU_RGD5_WORD2_M1UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD5_WORD2_M1UM_SHIFT))&MPU_RGD5_WORD2_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGD5_WORD2_M1SM_MASK    0x400u
#define  MPU_RGD5_WORD2_M1SM_SHIFT   9
#define  MPU_RGD5_WORD2_M1SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD5_WORD2_M1SM_SHIFT))&MPU_RGD5_WORD2_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGD5_WORD2_M2UM_MASK    0x3000u
#define  MPU_RGD5_WORD2_M2UM_SHIFT   12
#define  MPU_RGD5_WORD2_M2UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD5_WORD2_M2UM_SHIFT))&MPU_RGD5_WORD2_M2UM_MASK)
//   Bus Master 2 User Mode Access control

#define  MPU_RGD5_WORD2_M2SM_MASK    0x10000u
#define  MPU_RGD5_WORD2_M2SM_SHIFT   15
#define  MPU_RGD5_WORD2_M2SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD5_WORD2_M2SM_SHIFT))&MPU_RGD5_WORD2_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGD5_WORD2_M3UM_MASK    0xC0000u
#define  MPU_RGD5_WORD2_M3UM_SHIFT   18
#define  MPU_RGD5_WORD2_M3UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD5_WORD2_M3UM_SHIFT))&MPU_RGD5_WORD2_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGD5_WORD2_M3SM_MASK    0x400000u
#define  MPU_RGD5_WORD2_M3SM_SHIFT   21
#define  MPU_RGD5_WORD2_M3SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD5_WORD2_M3SM_SHIFT))&MPU_RGD5_WORD2_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGD5_WORD2_M4WE_MASK    0x1000000u
#define  MPU_RGD5_WORD2_M4WE_SHIFT   24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGD5_WORD2_M4RE_MASK    0x2000000u
#define  MPU_RGD5_WORD2_M4RE_SHIFT   25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGD5_WORD2_M5WE_MASK    0x4000000u
#define  MPU_RGD5_WORD2_M5WE_SHIFT   26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGD5_WORD2_M5RE_MASK    0x8000000u
#define  MPU_RGD5_WORD2_M5RE_SHIFT   27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGD5_WORD2_M6WE_MASK    0x10000000u
#define  MPU_RGD5_WORD2_M6WE_SHIFT   28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGD5_WORD2_M6RE_MASK    0x20000000u
#define  MPU_RGD5_WORD2_M6RE_SHIFT   29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGD5_WORD2_M7WE_MASK    0x40000000u
#define  MPU_RGD5_WORD2_M7WE_SHIFT   30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGD5_WORD2_M7RE_MASK    0x80000000u
#define  MPU_RGD5_WORD2_M7RE_SHIFT   31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MPU_RGD5_WORD3_VLD_MASK     0x1u
#define  MPU_RGD5_WORD3_VLD_SHIFT    0
//   Valid
//     0b0 ==> Region descriptor is invalid
//     0b1 ==> Region descriptor is valid

#define  MPU_RGD6_WORD0_SRTADDR_MASK 0x360u
#define  MPU_RGD6_WORD0_SRTADDR_SHIFT 5
#define  MPU_RGD6_WORD0_SRTADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD6_WORD0_SRTADDR_SHIFT))&MPU_RGD6_WORD0_SRTADDR_MASK)
//   Start Address

#define  MPU_RGD6_WORD1_ENDADDR_MASK 0x360u
#define  MPU_RGD6_WORD1_ENDADDR_SHIFT 5
#define  MPU_RGD6_WORD1_ENDADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD6_WORD1_ENDADDR_SHIFT))&MPU_RGD6_WORD1_ENDADDR_MASK)
//   End Address

#define  MPU_RGD6_WORD2_M0UM_MASK    0x3u
#define  MPU_RGD6_WORD2_M0UM_SHIFT   0
#define  MPU_RGD6_WORD2_M0UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD6_WORD2_M0UM_SHIFT))&MPU_RGD6_WORD2_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGD6_WORD2_M0SM_MASK    0x10u
#define  MPU_RGD6_WORD2_M0SM_SHIFT   3
#define  MPU_RGD6_WORD2_M0SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD6_WORD2_M0SM_SHIFT))&MPU_RGD6_WORD2_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGD6_WORD2_M1UM_MASK    0xC0u
#define  MPU_RGD6_WORD2_M1UM_SHIFT   6
#define  MPU_RGD6_WORD2_M1UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD6_WORD2_M1UM_SHIFT))&MPU_RGD6_WORD2_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGD6_WORD2_M1SM_MASK    0x400u
#define  MPU_RGD6_WORD2_M1SM_SHIFT   9
#define  MPU_RGD6_WORD2_M1SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD6_WORD2_M1SM_SHIFT))&MPU_RGD6_WORD2_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGD6_WORD2_M2UM_MASK    0x3000u
#define  MPU_RGD6_WORD2_M2UM_SHIFT   12
#define  MPU_RGD6_WORD2_M2UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD6_WORD2_M2UM_SHIFT))&MPU_RGD6_WORD2_M2UM_MASK)
//   Bus Master 2 User Mode Access control

#define  MPU_RGD6_WORD2_M2SM_MASK    0x10000u
#define  MPU_RGD6_WORD2_M2SM_SHIFT   15
#define  MPU_RGD6_WORD2_M2SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD6_WORD2_M2SM_SHIFT))&MPU_RGD6_WORD2_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGD6_WORD2_M3UM_MASK    0xC0000u
#define  MPU_RGD6_WORD2_M3UM_SHIFT   18
#define  MPU_RGD6_WORD2_M3UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD6_WORD2_M3UM_SHIFT))&MPU_RGD6_WORD2_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGD6_WORD2_M3SM_MASK    0x400000u
#define  MPU_RGD6_WORD2_M3SM_SHIFT   21
#define  MPU_RGD6_WORD2_M3SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD6_WORD2_M3SM_SHIFT))&MPU_RGD6_WORD2_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGD6_WORD2_M4WE_MASK    0x1000000u
#define  MPU_RGD6_WORD2_M4WE_SHIFT   24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGD6_WORD2_M4RE_MASK    0x2000000u
#define  MPU_RGD6_WORD2_M4RE_SHIFT   25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGD6_WORD2_M5WE_MASK    0x4000000u
#define  MPU_RGD6_WORD2_M5WE_SHIFT   26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGD6_WORD2_M5RE_MASK    0x8000000u
#define  MPU_RGD6_WORD2_M5RE_SHIFT   27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGD6_WORD2_M6WE_MASK    0x10000000u
#define  MPU_RGD6_WORD2_M6WE_SHIFT   28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGD6_WORD2_M6RE_MASK    0x20000000u
#define  MPU_RGD6_WORD2_M6RE_SHIFT   29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGD6_WORD2_M7WE_MASK    0x40000000u
#define  MPU_RGD6_WORD2_M7WE_SHIFT   30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGD6_WORD2_M7RE_MASK    0x80000000u
#define  MPU_RGD6_WORD2_M7RE_SHIFT   31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MPU_RGD6_WORD3_VLD_MASK     0x1u
#define  MPU_RGD6_WORD3_VLD_SHIFT    0
//   Valid
//     0b0 ==> Region descriptor is invalid
//     0b1 ==> Region descriptor is valid

#define  MPU_RGD7_WORD0_SRTADDR_MASK 0x360u
#define  MPU_RGD7_WORD0_SRTADDR_SHIFT 5
#define  MPU_RGD7_WORD0_SRTADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD7_WORD0_SRTADDR_SHIFT))&MPU_RGD7_WORD0_SRTADDR_MASK)
//   Start Address

#define  MPU_RGD7_WORD1_ENDADDR_MASK 0x360u
#define  MPU_RGD7_WORD1_ENDADDR_SHIFT 5
#define  MPU_RGD7_WORD1_ENDADDR(x)   (((uint32_t)(((uint32_t)(x))<<MPU_RGD7_WORD1_ENDADDR_SHIFT))&MPU_RGD7_WORD1_ENDADDR_MASK)
//   End Address

#define  MPU_RGD7_WORD2_M0UM_MASK    0x3u
#define  MPU_RGD7_WORD2_M0UM_SHIFT   0
#define  MPU_RGD7_WORD2_M0UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD7_WORD2_M0UM_SHIFT))&MPU_RGD7_WORD2_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGD7_WORD2_M0SM_MASK    0x10u
#define  MPU_RGD7_WORD2_M0SM_SHIFT   3
#define  MPU_RGD7_WORD2_M0SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD7_WORD2_M0SM_SHIFT))&MPU_RGD7_WORD2_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGD7_WORD2_M1UM_MASK    0xC0u
#define  MPU_RGD7_WORD2_M1UM_SHIFT   6
#define  MPU_RGD7_WORD2_M1UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD7_WORD2_M1UM_SHIFT))&MPU_RGD7_WORD2_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGD7_WORD2_M1SM_MASK    0x400u
#define  MPU_RGD7_WORD2_M1SM_SHIFT   9
#define  MPU_RGD7_WORD2_M1SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD7_WORD2_M1SM_SHIFT))&MPU_RGD7_WORD2_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGD7_WORD2_M2UM_MASK    0x3000u
#define  MPU_RGD7_WORD2_M2UM_SHIFT   12
#define  MPU_RGD7_WORD2_M2UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD7_WORD2_M2UM_SHIFT))&MPU_RGD7_WORD2_M2UM_MASK)
//   Bus Master 2 User Mode Access control

#define  MPU_RGD7_WORD2_M2SM_MASK    0x10000u
#define  MPU_RGD7_WORD2_M2SM_SHIFT   15
#define  MPU_RGD7_WORD2_M2SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD7_WORD2_M2SM_SHIFT))&MPU_RGD7_WORD2_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGD7_WORD2_M3UM_MASK    0xC0000u
#define  MPU_RGD7_WORD2_M3UM_SHIFT   18
#define  MPU_RGD7_WORD2_M3UM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD7_WORD2_M3UM_SHIFT))&MPU_RGD7_WORD2_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGD7_WORD2_M3SM_MASK    0x400000u
#define  MPU_RGD7_WORD2_M3SM_SHIFT   21
#define  MPU_RGD7_WORD2_M3SM(x)      (((uint32_t)(((uint32_t)(x))<<MPU_RGD7_WORD2_M3SM_SHIFT))&MPU_RGD7_WORD2_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGD7_WORD2_M4WE_MASK    0x1000000u
#define  MPU_RGD7_WORD2_M4WE_SHIFT   24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGD7_WORD2_M4RE_MASK    0x2000000u
#define  MPU_RGD7_WORD2_M4RE_SHIFT   25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGD7_WORD2_M5WE_MASK    0x4000000u
#define  MPU_RGD7_WORD2_M5WE_SHIFT   26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGD7_WORD2_M5RE_MASK    0x8000000u
#define  MPU_RGD7_WORD2_M5RE_SHIFT   27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGD7_WORD2_M6WE_MASK    0x10000000u
#define  MPU_RGD7_WORD2_M6WE_SHIFT   28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGD7_WORD2_M6RE_MASK    0x20000000u
#define  MPU_RGD7_WORD2_M6RE_SHIFT   29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGD7_WORD2_M7WE_MASK    0x40000000u
#define  MPU_RGD7_WORD2_M7WE_SHIFT   30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGD7_WORD2_M7RE_MASK    0x80000000u
#define  MPU_RGD7_WORD2_M7RE_SHIFT   31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MPU_RGD7_WORD3_VLD_MASK     0x1u
#define  MPU_RGD7_WORD3_VLD_SHIFT    0
//   Valid
//     0b0 ==> Region descriptor is invalid
//     0b1 ==> Region descriptor is valid

#define  MPU_RGDAAC0_M0UM_MASK       0x3u
#define  MPU_RGDAAC0_M0UM_SHIFT      0
#define  MPU_RGDAAC0_M0UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC0_M0UM_SHIFT))&MPU_RGDAAC0_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGDAAC0_M0SM_MASK       0x10u
#define  MPU_RGDAAC0_M0SM_SHIFT      3
#define  MPU_RGDAAC0_M0SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC0_M0SM_SHIFT))&MPU_RGDAAC0_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGDAAC0_M1UM_MASK       0xC0u
#define  MPU_RGDAAC0_M1UM_SHIFT      6
#define  MPU_RGDAAC0_M1UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC0_M1UM_SHIFT))&MPU_RGDAAC0_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGDAAC0_M1SM_MASK       0x400u
#define  MPU_RGDAAC0_M1SM_SHIFT      9
#define  MPU_RGDAAC0_M1SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC0_M1SM_SHIFT))&MPU_RGDAAC0_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGDAAC0_M2UM_MASK       0x3000u
#define  MPU_RGDAAC0_M2UM_SHIFT      12
#define  MPU_RGDAAC0_M2UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC0_M2UM_SHIFT))&MPU_RGDAAC0_M2UM_MASK)
//   Bus Master 2 User Mode Access Control

#define  MPU_RGDAAC0_M2SM_MASK       0x10000u
#define  MPU_RGDAAC0_M2SM_SHIFT      15
#define  MPU_RGDAAC0_M2SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC0_M2SM_SHIFT))&MPU_RGDAAC0_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGDAAC0_M3UM_MASK       0xC0000u
#define  MPU_RGDAAC0_M3UM_SHIFT      18
#define  MPU_RGDAAC0_M3UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC0_M3UM_SHIFT))&MPU_RGDAAC0_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGDAAC0_M3SM_MASK       0x400000u
#define  MPU_RGDAAC0_M3SM_SHIFT      21
#define  MPU_RGDAAC0_M3SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC0_M3SM_SHIFT))&MPU_RGDAAC0_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGDAAC0_M4WE_MASK       0x1000000u
#define  MPU_RGDAAC0_M4WE_SHIFT      24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGDAAC0_M4RE_MASK       0x2000000u
#define  MPU_RGDAAC0_M4RE_SHIFT      25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGDAAC0_M5WE_MASK       0x4000000u
#define  MPU_RGDAAC0_M5WE_SHIFT      26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGDAAC0_M5RE_MASK       0x8000000u
#define  MPU_RGDAAC0_M5RE_SHIFT      27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGDAAC0_M6WE_MASK       0x10000000u
#define  MPU_RGDAAC0_M6WE_SHIFT      28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGDAAC0_M6RE_MASK       0x20000000u
#define  MPU_RGDAAC0_M6RE_SHIFT      29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGDAAC0_M7WE_MASK       0x40000000u
#define  MPU_RGDAAC0_M7WE_SHIFT      30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGDAAC0_M7RE_MASK       0x80000000u
#define  MPU_RGDAAC0_M7RE_SHIFT      31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MPU_RGDAAC1_M0UM_MASK       0x3u
#define  MPU_RGDAAC1_M0UM_SHIFT      0
#define  MPU_RGDAAC1_M0UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC1_M0UM_SHIFT))&MPU_RGDAAC1_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGDAAC1_M0SM_MASK       0x10u
#define  MPU_RGDAAC1_M0SM_SHIFT      3
#define  MPU_RGDAAC1_M0SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC1_M0SM_SHIFT))&MPU_RGDAAC1_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGDAAC1_M1UM_MASK       0xC0u
#define  MPU_RGDAAC1_M1UM_SHIFT      6
#define  MPU_RGDAAC1_M1UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC1_M1UM_SHIFT))&MPU_RGDAAC1_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGDAAC1_M1SM_MASK       0x400u
#define  MPU_RGDAAC1_M1SM_SHIFT      9
#define  MPU_RGDAAC1_M1SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC1_M1SM_SHIFT))&MPU_RGDAAC1_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGDAAC1_M2UM_MASK       0x3000u
#define  MPU_RGDAAC1_M2UM_SHIFT      12
#define  MPU_RGDAAC1_M2UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC1_M2UM_SHIFT))&MPU_RGDAAC1_M2UM_MASK)
//   Bus Master 2 User Mode Access Control

#define  MPU_RGDAAC1_M2SM_MASK       0x10000u
#define  MPU_RGDAAC1_M2SM_SHIFT      15
#define  MPU_RGDAAC1_M2SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC1_M2SM_SHIFT))&MPU_RGDAAC1_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGDAAC1_M3UM_MASK       0xC0000u
#define  MPU_RGDAAC1_M3UM_SHIFT      18
#define  MPU_RGDAAC1_M3UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC1_M3UM_SHIFT))&MPU_RGDAAC1_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGDAAC1_M3SM_MASK       0x400000u
#define  MPU_RGDAAC1_M3SM_SHIFT      21
#define  MPU_RGDAAC1_M3SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC1_M3SM_SHIFT))&MPU_RGDAAC1_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGDAAC1_M4WE_MASK       0x1000000u
#define  MPU_RGDAAC1_M4WE_SHIFT      24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGDAAC1_M4RE_MASK       0x2000000u
#define  MPU_RGDAAC1_M4RE_SHIFT      25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGDAAC1_M5WE_MASK       0x4000000u
#define  MPU_RGDAAC1_M5WE_SHIFT      26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGDAAC1_M5RE_MASK       0x8000000u
#define  MPU_RGDAAC1_M5RE_SHIFT      27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGDAAC1_M6WE_MASK       0x10000000u
#define  MPU_RGDAAC1_M6WE_SHIFT      28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGDAAC1_M6RE_MASK       0x20000000u
#define  MPU_RGDAAC1_M6RE_SHIFT      29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGDAAC1_M7WE_MASK       0x40000000u
#define  MPU_RGDAAC1_M7WE_SHIFT      30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGDAAC1_M7RE_MASK       0x80000000u
#define  MPU_RGDAAC1_M7RE_SHIFT      31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MPU_RGDAAC2_M0UM_MASK       0x3u
#define  MPU_RGDAAC2_M0UM_SHIFT      0
#define  MPU_RGDAAC2_M0UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC2_M0UM_SHIFT))&MPU_RGDAAC2_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGDAAC2_M0SM_MASK       0x10u
#define  MPU_RGDAAC2_M0SM_SHIFT      3
#define  MPU_RGDAAC2_M0SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC2_M0SM_SHIFT))&MPU_RGDAAC2_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGDAAC2_M1UM_MASK       0xC0u
#define  MPU_RGDAAC2_M1UM_SHIFT      6
#define  MPU_RGDAAC2_M1UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC2_M1UM_SHIFT))&MPU_RGDAAC2_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGDAAC2_M1SM_MASK       0x400u
#define  MPU_RGDAAC2_M1SM_SHIFT      9
#define  MPU_RGDAAC2_M1SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC2_M1SM_SHIFT))&MPU_RGDAAC2_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGDAAC2_M2UM_MASK       0x3000u
#define  MPU_RGDAAC2_M2UM_SHIFT      12
#define  MPU_RGDAAC2_M2UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC2_M2UM_SHIFT))&MPU_RGDAAC2_M2UM_MASK)
//   Bus Master 2 User Mode Access Control

#define  MPU_RGDAAC2_M2SM_MASK       0x10000u
#define  MPU_RGDAAC2_M2SM_SHIFT      15
#define  MPU_RGDAAC2_M2SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC2_M2SM_SHIFT))&MPU_RGDAAC2_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGDAAC2_M3UM_MASK       0xC0000u
#define  MPU_RGDAAC2_M3UM_SHIFT      18
#define  MPU_RGDAAC2_M3UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC2_M3UM_SHIFT))&MPU_RGDAAC2_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGDAAC2_M3SM_MASK       0x400000u
#define  MPU_RGDAAC2_M3SM_SHIFT      21
#define  MPU_RGDAAC2_M3SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC2_M3SM_SHIFT))&MPU_RGDAAC2_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGDAAC2_M4WE_MASK       0x1000000u
#define  MPU_RGDAAC2_M4WE_SHIFT      24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGDAAC2_M4RE_MASK       0x2000000u
#define  MPU_RGDAAC2_M4RE_SHIFT      25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGDAAC2_M5WE_MASK       0x4000000u
#define  MPU_RGDAAC2_M5WE_SHIFT      26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGDAAC2_M5RE_MASK       0x8000000u
#define  MPU_RGDAAC2_M5RE_SHIFT      27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGDAAC2_M6WE_MASK       0x10000000u
#define  MPU_RGDAAC2_M6WE_SHIFT      28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGDAAC2_M6RE_MASK       0x20000000u
#define  MPU_RGDAAC2_M6RE_SHIFT      29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGDAAC2_M7WE_MASK       0x40000000u
#define  MPU_RGDAAC2_M7WE_SHIFT      30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGDAAC2_M7RE_MASK       0x80000000u
#define  MPU_RGDAAC2_M7RE_SHIFT      31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MPU_RGDAAC3_M0UM_MASK       0x3u
#define  MPU_RGDAAC3_M0UM_SHIFT      0
#define  MPU_RGDAAC3_M0UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC3_M0UM_SHIFT))&MPU_RGDAAC3_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGDAAC3_M0SM_MASK       0x10u
#define  MPU_RGDAAC3_M0SM_SHIFT      3
#define  MPU_RGDAAC3_M0SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC3_M0SM_SHIFT))&MPU_RGDAAC3_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGDAAC3_M1UM_MASK       0xC0u
#define  MPU_RGDAAC3_M1UM_SHIFT      6
#define  MPU_RGDAAC3_M1UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC3_M1UM_SHIFT))&MPU_RGDAAC3_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGDAAC3_M1SM_MASK       0x400u
#define  MPU_RGDAAC3_M1SM_SHIFT      9
#define  MPU_RGDAAC3_M1SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC3_M1SM_SHIFT))&MPU_RGDAAC3_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGDAAC3_M2UM_MASK       0x3000u
#define  MPU_RGDAAC3_M2UM_SHIFT      12
#define  MPU_RGDAAC3_M2UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC3_M2UM_SHIFT))&MPU_RGDAAC3_M2UM_MASK)
//   Bus Master 2 User Mode Access Control

#define  MPU_RGDAAC3_M2SM_MASK       0x10000u
#define  MPU_RGDAAC3_M2SM_SHIFT      15
#define  MPU_RGDAAC3_M2SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC3_M2SM_SHIFT))&MPU_RGDAAC3_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGDAAC3_M3UM_MASK       0xC0000u
#define  MPU_RGDAAC3_M3UM_SHIFT      18
#define  MPU_RGDAAC3_M3UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC3_M3UM_SHIFT))&MPU_RGDAAC3_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGDAAC3_M3SM_MASK       0x400000u
#define  MPU_RGDAAC3_M3SM_SHIFT      21
#define  MPU_RGDAAC3_M3SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC3_M3SM_SHIFT))&MPU_RGDAAC3_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGDAAC3_M4WE_MASK       0x1000000u
#define  MPU_RGDAAC3_M4WE_SHIFT      24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGDAAC3_M4RE_MASK       0x2000000u
#define  MPU_RGDAAC3_M4RE_SHIFT      25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGDAAC3_M5WE_MASK       0x4000000u
#define  MPU_RGDAAC3_M5WE_SHIFT      26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGDAAC3_M5RE_MASK       0x8000000u
#define  MPU_RGDAAC3_M5RE_SHIFT      27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGDAAC3_M6WE_MASK       0x10000000u
#define  MPU_RGDAAC3_M6WE_SHIFT      28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGDAAC3_M6RE_MASK       0x20000000u
#define  MPU_RGDAAC3_M6RE_SHIFT      29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGDAAC3_M7WE_MASK       0x40000000u
#define  MPU_RGDAAC3_M7WE_SHIFT      30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGDAAC3_M7RE_MASK       0x80000000u
#define  MPU_RGDAAC3_M7RE_SHIFT      31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MPU_RGDAAC4_M0UM_MASK       0x3u
#define  MPU_RGDAAC4_M0UM_SHIFT      0
#define  MPU_RGDAAC4_M0UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC4_M0UM_SHIFT))&MPU_RGDAAC4_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGDAAC4_M0SM_MASK       0x10u
#define  MPU_RGDAAC4_M0SM_SHIFT      3
#define  MPU_RGDAAC4_M0SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC4_M0SM_SHIFT))&MPU_RGDAAC4_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGDAAC4_M1UM_MASK       0xC0u
#define  MPU_RGDAAC4_M1UM_SHIFT      6
#define  MPU_RGDAAC4_M1UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC4_M1UM_SHIFT))&MPU_RGDAAC4_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGDAAC4_M1SM_MASK       0x400u
#define  MPU_RGDAAC4_M1SM_SHIFT      9
#define  MPU_RGDAAC4_M1SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC4_M1SM_SHIFT))&MPU_RGDAAC4_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGDAAC4_M2UM_MASK       0x3000u
#define  MPU_RGDAAC4_M2UM_SHIFT      12
#define  MPU_RGDAAC4_M2UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC4_M2UM_SHIFT))&MPU_RGDAAC4_M2UM_MASK)
//   Bus Master 2 User Mode Access Control

#define  MPU_RGDAAC4_M2SM_MASK       0x10000u
#define  MPU_RGDAAC4_M2SM_SHIFT      15
#define  MPU_RGDAAC4_M2SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC4_M2SM_SHIFT))&MPU_RGDAAC4_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGDAAC4_M3UM_MASK       0xC0000u
#define  MPU_RGDAAC4_M3UM_SHIFT      18
#define  MPU_RGDAAC4_M3UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC4_M3UM_SHIFT))&MPU_RGDAAC4_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGDAAC4_M3SM_MASK       0x400000u
#define  MPU_RGDAAC4_M3SM_SHIFT      21
#define  MPU_RGDAAC4_M3SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC4_M3SM_SHIFT))&MPU_RGDAAC4_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGDAAC4_M4WE_MASK       0x1000000u
#define  MPU_RGDAAC4_M4WE_SHIFT      24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGDAAC4_M4RE_MASK       0x2000000u
#define  MPU_RGDAAC4_M4RE_SHIFT      25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGDAAC4_M5WE_MASK       0x4000000u
#define  MPU_RGDAAC4_M5WE_SHIFT      26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGDAAC4_M5RE_MASK       0x8000000u
#define  MPU_RGDAAC4_M5RE_SHIFT      27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGDAAC4_M6WE_MASK       0x10000000u
#define  MPU_RGDAAC4_M6WE_SHIFT      28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGDAAC4_M6RE_MASK       0x20000000u
#define  MPU_RGDAAC4_M6RE_SHIFT      29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGDAAC4_M7WE_MASK       0x40000000u
#define  MPU_RGDAAC4_M7WE_SHIFT      30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGDAAC4_M7RE_MASK       0x80000000u
#define  MPU_RGDAAC4_M7RE_SHIFT      31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MPU_RGDAAC5_M0UM_MASK       0x3u
#define  MPU_RGDAAC5_M0UM_SHIFT      0
#define  MPU_RGDAAC5_M0UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC5_M0UM_SHIFT))&MPU_RGDAAC5_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGDAAC5_M0SM_MASK       0x10u
#define  MPU_RGDAAC5_M0SM_SHIFT      3
#define  MPU_RGDAAC5_M0SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC5_M0SM_SHIFT))&MPU_RGDAAC5_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGDAAC5_M1UM_MASK       0xC0u
#define  MPU_RGDAAC5_M1UM_SHIFT      6
#define  MPU_RGDAAC5_M1UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC5_M1UM_SHIFT))&MPU_RGDAAC5_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGDAAC5_M1SM_MASK       0x400u
#define  MPU_RGDAAC5_M1SM_SHIFT      9
#define  MPU_RGDAAC5_M1SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC5_M1SM_SHIFT))&MPU_RGDAAC5_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGDAAC5_M2UM_MASK       0x3000u
#define  MPU_RGDAAC5_M2UM_SHIFT      12
#define  MPU_RGDAAC5_M2UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC5_M2UM_SHIFT))&MPU_RGDAAC5_M2UM_MASK)
//   Bus Master 2 User Mode Access Control

#define  MPU_RGDAAC5_M2SM_MASK       0x10000u
#define  MPU_RGDAAC5_M2SM_SHIFT      15
#define  MPU_RGDAAC5_M2SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC5_M2SM_SHIFT))&MPU_RGDAAC5_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGDAAC5_M3UM_MASK       0xC0000u
#define  MPU_RGDAAC5_M3UM_SHIFT      18
#define  MPU_RGDAAC5_M3UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC5_M3UM_SHIFT))&MPU_RGDAAC5_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGDAAC5_M3SM_MASK       0x400000u
#define  MPU_RGDAAC5_M3SM_SHIFT      21
#define  MPU_RGDAAC5_M3SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC5_M3SM_SHIFT))&MPU_RGDAAC5_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGDAAC5_M4WE_MASK       0x1000000u
#define  MPU_RGDAAC5_M4WE_SHIFT      24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGDAAC5_M4RE_MASK       0x2000000u
#define  MPU_RGDAAC5_M4RE_SHIFT      25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGDAAC5_M5WE_MASK       0x4000000u
#define  MPU_RGDAAC5_M5WE_SHIFT      26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGDAAC5_M5RE_MASK       0x8000000u
#define  MPU_RGDAAC5_M5RE_SHIFT      27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGDAAC5_M6WE_MASK       0x10000000u
#define  MPU_RGDAAC5_M6WE_SHIFT      28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGDAAC5_M6RE_MASK       0x20000000u
#define  MPU_RGDAAC5_M6RE_SHIFT      29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGDAAC5_M7WE_MASK       0x40000000u
#define  MPU_RGDAAC5_M7WE_SHIFT      30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGDAAC5_M7RE_MASK       0x80000000u
#define  MPU_RGDAAC5_M7RE_SHIFT      31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MPU_RGDAAC6_M0UM_MASK       0x3u
#define  MPU_RGDAAC6_M0UM_SHIFT      0
#define  MPU_RGDAAC6_M0UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC6_M0UM_SHIFT))&MPU_RGDAAC6_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGDAAC6_M0SM_MASK       0x10u
#define  MPU_RGDAAC6_M0SM_SHIFT      3
#define  MPU_RGDAAC6_M0SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC6_M0SM_SHIFT))&MPU_RGDAAC6_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGDAAC6_M1UM_MASK       0xC0u
#define  MPU_RGDAAC6_M1UM_SHIFT      6
#define  MPU_RGDAAC6_M1UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC6_M1UM_SHIFT))&MPU_RGDAAC6_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGDAAC6_M1SM_MASK       0x400u
#define  MPU_RGDAAC6_M1SM_SHIFT      9
#define  MPU_RGDAAC6_M1SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC6_M1SM_SHIFT))&MPU_RGDAAC6_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGDAAC6_M2UM_MASK       0x3000u
#define  MPU_RGDAAC6_M2UM_SHIFT      12
#define  MPU_RGDAAC6_M2UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC6_M2UM_SHIFT))&MPU_RGDAAC6_M2UM_MASK)
//   Bus Master 2 User Mode Access Control

#define  MPU_RGDAAC6_M2SM_MASK       0x10000u
#define  MPU_RGDAAC6_M2SM_SHIFT      15
#define  MPU_RGDAAC6_M2SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC6_M2SM_SHIFT))&MPU_RGDAAC6_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGDAAC6_M3UM_MASK       0xC0000u
#define  MPU_RGDAAC6_M3UM_SHIFT      18
#define  MPU_RGDAAC6_M3UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC6_M3UM_SHIFT))&MPU_RGDAAC6_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGDAAC6_M3SM_MASK       0x400000u
#define  MPU_RGDAAC6_M3SM_SHIFT      21
#define  MPU_RGDAAC6_M3SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC6_M3SM_SHIFT))&MPU_RGDAAC6_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGDAAC6_M4WE_MASK       0x1000000u
#define  MPU_RGDAAC6_M4WE_SHIFT      24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGDAAC6_M4RE_MASK       0x2000000u
#define  MPU_RGDAAC6_M4RE_SHIFT      25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGDAAC6_M5WE_MASK       0x4000000u
#define  MPU_RGDAAC6_M5WE_SHIFT      26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGDAAC6_M5RE_MASK       0x8000000u
#define  MPU_RGDAAC6_M5RE_SHIFT      27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGDAAC6_M6WE_MASK       0x10000000u
#define  MPU_RGDAAC6_M6WE_SHIFT      28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGDAAC6_M6RE_MASK       0x20000000u
#define  MPU_RGDAAC6_M6RE_SHIFT      29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGDAAC6_M7WE_MASK       0x40000000u
#define  MPU_RGDAAC6_M7WE_SHIFT      30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGDAAC6_M7RE_MASK       0x80000000u
#define  MPU_RGDAAC6_M7RE_SHIFT      31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MPU_RGDAAC7_M0UM_MASK       0x3u
#define  MPU_RGDAAC7_M0UM_SHIFT      0
#define  MPU_RGDAAC7_M0UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC7_M0UM_SHIFT))&MPU_RGDAAC7_M0UM_MASK)
//   Bus Master 0 User Mode Access Control

#define  MPU_RGDAAC7_M0SM_MASK       0x10u
#define  MPU_RGDAAC7_M0SM_SHIFT      3
#define  MPU_RGDAAC7_M0SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC7_M0SM_SHIFT))&MPU_RGDAAC7_M0SM_MASK)
//   Bus Master 0 Supervisor Mode Access Control

#define  MPU_RGDAAC7_M1UM_MASK       0xC0u
#define  MPU_RGDAAC7_M1UM_SHIFT      6
#define  MPU_RGDAAC7_M1UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC7_M1UM_SHIFT))&MPU_RGDAAC7_M1UM_MASK)
//   Bus Master 1 User Mode Access Control

#define  MPU_RGDAAC7_M1SM_MASK       0x400u
#define  MPU_RGDAAC7_M1SM_SHIFT      9
#define  MPU_RGDAAC7_M1SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC7_M1SM_SHIFT))&MPU_RGDAAC7_M1SM_MASK)
//   Bus Master 1 Supervisor Mode Access Control

#define  MPU_RGDAAC7_M2UM_MASK       0x3000u
#define  MPU_RGDAAC7_M2UM_SHIFT      12
#define  MPU_RGDAAC7_M2UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC7_M2UM_SHIFT))&MPU_RGDAAC7_M2UM_MASK)
//   Bus Master 2 User Mode Access Control

#define  MPU_RGDAAC7_M2SM_MASK       0x10000u
#define  MPU_RGDAAC7_M2SM_SHIFT      15
#define  MPU_RGDAAC7_M2SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC7_M2SM_SHIFT))&MPU_RGDAAC7_M2SM_MASK)
//   Bus Master 2 Supervisor Mode Access Control

#define  MPU_RGDAAC7_M3UM_MASK       0xC0000u
#define  MPU_RGDAAC7_M3UM_SHIFT      18
#define  MPU_RGDAAC7_M3UM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC7_M3UM_SHIFT))&MPU_RGDAAC7_M3UM_MASK)
//   Bus Master 3 User Mode Access Control
//     0b0 ==> An attempted access of that mode may be terminated with an access error (if not allowed by another descriptor) and the access not performed.
//     0b1 ==> Allows the given access type to occur

#define  MPU_RGDAAC7_M3SM_MASK       0x400000u
#define  MPU_RGDAAC7_M3SM_SHIFT      21
#define  MPU_RGDAAC7_M3SM(x)         (((uint32_t)(((uint32_t)(x))<<MPU_RGDAAC7_M3SM_SHIFT))&MPU_RGDAAC7_M3SM_MASK)
//   Bus Master 3 Supervisor Mode Access Control
//     0b00 ==> r/w/x; read, write and execute allowed
//     0b01 ==> r/x; read and execute allowed, but no write
//     0b10 ==> r/w; read and write allowed, but no execute
//     0b11 ==> Same as User mode defined in M3UM

#define  MPU_RGDAAC7_M4WE_MASK       0x1000000u
#define  MPU_RGDAAC7_M4WE_SHIFT      24
//   Bus Master 4 Write Enable
//     0b0 ==> Bus master 4 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 4 writes allowed

#define  MPU_RGDAAC7_M4RE_MASK       0x2000000u
#define  MPU_RGDAAC7_M4RE_SHIFT      25
//   Bus Master 4 Read Enable
//     0b0 ==> Bus master 4 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 4 reads allowed

#define  MPU_RGDAAC7_M5WE_MASK       0x4000000u
#define  MPU_RGDAAC7_M5WE_SHIFT      26
//   Bus Master 5 Write Enable
//     0b0 ==> Bus master 5 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 5 writes allowed

#define  MPU_RGDAAC7_M5RE_MASK       0x8000000u
#define  MPU_RGDAAC7_M5RE_SHIFT      27
//   Bus Master 5 Read Enable
//     0b0 ==> Bus master 5 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 5 reads allowed

#define  MPU_RGDAAC7_M6WE_MASK       0x10000000u
#define  MPU_RGDAAC7_M6WE_SHIFT      28
//   Bus Master 6 Write Enable
//     0b0 ==> Bus master 6 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 6 writes allowed

#define  MPU_RGDAAC7_M6RE_MASK       0x20000000u
#define  MPU_RGDAAC7_M6RE_SHIFT      29
//   Bus Master 6 Read Enable
//     0b0 ==> Bus master 6 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 6 reads allowed

#define  MPU_RGDAAC7_M7WE_MASK       0x40000000u
#define  MPU_RGDAAC7_M7WE_SHIFT      30
//   Bus Master 7 Write Enable
//     0b0 ==> Bus master 7 writes terminate with an access error and the write is not performed
//     0b1 ==> Bus master 7 writes allowed

#define  MPU_RGDAAC7_M7RE_MASK       0x80000000u
#define  MPU_RGDAAC7_M7RE_SHIFT      31
//   Bus Master 7 Read Enable
//     0b0 ==> Bus master 7 reads terminate with an access error and the read is not performed
//     0b1 ==> Bus master 7 reads allowed

#define  MTB_POSITION_WRAP_MASK      0x4u
#define  MTB_POSITION_WRAP_SHIFT     2

#define  MTB_POSITION_POINTER_MASK   0xE8u
#define  MTB_POSITION_POINTER_SHIFT  3
#define  MTB_POSITION_POINTER(x)     (((uint32_t)(((uint32_t)(x))<<MTB_POSITION_POINTER_SHIFT))&MTB_POSITION_POINTER_MASK)
//   Trace Packet Address Pointer

#define  MTB_MASTER_MASK_MASK        0x5u
#define  MTB_MASTER_MASK_SHIFT       0
#define  MTB_MASTER_MASK(x)          (((uint32_t)(((uint32_t)(x))<<MTB_MASTER_MASK_SHIFT))&MTB_MASTER_MASK_MASK)
//   Mask

#define  MTB_MASTER_TSTARTEN_MASK    0x20u
#define  MTB_MASTER_TSTARTEN_SHIFT   5
//   Trace start input enable

#define  MTB_MASTER_TSTOPEN_MASK     0x40u
#define  MTB_MASTER_TSTOPEN_SHIFT    6
//   Trace stop input enable

#define  MTB_MASTER_SFRWPRIV_MASK    0x80u
#define  MTB_MASTER_SFRWPRIV_SHIFT   7
//   Special Function Register Write Privilege bit

#define  MTB_MASTER_RAMPRIV_MASK     0x100u
#define  MTB_MASTER_RAMPRIV_SHIFT    8
//   RAM privilege bit

#define  MTB_MASTER_HALTREQ_MASK     0x200u
#define  MTB_MASTER_HALTREQ_SHIFT    9
//   Halt request bit

#define  MTB_MASTER_EN_MASK          0x80000000u
#define  MTB_MASTER_EN_SHIFT         31
//   Main trace enable bit

#define  MTB_FLOW_AUTOSTOP_MASK      0x1u
#define  MTB_FLOW_AUTOSTOP_SHIFT     0

#define  MTB_FLOW_AUTOHALT_MASK      0x2u
#define  MTB_FLOW_AUTOHALT_SHIFT     1

#define  MTB_FLOW_WATERMARK_MASK     0xE8u
#define  MTB_FLOW_WATERMARK_SHIFT    3
#define  MTB_FLOW_WATERMARK(x)       (((uint32_t)(((uint32_t)(x))<<MTB_FLOW_WATERMARK_SHIFT))&MTB_FLOW_WATERMARK_MASK)
//   WATERMARK value

#define  MTB_BASE_BASEADDR_MASK      0x20u
#define  MTB_BASE_BASEADDR_SHIFT     0
#define  MTB_BASE_BASEADDR(x)        (((uint32_t)(((uint32_t)(x))<<MTB_BASE_BASEADDR_SHIFT))&MTB_BASE_BASEADDR_MASK)

#define  MTB_MODECTRL_MODECTRL_MASK  0x20u
#define  MTB_MODECTRL_MODECTRL_SHIFT 0
#define  MTB_MODECTRL_MODECTRL(x)    (((uint32_t)(((uint32_t)(x))<<MTB_MODECTRL_MODECTRL_SHIFT))&MTB_MODECTRL_MODECTRL_MASK)

#define  MTB_TAGSET_TAGSET_MASK      0x20u
#define  MTB_TAGSET_TAGSET_SHIFT     0
#define  MTB_TAGSET_TAGSET(x)        (((uint32_t)(((uint32_t)(x))<<MTB_TAGSET_TAGSET_SHIFT))&MTB_TAGSET_TAGSET_MASK)

#define  MTB_TAGCLEAR_TAGCLEAR_MASK  0x20u
#define  MTB_TAGCLEAR_TAGCLEAR_SHIFT 0
#define  MTB_TAGCLEAR_TAGCLEAR(x)    (((uint32_t)(((uint32_t)(x))<<MTB_TAGCLEAR_TAGCLEAR_SHIFT))&MTB_TAGCLEAR_TAGCLEAR_MASK)

#define  MTB_LOCKACCESS_LOCKACCESS_MASK 0x20u
#define  MTB_LOCKACCESS_LOCKACCESS_SHIFT 0
#define  MTB_LOCKACCESS_LOCKACCESS(x) (((uint32_t)(((uint32_t)(x))<<MTB_LOCKACCESS_LOCKACCESS_SHIFT))&MTB_LOCKACCESS_LOCKACCESS_MASK)

#define  MTB_LOCKSTAT_LOCKSTAT_MASK  0x20u
#define  MTB_LOCKSTAT_LOCKSTAT_SHIFT 0
#define  MTB_LOCKSTAT_LOCKSTAT(x)    (((uint32_t)(((uint32_t)(x))<<MTB_LOCKSTAT_LOCKSTAT_SHIFT))&MTB_LOCKSTAT_LOCKSTAT_MASK)

#define  MTB_AUTHSTAT_BIT0_MASK      0x1u
#define  MTB_AUTHSTAT_BIT0_SHIFT     0

#define  MTB_AUTHSTAT_BIT1_MASK      0x2u
#define  MTB_AUTHSTAT_BIT1_SHIFT     1

#define  MTB_AUTHSTAT_BIT2_MASK      0x4u
#define  MTB_AUTHSTAT_BIT2_SHIFT     2

#define  MTB_AUTHSTAT_BIT3_MASK      0x8u
#define  MTB_AUTHSTAT_BIT3_SHIFT     3

#define  MTB_DEVICEARCH_DEVICEARCH_MASK 0x20u
#define  MTB_DEVICEARCH_DEVICEARCH_SHIFT 0
#define  MTB_DEVICEARCH_DEVICEARCH(x) (((uint32_t)(((uint32_t)(x))<<MTB_DEVICEARCH_DEVICEARCH_SHIFT))&MTB_DEVICEARCH_DEVICEARCH_MASK)

#define  MTB_DEVICECFG_DEVICECFG_MASK 0x20u
#define  MTB_DEVICECFG_DEVICECFG_SHIFT 0
#define  MTB_DEVICECFG_DEVICECFG(x)  (((uint32_t)(((uint32_t)(x))<<MTB_DEVICECFG_DEVICECFG_SHIFT))&MTB_DEVICECFG_DEVICECFG_MASK)

#define  MTB_DEVICETYPID_DEVICETYPID_MASK 0x20u
#define  MTB_DEVICETYPID_DEVICETYPID_SHIFT 0
#define  MTB_DEVICETYPID_DEVICETYPID(x) (((uint32_t)(((uint32_t)(x))<<MTB_DEVICETYPID_DEVICETYPID_SHIFT))&MTB_DEVICETYPID_DEVICETYPID_MASK)

#define  MTB_PERIPHID4_PERIPHID_MASK 0x20u
#define  MTB_PERIPHID4_PERIPHID_SHIFT 0
#define  MTB_PERIPHID4_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<MTB_PERIPHID4_PERIPHID_SHIFT))&MTB_PERIPHID4_PERIPHID_MASK)

#define  MTB_PERIPHID5_PERIPHID_MASK 0x20u
#define  MTB_PERIPHID5_PERIPHID_SHIFT 0
#define  MTB_PERIPHID5_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<MTB_PERIPHID5_PERIPHID_SHIFT))&MTB_PERIPHID5_PERIPHID_MASK)

#define  MTB_PERIPHID6_PERIPHID_MASK 0x20u
#define  MTB_PERIPHID6_PERIPHID_SHIFT 0
#define  MTB_PERIPHID6_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<MTB_PERIPHID6_PERIPHID_SHIFT))&MTB_PERIPHID6_PERIPHID_MASK)

#define  MTB_PERIPHID7_PERIPHID_MASK 0x20u
#define  MTB_PERIPHID7_PERIPHID_SHIFT 0
#define  MTB_PERIPHID7_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<MTB_PERIPHID7_PERIPHID_SHIFT))&MTB_PERIPHID7_PERIPHID_MASK)

#define  MTB_PERIPHID0_PERIPHID_MASK 0x20u
#define  MTB_PERIPHID0_PERIPHID_SHIFT 0
#define  MTB_PERIPHID0_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<MTB_PERIPHID0_PERIPHID_SHIFT))&MTB_PERIPHID0_PERIPHID_MASK)

#define  MTB_PERIPHID1_PERIPHID_MASK 0x20u
#define  MTB_PERIPHID1_PERIPHID_SHIFT 0
#define  MTB_PERIPHID1_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<MTB_PERIPHID1_PERIPHID_SHIFT))&MTB_PERIPHID1_PERIPHID_MASK)

#define  MTB_PERIPHID2_PERIPHID_MASK 0x20u
#define  MTB_PERIPHID2_PERIPHID_SHIFT 0
#define  MTB_PERIPHID2_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<MTB_PERIPHID2_PERIPHID_SHIFT))&MTB_PERIPHID2_PERIPHID_MASK)

#define  MTB_PERIPHID3_PERIPHID_MASK 0x20u
#define  MTB_PERIPHID3_PERIPHID_SHIFT 0
#define  MTB_PERIPHID3_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<MTB_PERIPHID3_PERIPHID_SHIFT))&MTB_PERIPHID3_PERIPHID_MASK)

#define  MTB_COMPID0_COMPID_MASK     0x20u
#define  MTB_COMPID0_COMPID_SHIFT    0
#define  MTB_COMPID0_COMPID(x)       (((uint32_t)(((uint32_t)(x))<<MTB_COMPID0_COMPID_SHIFT))&MTB_COMPID0_COMPID_MASK)
//   Component ID

#define  MTB_COMPID1_COMPID_MASK     0x20u
#define  MTB_COMPID1_COMPID_SHIFT    0
#define  MTB_COMPID1_COMPID(x)       (((uint32_t)(((uint32_t)(x))<<MTB_COMPID1_COMPID_SHIFT))&MTB_COMPID1_COMPID_MASK)
//   Component ID

#define  MTB_COMPID2_COMPID_MASK     0x20u
#define  MTB_COMPID2_COMPID_SHIFT    0
#define  MTB_COMPID2_COMPID(x)       (((uint32_t)(((uint32_t)(x))<<MTB_COMPID2_COMPID_SHIFT))&MTB_COMPID2_COMPID_MASK)
//   Component ID

#define  MTB_COMPID3_COMPID_MASK     0x20u
#define  MTB_COMPID3_COMPID_SHIFT    0
#define  MTB_COMPID3_COMPID(x)       (((uint32_t)(((uint32_t)(x))<<MTB_COMPID3_COMPID_SHIFT))&MTB_COMPID3_COMPID_MASK)
//   Component ID

#define  MTBDWT_CTRL_DWTCFGCTRL_MASK 0x1Cu
#define  MTBDWT_CTRL_DWTCFGCTRL_SHIFT 0
#define  MTBDWT_CTRL_DWTCFGCTRL(x)   (((uint32_t)(((uint32_t)(x))<<MTBDWT_CTRL_DWTCFGCTRL_SHIFT))&MTBDWT_CTRL_DWTCFGCTRL_MASK)
//   DWT configuration controls

#define  MTBDWT_CTRL_NUMCMP_MASK     0x40000000u
#define  MTBDWT_CTRL_NUMCMP_SHIFT    28
#define  MTBDWT_CTRL_NUMCMP(x)       (((uint32_t)(((uint32_t)(x))<<MTBDWT_CTRL_NUMCMP_SHIFT))&MTBDWT_CTRL_NUMCMP_MASK)
//   Number of comparators

#define  MTBDWT_COMP0_COMP_MASK      0x20u
#define  MTBDWT_COMP0_COMP_SHIFT     0
#define  MTBDWT_COMP0_COMP(x)        (((uint32_t)(((uint32_t)(x))<<MTBDWT_COMP0_COMP_SHIFT))&MTBDWT_COMP0_COMP_MASK)
//   Reference value for comparison

#define  MTBDWT_MASK0_MASK_MASK      0x5u
#define  MTBDWT_MASK0_MASK_SHIFT     0
#define  MTBDWT_MASK0_MASK(x)        (((uint32_t)(((uint32_t)(x))<<MTBDWT_MASK0_MASK_SHIFT))&MTBDWT_MASK0_MASK_MASK)
//   MASK

#define  MTBDWT_COMP1_COMP_MASK      0x20u
#define  MTBDWT_COMP1_COMP_SHIFT     0
#define  MTBDWT_COMP1_COMP(x)        (((uint32_t)(((uint32_t)(x))<<MTBDWT_COMP1_COMP_SHIFT))&MTBDWT_COMP1_COMP_MASK)
//   Reference value for comparison

#define  MTBDWT_MASK1_MASK_MASK      0x5u
#define  MTBDWT_MASK1_MASK_SHIFT     0
#define  MTBDWT_MASK1_MASK(x)        (((uint32_t)(((uint32_t)(x))<<MTBDWT_MASK1_MASK_SHIFT))&MTBDWT_MASK1_MASK_MASK)
//   MASK

#define  MTBDWT_FCT1_FUNCTION_MASK   0x4u
#define  MTBDWT_FCT1_FUNCTION_SHIFT  0
#define  MTBDWT_FCT1_FUNCTION(x)     (((uint32_t)(((uint32_t)(x))<<MTBDWT_FCT1_FUNCTION_SHIFT))&MTBDWT_FCT1_FUNCTION_MASK)
//   Function
//     0b0000 ==> Disabled.
//     0b0100 ==> Instruction fetch.
//     0b0101 ==> Data operand read.
//     0b0110 ==> Data operand write.
//     0b0111 ==> Data operand (read + write).

#define  MTBDWT_FCT1_MATCHED_MASK    0x1000000u
#define  MTBDWT_FCT1_MATCHED_SHIFT   24
//   Comparator match
//     0b0 ==> No match.
//     0b1 ==> Match occurred.

#define  MTBDWT_TBCTRL_ACOMP0_MASK   0x1u
#define  MTBDWT_TBCTRL_ACOMP0_SHIFT  0
//   Action based on Comparator 0 match
//     0b0 ==> Trigger TSTOP based on the assertion of MTBDWT_FUNCTION0[MATCHED].
//     0b1 ==> Trigger TSTART based on the assertion of MTBDWT_FUNCTION0[MATCHED].

#define  MTBDWT_TBCTRL_ACOMP1_MASK   0x2u
#define  MTBDWT_TBCTRL_ACOMP1_SHIFT  1
//   Action based on Comparator 1 match
//     0b0 ==> Trigger TSTOP based on the assertion of MTBDWT_FUNCTION1[MATCHED].
//     0b1 ==> Trigger TSTART based on the assertion of MTBDWT_FUNCTION1[MATCHED].

#define  MTBDWT_TBCTRL_NUMCOMP_MASK  0x40000000u
#define  MTBDWT_TBCTRL_NUMCOMP_SHIFT 28
#define  MTBDWT_TBCTRL_NUMCOMP(x)    (((uint32_t)(((uint32_t)(x))<<MTBDWT_TBCTRL_NUMCOMP_SHIFT))&MTBDWT_TBCTRL_NUMCOMP_MASK)
//   Number of Comparators

#define  MTBDWT_DEVICECFG_DEVICECFG_MASK 0x20u
#define  MTBDWT_DEVICECFG_DEVICECFG_SHIFT 0
#define  MTBDWT_DEVICECFG_DEVICECFG(x) (((uint32_t)(((uint32_t)(x))<<MTBDWT_DEVICECFG_DEVICECFG_SHIFT))&MTBDWT_DEVICECFG_DEVICECFG_MASK)

#define  MTBDWT_DEVICETYPID_DEVICETYPID_MASK 0x20u
#define  MTBDWT_DEVICETYPID_DEVICETYPID_SHIFT 0
#define  MTBDWT_DEVICETYPID_DEVICETYPID(x) (((uint32_t)(((uint32_t)(x))<<MTBDWT_DEVICETYPID_DEVICETYPID_SHIFT))&MTBDWT_DEVICETYPID_DEVICETYPID_MASK)

#define  MTBDWT_PERIPHID4_PERIPHID_MASK 0x20u
#define  MTBDWT_PERIPHID4_PERIPHID_SHIFT 0
#define  MTBDWT_PERIPHID4_PERIPHID(x) (((uint32_t)(((uint32_t)(x))<<MTBDWT_PERIPHID4_PERIPHID_SHIFT))&MTBDWT_PERIPHID4_PERIPHID_MASK)

#define  MTBDWT_PERIPHID5_PERIPHID_MASK 0x20u
#define  MTBDWT_PERIPHID5_PERIPHID_SHIFT 0
#define  MTBDWT_PERIPHID5_PERIPHID(x) (((uint32_t)(((uint32_t)(x))<<MTBDWT_PERIPHID5_PERIPHID_SHIFT))&MTBDWT_PERIPHID5_PERIPHID_MASK)

#define  MTBDWT_PERIPHID6_PERIPHID_MASK 0x20u
#define  MTBDWT_PERIPHID6_PERIPHID_SHIFT 0
#define  MTBDWT_PERIPHID6_PERIPHID(x) (((uint32_t)(((uint32_t)(x))<<MTBDWT_PERIPHID6_PERIPHID_SHIFT))&MTBDWT_PERIPHID6_PERIPHID_MASK)

#define  MTBDWT_PERIPHID7_PERIPHID_MASK 0x20u
#define  MTBDWT_PERIPHID7_PERIPHID_SHIFT 0
#define  MTBDWT_PERIPHID7_PERIPHID(x) (((uint32_t)(((uint32_t)(x))<<MTBDWT_PERIPHID7_PERIPHID_SHIFT))&MTBDWT_PERIPHID7_PERIPHID_MASK)

#define  MTBDWT_PERIPHID0_PERIPHID_MASK 0x20u
#define  MTBDWT_PERIPHID0_PERIPHID_SHIFT 0
#define  MTBDWT_PERIPHID0_PERIPHID(x) (((uint32_t)(((uint32_t)(x))<<MTBDWT_PERIPHID0_PERIPHID_SHIFT))&MTBDWT_PERIPHID0_PERIPHID_MASK)

#define  MTBDWT_PERIPHID1_PERIPHID_MASK 0x20u
#define  MTBDWT_PERIPHID1_PERIPHID_SHIFT 0
#define  MTBDWT_PERIPHID1_PERIPHID(x) (((uint32_t)(((uint32_t)(x))<<MTBDWT_PERIPHID1_PERIPHID_SHIFT))&MTBDWT_PERIPHID1_PERIPHID_MASK)

#define  MTBDWT_PERIPHID2_PERIPHID_MASK 0x20u
#define  MTBDWT_PERIPHID2_PERIPHID_SHIFT 0
#define  MTBDWT_PERIPHID2_PERIPHID(x) (((uint32_t)(((uint32_t)(x))<<MTBDWT_PERIPHID2_PERIPHID_SHIFT))&MTBDWT_PERIPHID2_PERIPHID_MASK)

#define  MTBDWT_PERIPHID3_PERIPHID_MASK 0x20u
#define  MTBDWT_PERIPHID3_PERIPHID_SHIFT 0
#define  MTBDWT_PERIPHID3_PERIPHID(x) (((uint32_t)(((uint32_t)(x))<<MTBDWT_PERIPHID3_PERIPHID_SHIFT))&MTBDWT_PERIPHID3_PERIPHID_MASK)

#define  MTBDWT_COMPID0_COMPID_MASK  0x20u
#define  MTBDWT_COMPID0_COMPID_SHIFT 0
#define  MTBDWT_COMPID0_COMPID(x)    (((uint32_t)(((uint32_t)(x))<<MTBDWT_COMPID0_COMPID_SHIFT))&MTBDWT_COMPID0_COMPID_MASK)
//   Component ID

#define  MTBDWT_COMPID1_COMPID_MASK  0x20u
#define  MTBDWT_COMPID1_COMPID_SHIFT 0
#define  MTBDWT_COMPID1_COMPID(x)    (((uint32_t)(((uint32_t)(x))<<MTBDWT_COMPID1_COMPID_SHIFT))&MTBDWT_COMPID1_COMPID_MASK)
//   Component ID

#define  MTBDWT_COMPID2_COMPID_MASK  0x20u
#define  MTBDWT_COMPID2_COMPID_SHIFT 0
#define  MTBDWT_COMPID2_COMPID(x)    (((uint32_t)(((uint32_t)(x))<<MTBDWT_COMPID2_COMPID_SHIFT))&MTBDWT_COMPID2_COMPID_MASK)
//   Component ID

#define  MTBDWT_COMPID3_COMPID_MASK  0x20u
#define  MTBDWT_COMPID3_COMPID_SHIFT 0
#define  MTBDWT_COMPID3_COMPID(x)    (((uint32_t)(((uint32_t)(x))<<MTBDWT_COMPID3_COMPID_SHIFT))&MTBDWT_COMPID3_COMPID_MASK)
//   Component ID

#define  OSC_CR_SC16P_MASK           0x1u
#define  OSC_CR_SC16P_SHIFT          0
//   Oscillator 16 pF Capacitor Load Configure
//     0b0 ==> Disable the selection.
//     0b1 ==> Add 16 pF capacitor to the oscillator load.

#define  OSC_CR_SC8P_MASK            0x2u
#define  OSC_CR_SC8P_SHIFT           1
//   Oscillator 8 pF Capacitor Load Configure
//     0b0 ==> Disable the selection.
//     0b1 ==> Add 8 pF capacitor to the oscillator load.

#define  OSC_CR_SC4P_MASK            0x4u
#define  OSC_CR_SC4P_SHIFT           2
//   Oscillator 4 pF Capacitor Load Configure
//     0b0 ==> Disable the selection.
//     0b1 ==> Add 4 pF capacitor to the oscillator load.

#define  OSC_CR_SC2P_MASK            0x8u
#define  OSC_CR_SC2P_SHIFT           3
//   Oscillator 2 pF Capacitor Load Configure
//     0b0 ==> Disable the selection.
//     0b1 ==> Add 2 pF capacitor to the oscillator load.

#define  OSC_CR_EREFSTEN_MASK        0x20u
#define  OSC_CR_EREFSTEN_SHIFT       5
//   External Reference Stop Enable
//     0b0 ==> External reference clock is disabled in Stop mode.
//     0b1 ==> External reference clock stays enabled in Stop mode if ERCLKEN is set before entering Stop mode.

#define  OSC_CR_ERCLKEN_MASK         0x80u
#define  OSC_CR_ERCLKEN_SHIFT        7
//   External Reference Enable
//     0b0 ==> External reference clock is inactive.
//     0b1 ==> External reference clock is enabled.

#define  PIT0_MCR_FRZ_MASK           0x1u
#define  PIT0_MCR_FRZ_SHIFT          0
//   Freeze
//     0b0 ==> Timers continue to run in Debug mode.
//     0b1 ==> Timers are stopped in Debug mode.

#define  PIT0_LDVAL0_TSV_MASK        0x20u
#define  PIT0_LDVAL0_TSV_SHIFT       0
#define  PIT0_LDVAL0_TSV(x)          (((uint32_t)(((uint32_t)(x))<<PIT0_LDVAL0_TSV_SHIFT))&PIT0_LDVAL0_TSV_MASK)
//   Timer Start Value

#define  PIT0_CVAL0_TVL_MASK         0x20u
#define  PIT0_CVAL0_TVL_SHIFT        0
#define  PIT0_CVAL0_TVL(x)           (((uint32_t)(((uint32_t)(x))<<PIT0_CVAL0_TVL_SHIFT))&PIT0_CVAL0_TVL_MASK)
//   Current Timer Value

#define  PIT0_TCTRL0_TEN_MASK        0x1u
#define  PIT0_TCTRL0_TEN_SHIFT       0
//   Timer Enable
//     0b0 ==> Timer n is disabled.
//     0b1 ==> Timer n is enabled.

#define  PIT0_TCTRL0_TIE_MASK        0x2u
#define  PIT0_TCTRL0_TIE_SHIFT       1
//   Timer Interrupt Enable
//     0b0 ==> Interrupt requests from Timer n are disabled.
//     0b1 ==> Interrupt will be requested whenever TIF is set.

#define  PIT0_TCTRL0_CHN_MASK        0x4u
#define  PIT0_TCTRL0_CHN_SHIFT       2
//   Chain Mode
//     0b0 ==> Timer is not chained.
//     0b1 ==> Timer is chained to previous timer. For example, for Channel 2, if this field is set, Timer 2 is chained to Timer 1.

#define  PIT0_TFLG0_TIF_MASK         0x1u
#define  PIT0_TFLG0_TIF_SHIFT        0
//   Timer Interrupt Flag
//     0b0 ==> Timeout has not yet occurred.
//     0b1 ==> Timeout has occurred.

#define  PIT0_LDVAL1_TSV_MASK        0x20u
#define  PIT0_LDVAL1_TSV_SHIFT       0
#define  PIT0_LDVAL1_TSV(x)          (((uint32_t)(((uint32_t)(x))<<PIT0_LDVAL1_TSV_SHIFT))&PIT0_LDVAL1_TSV_MASK)
//   Timer Start Value

#define  PIT0_CVAL1_TVL_MASK         0x20u
#define  PIT0_CVAL1_TVL_SHIFT        0
#define  PIT0_CVAL1_TVL(x)           (((uint32_t)(((uint32_t)(x))<<PIT0_CVAL1_TVL_SHIFT))&PIT0_CVAL1_TVL_MASK)
//   Current Timer Value

#define  PIT0_TCTRL1_TEN_MASK        0x1u
#define  PIT0_TCTRL1_TEN_SHIFT       0
//   Timer Enable
//     0b0 ==> Timer n is disabled.
//     0b1 ==> Timer n is enabled.

#define  PIT0_TCTRL1_TIE_MASK        0x2u
#define  PIT0_TCTRL1_TIE_SHIFT       1
//   Timer Interrupt Enable
//     0b0 ==> Interrupt requests from Timer n are disabled.
//     0b1 ==> Interrupt will be requested whenever TIF is set.

#define  PIT0_TCTRL1_CHN_MASK        0x4u
#define  PIT0_TCTRL1_CHN_SHIFT       2
//   Chain Mode
//     0b0 ==> Timer is not chained.
//     0b1 ==> Timer is chained to previous timer. For example, for Channel 2, if this field is set, Timer 2 is chained to Timer 1.

#define  PIT0_TFLG1_TIF_MASK         0x1u
#define  PIT0_TFLG1_TIF_SHIFT        0
//   Timer Interrupt Flag
//     0b0 ==> Timeout has not yet occurred.
//     0b1 ==> Timeout has occurred.

#define  PIT0_LDVAL2_TSV_MASK        0x20u
#define  PIT0_LDVAL2_TSV_SHIFT       0
#define  PIT0_LDVAL2_TSV(x)          (((uint32_t)(((uint32_t)(x))<<PIT0_LDVAL2_TSV_SHIFT))&PIT0_LDVAL2_TSV_MASK)
//   Timer Start Value

#define  PIT0_CVAL2_TVL_MASK         0x20u
#define  PIT0_CVAL2_TVL_SHIFT        0
#define  PIT0_CVAL2_TVL(x)           (((uint32_t)(((uint32_t)(x))<<PIT0_CVAL2_TVL_SHIFT))&PIT0_CVAL2_TVL_MASK)
//   Current Timer Value

#define  PIT0_TCTRL2_TEN_MASK        0x1u
#define  PIT0_TCTRL2_TEN_SHIFT       0
//   Timer Enable
//     0b0 ==> Timer n is disabled.
//     0b1 ==> Timer n is enabled.

#define  PIT0_TCTRL2_TIE_MASK        0x2u
#define  PIT0_TCTRL2_TIE_SHIFT       1
//   Timer Interrupt Enable
//     0b0 ==> Interrupt requests from Timer n are disabled.
//     0b1 ==> Interrupt will be requested whenever TIF is set.

#define  PIT0_TCTRL2_CHN_MASK        0x4u
#define  PIT0_TCTRL2_CHN_SHIFT       2
//   Chain Mode
//     0b0 ==> Timer is not chained.
//     0b1 ==> Timer is chained to previous timer. For example, for Channel 2, if this field is set, Timer 2 is chained to Timer 1.

#define  PIT0_TFLG2_TIF_MASK         0x1u
#define  PIT0_TFLG2_TIF_SHIFT        0
//   Timer Interrupt Flag
//     0b0 ==> Timeout has not yet occurred.
//     0b1 ==> Timeout has occurred.

#define  PIT0_LDVAL3_TSV_MASK        0x20u
#define  PIT0_LDVAL3_TSV_SHIFT       0
#define  PIT0_LDVAL3_TSV(x)          (((uint32_t)(((uint32_t)(x))<<PIT0_LDVAL3_TSV_SHIFT))&PIT0_LDVAL3_TSV_MASK)
//   Timer Start Value

#define  PIT0_CVAL3_TVL_MASK         0x20u
#define  PIT0_CVAL3_TVL_SHIFT        0
#define  PIT0_CVAL3_TVL(x)           (((uint32_t)(((uint32_t)(x))<<PIT0_CVAL3_TVL_SHIFT))&PIT0_CVAL3_TVL_MASK)
//   Current Timer Value

#define  PIT0_TCTRL3_TEN_MASK        0x1u
#define  PIT0_TCTRL3_TEN_SHIFT       0
//   Timer Enable
//     0b0 ==> Timer n is disabled.
//     0b1 ==> Timer n is enabled.

#define  PIT0_TCTRL3_TIE_MASK        0x2u
#define  PIT0_TCTRL3_TIE_SHIFT       1
//   Timer Interrupt Enable
//     0b0 ==> Interrupt requests from Timer n are disabled.
//     0b1 ==> Interrupt will be requested whenever TIF is set.

#define  PIT0_TCTRL3_CHN_MASK        0x4u
#define  PIT0_TCTRL3_CHN_SHIFT       2
//   Chain Mode
//     0b0 ==> Timer is not chained.
//     0b1 ==> Timer is chained to previous timer. For example, for Channel 2, if this field is set, Timer 2 is chained to Timer 1.

#define  PIT0_TFLG3_TIF_MASK         0x1u
#define  PIT0_TFLG3_TIF_SHIFT        0
//   Timer Interrupt Flag
//     0b0 ==> Timeout has not yet occurred.
//     0b1 ==> Timeout has occurred.

#define  PIT1_MCR_FRZ_MASK           0x1u
#define  PIT1_MCR_FRZ_SHIFT          0
//   Freeze
//     0b0 ==> Timers continue to run in Debug mode.
//     0b1 ==> Timers are stopped in Debug mode.

#define  PIT1_LDVAL0_TSV_MASK        0x20u
#define  PIT1_LDVAL0_TSV_SHIFT       0
#define  PIT1_LDVAL0_TSV(x)          (((uint32_t)(((uint32_t)(x))<<PIT1_LDVAL0_TSV_SHIFT))&PIT1_LDVAL0_TSV_MASK)
//   Timer Start Value

#define  PIT1_CVAL0_TVL_MASK         0x20u
#define  PIT1_CVAL0_TVL_SHIFT        0
#define  PIT1_CVAL0_TVL(x)           (((uint32_t)(((uint32_t)(x))<<PIT1_CVAL0_TVL_SHIFT))&PIT1_CVAL0_TVL_MASK)
//   Current Timer Value

#define  PIT1_TCTRL0_TEN_MASK        0x1u
#define  PIT1_TCTRL0_TEN_SHIFT       0
//   Timer Enable
//     0b0 ==> Timer n is disabled.
//     0b1 ==> Timer n is enabled.

#define  PIT1_TCTRL0_TIE_MASK        0x2u
#define  PIT1_TCTRL0_TIE_SHIFT       1
//   Timer Interrupt Enable
//     0b0 ==> Interrupt requests from Timer n are disabled.
//     0b1 ==> Interrupt will be requested whenever TIF is set.

#define  PIT1_TCTRL0_CHN_MASK        0x4u
#define  PIT1_TCTRL0_CHN_SHIFT       2
//   Chain Mode
//     0b0 ==> Timer is not chained.
//     0b1 ==> Timer is chained to previous timer. For example, for Channel 2, if this field is set, Timer 2 is chained to Timer 1.

#define  PIT1_TFLG0_TIF_MASK         0x1u
#define  PIT1_TFLG0_TIF_SHIFT        0
//   Timer Interrupt Flag
//     0b0 ==> Timeout has not yet occurred.
//     0b1 ==> Timeout has occurred.

#define  PIT1_LDVAL1_TSV_MASK        0x20u
#define  PIT1_LDVAL1_TSV_SHIFT       0
#define  PIT1_LDVAL1_TSV(x)          (((uint32_t)(((uint32_t)(x))<<PIT1_LDVAL1_TSV_SHIFT))&PIT1_LDVAL1_TSV_MASK)
//   Timer Start Value

#define  PIT1_CVAL1_TVL_MASK         0x20u
#define  PIT1_CVAL1_TVL_SHIFT        0
#define  PIT1_CVAL1_TVL(x)           (((uint32_t)(((uint32_t)(x))<<PIT1_CVAL1_TVL_SHIFT))&PIT1_CVAL1_TVL_MASK)
//   Current Timer Value

#define  PIT1_TCTRL1_TEN_MASK        0x1u
#define  PIT1_TCTRL1_TEN_SHIFT       0
//   Timer Enable
//     0b0 ==> Timer n is disabled.
//     0b1 ==> Timer n is enabled.

#define  PIT1_TCTRL1_TIE_MASK        0x2u
#define  PIT1_TCTRL1_TIE_SHIFT       1
//   Timer Interrupt Enable
//     0b0 ==> Interrupt requests from Timer n are disabled.
//     0b1 ==> Interrupt will be requested whenever TIF is set.

#define  PIT1_TCTRL1_CHN_MASK        0x4u
#define  PIT1_TCTRL1_CHN_SHIFT       2
//   Chain Mode
//     0b0 ==> Timer is not chained.
//     0b1 ==> Timer is chained to previous timer. For example, for Channel 2, if this field is set, Timer 2 is chained to Timer 1.

#define  PIT1_TFLG1_TIF_MASK         0x1u
#define  PIT1_TFLG1_TIF_SHIFT        0
//   Timer Interrupt Flag
//     0b0 ==> Timeout has not yet occurred.
//     0b1 ==> Timeout has occurred.

#define  PIT1_LDVAL2_TSV_MASK        0x20u
#define  PIT1_LDVAL2_TSV_SHIFT       0
#define  PIT1_LDVAL2_TSV(x)          (((uint32_t)(((uint32_t)(x))<<PIT1_LDVAL2_TSV_SHIFT))&PIT1_LDVAL2_TSV_MASK)
//   Timer Start Value

#define  PIT1_CVAL2_TVL_MASK         0x20u
#define  PIT1_CVAL2_TVL_SHIFT        0
#define  PIT1_CVAL2_TVL(x)           (((uint32_t)(((uint32_t)(x))<<PIT1_CVAL2_TVL_SHIFT))&PIT1_CVAL2_TVL_MASK)
//   Current Timer Value

#define  PIT1_TCTRL2_TEN_MASK        0x1u
#define  PIT1_TCTRL2_TEN_SHIFT       0
//   Timer Enable
//     0b0 ==> Timer n is disabled.
//     0b1 ==> Timer n is enabled.

#define  PIT1_TCTRL2_TIE_MASK        0x2u
#define  PIT1_TCTRL2_TIE_SHIFT       1
//   Timer Interrupt Enable
//     0b0 ==> Interrupt requests from Timer n are disabled.
//     0b1 ==> Interrupt will be requested whenever TIF is set.

#define  PIT1_TCTRL2_CHN_MASK        0x4u
#define  PIT1_TCTRL2_CHN_SHIFT       2
//   Chain Mode
//     0b0 ==> Timer is not chained.
//     0b1 ==> Timer is chained to previous timer. For example, for Channel 2, if this field is set, Timer 2 is chained to Timer 1.

#define  PIT1_TFLG2_TIF_MASK         0x1u
#define  PIT1_TFLG2_TIF_SHIFT        0
//   Timer Interrupt Flag
//     0b0 ==> Timeout has not yet occurred.
//     0b1 ==> Timeout has occurred.

#define  PIT1_LDVAL3_TSV_MASK        0x20u
#define  PIT1_LDVAL3_TSV_SHIFT       0
#define  PIT1_LDVAL3_TSV(x)          (((uint32_t)(((uint32_t)(x))<<PIT1_LDVAL3_TSV_SHIFT))&PIT1_LDVAL3_TSV_MASK)
//   Timer Start Value

#define  PIT1_CVAL3_TVL_MASK         0x20u
#define  PIT1_CVAL3_TVL_SHIFT        0
#define  PIT1_CVAL3_TVL(x)           (((uint32_t)(((uint32_t)(x))<<PIT1_CVAL3_TVL_SHIFT))&PIT1_CVAL3_TVL_MASK)
//   Current Timer Value

#define  PIT1_TCTRL3_TEN_MASK        0x1u
#define  PIT1_TCTRL3_TEN_SHIFT       0
//   Timer Enable
//     0b0 ==> Timer n is disabled.
//     0b1 ==> Timer n is enabled.

#define  PIT1_TCTRL3_TIE_MASK        0x2u
#define  PIT1_TCTRL3_TIE_SHIFT       1
//   Timer Interrupt Enable
//     0b0 ==> Interrupt requests from Timer n are disabled.
//     0b1 ==> Interrupt will be requested whenever TIF is set.

#define  PIT1_TCTRL3_CHN_MASK        0x4u
#define  PIT1_TCTRL3_CHN_SHIFT       2
//   Chain Mode
//     0b0 ==> Timer is not chained.
//     0b1 ==> Timer is chained to previous timer. For example, for Channel 2, if this field is set, Timer 2 is chained to Timer 1.

#define  PIT1_TFLG3_TIF_MASK         0x1u
#define  PIT1_TFLG3_TIF_SHIFT        0
//   Timer Interrupt Flag
//     0b0 ==> Timeout has not yet occurred.
//     0b1 ==> Timeout has occurred.

#define  PMC_LVDSC1_LVDV_MASK        0x2u
#define  PMC_LVDSC1_LVDV_SHIFT       0
#define  PMC_LVDSC1_LVDV(x)          (((uint8_t)(((uint8_t)(x))<<PMC_LVDSC1_LVDV_SHIFT))&PMC_LVDSC1_LVDV_MASK)
//   Low-Voltage Detect Voltage Select
//     0b00 ==> Low trip point selected (V LVD = V LVDL )
//     0b01 ==> High trip point selected (V LVD = V LVDH )
//     0b10 ==> Reserved
//     0b11 ==> Reserved

#define  PMC_LVDSC1_LVDRE_MASK       0x10u
#define  PMC_LVDSC1_LVDRE_SHIFT      4
//   Low-Voltage Detect Reset Enable
//     0b0 ==> LVDF does not generate hardware resets
//     0b1 ==> Force an MCU reset when LVDF = 1

#define  PMC_LVDSC1_LVDIE_MASK       0x20u
#define  PMC_LVDSC1_LVDIE_SHIFT      5
//   Low-Voltage Detect Interrupt Enable
//     0b0 ==> Hardware interrupt disabled (use polling)
//     0b1 ==> Request a hardware interrupt when LVDF = 1

#define  PMC_LVDSC1_LVDACK_MASK      0x40u
#define  PMC_LVDSC1_LVDACK_SHIFT     6
//   Low-Voltage Detect Acknowledge

#define  PMC_LVDSC1_LVDF_MASK        0x80u
#define  PMC_LVDSC1_LVDF_SHIFT       7
//   Low-Voltage Detect Flag
//     0b0 ==> Low-voltage event not detected
//     0b1 ==> Low-voltage event detected

#define  PMC_LVDSC2_LVWV_MASK        0x2u
#define  PMC_LVDSC2_LVWV_SHIFT       0
#define  PMC_LVDSC2_LVWV(x)          (((uint8_t)(((uint8_t)(x))<<PMC_LVDSC2_LVWV_SHIFT))&PMC_LVDSC2_LVWV_MASK)
//   Low-Voltage Warning Voltage Select
//     0b00 ==> Low trip point selected (VLVW = VLVW1)
//     0b01 ==> Mid 1 trip point selected (VLVW = VLVW2)
//     0b10 ==> Mid 2 trip point selected (VLVW = VLVW3)
//     0b11 ==> High trip point selected (VLVW = VLVW4)

#define  PMC_LVDSC2_LVWIE_MASK       0x20u
#define  PMC_LVDSC2_LVWIE_SHIFT      5
//   Low-Voltage Warning Interrupt Enable
//     0b0 ==> Hardware interrupt disabled (use polling)
//     0b1 ==> Request a hardware interrupt when LVWF = 1

#define  PMC_LVDSC2_LVWACK_MASK      0x40u
#define  PMC_LVDSC2_LVWACK_SHIFT     6
//   Low-Voltage Warning Acknowledge

#define  PMC_LVDSC2_LVWF_MASK        0x80u
#define  PMC_LVDSC2_LVWF_SHIFT       7
//   Low-Voltage Warning Flag
//     0b0 ==> Low-voltage warning event not detected
//     0b1 ==> Low-voltage warning event detected

#define  PMC_REGSC_BGBE_MASK         0x1u
#define  PMC_REGSC_BGBE_SHIFT        0
//   Bandgap Buffer Enable
//     0b0 ==> Bandgap buffer not enabled
//     0b1 ==> Bandgap buffer enabled

#define  PMC_REGSC_REGONS_MASK       0x4u
#define  PMC_REGSC_REGONS_SHIFT      2
//   Regulator In Run Regulation Status
//     0b0 ==> Regulator is in stop regulation or in transition to/from it
//     0b1 ==> Regulator is in run regulation

#define  PMC_REGSC_ACKISO_MASK       0x8u
#define  PMC_REGSC_ACKISO_SHIFT      3
//   Acknowledge Isolation
//     0b0 ==> Peripherals and I/O pads are in normal run state
//     0b1 ==> Certain peripherals and I/O pads are in an isolated and latched state

#define  PMC_REGSC_BGEN_MASK         0x10u
#define  PMC_REGSC_BGEN_SHIFT        4
//   Bandgap Enable In VLPx Operation
//     0b0 ==> Bandgap voltage reference is disabled in VLPx , and VLLSx modes
//     0b1 ==> Bandgap voltage reference is enabled in VLPx , and VLLSx modes

#define  RCM_SRS0_WAKEUP_MASK        0x1u
#define  RCM_SRS0_WAKEUP_SHIFT       0
//   Low Leakage Wakeup Reset
//     0b0 ==> Reset not caused by LLWU module wakeup source
//     0b1 ==> Reset caused by LLWU module wakeup source

#define  RCM_SRS0_LVD_MASK           0x2u
#define  RCM_SRS0_LVD_SHIFT          1
//   Low-Voltage Detect Reset
//     0b0 ==> Reset not caused by LVD trip or POR
//     0b1 ==> Reset caused by LVD trip or POR

#define  RCM_SRS0_LOC_MASK           0x4u
#define  RCM_SRS0_LOC_SHIFT          2
//   Loss-of-Clock Reset
//     0b0 ==> Reset not caused by a loss of external clock.
//     0b1 ==> Reset caused by a loss of external clock.

#define  RCM_SRS0_LOL_MASK           0x8u
#define  RCM_SRS0_LOL_SHIFT          3
//   Loss-of-Lock Reset
//     0b0 ==> Reset not caused by a loss of lock in the PLL
//     0b1 ==> Reset caused by a loss of lock in the PLL

#define  RCM_SRS0_WDOG_MASK          0x20u
#define  RCM_SRS0_WDOG_SHIFT         5
//   Watchdog
//     0b0 ==> Reset not caused by watchdog timeout
//     0b1 ==> Reset caused by watchdog timeout

#define  RCM_SRS0_PIN_MASK           0x40u
#define  RCM_SRS0_PIN_SHIFT          6
//   External Reset Pin
//     0b0 ==> Reset not caused by external reset pin
//     0b1 ==> Reset caused by external reset pin

#define  RCM_SRS0_POR_MASK           0x80u
#define  RCM_SRS0_POR_SHIFT          7
//   Power-On Reset
//     0b0 ==> Reset not caused by POR
//     0b1 ==> Reset caused by POR

#define  RCM_SRS1_LOCKUP_MASK        0x2u
#define  RCM_SRS1_LOCKUP_SHIFT       1
//   Core Lockup
//     0b0 ==> Reset not caused by core LOCKUP event
//     0b1 ==> Reset caused by core LOCKUP event

#define  RCM_SRS1_SW_MASK            0x4u
#define  RCM_SRS1_SW_SHIFT           2
//   Software
//     0b0 ==> Reset not caused by software setting of SYSRESETREQ bit
//     0b1 ==> Reset caused by software setting of SYSRESETREQ bit

#define  RCM_SRS1_MDM_AP_MASK        0x8u
#define  RCM_SRS1_MDM_AP_SHIFT       3
//   MDM-AP System Reset Request
//     0b0 ==> Reset not caused by host debugger system setting of the System Reset Request bit
//     0b1 ==> Reset caused by host debugger system setting of the System Reset Request bit

#define  RCM_SRS1_SACKERR_MASK       0x20u
#define  RCM_SRS1_SACKERR_SHIFT      5
//   Stop Mode Acknowledge Error Reset
//     0b0 ==> Reset not caused by peripheral failure to acknowledge attempt to enter stop mode
//     0b1 ==> Reset caused by peripheral failure to acknowledge attempt to enter stop mode

#define  RCM_RPFC_RSTFLTSRW_MASK     0x2u
#define  RCM_RPFC_RSTFLTSRW_SHIFT    0
#define  RCM_RPFC_RSTFLTSRW(x)       (((uint8_t)(((uint8_t)(x))<<RCM_RPFC_RSTFLTSRW_SHIFT))&RCM_RPFC_RSTFLTSRW_MASK)
//   Reset Pin Filter Select in Run and Wait Modes
//     0b00 ==> All filtering disabled
//     0b01 ==> Bus clock filter enabled for normal operation
//     0b10 ==> LPO clock filter enabled for normal operation
//     0b11 ==> Reserved

#define  RCM_RPFC_RSTFLTSS_MASK      0x4u
#define  RCM_RPFC_RSTFLTSS_SHIFT     2
//   Reset Pin Filter Select in Stop Mode
//     0b0 ==> All filtering disabled
//     0b1 ==> LPO clock filter enabled

#define  RCM_RPFW_RSTFLTSEL_MASK     0x5u
#define  RCM_RPFW_RSTFLTSEL_SHIFT    0
#define  RCM_RPFW_RSTFLTSEL(x)       (((uint8_t)(((uint8_t)(x))<<RCM_RPFW_RSTFLTSEL_SHIFT))&RCM_RPFW_RSTFLTSEL_MASK)
//   Reset Pin Filter Bus Clock Select
//     0b00000 ==> Bus clock filter count is 1
//     0b00001 ==> Bus clock filter count is 2
//     0b00010 ==> Bus clock filter count is 3
//     0b00011 ==> Bus clock filter count is 4
//     0b00100 ==> Bus clock filter count is 5
//     0b00101 ==> Bus clock filter count is 6
//     0b00110 ==> Bus clock filter count is 7
//     0b00111 ==> Bus clock filter count is 8
//     0b01000 ==> Bus clock filter count is 9
//     0b01001 ==> Bus clock filter count is 10
//     0b01010 ==> Bus clock filter count is 11
//     0b01011 ==> Bus clock filter count is 12
//     0b01100 ==> Bus clock filter count is 13
//     0b01101 ==> Bus clock filter count is 14
//     0b01110 ==> Bus clock filter count is 15
//     0b01111 ==> Bus clock filter count is 16
//     0b10000 ==> Bus clock filter count is 17
//     0b10001 ==> Bus clock filter count is 18
//     0b10010 ==> Bus clock filter count is 19
//     0b10011 ==> Bus clock filter count is 20
//     0b10100 ==> Bus clock filter count is 21
//     0b10101 ==> Bus clock filter count is 22
//     0b10110 ==> Bus clock filter count is 23
//     0b10111 ==> Bus clock filter count is 24
//     0b11000 ==> Bus clock filter count is 25
//     0b11001 ==> Bus clock filter count is 26
//     0b11010 ==> Bus clock filter count is 27
//     0b11011 ==> Bus clock filter count is 28
//     0b11100 ==> Bus clock filter count is 29
//     0b11101 ==> Bus clock filter count is 30
//     0b11110 ==> Bus clock filter count is 31
//     0b11111 ==> Bus clock filter count is 32

#define  RNG_CR_GO_MASK              0x1u
#define  RNG_CR_GO_SHIFT             0
//     0b0 ==> RNGA Output Register is not loaded with random data.
//     0b1 ==> RNGA Output Register is loaded with random data.

#define  RNG_CR_HA_MASK              0x2u
#define  RNG_CR_HA_SHIFT             1
//   High Assurance
//     0b0 ==> Notification of security violations is enabled.
//     0b1 ==> Notification of security violations is masked.

#define  RNG_CR_INTM_MASK            0x4u
#define  RNG_CR_INTM_SHIFT           2
//   Interrupt Mask
//     0b0 ==> Interrupt is enabled.
//     0b1 ==> Interrupt is masked.

#define  RNG_CR_CLRI_MASK            0x8u
#define  RNG_CR_CLRI_SHIFT           3
//   Clear Interrupt
//     0b0 ==> Do not clear the interrupt.
//     0b1 ==> Clear the interrupt.

#define  RNG_CR_SLP_MASK             0x10u
#define  RNG_CR_SLP_SHIFT            4
//   Sleep
//     0b0 ==> RNGA is not in Sleep mode.
//     0b1 ==> RNGA is in Sleep mode.

#define  RNG_SR_SECV_MASK            0x1u
#define  RNG_SR_SECV_SHIFT           0
//   Security Violation
//     0b0 ==> No security violations have occured or the High Assurance bit (HA) in the RNGA Control Register is not set.
//     0b1 ==> A security violation has occurred.

#define  RNG_SR_LRS_MASK             0x2u
#define  RNG_SR_LRS_SHIFT            1
//   Last Read Status
//     0b0 ==> Last read was performed while the RNGA Output Register was not empty.
//     0b1 ==> Last read was performed while the RNGA Output Register was empty (underflow condition).

#define  RNG_SR_ORU_MASK             0x4u
#define  RNG_SR_ORU_SHIFT            2
//   Output Register Underflow
//     0b0 ==> The RNGA Output Register has not been read while empty since last read of the RNGA Status Register.
//     0b1 ==> The RNGA Output Register has been read while empty since last read of the RNGA Status Register.

#define  RNG_SR_ERRI_MASK            0x8u
#define  RNG_SR_ERRI_SHIFT           3
//   Error Interrupt
//     0b0 ==> The RNGA Output Register has not been read while empty.
//     0b1 ==> The RNGA Output Register has been read while empty.

#define  RNG_SR_SLP_MASK             0x10u
#define  RNG_SR_SLP_SHIFT            4
//   Sleep
//     0b0 ==> The RNGA is not in Sleep mode.
//     0b1 ==> The RNGA is in Sleep mode.

#define  RNG_SR_OREG_LVL_MASK        0x800u
#define  RNG_SR_OREG_LVL_SHIFT       8
#define  RNG_SR_OREG_LVL(x)          (((uint32_t)(((uint32_t)(x))<<RNG_SR_OREG_LVL_SHIFT))&RNG_SR_OREG_LVL_MASK)
//   Output Register Level

#define  RNG_SR_OREG_SIZE_MASK       0x80000u
#define  RNG_SR_OREG_SIZE_SHIFT      16
#define  RNG_SR_OREG_SIZE(x)         (((uint32_t)(((uint32_t)(x))<<RNG_SR_OREG_SIZE_SHIFT))&RNG_SR_OREG_SIZE_MASK)
//   Output Register Size

#define  RNG_ER_EXT_ENT_MASK         0x20u
#define  RNG_ER_EXT_ENT_SHIFT        0
#define  RNG_ER_EXT_ENT(x)           (((uint32_t)(((uint32_t)(x))<<RNG_ER_EXT_ENT_SHIFT))&RNG_ER_EXT_ENT_MASK)
//   External Entropy

#define  RNG_OR_RANDOUT_MASK         0x20u
#define  RNG_OR_RANDOUT_SHIFT        0
#define  RNG_OR_RANDOUT(x)           (((uint32_t)(((uint32_t)(x))<<RNG_OR_RANDOUT_SHIFT))&RNG_OR_RANDOUT_MASK)
//   Random Output

#define  ROM_ENTRY0_ENTRY_MASK       0x20u
#define  ROM_ENTRY0_ENTRY_SHIFT      0
#define  ROM_ENTRY0_ENTRY(x)         (((uint32_t)(((uint32_t)(x))<<ROM_ENTRY0_ENTRY_SHIFT))&ROM_ENTRY0_ENTRY_MASK)
//   ENTRY

#define  ROM_ENTRY1_ENTRY_MASK       0x20u
#define  ROM_ENTRY1_ENTRY_SHIFT      0
#define  ROM_ENTRY1_ENTRY(x)         (((uint32_t)(((uint32_t)(x))<<ROM_ENTRY1_ENTRY_SHIFT))&ROM_ENTRY1_ENTRY_MASK)
//   ENTRY

#define  ROM_ENTRY2_ENTRY_MASK       0x20u
#define  ROM_ENTRY2_ENTRY_SHIFT      0
#define  ROM_ENTRY2_ENTRY(x)         (((uint32_t)(((uint32_t)(x))<<ROM_ENTRY2_ENTRY_SHIFT))&ROM_ENTRY2_ENTRY_MASK)
//   ENTRY

#define  ROM_TABLEMARK_MARK_MASK     0x20u
#define  ROM_TABLEMARK_MARK_SHIFT    0
#define  ROM_TABLEMARK_MARK(x)       (((uint32_t)(((uint32_t)(x))<<ROM_TABLEMARK_MARK_SHIFT))&ROM_TABLEMARK_MARK_MASK)

#define  ROM_PERIPHID4_PERIPHID_MASK 0x20u
#define  ROM_PERIPHID4_PERIPHID_SHIFT 0
#define  ROM_PERIPHID4_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID4_PERIPHID_SHIFT))&ROM_PERIPHID4_PERIPHID_MASK)

#define  ROM_SYSACCESS_SYSACCESS_MASK 0x20u
#define  ROM_SYSACCESS_SYSACCESS_SHIFT 0
#define  ROM_SYSACCESS_SYSACCESS(x)  (((uint32_t)(((uint32_t)(x))<<ROM_SYSACCESS_SYSACCESS_SHIFT))&ROM_SYSACCESS_SYSACCESS_MASK)

#define  ROM_PERIPHID5_PERIPHID_MASK 0x20u
#define  ROM_PERIPHID5_PERIPHID_SHIFT 0
#define  ROM_PERIPHID5_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID5_PERIPHID_SHIFT))&ROM_PERIPHID5_PERIPHID_MASK)

#define  ROM_PERIPHID6_PERIPHID_MASK 0x20u
#define  ROM_PERIPHID6_PERIPHID_SHIFT 0
#define  ROM_PERIPHID6_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID6_PERIPHID_SHIFT))&ROM_PERIPHID6_PERIPHID_MASK)

#define  ROM_PERIPHID7_PERIPHID_MASK 0x20u
#define  ROM_PERIPHID7_PERIPHID_SHIFT 0
#define  ROM_PERIPHID7_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID7_PERIPHID_SHIFT))&ROM_PERIPHID7_PERIPHID_MASK)

#define  ROM_PERIPHID0_PERIPHID_MASK 0x20u
#define  ROM_PERIPHID0_PERIPHID_SHIFT 0
#define  ROM_PERIPHID0_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID0_PERIPHID_SHIFT))&ROM_PERIPHID0_PERIPHID_MASK)

#define  ROM_PERIPHID1_PERIPHID_MASK 0x20u
#define  ROM_PERIPHID1_PERIPHID_SHIFT 0
#define  ROM_PERIPHID1_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID1_PERIPHID_SHIFT))&ROM_PERIPHID1_PERIPHID_MASK)

#define  ROM_PERIPHID2_PERIPHID_MASK 0x20u
#define  ROM_PERIPHID2_PERIPHID_SHIFT 0
#define  ROM_PERIPHID2_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID2_PERIPHID_SHIFT))&ROM_PERIPHID2_PERIPHID_MASK)

#define  ROM_PERIPHID3_PERIPHID_MASK 0x20u
#define  ROM_PERIPHID3_PERIPHID_SHIFT 0
#define  ROM_PERIPHID3_PERIPHID(x)   (((uint32_t)(((uint32_t)(x))<<ROM_PERIPHID3_PERIPHID_SHIFT))&ROM_PERIPHID3_PERIPHID_MASK)

#define  ROM_COMPID0_COMPID_MASK     0x20u
#define  ROM_COMPID0_COMPID_SHIFT    0
#define  ROM_COMPID0_COMPID(x)       (((uint32_t)(((uint32_t)(x))<<ROM_COMPID0_COMPID_SHIFT))&ROM_COMPID0_COMPID_MASK)
//   Component ID

#define  ROM_COMPID1_COMPID_MASK     0x20u
#define  ROM_COMPID1_COMPID_SHIFT    0
#define  ROM_COMPID1_COMPID(x)       (((uint32_t)(((uint32_t)(x))<<ROM_COMPID1_COMPID_SHIFT))&ROM_COMPID1_COMPID_MASK)
//   Component ID

#define  ROM_COMPID2_COMPID_MASK     0x20u
#define  ROM_COMPID2_COMPID_SHIFT    0
#define  ROM_COMPID2_COMPID(x)       (((uint32_t)(((uint32_t)(x))<<ROM_COMPID2_COMPID_SHIFT))&ROM_COMPID2_COMPID_MASK)
//   Component ID

#define  ROM_COMPID3_COMPID_MASK     0x20u
#define  ROM_COMPID3_COMPID_SHIFT    0
#define  ROM_COMPID3_COMPID(x)       (((uint32_t)(((uint32_t)(x))<<ROM_COMPID3_COMPID_SHIFT))&ROM_COMPID3_COMPID_MASK)
//   Component ID

#define  RTC_YEARMON_MON_CNT_MASK    0x4u
#define  RTC_YEARMON_MON_CNT_SHIFT   0
#define  RTC_YEARMON_MON_CNT(x)      (((uint16_t)(((uint16_t)(x))<<RTC_YEARMON_MON_CNT_SHIFT))&RTC_YEARMON_MON_CNT_MASK)
//     0b0 ==> Illegal Value
//     0b1 ==> January
//     0b10 ==> October
//     0b11 ==> November

#define  RTC_YEARMON_YROFST_MASK     0x800u
#define  RTC_YEARMON_YROFST_SHIFT    8
#define  RTC_YEARMON_YROFST(x)       (((uint16_t)(((uint16_t)(x))<<RTC_YEARMON_YROFST_SHIFT))&RTC_YEARMON_YROFST_MASK)
//   Year Offset Count Value

#define  RTC_DAYS_DAY_CNT_MASK       0x5u
#define  RTC_DAYS_DAY_CNT_SHIFT      0
#define  RTC_DAYS_DAY_CNT(x)         (((uint16_t)(((uint16_t)(x))<<RTC_DAYS_DAY_CNT_SHIFT))&RTC_DAYS_DAY_CNT_MASK)
//   Days Counter Value.

#define  RTC_DAYS_DOW_MASK           0x300u
#define  RTC_DAYS_DOW_SHIFT          8
#define  RTC_DAYS_DOW(x)             (((uint16_t)(((uint16_t)(x))<<RTC_DAYS_DOW_SHIFT))&RTC_DAYS_DOW_MASK)
//   Day of Week Counter Value.
//     0b0 ==> Sunday
//     0b1 ==> Monday

#define  RTC_HOURMIN_MIN_CNT_MASK    0x6u
#define  RTC_HOURMIN_MIN_CNT_SHIFT   0
#define  RTC_HOURMIN_MIN_CNT(x)      (((uint16_t)(((uint16_t)(x))<<RTC_HOURMIN_MIN_CNT_SHIFT))&RTC_HOURMIN_MIN_CNT_MASK)
//   Minutes Counter Value.

#define  RTC_HOURMIN_HOUR_CNT_MASK   0x500u
#define  RTC_HOURMIN_HOUR_CNT_SHIFT  8
#define  RTC_HOURMIN_HOUR_CNT(x)     (((uint16_t)(((uint16_t)(x))<<RTC_HOURMIN_HOUR_CNT_SHIFT))&RTC_HOURMIN_HOUR_CNT_MASK)
//   Hours Counter Value.

#define  RTC_SECONDS_SEC_CNT_MASK    0x6u
#define  RTC_SECONDS_SEC_CNT_SHIFT   0
#define  RTC_SECONDS_SEC_CNT(x)      (((uint16_t)(((uint16_t)(x))<<RTC_SECONDS_SEC_CNT_SHIFT))&RTC_SECONDS_SEC_CNT_MASK)
//   Seconds Counter Value.

#define  RTC_ALM_YEARMON_ALM_MON_MASK 0x4u
#define  RTC_ALM_YEARMON_ALM_MON_SHIFT 0
#define  RTC_ALM_YEARMON_ALM_MON(x)  (((uint16_t)(((uint16_t)(x))<<RTC_ALM_YEARMON_ALM_MON_SHIFT))&RTC_ALM_YEARMON_ALM_MON_MASK)
//   Months Value for Alarm.

#define  RTC_ALM_YEARMON_ALM_YEAR_MASK 0x800u
#define  RTC_ALM_YEARMON_ALM_YEAR_SHIFT 8
#define  RTC_ALM_YEARMON_ALM_YEAR(x) (((uint16_t)(((uint16_t)(x))<<RTC_ALM_YEARMON_ALM_YEAR_SHIFT))&RTC_ALM_YEARMON_ALM_YEAR_MASK)
//   Year Value for Alarm.

#define  RTC_ALM_DAYS_ALM_DAY_MASK   0x5u
#define  RTC_ALM_DAYS_ALM_DAY_SHIFT  0
#define  RTC_ALM_DAYS_ALM_DAY(x)     (((uint16_t)(((uint16_t)(x))<<RTC_ALM_DAYS_ALM_DAY_SHIFT))&RTC_ALM_DAYS_ALM_DAY_MASK)
//   Days Value for Alarm.

#define  RTC_ALM_HOURMIN_ALM_MIN_MASK 0x6u
#define  RTC_ALM_HOURMIN_ALM_MIN_SHIFT 0
#define  RTC_ALM_HOURMIN_ALM_MIN(x)  (((uint16_t)(((uint16_t)(x))<<RTC_ALM_HOURMIN_ALM_MIN_SHIFT))&RTC_ALM_HOURMIN_ALM_MIN_MASK)
//   Minutes Value for Alarm.

#define  RTC_ALM_HOURMIN_ALM_HOUR_MASK 0x500u
#define  RTC_ALM_HOURMIN_ALM_HOUR_SHIFT 8
#define  RTC_ALM_HOURMIN_ALM_HOUR(x) (((uint16_t)(((uint16_t)(x))<<RTC_ALM_HOURMIN_ALM_HOUR_SHIFT))&RTC_ALM_HOURMIN_ALM_HOUR_MASK)
//   Hours Value for Alarm.

#define  RTC_ALM_SECONDS_ALM_SEC_MASK 0x6u
#define  RTC_ALM_SECONDS_ALM_SEC_SHIFT 0
#define  RTC_ALM_SECONDS_ALM_SEC(x)  (((uint16_t)(((uint16_t)(x))<<RTC_ALM_SECONDS_ALM_SEC_SHIFT))&RTC_ALM_SECONDS_ALM_SEC_MASK)

#define  RTC_ALM_SECONDS_DEC_SEC_MASK 0x100u
#define  RTC_ALM_SECONDS_DEC_SEC_SHIFT 8
//   Decrement Seconds Counter by 1.

#define  RTC_ALM_SECONDS_INC_SEC_MASK 0x200u
#define  RTC_ALM_SECONDS_INC_SEC_SHIFT 9
//   Increment Seconds Counter by 1.

#define  RTC_CTRL_FINEEN_MASK        0x1u
#define  RTC_CTRL_FINEEN_SHIFT       0
//   Fine compensation enable bit
//     0b1 ==> Fine compensation is enabled.
//     0b0 ==> Fine compensation is disabled

#define  RTC_CTRL_COMP_EN_MASK       0x2u
#define  RTC_CTRL_COMP_EN_SHIFT      1

#define  RTC_CTRL_ALM_MATCH_MASK     0x8u
#define  RTC_CTRL_ALM_MATCH_SHIFT    2
#define  RTC_CTRL_ALM_MATCH(x)       (((uint16_t)(((uint16_t)(x))<<RTC_CTRL_ALM_MATCH_SHIFT))&RTC_CTRL_ALM_MATCH_MASK)
//   Alarm Match bits.
//     0b00 ==> Only Seconds, Minutes, and Hours matched.
//     0b01 ==> Only Seconds, Minutes, Hours, and Days matched.
//     0b10 ==> Only Seconds, Minutes, Hours, Days, and Months matched.

#define  RTC_CTRL_TIMER_STB_MASK_MASK 0x10u
#define  RTC_CTRL_TIMER_STB_MASK_SHIFT 4
//   Sampling timer clocks mask
//     0b1 ==> Sampling clocks are gated in standby mode
//     0b0 ==> Sampling clocks are not gated when in standby mode

#define  RTC_CTRL_EXTRN_CLK_EN_MASK  0x20u
#define  RTC_CTRL_EXTRN_CLK_EN_SHIFT 5
//   External 1Hz clock enable signal
//     0b1 ==> External 1Hz clock is enabled for the calendaring and counters clock.
//     0b0 ==> External 1Hz clock is disabled for the calendaring and counters clock.

#define  RTC_CTRL_DST_EN_MASK        0x40u
#define  RTC_CTRL_DST_EN_SHIFT       6
//   Daylight Saving Enable.
//     0b0 ==> Disabled. Daylight saving changes are not applied. Daylight saving registers can be modified.
//     0b1 ==> Enabled. Daylight saving changes are applied.

#define  RTC_CTRL_SWR_MASK           0x100u
#define  RTC_CTRL_SWR_SHIFT          8
//   Software Reset bit.
//     0b0 ==> Software Reset cleared.
//     0b1 ==> Software Reset asserted.

#define  RTC_STATUS_INVAL_BIT_MASK   0x1u
#define  RTC_STATUS_INVAL_BIT_SHIFT  0
//   Invalidate CPU read/write access bit.
//     0b0 ==> Time /Date Counters can be read/written. Time /Date is valid.
//     0b1 ==> Time /Date Counter values are changing or Time /Date is invalid and cannot be read or written.

#define  RTC_STATUS_CPU_LOW_VOLT_MASK 0x4u
#define  RTC_STATUS_CPU_LOW_VOLT_SHIFT 2
//   CPU Low Voltage Warning status bit.
//     0b0 ==> CPU in Normal Operating Voltage.
//     0b1 ==> CPU Voltage is below Normal Operating Voltage. RTC Registers in read-only mode.

#define  RTC_STATUS_RST_SRC_MASK     0x8u
#define  RTC_STATUS_RST_SRC_SHIFT    3
//   Reset Source bit.
//     0b0 ==> Part was reset due to Standby Mode Exit (that is when VDD is powered up and VBAT was not powered down at all).
//     0b1 ==> Part was reset due to Power-On Reset (that is Power On Reset when both VBAT and VDD are powered up).

#define  RTC_STATUS_CMP_INT_MASK     0x20u
#define  RTC_STATUS_CMP_INT_SHIFT    5
//   Compensation Interval bit.

#define  RTC_STATUS_BUS_ERR_MASK     0x100u
#define  RTC_STATUS_BUS_ERR_SHIFT    8
//   Bus Error bit.
//     0b0 ==> Read and Write accesses are normal.
//     0b1 ==> Read or Write accesses occurred when INVAL_BIT was asserted.

#define  RTC_STATUS_CMP_DONE_MASK    0x800u
#define  RTC_STATUS_CMP_DONE_SHIFT   11
//   Compensation Done bit.
//     0b0 ==> Compensation busy or not enabled.
//     0b1 ==> Compensation completed.

#define  RTC_ISR_TAMPER_IS_MASK      0x1u
#define  RTC_ISR_TAMPER_IS_SHIFT     0
//   Tamper Interrupt Status bit.
//     0b0 ==> Interrupt is de-asserted.
//     0b1 ==> Interrupt is asserted (Default on reset) .

#define  RTC_ISR_ALM_IS_MASK         0x4u
#define  RTC_ISR_ALM_IS_SHIFT        2
//   Alarm Interrupt Status bit.
//     0b0 ==> Interrupt is de-asserted.
//     0b1 ==> Interrupt is asserted.

#define  RTC_ISR_DAY_IS_MASK         0x8u
#define  RTC_ISR_DAY_IS_SHIFT        3
//   Days Interrupt Status bit.
//     0b0 ==> Interrupt is de-asserted.
//     0b1 ==> Interrupt is asserted.

#define  RTC_ISR_HOUR_IS_MASK        0x10u
#define  RTC_ISR_HOUR_IS_SHIFT       4
//   Hours Interrupt Status bit.
//     0b0 ==> Interrupt is de-asserted.
//     0b1 ==> Interrupt is asserted.

#define  RTC_ISR_MIN_IS_MASK         0x20u
#define  RTC_ISR_MIN_IS_SHIFT        5
//   Minutes Interrupt Status bit.
//     0b0 ==> Interrupt is de-asserted.
//     0b1 ==> Interrupt is asserted.

#define  RTC_ISR_IS_1Hz_MASK         0x40u
#define  RTC_ISR_IS_1Hz_SHIFT        6
//   1 Hz Interval Interrupt Status bit.
//     0b0 ==> Interrupt is de-asserted.
//     0b1 ==> Interrupt is asserted.

#define  RTC_ISR_IS_2Hz_MASK         0x80u
#define  RTC_ISR_IS_2Hz_SHIFT        7
//   2 Hz Interval Interrupt Status bit.
//     0b0 ==> Interrupt is de-asserted.
//     0b1 ==> Interrupt is asserted.

#define  RTC_ISR_IS_4Hz_MASK         0x100u
#define  RTC_ISR_IS_4Hz_SHIFT        8
//   4 Hz Interval Interrupt Status bit.
//     0b0 ==> Interrupt is de-asserted.
//     0b1 ==> Interrupt is asserted.

#define  RTC_ISR_IS_8Hz_MASK         0x200u
#define  RTC_ISR_IS_8Hz_SHIFT        9
//   8 Hz Interval Interrupt Status bit.
//     0b0 ==> Interrupt is de-asserted.
//     0b1 ==> Interrupt is asserted.

#define  RTC_ISR_IS_16Hz_MASK        0x400u
#define  RTC_ISR_IS_16Hz_SHIFT       10
//   16 Hz Interval Interrupt Status bit.
//     0b0 ==> Interrupt is de-asserted.
//     0b1 ==> Interrupt is asserted.

#define  RTC_ISR_IS_32Hz_MASK        0x800u
#define  RTC_ISR_IS_32Hz_SHIFT       11
//   32 Hz Interval Interrupt Status bit.
//     0b0 ==> Interrupt is de-asserted.
//     0b1 ==> Interrupt is asserted.

#define  RTC_ISR_IS_64Hz_MASK        0x1000u
#define  RTC_ISR_IS_64Hz_SHIFT       12
//   64 Hz Interval Interrupt Status bit.
//     0b0 ==> Interrupt is de-asserted.
//     0b1 ==> Interrupt is asserted.

#define  RTC_ISR_IS_128Hz_MASK       0x2000u
#define  RTC_ISR_IS_128Hz_SHIFT      13
//   128 Hz Interval Interrupt Status bit.
//     0b0 ==> Interrupt is de-asserted.
//     0b1 ==> Interrupt is asserted.

#define  RTC_ISR_IS_256Hz_MASK       0x4000u
#define  RTC_ISR_IS_256Hz_SHIFT      14
//   256 Hz Interval Interrupt Status bit.
//     0b0 ==> Interrupt is de-asserted.
//     0b1 ==> Interrupt is asserted.

#define  RTC_ISR_IS_512Hz_MASK       0x8000u
#define  RTC_ISR_IS_512Hz_SHIFT      15
//   512 Hz Interval Interrupt Status bit.
//     0b0 ==> Interrupt is de-asserted.
//     0b1 ==> Interrupt is asserted.

#define  RTC_IER_TAMPER_IE_MASK      0x1u
#define  RTC_IER_TAMPER_IE_SHIFT     0
//   Tamper Interrupt Enable bit.
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled (Default on reset).

#define  RTC_IER_ALARM_IE_MASK       0x4u
#define  RTC_IER_ALARM_IE_SHIFT      2
//   Alarm Interrupt Enable bit.
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  RTC_IER_DAYS_IE_MASK        0x8u
#define  RTC_IER_DAYS_IE_SHIFT       3
//   Days Interrupt Enable bit.
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  RTC_IER_HOURS_IE_MASK       0x10u
#define  RTC_IER_HOURS_IE_SHIFT      4
//   Hours Interrupt Enable bit.
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  RTC_IER_MIN_IE_MASK         0x20u
#define  RTC_IER_MIN_IE_SHIFT        5
//   Minutes Interrupt Enable bit.
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  RTC_IER_IE_1Hz_MASK         0x40u
#define  RTC_IER_IE_1Hz_SHIFT        6
//   1 Hz Interval Interrupt Enable bit.
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  RTC_IER_IE_2Hz_MASK         0x80u
#define  RTC_IER_IE_2Hz_SHIFT        7
//   2 Hz Interval Interrupt Enable bit.
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  RTC_IER_IE_4Hz_MASK         0x100u
#define  RTC_IER_IE_4Hz_SHIFT        8
//   4 Hz Interval Interrupt Enable bit.
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  RTC_IER_IE_8Hz_MASK         0x200u
#define  RTC_IER_IE_8Hz_SHIFT        9
//   8 Hz Interval Interrupt Enable bit.
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  RTC_IER_IE_16Hz_MASK        0x400u
#define  RTC_IER_IE_16Hz_SHIFT       10
//   16 Hz Interval Interrupt Enable bit.
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  RTC_IER_IE_32Hz_MASK        0x800u
#define  RTC_IER_IE_32Hz_SHIFT       11
//   32 Hz Interval Interrupt Enable bit.
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  RTC_IER_IE_64Hz_MASK        0x1000u
#define  RTC_IER_IE_64Hz_SHIFT       12
//   64 Hz Interval Interrupt Enable bit.
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  RTC_IER_IE_128Hz_MASK       0x2000u
#define  RTC_IER_IE_128Hz_SHIFT      13
//   128 Hz Interval Interrupt Enable bit.
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  RTC_IER_IE_256Hz_MASK       0x4000u
#define  RTC_IER_IE_256Hz_SHIFT      14
//   256 Hz Interval Interrupt Enable bit.
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  RTC_IER_IE_512Hz_MASK       0x8000u
#define  RTC_IER_IE_512Hz_SHIFT      15
//   512 Hz Interval Interrupt Enable bit.
//     0b0 ==> Interrupt is disabled.
//     0b1 ==> Interrupt is enabled.

#define  RTC_GP_DATA_REG_GP_DATA_REG_MASK 0x10u
#define  RTC_GP_DATA_REG_GP_DATA_REG_SHIFT 0
#define  RTC_GP_DATA_REG_GP_DATA_REG(x) (((uint16_t)(((uint16_t)(x))<<RTC_GP_DATA_REG_GP_DATA_REG_SHIFT))&RTC_GP_DATA_REG_GP_DATA_REG_MASK)

#define  RTC_DST_HOUR_DST_END_HOUR_MASK 0x5u
#define  RTC_DST_HOUR_DST_END_HOUR_SHIFT 0
#define  RTC_DST_HOUR_DST_END_HOUR(x) (((uint16_t)(((uint16_t)(x))<<RTC_DST_HOUR_DST_END_HOUR_SHIFT))&RTC_DST_HOUR_DST_END_HOUR_MASK)
//   Daylight Saving Time (DST) Hours End Value.

#define  RTC_DST_HOUR_DST_START_HOUR_MASK 0x500u
#define  RTC_DST_HOUR_DST_START_HOUR_SHIFT 8
#define  RTC_DST_HOUR_DST_START_HOUR(x) (((uint16_t)(((uint16_t)(x))<<RTC_DST_HOUR_DST_START_HOUR_SHIFT))&RTC_DST_HOUR_DST_START_HOUR_MASK)
//   Daylight Saving Time (DST) Hours Start Value.

#define  RTC_DST_MONTH_DST_END_MONTH_MASK 0x4u
#define  RTC_DST_MONTH_DST_END_MONTH_SHIFT 0
#define  RTC_DST_MONTH_DST_END_MONTH(x) (((uint16_t)(((uint16_t)(x))<<RTC_DST_MONTH_DST_END_MONTH_SHIFT))&RTC_DST_MONTH_DST_END_MONTH_MASK)
//   Daylight Saving Time (DST) Month End Value.

#define  RTC_DST_MONTH_DST_START_MONTH_MASK 0x400u
#define  RTC_DST_MONTH_DST_START_MONTH_SHIFT 8
#define  RTC_DST_MONTH_DST_START_MONTH(x) (((uint16_t)(((uint16_t)(x))<<RTC_DST_MONTH_DST_START_MONTH_SHIFT))&RTC_DST_MONTH_DST_START_MONTH_MASK)
//   Daylight Saving Time (DST) Month Start Value.

#define  RTC_DST_DAY_DST_END_DAY_MASK 0x5u
#define  RTC_DST_DAY_DST_END_DAY_SHIFT 0
#define  RTC_DST_DAY_DST_END_DAY(x)  (((uint16_t)(((uint16_t)(x))<<RTC_DST_DAY_DST_END_DAY_SHIFT))&RTC_DST_DAY_DST_END_DAY_MASK)
//   Daylight Saving Time (DST) Day End Value.

#define  RTC_DST_DAY_DST_START_DAY_MASK 0x500u
#define  RTC_DST_DAY_DST_START_DAY_SHIFT 8
#define  RTC_DST_DAY_DST_START_DAY(x) (((uint16_t)(((uint16_t)(x))<<RTC_DST_DAY_DST_START_DAY_SHIFT))&RTC_DST_DAY_DST_START_DAY_MASK)
//   Daylight Saving Time (DST) Start Day Value.

#define  RTC_COMPEN_COMPEN_VAL_MASK  0x10u
#define  RTC_COMPEN_COMPEN_VAL_SHIFT 0
#define  RTC_COMPEN_COMPEN_VAL(x)    (((uint16_t)(((uint16_t)(x))<<RTC_COMPEN_COMPEN_VAL_SHIFT))&RTC_COMPEN_COMPEN_VAL_MASK)
//   Compensation Value

#define  RTC_TAMPER_DIRECTIONAL_A_P_TAMP_MASK 0x4u
#define  RTC_TAMPER_DIRECTIONAL_A_P_TAMP_SHIFT 0
#define  RTC_TAMPER_DIRECTIONAL_A_P_TAMP(x) (((uint16_t)(((uint16_t)(x))<<RTC_TAMPER_DIRECTIONAL_A_P_TAMP_SHIFT))&RTC_TAMPER_DIRECTIONAL_A_P_TAMP_MASK)

#define  RTC_TAMPER_DIRECTIONAL_I_O_TAMP_MASK 0x400u
#define  RTC_TAMPER_DIRECTIONAL_I_O_TAMP_SHIFT 8
#define  RTC_TAMPER_DIRECTIONAL_I_O_TAMP(x) (((uint16_t)(((uint16_t)(x))<<RTC_TAMPER_DIRECTIONAL_I_O_TAMP_SHIFT))&RTC_TAMPER_DIRECTIONAL_I_O_TAMP_MASK)

#define  RTC_TAMPER_INT_Q_FULL_MASK  0x1u
#define  RTC_TAMPER_INT_Q_FULL_SHIFT 0

#define  RTC_TAMPER_INT_Q_FULL_INT_EN_MASK 0x2u
#define  RTC_TAMPER_INT_Q_FULL_INT_EN_SHIFT 1

#define  RTC_TAMPER_INT_Q_CLEAR_MASK 0x4u
#define  RTC_TAMPER_INT_Q_CLEAR_SHIFT 2

#define  RTC_TAMPER_INT_LFSR_CLK_SEL_MASK 0x300u
#define  RTC_TAMPER_INT_LFSR_CLK_SEL_SHIFT 8
#define  RTC_TAMPER_INT_LFSR_CLK_SEL(x) (((uint16_t)(((uint16_t)(x))<<RTC_TAMPER_INT_LFSR_CLK_SEL_SHIFT))&RTC_TAMPER_INT_LFSR_CLK_SEL_MASK)

#define  RTC_TAMPER_INT_LFSR_DURATION_MASK 0x4000u
#define  RTC_TAMPER_INT_LFSR_DURATION_SHIFT 12
#define  RTC_TAMPER_INT_LFSR_DURATION(x) (((uint16_t)(((uint16_t)(x))<<RTC_TAMPER_INT_LFSR_DURATION_SHIFT))&RTC_TAMPER_INT_LFSR_DURATION_MASK)

#define  RTC_TTSR_YEAR_TS_YROFST_MASK 0x800u
#define  RTC_TTSR_YEAR_TS_YROFST_SHIFT 8
#define  RTC_TTSR_YEAR_TS_YROFST(x)  (((uint16_t)(((uint16_t)(x))<<RTC_TTSR_YEAR_TS_YROFST_SHIFT))&RTC_TTSR_YEAR_TS_YROFST_MASK)
//   Year Value for Tamper Time Stamp

#define  RTC_FILTER01_CFG_FIL_DUR1_MASK 0x4u
#define  RTC_FILTER01_CFG_FIL_DUR1_SHIFT 0
#define  RTC_FILTER01_CFG_FIL_DUR1(x) (((uint16_t)(((uint16_t)(x))<<RTC_FILTER01_CFG_FIL_DUR1_SHIFT))&RTC_FILTER01_CFG_FIL_DUR1_MASK)
//   Tamper Detect Bit 1 Filter Duration
//     0b0 ==> Filtering operation disabled.

#define  RTC_FILTER01_CFG_CLK_SEL1_MASK 0x30u
#define  RTC_FILTER01_CFG_CLK_SEL1_SHIFT 4
#define  RTC_FILTER01_CFG_CLK_SEL1(x) (((uint16_t)(((uint16_t)(x))<<RTC_FILTER01_CFG_CLK_SEL1_SHIFT))&RTC_FILTER01_CFG_CLK_SEL1_MASK)
//   Tamper Filter 1 Clock Select
//     0b0 ==> Clock to tamper filter 1 is 32.768 kHz (Oscillator clock)
//     0b1 ==> Clock to tamper filter 1 is 512 Hz Tamper filter duration is 2.85ms (i.e. 1.5 clock) to 125ms (64 clocks) in increments of 1.95ms.

#define  RTC_FILTER01_CFG_POL1_MASK  0x80u
#define  RTC_FILTER01_CFG_POL1_SHIFT 7
//   Tamper Detect Input Bit 1 Polarity Control
//     0b0 ==> Tamper detect input bit 1 is active high.
//     0b1 ==> Tamper detect input bit 1 is active low.

#define  RTC_FILTER01_CFG_FIL_DUR0_MASK 0x400u
#define  RTC_FILTER01_CFG_FIL_DUR0_SHIFT 8
#define  RTC_FILTER01_CFG_FIL_DUR0(x) (((uint16_t)(((uint16_t)(x))<<RTC_FILTER01_CFG_FIL_DUR0_SHIFT))&RTC_FILTER01_CFG_FIL_DUR0_MASK)
//   Tamper Detect Bit 0 Filter Duration
//     0b0 ==> Filtering operation disabled.

#define  RTC_FILTER01_CFG_CLK_SEL0_MASK 0x3000u
#define  RTC_FILTER01_CFG_CLK_SEL0_SHIFT 12
#define  RTC_FILTER01_CFG_CLK_SEL0(x) (((uint16_t)(((uint16_t)(x))<<RTC_FILTER01_CFG_CLK_SEL0_SHIFT))&RTC_FILTER01_CFG_CLK_SEL0_MASK)
//   Tamper Filter 0 Clock Select
//     0b0 ==> Clock to tamper filter 0 is 32.768 kHz (Oscillator clock) Tamper filter duration is 45.5us (i.e. 1.5 clock) to 1.95ms (64 clocks) in increments of 30.5us.
//     0b1 ==> Clock to tamper filter 0 is 512 Hz Tamper filter duration is 2.85ms (i.e. 1.5 clock) to 125ms (64 clocks) in increments of 1.95ms.

#define  RTC_FILTER01_CFG_POL0_MASK  0x8000u
#define  RTC_FILTER01_CFG_POL0_SHIFT 15
//   Tamper Detect Input Bit 0 Polarity Control
//     0b0 ==> Tamper detect input bit 0 is active high.
//     0b1 ==> Tamper detect input bit 0 is active low.

#define  RTC_FILTER23_CFG_FIL_DUR3_MASK 0x4u
#define  RTC_FILTER23_CFG_FIL_DUR3_SHIFT 0
#define  RTC_FILTER23_CFG_FIL_DUR3(x) (((uint16_t)(((uint16_t)(x))<<RTC_FILTER23_CFG_FIL_DUR3_SHIFT))&RTC_FILTER23_CFG_FIL_DUR3_MASK)
//   Tamper Detect Bit 3 Filter Duration
//     0b0 ==> Filtering operation disabled.

#define  RTC_FILTER23_CFG_CLK_SEL3_MASK 0x30u
#define  RTC_FILTER23_CFG_CLK_SEL3_SHIFT 4
#define  RTC_FILTER23_CFG_CLK_SEL3(x) (((uint16_t)(((uint16_t)(x))<<RTC_FILTER23_CFG_CLK_SEL3_SHIFT))&RTC_FILTER23_CFG_CLK_SEL3_MASK)
//   Tamper Filter 3 Clock Select
//     0b0 ==> Clock to tamper filter 3 is 32.768 kHz (Oscillator clock) Tamper filter duration is 45.5us (i.e. 1.5 clock) to 1.95ms (64 clocks) in increments of 30.5us.
//     0b1 ==> Clock to tamper filter 3is 512 Hz Tamper filter duration is 2.85ms (i.e. 1.5 clock) to 125ms (64 clocks) in increments of 1.95ms.

#define  RTC_FILTER23_CFG_POL3_MASK  0x80u
#define  RTC_FILTER23_CFG_POL3_SHIFT 7
//   Tamper Detect Input Bit 3 Polarity Control
//     0b0 ==> Tamper detect input bit 3 is active high.
//     0b1 ==> Tamper detect input bit 3 is active low.

#define  RTC_FILTER23_CFG_FIL_DUR2_MASK 0x400u
#define  RTC_FILTER23_CFG_FIL_DUR2_SHIFT 8
#define  RTC_FILTER23_CFG_FIL_DUR2(x) (((uint16_t)(((uint16_t)(x))<<RTC_FILTER23_CFG_FIL_DUR2_SHIFT))&RTC_FILTER23_CFG_FIL_DUR2_MASK)
//   Tamper Detect Bit 2 Filter Duration
//     0b0 ==> Filtering operation disabled.

#define  RTC_FILTER23_CFG_CLK_SEL2_MASK 0x3000u
#define  RTC_FILTER23_CFG_CLK_SEL2_SHIFT 12
#define  RTC_FILTER23_CFG_CLK_SEL2(x) (((uint16_t)(((uint16_t)(x))<<RTC_FILTER23_CFG_CLK_SEL2_SHIFT))&RTC_FILTER23_CFG_CLK_SEL2_MASK)
//   Tamper Filter 2 Clock Select
//     0b0 ==> Clock to tamper filter 2 is 32.768 kHz (Oscillator clock) Tamper filter duration is 45.5us (i.e. 1.5 clock) to 1.95ms (64 clocks) in increments of 30.5us.
//     0b1 ==> Clock to tamper filter 2 is 512 Hz Tamper filter duration is 2.85ms (i.e. 1.5 clock) to 125ms (64 clocks) in increments of 1.95ms.

#define  RTC_FILTER23_CFG_POL2_MASK  0x8000u
#define  RTC_FILTER23_CFG_POL2_SHIFT 15
//   Tamper Detect Input Bit 2 Polarity Control
//     0b0 ==> Tamper detect input bit 2 is active high.
//     0b1 ==> Tamper detect input bit 2 is active low.

#define  RTC_TAMPER_QUEUE_TAMPER_DATA_MASK 0x10u
#define  RTC_TAMPER_QUEUE_TAMPER_DATA_SHIFT 0
#define  RTC_TAMPER_QUEUE_TAMPER_DATA(x) (((uint16_t)(((uint16_t)(x))<<RTC_TAMPER_QUEUE_TAMPER_DATA_SHIFT))&RTC_TAMPER_QUEUE_TAMPER_DATA_MASK)
//   Tamper type stamp and pin number information register

#define  SIM_SOPT1_SRAM_SIZE_MASK    0x4000u
#define  SIM_SOPT1_SRAM_SIZE_SHIFT   12
#define  SIM_SOPT1_SRAM_SIZE(x)      (((uint32_t)(((uint32_t)(x))<<SIM_SOPT1_SRAM_SIZE_SHIFT))&SIM_SOPT1_SRAM_SIZE_MASK)
//   Returns the size of the system RAM
//     0b0101 ==> 16kB System RAM

#define  SIM_SOPT1_OSC32KSEL_MASK    0x80000u
#define  SIM_SOPT1_OSC32KSEL_SHIFT   18
#define  SIM_SOPT1_OSC32KSEL(x)      (((uint32_t)(((uint32_t)(x))<<SIM_SOPT1_OSC32KSEL_SHIFT))&SIM_SOPT1_OSC32KSEL_MASK)
//   32K oscillator clock select
//     0b00 ==> RTC 32KHz
//     0b01 ==> MHz Oscillator 32K Clock
//     0b10 ==> MCG irclk
//     0b11 ==> LPO 1kHz

#define  SIM_SOPT1_CFG_lptimer_ch1_sel_MASK 0x2u
#define  SIM_SOPT1_CFG_lptimer_ch1_sel_SHIFT 0
#define  SIM_SOPT1_CFG_lptimer_ch1_sel(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT1_CFG_lptimer_ch1_sel_SHIFT))&SIM_SOPT1_CFG_lptimer_ch1_sel_MASK)
//   LP timer Channel1 Select
//     0b00 ==> = Pad PTE4
//     0b01 ==> = Pad PTF4
//     0b10 ==> = Pad PTG1
//     0b11 ==> = Reserved

#define  SIM_SOPT1_CFG_lptimer_ch2_sel_MASK 0x8u
#define  SIM_SOPT1_CFG_lptimer_ch2_sel_SHIFT 2
#define  SIM_SOPT1_CFG_lptimer_ch2_sel(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT1_CFG_lptimer_ch2_sel_SHIFT))&SIM_SOPT1_CFG_lptimer_ch2_sel_MASK)
//   LP timer Channel2 Select
//     0b00 ==> = Pad PTD6
//     0b01 ==> = Pad PTF3
//     0b10 ==> = Pad PTG5
//     0b11 ==> = Reserved

#define  SIM_SOPT1_CFG_lptimer_ch3_sel_MASK 0x20u
#define  SIM_SOPT1_CFG_lptimer_ch3_sel_SHIFT 4
#define  SIM_SOPT1_CFG_lptimer_ch3_sel(x) (((uint32_t)(((uint32_t)(x))<<SIM_SOPT1_CFG_lptimer_ch3_sel_SHIFT))&SIM_SOPT1_CFG_lptimer_ch3_sel_MASK)
//   LP timer Channel3 Select
//     0b00 ==> = Pad PTD5
//     0b01 ==> = Pad PTG0
//     0b10 ==> = Pad PTG6
//     0b11 ==> = Reserved

#define  SIM_SOPT1_CFG_cmpout_lptim_ch_sel_MASK 0x40u
#define  SIM_SOPT1_CFG_cmpout_lptim_ch_sel_SHIFT 6
//   Comparator output selection for LPTIMER channel0
//     0b0 ==> CMP[1] output selected as LPTIMER input[0]
//     0b1 ==> CMP[0] output selected as LPTIMER input[0]

#define  SIM_SOPT1_CFG_RAMSBDIS_MASK 0x100u
#define  SIM_SOPT1_CFG_RAMSBDIS_SHIFT 8
//     0b0 ==> = Source bias of System SRAM enabled during VLPR and VLPW modes.
//     0b1 ==> = Source bias of System SRAM disabled during VLPR and VLPW modes.

#define  SIM_SOPT1_CFG_RAMBPEN_MASK  0x200u
#define  SIM_SOPT1_CFG_RAMBPEN_SHIFT 9
//   RAM Bitline Precharge Enable
//     0b0 ==> = Bitline precharge of system SRAM disabled during VLPR and VLPW modes.
//     0b1 ==> = Bitline precharge of system SRAM enabled during VLPR and VLPW modes.

#define  SIM_CTRL_REG_NMI_DIS_MASK   0x1u
#define  SIM_CTRL_REG_NMI_DIS_SHIFT  0
//   NMI Disable
//     0b0 ==> = NMI enabled
//     0b1 ==> = NMI disabled

#define  SIM_CTRL_REG_Clkout_sel_MASK 0x60u
#define  SIM_CTRL_REG_Clkout_sel_SHIFT 5
#define  SIM_CTRL_REG_Clkout_sel(x)  (((uint32_t)(((uint32_t)(x))<<SIM_CTRL_REG_Clkout_sel_SHIFT))&SIM_CTRL_REG_Clkout_sel_MASK)
//   Clock out Select
//     0b000 ==> = Disabled
//     0b001 ==> = Gated Core Clk
//     0b010 ==> = Bus/Flash Clk
//     0b011 ==> = LPO clock from PMC (may be removed since this is part of Muxed 32Khz source)
//     0b100 ==> = IRC clock from MCG (may be removed since this is part of Muxed 32Khz source)
//     0b101 ==> Muxed 32Khz source (please refer SOPT1[19:18] for possible options)
//     0b110 ==> Mhz Oscillator external reference clock
//     0b111 ==> PLL clock output from MCG

#define  SIM_CTRL_REG_Trace_clk_sel_MASK 0x1000u
#define  SIM_CTRL_REG_Trace_clk_sel_SHIFT 12
//   Trace clock select
//     0b0 ==> = mcg system output (mcg_out_clk)
//     0b1 ==> = platform free running clock

#define  SIM_SOPT3_VALDIS_MASK       0x100u
#define  SIM_SOPT3_VALDIS_SHIFT      8
//   Self-Time Adjust Disable
//     0b0 ==> Enable self-time adjust of compiled memories.
//     0b1 ==> Disable self-time adjust of compiled memories.

#define  SIM_SOPT3_VALRD_MASK        0x600u
#define  SIM_SOPT3_VALRD_SHIFT       9
#define  SIM_SOPT3_VALRD(x)          (((uint32_t)(((uint32_t)(x))<<SIM_SOPT3_VALRD_SHIFT))&SIM_SOPT3_VALRD_MASK)
//   Read Timing Control Value

#define  SIM_SOPT3_VALWR_MASK        0x3000u
#define  SIM_SOPT3_VALWR_SHIFT       12
#define  SIM_SOPT3_VALWR(x)          (((uint32_t)(((uint32_t)(x))<<SIM_SOPT3_VALWR_SHIFT))&SIM_SOPT3_VALWR_MASK)
//   Write Timing Control Value

#define  SIM_SOPT3_IFR_LCD_EN_MASK   0x8000u
#define  SIM_SOPT3_IFR_LCD_EN_SHIFT  15
//   LCD IFR control

#define  SIM_SDID_PINID_MASK         0x4u
#define  SIM_SDID_PINID_SHIFT        0
#define  SIM_SDID_PINID(x)           (((uint32_t)(((uint32_t)(x))<<SIM_SDID_PINID_SHIFT))&SIM_SDID_PINID_MASK)
//   Pincount identification
//     0b0011 ==> 44-pin
//     0b0100 ==> Reserved
//     0b0101 ==> 64-pin
//     0b0110 ==> Reserved
//     0b0111 ==> Reserved
//     0b1000 ==> 100-pin
//     0b1001 ==> Reserved
//     0b1010 ==> Reserved
//     0b1011 ==> Reserved
//     0b1100 ==> Reserved
//     0b1101 ==> Reserved
//     0b1110 ==> Reserved
//     0b1111 ==> Reserved

#define  SIM_SDID_DIE_ID_MASK        0x40u
#define  SIM_SDID_DIE_ID_SHIFT       4
#define  SIM_SDID_DIE_ID(x)          (((uint32_t)(((uint32_t)(x))<<SIM_SDID_DIE_ID_SHIFT))&SIM_SDID_DIE_ID_MASK)
//     0b0000 ==> First cut Photon Part

#define  SIM_SDID_REV_ID_MASK        0x400u
#define  SIM_SDID_REV_ID_SHIFT       8
#define  SIM_SDID_REV_ID(x)          (((uint32_t)(((uint32_t)(x))<<SIM_SDID_REV_ID_SHIFT))&SIM_SDID_REV_ID_MASK)
//     0b0000 ==> First cut Photon part

#define  SIM_SDID_SRAM_SIZE_MASK     0x4000u
#define  SIM_SDID_SRAM_SIZE_SHIFT    12
#define  SIM_SDID_SRAM_SIZE(x)       (((uint32_t)(((uint32_t)(x))<<SIM_SDID_SRAM_SIZE_SHIFT))&SIM_SDID_SRAM_SIZE_MASK)
//     0b0101 ==> 16kB SRAM

#define  SIM_SDID_ATTR_MASK          0x40000u
#define  SIM_SDID_ATTR_SHIFT         16
#define  SIM_SDID_ATTR(x)            (((uint32_t)(((uint32_t)(x))<<SIM_SDID_ATTR_SHIFT))&SIM_SDID_ATTR_MASK)
//     0b0000 ==> M0+ core

#define  SIM_SDID_SERIES_ID_MASK     0x400000u
#define  SIM_SDID_SERIES_ID_SHIFT    20
#define  SIM_SDID_SERIES_ID(x)       (((uint32_t)(((uint32_t)(x))<<SIM_SDID_SERIES_ID_SHIFT))&SIM_SDID_SERIES_ID_MASK)
//   Kinetis Attribute ID
//     0b0011 ==> Metering Series

#define  SIM_SDID_SUB_FAM_ID_MASK    0x4000000u
#define  SIM_SDID_SUB_FAM_ID_SHIFT   24
#define  SIM_SDID_SUB_FAM_ID(x)      (((uint32_t)(((uint32_t)(x))<<SIM_SDID_SUB_FAM_ID_SHIFT))&SIM_SDID_SUB_FAM_ID_MASK)
//   Kinetis Sub-Family ID
//     0b0010 ==> Photon with 2 AFE enabled
//     0b0011 ==> Photon with 3 AFE enabled
//     0b0100 ==> Photon with 4 AFE enabled

#define  SIM_SDID_FAM_ID_MASK        0x40000000u
#define  SIM_SDID_FAM_ID_SHIFT       28
#define  SIM_SDID_FAM_ID(x)          (((uint32_t)(((uint32_t)(x))<<SIM_SDID_FAM_ID_SHIFT))&SIM_SDID_FAM_ID_MASK)
//   Metering family ID
//     0b0001 ==> Photon without LCD
//     0b0011 ==> Photon with LCD

#define  SIM_SCGC4_EWM_MASK          0x2u
#define  SIM_SCGC4_EWM_SHIFT         1
//   External Watchdog Monitor Clock gate control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_MCG_MASK          0x10u
#define  SIM_SCGC4_MCG_SHIFT         4
//   MCG clock gate control.
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_OSC_32K_MASK      0x20u
#define  SIM_SCGC4_OSC_32K_SHIFT     5
//   Oscillator (32Khz) Clock gate control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_OSC_MASK          0x40u
#define  SIM_SCGC4_OSC_SHIFT         6
//   Oscillator (Mhz) Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_IIC0_MASK         0x80u
#define  SIM_SCGC4_IIC0_SHIFT        7
//   IIC0 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_IIC1_MASK         0x100u
#define  SIM_SCGC4_IIC1_SHIFT        8
//   IIC1 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_SCI0_MASK         0x400u
#define  SIM_SCGC4_SCI0_SHIFT        10
//   SCI0 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_SCI1_MASK         0x800u
#define  SIM_SCGC4_SCI1_SHIFT        11
//   SCI1 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_SCI2_MASK         0x1000u
#define  SIM_SCGC4_SCI2_SHIFT        12
//   SCI2 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_SCI3_MASK         0x2000u
#define  SIM_SCGC4_SCI3_SHIFT        13
//   SCI3 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_VREF_MASK         0x8000u
#define  SIM_SCGC4_VREF_SHIFT        15
//   VREF Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_HSCMP0_MASK       0x40000u
#define  SIM_SCGC4_HSCMP0_SHIFT      18
//   High Speed Comparator0 Clock Gate Control.
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_HSCMP1_MASK       0x80000u
#define  SIM_SCGC4_HSCMP1_SHIFT      19
//   High Speed Comparator1 Clock Gate Control.
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_SPI0_MASK         0x200000u
#define  SIM_SCGC4_SPI0_SHIFT        21
//   SPI0 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_SPI1_MASK         0x400000u
#define  SIM_SCGC4_SPI1_SHIFT        22
//   SPI1 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_RCM_MASK          0x8000000u
#define  SIM_SCGC4_RCM_SHIFT         27
//   Reset Controller Module Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_LLWU_MASK         0x10000000u
#define  SIM_SCGC4_LLWU_SHIFT        28
//   Low Leakage Wake Up Unit Clock Gate Control.
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_PMC_MASK          0x20000000u
#define  SIM_SCGC4_PMC_SHIFT         29
//   Power Mode Controller Clock Gate Control.
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC4_SMC_MASK          0x40000000u
#define  SIM_SCGC4_SMC_SHIFT         30
//   System Mode Controller Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_ATX_MASK          0x2u
#define  SIM_SCGC5_ATX_SHIFT         1
//   ATX clock gating control
//     0b0 ==> Disables the clock for the ATX module (default)
//     0b1 ==> Enables the clock for the ATX module

#define  SIM_SCGC5_SGLCD_MASK        0x8u
#define  SIM_SCGC5_SGLCD_SHIFT       3
//   Segmented LCD Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_PCTLA_MASK        0x40u
#define  SIM_SCGC5_PCTLA_SHIFT       6
//   PCTLA Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_PCTLB_MASK        0x80u
#define  SIM_SCGC5_PCTLB_SHIFT       7
//   PCTLB Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_PCTLC_MASK        0x100u
#define  SIM_SCGC5_PCTLC_SHIFT       8
//   PCTLC Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_PCTLD_MASK        0x200u
#define  SIM_SCGC5_PCTLD_SHIFT       9
//   PCTLD Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_PCTLE_MASK        0x400u
#define  SIM_SCGC5_PCTLE_SHIFT       10
//   PCTLE Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_PCTLF_MASK        0x800u
#define  SIM_SCGC5_PCTLF_SHIFT       11
//   PCTLF Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_PCTLG_MASK        0x1000u
#define  SIM_SCGC5_PCTLG_SHIFT       12
//   PCTLG Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_PCTLH_MASK        0x2000u
#define  SIM_SCGC5_PCTLH_SHIFT       13
//   PCTLH Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_PCTLI_MASK        0x4000u
#define  SIM_SCGC5_PCTLI_SHIFT       14
//   PCTLI Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_IRTC_MASK         0x10000u
#define  SIM_SCGC5_IRTC_SHIFT        16
//   IRTC Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_IRTC_REG_FILE_MASK 0x20000u
#define  SIM_SCGC5_IRTC_REG_FILE_SHIFT 17
//   IRTC_REG_FILE Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_WDOG_MASK         0x80000u
#define  SIM_SCGC5_WDOG_SHIFT        19
//   Watchdog Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_PXBAR_MASK        0x200000u
#define  SIM_SCGC5_PXBAR_SHIFT       21
//   Peripheral Crossbar Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_QTMR0_MASK        0x800000u
#define  SIM_SCGC5_QTMR0_SHIFT       23
//   Quadtimer0 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_QTMR1_MASK        0x1000000u
#define  SIM_SCGC5_QTMR1_SHIFT       24
//   Quadtimer1 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_QTMR2_MASK        0x2000000u
#define  SIM_SCGC5_QTMR2_SHIFT       25
//   Quadtimer2 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC5_QTMR3_MASK        0x4000000u
#define  SIM_SCGC5_QTMR3_SHIFT       26
//   Quadtimer3 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_FTFA_MASK         0x1u
#define  SIM_SCGC6_FTFA_SHIFT        0
//   FTFA Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_DMA_MUX0_MASK     0x2u
#define  SIM_SCGC6_DMA_MUX0_SHIFT    1
//   DMA_MUX0 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_DMA_MUX1_MASK     0x4u
#define  SIM_SCGC6_DMA_MUX1_SHIFT    2
//   DMA_MUX1 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_DMA_MUX2_MASK     0x8u
#define  SIM_SCGC6_DMA_MUX2_SHIFT    3
//   DMA_MUX2 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_DMA_MUX3_MASK     0x10u
#define  SIM_SCGC6_DMA_MUX3_SHIFT    4
//   DMA_MUX3 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_RNGA_MASK         0x200u
#define  SIM_SCGC6_RNGA_SHIFT        9
//   RNGA Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_SAR_ADC_MASK      0x800u
#define  SIM_SCGC6_SAR_ADC_SHIFT     11
//   SAR ADC Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_PIT0_MASK         0x2000u
#define  SIM_SCGC6_PIT0_SHIFT        13
//   PIT0 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_PIT1_MASK         0x4000u
#define  SIM_SCGC6_PIT1_SHIFT        14
//   PIT1 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_AFE_CH1_MASK      0x10000u
#define  SIM_SCGC6_AFE_CH1_SHIFT     16
//   AFE Channel1 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_AFE_CH2_MASK      0x20000u
#define  SIM_SCGC6_AFE_CH2_SHIFT     17
//   AFE Channel2 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_AFE_CH3_MASK      0x40000u
#define  SIM_SCGC6_AFE_CH3_SHIFT     18
//   AFE Channel3 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_AFE_CH4_MASK      0x80000u
#define  SIM_SCGC6_AFE_CH4_SHIFT     19
//   AFE Channel4 Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_PCRC_MASK         0x100000u
#define  SIM_SCGC6_PCRC_SHIFT        20
//   Programmable CRC Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_LPTIMER_MASK      0x10000000u
#define  SIM_SCGC6_LPTIMER_SHIFT     28
//   LPTIMER Clock Gate Control
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC6_SIM_DGO_MASK      0x40000000u
#define  SIM_SCGC6_SIM_DGO_SHIFT     30
//   SIM_DGO Clock Gate Control
//     0b1 ==> = Clock is enabled
//     0b0 ==> = Clock is disabled

#define  SIM_SCGC6_SIM_MASK          0x80000000u
#define  SIM_SCGC6_SIM_SHIFT         31
//   SIM Clock Gate Control
//     0b1 ==> = Clock is always enabled to SIM

#define  SIM_SCGC7_MPU_MASK          0x1u
#define  SIM_SCGC7_MPU_SHIFT         0
//   MPU Clock Gate control.
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_SCGC7_DMA_MASK          0x2u
#define  SIM_SCGC7_DMA_SHIFT         1
//   DMA Clock Gate control.
//     0b0 ==> = Clock disabled
//     0b1 ==> = Clock enabled

#define  SIM_CLKDIV1_SYS_CLK_MODE_MASK 0x8000000u
#define  SIM_CLKDIV1_SYS_CLK_MODE_SHIFT 27
//   System Clock Mode
//     0b0 ==> = 1:1:1
//     0b1 ==> = 2:1:1

#define  SIM_CLKDIV1_SYS_DIV_MASK    0x40000000u
#define  SIM_CLKDIV1_SYS_DIV_SHIFT   28
#define  SIM_CLKDIV1_SYS_DIV(x)      (((uint32_t)(((uint32_t)(x))<<SIM_CLKDIV1_SYS_DIV_SHIFT))&SIM_CLKDIV1_SYS_DIV_MASK)
//   System Clock divider
//     0b0000 ==> = Divide by 1
//     0b0001 ==> = Divide by 2
//     0b0010 ==> = Divide by 3
//     0b0011 ==> = Divide by 4 and so on..... If FOPT[0] is set the divider is set to div-by-8 after system reset is deasserted (after completion of system initialization sequence)

#define  SIM_FCFG1_FLASHDIS_MASK     0x1u
#define  SIM_FCFG1_FLASHDIS_SHIFT    0
//   Flash Disable
//     0b0 ==> Flash is enabled
//     0b1 ==> Flash is disabled

#define  SIM_FCFG1_FLASHDOZE_MASK    0x2u
#define  SIM_FCFG1_FLASHDOZE_SHIFT   1
//   Flash Doze
//     0b0 ==> Flash remains enabled during Wait mode
//     0b1 ==> Flash is disabled for the duration of Wait mode

#define  SIM_FCFG1_PFSIZE_MASK       0x4000000u
#define  SIM_FCFG1_PFSIZE_SHIFT      24
#define  SIM_FCFG1_PFSIZE(x)         (((uint32_t)(((uint32_t)(x))<<SIM_FCFG1_PFSIZE_SHIFT))&SIM_FCFG1_PFSIZE_MASK)
//   Program flash size
//     0b0000 ==> 0 KB of program flash memory, 0 KB protection region
//     0b0001 ==> 8 KB of program flash memory, 1 KB protection region
//     0b0011 ==> 16 KB of program flash memory, 1 KB protection region
//     0b0100 ==> 24 KB of program flash memory, 1 KB protection region
//     0b0101 ==> 32 KB of program flash memory, 1 KB protection region
//     0b0110 ==> 48 KB of program flash memory, 2 KB protection region
//     0b0111 ==> 64 KB of program flash memory, 2 KB protection region
//     0b1000 ==> 96 KB of program flash memory, 4 KB protection region
//     0b1001 ==> 128 KB of program flash memory, 4 KB protection region
//     0b1111 ==> (Default)

#define  SIM_FCFG2_MAXADDR_MASK      0x7000000u
#define  SIM_FCFG2_MAXADDR_SHIFT     24
#define  SIM_FCFG2_MAXADDR(x)        (((uint32_t)(((uint32_t)(x))<<SIM_FCFG2_MAXADDR_SHIFT))&SIM_FCFG2_MAXADDR_MASK)
//   Max address block

#define  SIM_UID0_UID_MASK           0x20u
#define  SIM_UID0_UID_SHIFT          0
#define  SIM_UID0_UID(x)             (((uint32_t)(((uint32_t)(x))<<SIM_UID0_UID_SHIFT))&SIM_UID0_UID_MASK)
//   Unique Identification

#define  SIM_UID1_UID_MASK           0x20u
#define  SIM_UID1_UID_SHIFT          0
#define  SIM_UID1_UID(x)             (((uint32_t)(((uint32_t)(x))<<SIM_UID1_UID_SHIFT))&SIM_UID1_UID_MASK)
//   Unique Identification

#define  SIM_UID2_UID_MASK           0x20u
#define  SIM_UID2_UID_SHIFT          0
#define  SIM_UID2_UID(x)             (((uint32_t)(((uint32_t)(x))<<SIM_UID2_UID_SHIFT))&SIM_UID2_UID_MASK)
//   Unique Identification

#define  SIM_UID3_UID_MASK           0x20u
#define  SIM_UID3_UID_SHIFT          0
#define  SIM_UID3_UID(x)             (((uint32_t)(((uint32_t)(x))<<SIM_UID3_UID_SHIFT))&SIM_UID3_UID_MASK)
//   Unique Identification

#define  SIM_MISC_CTL_afe_mod_out_sel_MASK 0x2u
#define  SIM_MISC_CTL_afe_mod_out_sel_SHIFT 0
#define  SIM_MISC_CTL_afe_mod_out_sel(x) (((uint32_t)(((uint32_t)(x))<<SIM_MISC_CTL_afe_mod_out_sel_SHIFT))&SIM_MISC_CTL_afe_mod_out_sel_MASK)
//   AFE Modulator Output Selection
//     0b00 ==> = Sigma Delta Modulator 0 data output
//     0b01 ==> = Sigma Delta Modulator 1 data output
//     0b10 ==> = Sigma Delta Modulator 2 data output
//     0b11 ==> = Sigma Delta Modulator 3 data output

#define  SIM_MISC_CTL_dma_done_sel_MASK 0x8u
#define  SIM_MISC_CTL_dma_done_sel_SHIFT 2
#define  SIM_MISC_CTL_dma_done_sel(x) (((uint32_t)(((uint32_t)(x))<<SIM_MISC_CTL_dma_done_sel_SHIFT))&SIM_MISC_CTL_dma_done_sel_MASK)
//   DMA Done flag select
//     0b00 ==> = DMA0
//     0b01 ==> = DMA1
//     0b10 ==> = DMA2
//     0b11 ==> = DMA3

#define  SIM_MISC_CTL_sci0_irda_sel_MASK 0x100u
#define  SIM_MISC_CTL_sci0_irda_sel_SHIFT 8
//   Selects SCI0 for IRDA operation
//     0b0 ==> = Pad RX input (PTD[0] or PTF[3], as selected in Pinmux control) selected for RX input of SCI0 and SCI0 TX signal is not used for modulation
//     0b1 ==> = SCI0 selected for IRDA modulation. SCI0 TX modulated by PXBAR_OUT[14] and SCI0 RX input connected to PXBAR_OUT[13]

#define  SIM_MISC_CTL_sci1_irda_sel_MASK 0x200u
#define  SIM_MISC_CTL_sci1_irda_sel_SHIFT 9
//   Selects SCI1 for IRDA operation
//     0b0 ==> = Pad RX input (PTD[2] or PTI[0], as selected in Pinmux control) selected for RX input of SCI1 and SCI1 TX signal is not used for modulation
//     0b1 ==> = SCI1 selected for IRDA modulation. SCI1 TX modulated by PXBAR_OUT[14] and SCI1 RX input connected to PXBAR_OUT[13]

#define  SIM_MISC_CTL_sci2_irda_sel_MASK 0x400u
#define  SIM_MISC_CTL_sci2_irda_sel_SHIFT 10
//   Selects SCI2 for IRDA operation
//     0b0 ==> = = Pad RX input PTE[6] selected for RX input of SCI2 and SCI2 TX signal is not used for modulation
//     0b1 ==> = SCI2 selected for IRDA modulation. SCI2 TX modulated by PXBAR_OUT[14] and SCI2 RX input connected to PXBAR_OUT[13].

#define  SIM_MISC_CTL_sci3_irda_sel_MASK 0x800u
#define  SIM_MISC_CTL_sci3_irda_sel_SHIFT 11
//   Selects SCI3 for IRDA operation
//     0b0 ==> = Pad RX input (PTC[3] or PTD[7], as selected in Pinmux control) selected for RX input of SCI3 and SCI3 TX signal is not used for modulation
//     0b1 ==> = SCI3 selected for IRDA modulation. SCI3 TX modulated by PXBAR_OUT[14] and SCI3 RX input connected to PXBAR_OUT[13].

#define  SIM_MISC_CTL_pit_sel_MASK   0x2000u
#define  SIM_MISC_CTL_pit_sel_SHIFT  12
#define  SIM_MISC_CTL_pit_sel(x)     (((uint32_t)(((uint32_t)(x))<<SIM_MISC_CTL_pit_sel_SHIFT))&SIM_MISC_CTL_pit_sel_MASK)
//   PIT Output select
//     0b00 ==> = PIT0 [0] (default)
//     0b01 ==> = PIT0 [1]
//     0b10 ==> = PIT1[0]
//     0b11 ==> = PIT1[1]

#define  SIM_MISC_CTL_EWM_IN_Sel_MASK 0x4000u
#define  SIM_MISC_CTL_EWM_IN_Sel_SHIFT 14
//   External Watchdog Monitor Select
//     0b0 ==> = Input from PAD (PTE[2] or PTE[4] as selected from Pinmux control )
//     0b1 ==> = Peripheral Crossbar (PXBAR) Output[11]

#define  SIM_MISC_CTL_tmr_ch0_sel_MASK 0x10000u
#define  SIM_MISC_CTL_tmr_ch0_sel_SHIFT 16
//   Quadtimer Channel0 Select
//     0b0 ==> = Pad PTF1 or PTD5, depending upon PCTL configuration.
//     0b1 ==> = Peripheral Crossbar (PXBAR) Output[5]

#define  SIM_MISC_CTL_tmr_ch1_sel_MASK 0x20000u
#define  SIM_MISC_CTL_tmr_ch1_sel_SHIFT 17
//   Quadtimer Channel1 Select
//     0b0 ==> = Pad PTG0 or PTC6, depending upon PCTL configuration.
//     0b1 ==> = Peripheral Crossbar (PXBAR) Output[6]

#define  SIM_MISC_CTL_tmr_ch2_sel_MASK 0x40000u
#define  SIM_MISC_CTL_tmr_ch2_sel_SHIFT 18
//   Quadtimer Channel2 Select
//     0b0 ==> = Pad PTF7 or PTF0, depending upon PCTL configuration.
//     0b1 ==> = Peripheral Crossbar (PXBAR) Output[7]

#define  SIM_MISC_CTL_tmr_ch3_sel_MASK 0x80000u
#define  SIM_MISC_CTL_tmr_ch3_sel_SHIFT 19
//   Quadtimer Channel3 Select
//     0b0 ==> = Pad PTE5 or PTD1, depending upon PCTL configuration.
//     0b1 ==> = Peripheral Crossbar (PXBAR) Output[8]

#define  SIM_MISC_CTL_tmr_clk0_sel_MASK 0x200000u
#define  SIM_MISC_CTL_tmr_clk0_sel_SHIFT 20
#define  SIM_MISC_CTL_tmr_clk0_sel(x) (((uint32_t)(((uint32_t)(x))<<SIM_MISC_CTL_tmr_clk0_sel_SHIFT))&SIM_MISC_CTL_tmr_clk0_sel_MASK)
//   Quadtimer Channel0 Clock Select
//     0b00 ==> = Bus Clock
//     0b01 ==> = Peripheral Crossbar Output [9]
//     0b10 ==> = Peripheral Crossbar Output [10]
//     0b11 ==> = Reserved

#define  SIM_MISC_CTL_tmr_clk1_sel_MASK 0x800000u
#define  SIM_MISC_CTL_tmr_clk1_sel_SHIFT 22
#define  SIM_MISC_CTL_tmr_clk1_sel(x) (((uint32_t)(((uint32_t)(x))<<SIM_MISC_CTL_tmr_clk1_sel_SHIFT))&SIM_MISC_CTL_tmr_clk1_sel_MASK)
//   Quadtimer Channel1 Clock Select
//     0b00 ==> = Bus Clock
//     0b01 ==> = Peripheral Crossbar Output [9]
//     0b10 ==> = Peripheral Crossbar Output [10]
//     0b11 ==> = Reserved

#define  SIM_MISC_CTL_tmr_clk2_sel_MASK 0x2000000u
#define  SIM_MISC_CTL_tmr_clk2_sel_SHIFT 24
#define  SIM_MISC_CTL_tmr_clk2_sel(x) (((uint32_t)(((uint32_t)(x))<<SIM_MISC_CTL_tmr_clk2_sel_SHIFT))&SIM_MISC_CTL_tmr_clk2_sel_MASK)
//   Quadtimer Channel2 Clock Select
//     0b00 ==> = Bus Clock
//     0b01 ==> = Peripheral Crossbar Input [9]
//     0b10 ==> = Peripheral Crossbar Input [10]
//     0b11 ==> = Reserved

#define  SIM_MISC_CTL_tmr_clk3_sel_MASK 0x8000000u
#define  SIM_MISC_CTL_tmr_clk3_sel_SHIFT 26
#define  SIM_MISC_CTL_tmr_clk3_sel(x) (((uint32_t)(((uint32_t)(x))<<SIM_MISC_CTL_tmr_clk3_sel_SHIFT))&SIM_MISC_CTL_tmr_clk3_sel_MASK)
//   Quadtimer Channel3 Clock Select
//     0b00 ==> = Bus Clock
//     0b01 ==> = Peripheral Crossbar Input [9]
//     0b10 ==> = Peripheral Crossbar Input [10]
//     0b11 ==> = Reserved

#define  SIM_MISC_CTL_rtc_ir_clk_sel_MASK 0x10000000u
#define  SIM_MISC_CTL_rtc_ir_clk_sel_SHIFT 28
//   RTC IRC Clock select
//     0b0 ==> = RTC OSC_32K clock selected
//     0b1 ==> = IRC Clock selected

#define  SIM_MISC_CTL_Vref_buffer_cap_sel_MASK 0x20000000u
#define  SIM_MISC_CTL_Vref_buffer_cap_sel_SHIFT 29
//   Operates switch in VrefBuffer to enable the internal 1.2v reference to be driven to VREF pad.
//     0b0 ==> Buffer does not drive PAD
//     0b1 ==> Buffer drives selected voltage (selected by vref_buffer_sel) on pad

#define  SIM_MISC_CTL_Vref_buffer_sel_MASK 0x40000000u
#define  SIM_MISC_CTL_Vref_buffer_sel_SHIFT 30
//   Slects Between Internal 1.2v reference and external reference for SAR and DAC
//     0b0 ==> Internal Reference selected as Buffer Input
//     0b1 ==> External Reference selected as Buffer Input

#define  SIM_MISC_CTL_Vref_buffer_pd_MASK 0x80000000u
#define  SIM_MISC_CTL_Vref_buffer_pd_SHIFT 31
//   Powers down VrefBuffer when set
//     0b0 ==> Buffer Enabled
//     0b1 ==> Buffer Powered Down

#define  SIM_ADC_COMP0_ADC_COMP_VAL0_MASK 0x10u
#define  SIM_ADC_COMP0_ADC_COMP_VAL0_SHIFT 0
#define  SIM_ADC_COMP0_ADC_COMP_VAL0(x) (((uint32_t)(((uint32_t)(x))<<SIM_ADC_COMP0_ADC_COMP_VAL0_SHIFT))&SIM_ADC_COMP0_ADC_COMP_VAL0_MASK)
//   ADC Temperature Compensation Value

#define  SIM_ADC_COMP0_ADC_COMP_VAL1_MASK 0x100000u
#define  SIM_ADC_COMP0_ADC_COMP_VAL1_SHIFT 16
#define  SIM_ADC_COMP0_ADC_COMP_VAL1(x) (((uint32_t)(((uint32_t)(x))<<SIM_ADC_COMP0_ADC_COMP_VAL1_SHIFT))&SIM_ADC_COMP0_ADC_COMP_VAL1_MASK)
//   ADC Temperature Compensation Value

#define  SIM_ADC_COMP1_ADC_COMP_VAL2_MASK 0x10u
#define  SIM_ADC_COMP1_ADC_COMP_VAL2_SHIFT 0
#define  SIM_ADC_COMP1_ADC_COMP_VAL2(x) (((uint32_t)(((uint32_t)(x))<<SIM_ADC_COMP1_ADC_COMP_VAL2_SHIFT))&SIM_ADC_COMP1_ADC_COMP_VAL2_MASK)
//   ADC Temperature Compensation Value

#define  SMC_PMPROT_AVLLS_MASK       0x2u
#define  SMC_PMPROT_AVLLS_SHIFT      1
//   Allow Very-Low-Leakage Stop Mode
//     0b0 ==> Any VLLSx mode is not allowed
//     0b1 ==> Any VLLSx mode is allowed

#define  SMC_PMPROT_AVLP_MASK        0x20u
#define  SMC_PMPROT_AVLP_SHIFT       5
//   Allow Very-Low-Power Modes
//     0b0 ==> VLPR, VLPW and VLPS are not allowed
//     0b1 ==> VLPR, VLPW and VLPS are allowed

#define  SMC_PMCTRL_STOPM_MASK       0x3u
#define  SMC_PMCTRL_STOPM_SHIFT      0
#define  SMC_PMCTRL_STOPM(x)         (((uint8_t)(((uint8_t)(x))<<SMC_PMCTRL_STOPM_SHIFT))&SMC_PMCTRL_STOPM_MASK)
//   Stop Mode Control
//     0b000 ==> Normal Stop (STOP)
//     0b001 ==> Reserved
//     0b010 ==> Very-Low-Power Stop (VLPS)
//     0b011 ==> Reserved
//     0b100 ==> Very-Low-Leakage Stop (VLLSx)
//     0b101 ==> Reserved
//     0b110 ==> Reseved
//     0b111 ==> Reserved

#define  SMC_PMCTRL_STOPA_MASK       0x8u
#define  SMC_PMCTRL_STOPA_SHIFT      3
//   Stop Aborted
//     0b0 ==> The previous stop mode entry was successsful.
//     0b1 ==> The previous stop mode entry was aborted.

#define  SMC_PMCTRL_RUNM_MASK        0x40u
#define  SMC_PMCTRL_RUNM_SHIFT       5
#define  SMC_PMCTRL_RUNM(x)          (((uint8_t)(((uint8_t)(x))<<SMC_PMCTRL_RUNM_SHIFT))&SMC_PMCTRL_RUNM_MASK)
//   Run Mode Control
//     0b00 ==> Normal Run mode (RUN)
//     0b01 ==> Reserved
//     0b10 ==> Very-Low-Power Run mode (VLPR)
//     0b11 ==> Reserved

#define  SMC_STOPCTRL_VLLSM_MASK     0x3u
#define  SMC_STOPCTRL_VLLSM_SHIFT    0
#define  SMC_STOPCTRL_VLLSM(x)       (((uint8_t)(((uint8_t)(x))<<SMC_STOPCTRL_VLLSM_SHIFT))&SMC_STOPCTRL_VLLSM_MASK)
//   VLLS Mode Control.
//     0b000 ==> VLLS0
//     0b001 ==> VLLS1
//     0b010 ==> VLLS2
//     0b011 ==> VLLS3
//     0b100 ==> Reserved
//     0b101 ==> Reserved
//     0b110 ==> Reserved
//     0b111 ==> Reserved

#define  SMC_STOPCTRL_PORPO_MASK     0x20u
#define  SMC_STOPCTRL_PORPO_SHIFT    5
//   POR Power Option
//     0b0 ==> POR detect circuit is enabled in VLLS0
//     0b1 ==> POR detect circuit is disabled in VLLS0

#define  SMC_STOPCTRL_PSTOPO_MASK    0x80u
#define  SMC_STOPCTRL_PSTOPO_SHIFT   6
#define  SMC_STOPCTRL_PSTOPO(x)      (((uint8_t)(((uint8_t)(x))<<SMC_STOPCTRL_PSTOPO_SHIFT))&SMC_STOPCTRL_PSTOPO_MASK)
//   Partial Stop Option
//     0b00 ==> STOP - Normal Stop mode
//     0b01 ==> PSTOP1 - Partial Stop with both system and bus clocks disabled
//     0b10 ==> PSTOP2 - Partial Stop with system clock disabled and bus clock enabled
//     0b11 ==> Reserved

#define  SMC_PMSTAT_PMSTAT_MASK      0x7u
#define  SMC_PMSTAT_PMSTAT_SHIFT     0
#define  SMC_PMSTAT_PMSTAT(x)        (((uint8_t)(((uint8_t)(x))<<SMC_PMSTAT_PMSTAT_SHIFT))&SMC_PMSTAT_PMSTAT_MASK)

#define  SPI0_C1_LSBFE_MASK          0x1u
#define  SPI0_C1_LSBFE_SHIFT         0
//   LSB first (shifter direction)
//     0b0 ==> SPI serial data transfers start with most significant bit
//     0b1 ==> SPI serial data transfers start with least significant bit

#define  SPI0_C1_SSOE_MASK           0x2u
#define  SPI0_C1_SSOE_SHIFT          1
//   Slave select output enable
//     0b0 ==> When MODFEN is 0: In master mode, SS pin function is general-purpose I/O (not SPI). In slave mode, SS pin function is slave select input. When MODFEN is 1: In master mode, SS pin function is SS input for mode fault. In slave mode, SS pin function is slave select input.
//     0b1 ==> When MODFEN is 0: In master mode, SS pin function is general-purpose I/O (not SPI). In slave mode, SS pin function is slave select input. When MODFEN is 1: In master mode, SS pin function is automatic SS output. In slave mode: SS pin function is slave select input.

#define  SPI0_C1_CPHA_MASK           0x4u
#define  SPI0_C1_CPHA_SHIFT          2
//   Clock phase
//     0b0 ==> First edge on SPSCK occurs at the middle of the first cycle of a data transfer
//     0b1 ==> First edge on SPSCK occurs at the start of the first cycle of a data transfer

#define  SPI0_C1_CPOL_MASK           0x8u
#define  SPI0_C1_CPOL_SHIFT          3
//   Clock polarity
//     0b0 ==> Active-high SPI clock (idles low)
//     0b1 ==> Active-low SPI clock (idles high)

#define  SPI0_C1_MSTR_MASK           0x10u
#define  SPI0_C1_MSTR_SHIFT          4
//   Master/slave mode select
//     0b0 ==> SPI module configured as a slave SPI device
//     0b1 ==> SPI module configured as a master SPI device

#define  SPI0_C1_SPTIE_MASK          0x20u
#define  SPI0_C1_SPTIE_SHIFT         5
//   SPI transmit interrupt enable
//     0b0 ==> Interrupts from SPTEF inhibited (use polling)
//     0b1 ==> When SPTEF is 1, hardware interrupt requested

#define  SPI0_C1_SPE_MASK            0x40u
#define  SPI0_C1_SPE_SHIFT           6
//   SPI system enable
//     0b0 ==> SPI system inactive
//     0b1 ==> SPI system enabled

#define  SPI0_C1_SPIE_MASK           0x80u
#define  SPI0_C1_SPIE_SHIFT          7
//   SPI interrupt enable: for SPRF and MODF (when FIFO is not supported or not enabled) or for read FIFO (when FIFO is supported and enabled)
//     0b0 ==> Interrupts from SPRF and MODF are inhibited-use polling (when FIFOMODE is not present or is 0) or Read FIFO Full Interrupts are disabled (when FIFOMODE is 1)
//     0b1 ==> Request a hardware interrupt when SPRF or MODF is 1 (when FIFOMODE is not present or is 0) or Read FIFO Full Interrupts are enabled (when FIFOMODE is 1)

#define  SPI0_C2_SPC0_MASK           0x1u
#define  SPI0_C2_SPC0_SHIFT          0
//   SPI pin control 0
//     0b0 ==> SPI uses separate pins for data input and data output (pin mode is normal). In master mode of operation: MISO is master in and MOSI is master out. In slave mode of operation: MISO is slave out and MOSI is slave in.
//     0b1 ==> SPI configured for single-wire bidirectional operation (pin mode is bidirectional). In master mode of operation: MISO is not used by SPI; MOSI is master in when BIDIROE is 0 or master I/O when BIDIROE is 1. In slave mode of operation: MISO is slave in when BIDIROE is 0 or slave I/O when BIDIROE is 1; MOSI is not used by SPI.

#define  SPI0_C2_SPISWAI_MASK        0x2u
#define  SPI0_C2_SPISWAI_SHIFT       1
//   SPI stop in wait mode
//     0b0 ==> SPI clocks continue to operate in wait mode
//     0b1 ==> SPI clocks stop when the MCU enters wait mode

#define  SPI0_C2_RXDMAE_MASK         0x4u
#define  SPI0_C2_RXDMAE_SHIFT        2
//   Receive DMA enable
//     0b0 ==> DMA request for receive is disabled and interrupt from SPRF is allowed
//     0b1 ==> DMA request for receive is enabled and interrupt from SPRF is disabled

#define  SPI0_C2_BIDIROE_MASK        0x8u
#define  SPI0_C2_BIDIROE_SHIFT       3
//   Bidirectional mode output enable
//     0b0 ==> Output driver disabled so SPI data I/O pin acts as an input
//     0b1 ==> SPI I/O pin enabled as an output

#define  SPI0_C2_MODFEN_MASK         0x10u
#define  SPI0_C2_MODFEN_SHIFT        4
//   Master mode-fault function enable
//     0b0 ==> Mode fault function disabled, master SS pin reverts to general-purpose I/O not controlled by SPI
//     0b1 ==> Mode fault function enabled, master SS pin acts as the mode fault input or the slave select output

#define  SPI0_C2_TXDMAE_MASK         0x20u
#define  SPI0_C2_TXDMAE_SHIFT        5
//   Transmit DMA enable
//     0b0 ==> DMA request for transmit is disabled and interrupt from SPTEF is allowed
//     0b1 ==> DMA request for transmit is enabled and interrupt from SPTEF is disabled

#define  SPI0_C2_SPIMODE_MASK        0x40u
#define  SPI0_C2_SPIMODE_SHIFT       6
//   SPI 8-bit or 16-bit mode
//     0b0 ==> 8-bit SPI shift register, match register, and buffers
//     0b1 ==> 16-bit SPI shift register, match register, and buffers

#define  SPI0_C2_SPMIE_MASK          0x80u
#define  SPI0_C2_SPMIE_SHIFT         7
//   SPI match interrupt enable
//     0b0 ==> Interrupts from SPMF inhibited (use polling)
//     0b1 ==> When SPMF is 1, requests a hardware interrupt

#define  SPI0_BR_SPR_MASK            0x4u
#define  SPI0_BR_SPR_SHIFT           0
#define  SPI0_BR_SPR(x)              (((uint8_t)(((uint8_t)(x))<<SPI0_BR_SPR_SHIFT))&SPI0_BR_SPR_MASK)
//   SPI baud rate divisor
//     0b0000 ==> Baud rate divisor is 2
//     0b0001 ==> Baud rate divisor is 4
//     0b0010 ==> Baud rate divisor is 8
//     0b0011 ==> Baud rate divisor is 16
//     0b0100 ==> Baud rate divisor is 32
//     0b0101 ==> Baud rate divisor is 64
//     0b0110 ==> Baud rate divisor is 128
//     0b0111 ==> Baud rate divisor is 256
//     0b1000 ==> Baud rate divisor is 512

#define  SPI0_BR_SPPR_MASK           0x30u
#define  SPI0_BR_SPPR_SHIFT          4
#define  SPI0_BR_SPPR(x)             (((uint8_t)(((uint8_t)(x))<<SPI0_BR_SPPR_SHIFT))&SPI0_BR_SPPR_MASK)
//   SPI baud rate prescale divisor
//     0b000 ==> Baud rate prescaler divisor is 1
//     0b001 ==> Baud rate prescaler divisor is 2
//     0b010 ==> Baud rate prescaler divisor is 3
//     0b011 ==> Baud rate prescaler divisor is 4
//     0b100 ==> Baud rate prescaler divisor is 5
//     0b101 ==> Baud rate prescaler divisor is 6
//     0b110 ==> Baud rate prescaler divisor is 7
//     0b111 ==> Baud rate prescaler divisor is 8

#define  SPI0_S_RFIFOEF_MASK         0x1u
#define  SPI0_S_RFIFOEF_SHIFT        0
//   SPI read FIFO empty flag
//     0b0 ==> Read FIFO has data. Reads of the DH:DL registers in 16-bit mode or the DL register in 8-bit mode will empty the read FIFO.
//     0b1 ==> Read FIFO is empty.

#define  SPI0_S_TXFULLF_MASK         0x2u
#define  SPI0_S_TXFULLF_SHIFT        1
//   Transmit FIFO full flag
//     0b0 ==> Transmit FIFO has less than 8 bytes
//     0b1 ==> Transmit FIFO has 8 bytes of data

#define  SPI0_S_TNEAREF_MASK         0x4u
#define  SPI0_S_TNEAREF_SHIFT        2
//   Transmit FIFO nearly empty flag
//     0b0 ==> Transmit FIFO has more than 16 bits (when C3[5] is 0) or more than 32 bits (when C3[5] is 1) remaining to transmit
//     0b1 ==> Transmit FIFO has an amount of data equal to or less than 16 bits (when C3[5] is 0) or 32 bits (when C3[5] is 1) remaining to transmit

#define  SPI0_S_RNFULLF_MASK         0x8u
#define  SPI0_S_RNFULLF_SHIFT        3
//   Receive FIFO nearly full flag
//     0b0 ==> Receive FIFO has received less than 48 bits (when C3[4] is 0) or less than 32 bits (when C3[4] is 1)
//     0b1 ==> Receive FIFO has received data of an amount equal to or greater than 48 bits (when C3[4] is 0) or 32 bits (when C3[4] is 1)

#define  SPI0_S_MODF_MASK            0x10u
#define  SPI0_S_MODF_SHIFT           4
//   Master mode fault flag
//     0b0 ==> No mode fault error
//     0b1 ==> Mode fault error detected

#define  SPI0_S_SPTEF_MASK           0x20u
#define  SPI0_S_SPTEF_SHIFT          5
//   SPI transmit buffer empty flag (when FIFO is not supported or not enabled) or SPI transmit FIFO empty flag (when FIFO is supported and enabled)
//     0b0 ==> SPI transmit buffer not empty (when FIFOMODE is not present or is 0) or SPI FIFO not empty (when FIFOMODE is 1)
//     0b1 ==> SPI transmit buffer empty (when FIFOMODE is not present or is 0) or SPI FIFO empty (when FIFOMODE is 1)

#define  SPI0_S_SPMF_MASK            0x40u
#define  SPI0_S_SPMF_SHIFT           6
//   SPI match flag
//     0b0 ==> Value in the receive data buffer does not match the value in the MH:ML registers
//     0b1 ==> Value in the receive data buffer matches the value in the MH:ML registers

#define  SPI0_S_SPRF_MASK            0x80u
#define  SPI0_S_SPRF_SHIFT           7
//   SPI read buffer full flag (when FIFO is not supported or not enabled) or SPI read FIFO FULL flag (when FIFO is supported and enabled)
//     0b0 ==> No data available in the receive data buffer (when FIFOMODE is not present or is 0) or Read FIFO is not full (when FIFOMODE is 1)
//     0b1 ==> Data available in the receive data buffer (when FIFOMODE is not present or is 0) or Read FIFO is full (when FIFOMODE is 1)

#define  SPI0_DH_Bits_MASK           0x8u
#define  SPI0_DH_Bits_SHIFT          0
#define  SPI0_DH_Bits(x)             (((uint8_t)(((uint8_t)(x))<<SPI0_DH_Bits_SHIFT))&SPI0_DH_Bits_MASK)
//   Data (high byte)

#define  SPI0_DL_Bits_MASK           0x8u
#define  SPI0_DL_Bits_SHIFT          0
#define  SPI0_DL_Bits(x)             (((uint8_t)(((uint8_t)(x))<<SPI0_DL_Bits_SHIFT))&SPI0_DL_Bits_MASK)
//   Data (low byte)

#define  SPI0_MH_Bits_MASK           0x8u
#define  SPI0_MH_Bits_SHIFT          0
#define  SPI0_MH_Bits(x)             (((uint8_t)(((uint8_t)(x))<<SPI0_MH_Bits_SHIFT))&SPI0_MH_Bits_MASK)
//   Hardware compare value (high byte)

#define  SPI0_ML_Bits_MASK           0x8u
#define  SPI0_ML_Bits_SHIFT          0
#define  SPI0_ML_Bits(x)             (((uint8_t)(((uint8_t)(x))<<SPI0_ML_Bits_SHIFT))&SPI0_ML_Bits_MASK)
//   Hardware compare value (low byte)

#define  SPI0_C3_FIFOMODE_MASK       0x1u
#define  SPI0_C3_FIFOMODE_SHIFT      0
//   FIFO mode enable
//     0b0 ==> Buffer mode disabled
//     0b1 ==> Data available in the receive data buffer

#define  SPI0_C3_RNFULLIEN_MASK      0x2u
#define  SPI0_C3_RNFULLIEN_SHIFT     1
//   Receive FIFO nearly full interrupt enable
//     0b0 ==> No interrupt upon RNFULLF being set
//     0b1 ==> Enable interrupts upon RNFULLF being set

#define  SPI0_C3_TNEARIEN_MASK       0x4u
#define  SPI0_C3_TNEARIEN_SHIFT      2
//   Transmit FIFO nearly empty interrupt enable
//     0b0 ==> No interrupt upon TNEAREF being set
//     0b1 ==> Enable interrupts upon TNEAREF being set

#define  SPI0_C3_INTCLR_MASK         0x8u
#define  SPI0_C3_INTCLR_SHIFT        3
//   Interrupt clearing mechanism select
//     0b0 ==> These interrupts are cleared when the corresponding flags are cleared depending on the state of the FIFOs
//     0b1 ==> These interrupts are cleared by writing the corresponding bits in the CI register

#define  SPI0_C3_RNFULLF_MARK_MASK   0x10u
#define  SPI0_C3_RNFULLF_MARK_SHIFT  4
//   Receive FIFO nearly full watermark
//     0b0 ==> RNFULLF is set when the receive FIFO has 48 bits or more
//     0b1 ==> RNFULLF is set when the receive FIFO has 32 bits or more

#define  SPI0_C3_TNEAREF_MARK_MASK   0x20u
#define  SPI0_C3_TNEAREF_MARK_SHIFT  5
//   Transmit FIFO nearly empty watermark
//     0b0 ==> TNEAREF is set when the transmit FIFO has 16 bits or less
//     0b1 ==> TNEAREF is set when the transmit FIFO has 32 bits or less

#define  SPI0_CI_SPRFCI_MASK         0x1u
#define  SPI0_CI_SPRFCI_SHIFT        0
//   Receive FIFO full flag clear interrupt

#define  SPI0_CI_SPTEFCI_MASK        0x2u
#define  SPI0_CI_SPTEFCI_SHIFT       1
//   Transmit FIFO empty flag clear interrupt

#define  SPI0_CI_RNFULLFCI_MASK      0x4u
#define  SPI0_CI_RNFULLFCI_SHIFT     2
//   Receive FIFO nearly full flag clear interrupt

#define  SPI0_CI_TNEAREFCI_MASK      0x8u
#define  SPI0_CI_TNEAREFCI_SHIFT     3
//   Transmit FIFO nearly empty flag clear interrupt

#define  SPI0_CI_RXFOF_MASK          0x10u
#define  SPI0_CI_RXFOF_SHIFT         4
//   Receive FIFO overflow flag
//     0b0 ==> Receive FIFO overflow condition has not occurred
//     0b1 ==> Receive FIFO overflow condition occurred

#define  SPI0_CI_TXFOF_MASK          0x20u
#define  SPI0_CI_TXFOF_SHIFT         5
//   Transmit FIFO overflow flag
//     0b0 ==> Transmit FIFO overflow condition has not occurred
//     0b1 ==> Transmit FIFO overflow condition occurred

#define  SPI0_CI_RXFERR_MASK         0x40u
#define  SPI0_CI_RXFERR_SHIFT        6
//   Receive FIFO error flag
//     0b0 ==> No receive FIFO error occurred
//     0b1 ==> A receive FIFO error occurred

#define  SPI0_CI_TXFERR_MASK         0x80u
#define  SPI0_CI_TXFERR_SHIFT        7
//   Transmit FIFO error flag
//     0b0 ==> No transmit FIFO error occurred
//     0b1 ==> A transmit FIFO error occurred

#define  SPI1_C1_LSBFE_MASK          0x1u
#define  SPI1_C1_LSBFE_SHIFT         0
//   LSB first (shifter direction)
//     0b0 ==> SPI serial data transfers start with most significant bit
//     0b1 ==> SPI serial data transfers start with least significant bit

#define  SPI1_C1_SSOE_MASK           0x2u
#define  SPI1_C1_SSOE_SHIFT          1
//   Slave select output enable
//     0b0 ==> When MODFEN is 0: In master mode, SS pin function is general-purpose I/O (not SPI). In slave mode, SS pin function is slave select input. When MODFEN is 1: In master mode, SS pin function is SS input for mode fault. In slave mode, SS pin function is slave select input.
//     0b1 ==> When MODFEN is 0: In master mode, SS pin function is general-purpose I/O (not SPI). In slave mode, SS pin function is slave select input. When MODFEN is 1: In master mode, SS pin function is automatic SS output. In slave mode: SS pin function is slave select input.

#define  SPI1_C1_CPHA_MASK           0x4u
#define  SPI1_C1_CPHA_SHIFT          2
//   Clock phase
//     0b0 ==> First edge on SPSCK occurs at the middle of the first cycle of a data transfer
//     0b1 ==> First edge on SPSCK occurs at the start of the first cycle of a data transfer

#define  SPI1_C1_CPOL_MASK           0x8u
#define  SPI1_C1_CPOL_SHIFT          3
//   Clock polarity
//     0b0 ==> Active-high SPI clock (idles low)
//     0b1 ==> Active-low SPI clock (idles high)

#define  SPI1_C1_MSTR_MASK           0x10u
#define  SPI1_C1_MSTR_SHIFT          4
//   Master/slave mode select
//     0b0 ==> SPI module configured as a slave SPI device
//     0b1 ==> SPI module configured as a master SPI device

#define  SPI1_C1_SPTIE_MASK          0x20u
#define  SPI1_C1_SPTIE_SHIFT         5
//   SPI transmit interrupt enable
//     0b0 ==> Interrupts from SPTEF inhibited (use polling)
//     0b1 ==> When SPTEF is 1, hardware interrupt requested

#define  SPI1_C1_SPE_MASK            0x40u
#define  SPI1_C1_SPE_SHIFT           6
//   SPI system enable
//     0b0 ==> SPI system inactive
//     0b1 ==> SPI system enabled

#define  SPI1_C1_SPIE_MASK           0x80u
#define  SPI1_C1_SPIE_SHIFT          7
//   SPI interrupt enable: for SPRF and MODF (when FIFO is not supported or not enabled) or for read FIFO (when FIFO is supported and enabled)
//     0b0 ==> Interrupts from SPRF and MODF are inhibited-use polling (when FIFOMODE is not present or is 0) or Read FIFO Full Interrupts are disabled (when FIFOMODE is 1)
//     0b1 ==> Request a hardware interrupt when SPRF or MODF is 1 (when FIFOMODE is not present or is 0) or Read FIFO Full Interrupts are enabled (when FIFOMODE is 1)

#define  SPI1_C2_SPC0_MASK           0x1u
#define  SPI1_C2_SPC0_SHIFT          0
//   SPI pin control 0
//     0b0 ==> SPI uses separate pins for data input and data output (pin mode is normal). In master mode of operation: MISO is master in and MOSI is master out. In slave mode of operation: MISO is slave out and MOSI is slave in.
//     0b1 ==> SPI configured for single-wire bidirectional operation (pin mode is bidirectional). In master mode of operation: MISO is not used by SPI; MOSI is master in when BIDIROE is 0 or master I/O when BIDIROE is 1. In slave mode of operation: MISO is slave in when BIDIROE is 0 or slave I/O when BIDIROE is 1; MOSI is not used by SPI.

#define  SPI1_C2_SPISWAI_MASK        0x2u
#define  SPI1_C2_SPISWAI_SHIFT       1
//   SPI stop in wait mode
//     0b0 ==> SPI clocks continue to operate in wait mode
//     0b1 ==> SPI clocks stop when the MCU enters wait mode

#define  SPI1_C2_RXDMAE_MASK         0x4u
#define  SPI1_C2_RXDMAE_SHIFT        2
//   Receive DMA enable
//     0b0 ==> DMA request for receive is disabled and interrupt from SPRF is allowed
//     0b1 ==> DMA request for receive is enabled and interrupt from SPRF is disabled

#define  SPI1_C2_BIDIROE_MASK        0x8u
#define  SPI1_C2_BIDIROE_SHIFT       3
//   Bidirectional mode output enable
//     0b0 ==> Output driver disabled so SPI data I/O pin acts as an input
//     0b1 ==> SPI I/O pin enabled as an output

#define  SPI1_C2_MODFEN_MASK         0x10u
#define  SPI1_C2_MODFEN_SHIFT        4
//   Master mode-fault function enable
//     0b0 ==> Mode fault function disabled, master SS pin reverts to general-purpose I/O not controlled by SPI
//     0b1 ==> Mode fault function enabled, master SS pin acts as the mode fault input or the slave select output

#define  SPI1_C2_TXDMAE_MASK         0x20u
#define  SPI1_C2_TXDMAE_SHIFT        5
//   Transmit DMA enable
//     0b0 ==> DMA request for transmit is disabled and interrupt from SPTEF is allowed
//     0b1 ==> DMA request for transmit is enabled and interrupt from SPTEF is disabled

#define  SPI1_C2_SPIMODE_MASK        0x40u
#define  SPI1_C2_SPIMODE_SHIFT       6
//   SPI 8-bit or 16-bit mode
//     0b0 ==> 8-bit SPI shift register, match register, and buffers
//     0b1 ==> 16-bit SPI shift register, match register, and buffers

#define  SPI1_C2_SPMIE_MASK          0x80u
#define  SPI1_C2_SPMIE_SHIFT         7
//   SPI match interrupt enable
//     0b0 ==> Interrupts from SPMF inhibited (use polling)
//     0b1 ==> When SPMF is 1, requests a hardware interrupt

#define  SPI1_BR_SPR_MASK            0x4u
#define  SPI1_BR_SPR_SHIFT           0
#define  SPI1_BR_SPR(x)              (((uint8_t)(((uint8_t)(x))<<SPI1_BR_SPR_SHIFT))&SPI1_BR_SPR_MASK)
//   SPI baud rate divisor
//     0b0000 ==> Baud rate divisor is 2
//     0b0001 ==> Baud rate divisor is 4
//     0b0010 ==> Baud rate divisor is 8
//     0b0011 ==> Baud rate divisor is 16
//     0b0100 ==> Baud rate divisor is 32
//     0b0101 ==> Baud rate divisor is 64
//     0b0110 ==> Baud rate divisor is 128
//     0b0111 ==> Baud rate divisor is 256
//     0b1000 ==> Baud rate divisor is 512

#define  SPI1_BR_SPPR_MASK           0x30u
#define  SPI1_BR_SPPR_SHIFT          4
#define  SPI1_BR_SPPR(x)             (((uint8_t)(((uint8_t)(x))<<SPI1_BR_SPPR_SHIFT))&SPI1_BR_SPPR_MASK)
//   SPI baud rate prescale divisor
//     0b000 ==> Baud rate prescaler divisor is 1
//     0b001 ==> Baud rate prescaler divisor is 2
//     0b010 ==> Baud rate prescaler divisor is 3
//     0b011 ==> Baud rate prescaler divisor is 4
//     0b100 ==> Baud rate prescaler divisor is 5
//     0b101 ==> Baud rate prescaler divisor is 6
//     0b110 ==> Baud rate prescaler divisor is 7
//     0b111 ==> Baud rate prescaler divisor is 8

#define  SPI1_S_RFIFOEF_MASK         0x1u
#define  SPI1_S_RFIFOEF_SHIFT        0
//   SPI read FIFO empty flag
//     0b0 ==> Read FIFO has data. Reads of the DH:DL registers in 16-bit mode or the DL register in 8-bit mode will empty the read FIFO.
//     0b1 ==> Read FIFO is empty.

#define  SPI1_S_TXFULLF_MASK         0x2u
#define  SPI1_S_TXFULLF_SHIFT        1
//   Transmit FIFO full flag
//     0b0 ==> Transmit FIFO has less than 8 bytes
//     0b1 ==> Transmit FIFO has 8 bytes of data

#define  SPI1_S_TNEAREF_MASK         0x4u
#define  SPI1_S_TNEAREF_SHIFT        2
//   Transmit FIFO nearly empty flag
//     0b0 ==> Transmit FIFO has more than 16 bits (when C3[5] is 0) or more than 32 bits (when C3[5] is 1) remaining to transmit
//     0b1 ==> Transmit FIFO has an amount of data equal to or less than 16 bits (when C3[5] is 0) or 32 bits (when C3[5] is 1) remaining to transmit

#define  SPI1_S_RNFULLF_MASK         0x8u
#define  SPI1_S_RNFULLF_SHIFT        3
//   Receive FIFO nearly full flag
//     0b0 ==> Receive FIFO has received less than 48 bits (when C3[4] is 0) or less than 32 bits (when C3[4] is 1)
//     0b1 ==> Receive FIFO has received data of an amount equal to or greater than 48 bits (when C3[4] is 0) or 32 bits (when C3[4] is 1)

#define  SPI1_S_MODF_MASK            0x10u
#define  SPI1_S_MODF_SHIFT           4
//   Master mode fault flag
//     0b0 ==> No mode fault error
//     0b1 ==> Mode fault error detected

#define  SPI1_S_SPTEF_MASK           0x20u
#define  SPI1_S_SPTEF_SHIFT          5
//   SPI transmit buffer empty flag (when FIFO is not supported or not enabled) or SPI transmit FIFO empty flag (when FIFO is supported and enabled)
//     0b0 ==> SPI transmit buffer not empty (when FIFOMODE is not present or is 0) or SPI FIFO not empty (when FIFOMODE is 1)
//     0b1 ==> SPI transmit buffer empty (when FIFOMODE is not present or is 0) or SPI FIFO empty (when FIFOMODE is 1)

#define  SPI1_S_SPMF_MASK            0x40u
#define  SPI1_S_SPMF_SHIFT           6
//   SPI match flag
//     0b0 ==> Value in the receive data buffer does not match the value in the MH:ML registers
//     0b1 ==> Value in the receive data buffer matches the value in the MH:ML registers

#define  SPI1_S_SPRF_MASK            0x80u
#define  SPI1_S_SPRF_SHIFT           7
//   SPI read buffer full flag (when FIFO is not supported or not enabled) or SPI read FIFO FULL flag (when FIFO is supported and enabled)
//     0b0 ==> No data available in the receive data buffer (when FIFOMODE is not present or is 0) or Read FIFO is not full (when FIFOMODE is 1)
//     0b1 ==> Data available in the receive data buffer (when FIFOMODE is not present or is 0) or Read FIFO is full (when FIFOMODE is 1)

#define  SPI1_DH_Bits_MASK           0x8u
#define  SPI1_DH_Bits_SHIFT          0
#define  SPI1_DH_Bits(x)             (((uint8_t)(((uint8_t)(x))<<SPI1_DH_Bits_SHIFT))&SPI1_DH_Bits_MASK)
//   Data (high byte)

#define  SPI1_DL_Bits_MASK           0x8u
#define  SPI1_DL_Bits_SHIFT          0
#define  SPI1_DL_Bits(x)             (((uint8_t)(((uint8_t)(x))<<SPI1_DL_Bits_SHIFT))&SPI1_DL_Bits_MASK)
//   Data (low byte)

#define  SPI1_MH_Bits_MASK           0x8u
#define  SPI1_MH_Bits_SHIFT          0
#define  SPI1_MH_Bits(x)             (((uint8_t)(((uint8_t)(x))<<SPI1_MH_Bits_SHIFT))&SPI1_MH_Bits_MASK)
//   Hardware compare value (high byte)

#define  SPI1_ML_Bits_MASK           0x8u
#define  SPI1_ML_Bits_SHIFT          0
#define  SPI1_ML_Bits(x)             (((uint8_t)(((uint8_t)(x))<<SPI1_ML_Bits_SHIFT))&SPI1_ML_Bits_MASK)
//   Hardware compare value (low byte)

#define  SPI1_C3_FIFOMODE_MASK       0x1u
#define  SPI1_C3_FIFOMODE_SHIFT      0
//   FIFO mode enable
//     0b0 ==> Buffer mode disabled
//     0b1 ==> Data available in the receive data buffer

#define  SPI1_C3_RNFULLIEN_MASK      0x2u
#define  SPI1_C3_RNFULLIEN_SHIFT     1
//   Receive FIFO nearly full interrupt enable
//     0b0 ==> No interrupt upon RNFULLF being set
//     0b1 ==> Enable interrupts upon RNFULLF being set

#define  SPI1_C3_TNEARIEN_MASK       0x4u
#define  SPI1_C3_TNEARIEN_SHIFT      2
//   Transmit FIFO nearly empty interrupt enable
//     0b0 ==> No interrupt upon TNEAREF being set
//     0b1 ==> Enable interrupts upon TNEAREF being set

#define  SPI1_C3_INTCLR_MASK         0x8u
#define  SPI1_C3_INTCLR_SHIFT        3
//   Interrupt clearing mechanism select
//     0b0 ==> These interrupts are cleared when the corresponding flags are cleared depending on the state of the FIFOs
//     0b1 ==> These interrupts are cleared by writing the corresponding bits in the CI register

#define  SPI1_C3_RNFULLF_MARK_MASK   0x10u
#define  SPI1_C3_RNFULLF_MARK_SHIFT  4
//   Receive FIFO nearly full watermark
//     0b0 ==> RNFULLF is set when the receive FIFO has 48 bits or more
//     0b1 ==> RNFULLF is set when the receive FIFO has 32 bits or more

#define  SPI1_C3_TNEAREF_MARK_MASK   0x20u
#define  SPI1_C3_TNEAREF_MARK_SHIFT  5
//   Transmit FIFO nearly empty watermark
//     0b0 ==> TNEAREF is set when the transmit FIFO has 16 bits or less
//     0b1 ==> TNEAREF is set when the transmit FIFO has 32 bits or less

#define  SPI1_CI_SPRFCI_MASK         0x1u
#define  SPI1_CI_SPRFCI_SHIFT        0
//   Receive FIFO full flag clear interrupt

#define  SPI1_CI_SPTEFCI_MASK        0x2u
#define  SPI1_CI_SPTEFCI_SHIFT       1
//   Transmit FIFO empty flag clear interrupt

#define  SPI1_CI_RNFULLFCI_MASK      0x4u
#define  SPI1_CI_RNFULLFCI_SHIFT     2
//   Receive FIFO nearly full flag clear interrupt

#define  SPI1_CI_TNEAREFCI_MASK      0x8u
#define  SPI1_CI_TNEAREFCI_SHIFT     3
//   Transmit FIFO nearly empty flag clear interrupt

#define  SPI1_CI_RXFOF_MASK          0x10u
#define  SPI1_CI_RXFOF_SHIFT         4
//   Receive FIFO overflow flag
//     0b0 ==> Receive FIFO overflow condition has not occurred
//     0b1 ==> Receive FIFO overflow condition occurred

#define  SPI1_CI_TXFOF_MASK          0x20u
#define  SPI1_CI_TXFOF_SHIFT         5
//   Transmit FIFO overflow flag
//     0b0 ==> Transmit FIFO overflow condition has not occurred
//     0b1 ==> Transmit FIFO overflow condition occurred

#define  SPI1_CI_RXFERR_MASK         0x40u
#define  SPI1_CI_RXFERR_SHIFT        6
//   Receive FIFO error flag
//     0b0 ==> No receive FIFO error occurred
//     0b1 ==> A receive FIFO error occurred

#define  SPI1_CI_TXFERR_MASK         0x80u
#define  SPI1_CI_TXFERR_SHIFT        7
//   Transmit FIFO error flag
//     0b0 ==> No transmit FIFO error occurred
//     0b1 ==> A transmit FIFO error occurred

#define  TMR0_0_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR0_0_COMP1_COMPARISON_1_SHIFT 0
#define  TMR0_0_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR0_0_COMP1_COMPARISON_1_SHIFT))&TMR0_0_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR0_0_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR0_0_COMP2_COMPARISON_2_SHIFT 0
#define  TMR0_0_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR0_0_COMP2_COMPARISON_2_SHIFT))&TMR0_0_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR0_0_CAPT_CAPTURE_MASK    0x10u
#define  TMR0_0_CAPT_CAPTURE_SHIFT   0
#define  TMR0_0_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR0_0_CAPT_CAPTURE_SHIFT))&TMR0_0_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR0_0_LOAD_LOAD_MASK       0x10u
#define  TMR0_0_LOAD_LOAD_SHIFT      0
#define  TMR0_0_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR0_0_LOAD_LOAD_SHIFT))&TMR0_0_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR0_0_HOLD_HOLD_MASK       0x10u
#define  TMR0_0_HOLD_HOLD_SHIFT      0
#define  TMR0_0_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR0_0_HOLD_HOLD_SHIFT))&TMR0_0_HOLD_HOLD_MASK)

#define  TMR0_0_CNTR_COUNTER_MASK    0x10u
#define  TMR0_0_CNTR_COUNTER_SHIFT   0
#define  TMR0_0_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR0_0_CNTR_COUNTER_SHIFT))&TMR0_0_CNTR_COUNTER_MASK)

#define  TMR0_0_CTRL_OUTMODE_MASK    0x3u
#define  TMR0_0_CTRL_OUTMODE_SHIFT   0
#define  TMR0_0_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR0_0_CTRL_OUTMODE_SHIFT))&TMR0_0_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR0_0_CTRL_COINIT_MASK     0x8u
#define  TMR0_0_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR0_0_CTRL_DIR_MASK        0x10u
#define  TMR0_0_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR0_0_CTRL_LENGTH_MASK     0x20u
#define  TMR0_0_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR0_0_CTRL_ONCE_MASK       0x40u
#define  TMR0_0_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR0_0_CTRL_SCS_MASK        0x100u
#define  TMR0_0_CTRL_SCS_SHIFT       7
#define  TMR0_0_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR0_0_CTRL_SCS_SHIFT))&TMR0_0_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR0_0_CTRL_PCS_MASK        0x800u
#define  TMR0_0_CTRL_PCS_SHIFT       9
#define  TMR0_0_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR0_0_CTRL_PCS_SHIFT))&TMR0_0_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR0_0_CTRL_CM_MASK         0x6000u
#define  TMR0_0_CTRL_CM_SHIFT        13
#define  TMR0_0_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR0_0_CTRL_CM_SHIFT))&TMR0_0_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR0_0_SCTRL_OEN_MASK       0x1u
#define  TMR0_0_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR0_0_SCTRL_OPS_MASK       0x2u
#define  TMR0_0_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR0_0_SCTRL_FORCE_MASK     0x4u
#define  TMR0_0_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR0_0_SCTRL_VAL_MASK       0x8u
#define  TMR0_0_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR0_0_SCTRL_EEOF_MASK      0x10u
#define  TMR0_0_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR0_0_SCTRL_MSTR_MASK      0x20u
#define  TMR0_0_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR0_0_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR0_0_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR0_0_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR0_0_SCTRL_CAPTURE_MODE_SHIFT))&TMR0_0_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR0_0_SCTRL_INPUT_MASK     0x100u
#define  TMR0_0_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR0_0_SCTRL_IPS_MASK       0x200u
#define  TMR0_0_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR0_0_SCTRL_IEFIE_MASK     0x400u
#define  TMR0_0_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR0_0_SCTRL_IEF_MASK       0x800u
#define  TMR0_0_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR0_0_SCTRL_TOFIE_MASK     0x1000u
#define  TMR0_0_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR0_0_SCTRL_TOF_MASK       0x2000u
#define  TMR0_0_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR0_0_SCTRL_TCFIE_MASK     0x4000u
#define  TMR0_0_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR0_0_SCTRL_TCF_MASK       0x8000u
#define  TMR0_0_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR0_0_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR0_0_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR0_0_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR0_0_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR0_0_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR0_0_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR0_0_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR0_0_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR0_0_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR0_0_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR0_0_CSCTRL_CL1_MASK      0x2u
#define  TMR0_0_CSCTRL_CL1_SHIFT     0
#define  TMR0_0_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR0_0_CSCTRL_CL1_SHIFT))&TMR0_0_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR0_0_CSCTRL_CL2_MASK      0x8u
#define  TMR0_0_CSCTRL_CL2_SHIFT     2
#define  TMR0_0_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR0_0_CSCTRL_CL2_SHIFT))&TMR0_0_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR0_0_CSCTRL_TCF1_MASK     0x10u
#define  TMR0_0_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR0_0_CSCTRL_TCF2_MASK     0x20u
#define  TMR0_0_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR0_0_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR0_0_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR0_0_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR0_0_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR0_0_CSCTRL_UP_MASK       0x200u
#define  TMR0_0_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR0_0_CSCTRL_TCI_MASK      0x400u
#define  TMR0_0_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR0_0_CSCTRL_ROC_MASK      0x800u
#define  TMR0_0_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR0_0_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR0_0_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR0_0_CSCTRL_FAULT_MASK    0x2000u
#define  TMR0_0_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR0_0_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR0_0_CSCTRL_DBG_EN_SHIFT  14
#define  TMR0_0_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR0_0_CSCTRL_DBG_EN_SHIFT))&TMR0_0_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR0_0_FILT_FILT_PER_MASK   0x8u
#define  TMR0_0_FILT_FILT_PER_SHIFT  0
#define  TMR0_0_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR0_0_FILT_FILT_PER_SHIFT))&TMR0_0_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR0_0_FILT_FILT_CNT_MASK   0x300u
#define  TMR0_0_FILT_FILT_CNT_SHIFT  8
#define  TMR0_0_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR0_0_FILT_FILT_CNT_SHIFT))&TMR0_0_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR0_0_DMA_IEFDE_MASK       0x1u
#define  TMR0_0_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR0_0_DMA_CMPLD1DE_MASK    0x2u
#define  TMR0_0_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR0_0_DMA_CMPLD2DE_MASK    0x4u
#define  TMR0_0_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  TMR0_0_ENBL_ENBL_MASK       0x4u
#define  TMR0_0_ENBL_ENBL_SHIFT      0
#define  TMR0_0_ENBL_ENBL(x)         (((uint16_t)(((uint16_t)(x))<<TMR0_0_ENBL_ENBL_SHIFT))&TMR0_0_ENBL_ENBL_MASK)
//   Timer Channel Enable
//     0b0 ==> Timer channel is disabled.
//     0b1 ==> Timer channel is enabled. (default)

#define  TMR0_1_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR0_1_COMP1_COMPARISON_1_SHIFT 0
#define  TMR0_1_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR0_1_COMP1_COMPARISON_1_SHIFT))&TMR0_1_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR0_1_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR0_1_COMP2_COMPARISON_2_SHIFT 0
#define  TMR0_1_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR0_1_COMP2_COMPARISON_2_SHIFT))&TMR0_1_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR0_1_CAPT_CAPTURE_MASK    0x10u
#define  TMR0_1_CAPT_CAPTURE_SHIFT   0
#define  TMR0_1_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR0_1_CAPT_CAPTURE_SHIFT))&TMR0_1_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR0_1_LOAD_LOAD_MASK       0x10u
#define  TMR0_1_LOAD_LOAD_SHIFT      0
#define  TMR0_1_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR0_1_LOAD_LOAD_SHIFT))&TMR0_1_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR0_1_HOLD_HOLD_MASK       0x10u
#define  TMR0_1_HOLD_HOLD_SHIFT      0
#define  TMR0_1_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR0_1_HOLD_HOLD_SHIFT))&TMR0_1_HOLD_HOLD_MASK)

#define  TMR0_1_CNTR_COUNTER_MASK    0x10u
#define  TMR0_1_CNTR_COUNTER_SHIFT   0
#define  TMR0_1_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR0_1_CNTR_COUNTER_SHIFT))&TMR0_1_CNTR_COUNTER_MASK)

#define  TMR0_1_CTRL_OUTMODE_MASK    0x3u
#define  TMR0_1_CTRL_OUTMODE_SHIFT   0
#define  TMR0_1_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR0_1_CTRL_OUTMODE_SHIFT))&TMR0_1_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR0_1_CTRL_COINIT_MASK     0x8u
#define  TMR0_1_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR0_1_CTRL_DIR_MASK        0x10u
#define  TMR0_1_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR0_1_CTRL_LENGTH_MASK     0x20u
#define  TMR0_1_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR0_1_CTRL_ONCE_MASK       0x40u
#define  TMR0_1_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR0_1_CTRL_SCS_MASK        0x100u
#define  TMR0_1_CTRL_SCS_SHIFT       7
#define  TMR0_1_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR0_1_CTRL_SCS_SHIFT))&TMR0_1_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR0_1_CTRL_PCS_MASK        0x800u
#define  TMR0_1_CTRL_PCS_SHIFT       9
#define  TMR0_1_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR0_1_CTRL_PCS_SHIFT))&TMR0_1_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR0_1_CTRL_CM_MASK         0x6000u
#define  TMR0_1_CTRL_CM_SHIFT        13
#define  TMR0_1_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR0_1_CTRL_CM_SHIFT))&TMR0_1_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR0_1_SCTRL_OEN_MASK       0x1u
#define  TMR0_1_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR0_1_SCTRL_OPS_MASK       0x2u
#define  TMR0_1_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR0_1_SCTRL_FORCE_MASK     0x4u
#define  TMR0_1_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR0_1_SCTRL_VAL_MASK       0x8u
#define  TMR0_1_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR0_1_SCTRL_EEOF_MASK      0x10u
#define  TMR0_1_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR0_1_SCTRL_MSTR_MASK      0x20u
#define  TMR0_1_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR0_1_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR0_1_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR0_1_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR0_1_SCTRL_CAPTURE_MODE_SHIFT))&TMR0_1_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR0_1_SCTRL_INPUT_MASK     0x100u
#define  TMR0_1_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR0_1_SCTRL_IPS_MASK       0x200u
#define  TMR0_1_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR0_1_SCTRL_IEFIE_MASK     0x400u
#define  TMR0_1_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR0_1_SCTRL_IEF_MASK       0x800u
#define  TMR0_1_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR0_1_SCTRL_TOFIE_MASK     0x1000u
#define  TMR0_1_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR0_1_SCTRL_TOF_MASK       0x2000u
#define  TMR0_1_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR0_1_SCTRL_TCFIE_MASK     0x4000u
#define  TMR0_1_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR0_1_SCTRL_TCF_MASK       0x8000u
#define  TMR0_1_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR0_1_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR0_1_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR0_1_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR0_1_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR0_1_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR0_1_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR0_1_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR0_1_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR0_1_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR0_1_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR0_1_CSCTRL_CL1_MASK      0x2u
#define  TMR0_1_CSCTRL_CL1_SHIFT     0
#define  TMR0_1_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR0_1_CSCTRL_CL1_SHIFT))&TMR0_1_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR0_1_CSCTRL_CL2_MASK      0x8u
#define  TMR0_1_CSCTRL_CL2_SHIFT     2
#define  TMR0_1_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR0_1_CSCTRL_CL2_SHIFT))&TMR0_1_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR0_1_CSCTRL_TCF1_MASK     0x10u
#define  TMR0_1_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR0_1_CSCTRL_TCF2_MASK     0x20u
#define  TMR0_1_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR0_1_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR0_1_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR0_1_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR0_1_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR0_1_CSCTRL_UP_MASK       0x200u
#define  TMR0_1_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR0_1_CSCTRL_TCI_MASK      0x400u
#define  TMR0_1_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR0_1_CSCTRL_ROC_MASK      0x800u
#define  TMR0_1_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR0_1_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR0_1_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR0_1_CSCTRL_FAULT_MASK    0x2000u
#define  TMR0_1_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR0_1_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR0_1_CSCTRL_DBG_EN_SHIFT  14
#define  TMR0_1_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR0_1_CSCTRL_DBG_EN_SHIFT))&TMR0_1_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR0_1_FILT_FILT_PER_MASK   0x8u
#define  TMR0_1_FILT_FILT_PER_SHIFT  0
#define  TMR0_1_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR0_1_FILT_FILT_PER_SHIFT))&TMR0_1_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR0_1_FILT_FILT_CNT_MASK   0x300u
#define  TMR0_1_FILT_FILT_CNT_SHIFT  8
#define  TMR0_1_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR0_1_FILT_FILT_CNT_SHIFT))&TMR0_1_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR0_1_DMA_IEFDE_MASK       0x1u
#define  TMR0_1_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR0_1_DMA_CMPLD1DE_MASK    0x2u
#define  TMR0_1_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR0_1_DMA_CMPLD2DE_MASK    0x4u
#define  TMR0_1_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  TMR0_2_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR0_2_COMP1_COMPARISON_1_SHIFT 0
#define  TMR0_2_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR0_2_COMP1_COMPARISON_1_SHIFT))&TMR0_2_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR0_2_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR0_2_COMP2_COMPARISON_2_SHIFT 0
#define  TMR0_2_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR0_2_COMP2_COMPARISON_2_SHIFT))&TMR0_2_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR0_2_CAPT_CAPTURE_MASK    0x10u
#define  TMR0_2_CAPT_CAPTURE_SHIFT   0
#define  TMR0_2_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR0_2_CAPT_CAPTURE_SHIFT))&TMR0_2_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR0_2_LOAD_LOAD_MASK       0x10u
#define  TMR0_2_LOAD_LOAD_SHIFT      0
#define  TMR0_2_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR0_2_LOAD_LOAD_SHIFT))&TMR0_2_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR0_2_HOLD_HOLD_MASK       0x10u
#define  TMR0_2_HOLD_HOLD_SHIFT      0
#define  TMR0_2_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR0_2_HOLD_HOLD_SHIFT))&TMR0_2_HOLD_HOLD_MASK)

#define  TMR0_2_CNTR_COUNTER_MASK    0x10u
#define  TMR0_2_CNTR_COUNTER_SHIFT   0
#define  TMR0_2_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR0_2_CNTR_COUNTER_SHIFT))&TMR0_2_CNTR_COUNTER_MASK)

#define  TMR0_2_CTRL_OUTMODE_MASK    0x3u
#define  TMR0_2_CTRL_OUTMODE_SHIFT   0
#define  TMR0_2_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR0_2_CTRL_OUTMODE_SHIFT))&TMR0_2_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR0_2_CTRL_COINIT_MASK     0x8u
#define  TMR0_2_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR0_2_CTRL_DIR_MASK        0x10u
#define  TMR0_2_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR0_2_CTRL_LENGTH_MASK     0x20u
#define  TMR0_2_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR0_2_CTRL_ONCE_MASK       0x40u
#define  TMR0_2_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR0_2_CTRL_SCS_MASK        0x100u
#define  TMR0_2_CTRL_SCS_SHIFT       7
#define  TMR0_2_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR0_2_CTRL_SCS_SHIFT))&TMR0_2_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR0_2_CTRL_PCS_MASK        0x800u
#define  TMR0_2_CTRL_PCS_SHIFT       9
#define  TMR0_2_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR0_2_CTRL_PCS_SHIFT))&TMR0_2_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR0_2_CTRL_CM_MASK         0x6000u
#define  TMR0_2_CTRL_CM_SHIFT        13
#define  TMR0_2_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR0_2_CTRL_CM_SHIFT))&TMR0_2_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR0_2_SCTRL_OEN_MASK       0x1u
#define  TMR0_2_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR0_2_SCTRL_OPS_MASK       0x2u
#define  TMR0_2_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR0_2_SCTRL_FORCE_MASK     0x4u
#define  TMR0_2_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR0_2_SCTRL_VAL_MASK       0x8u
#define  TMR0_2_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR0_2_SCTRL_EEOF_MASK      0x10u
#define  TMR0_2_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR0_2_SCTRL_MSTR_MASK      0x20u
#define  TMR0_2_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR0_2_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR0_2_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR0_2_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR0_2_SCTRL_CAPTURE_MODE_SHIFT))&TMR0_2_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR0_2_SCTRL_INPUT_MASK     0x100u
#define  TMR0_2_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR0_2_SCTRL_IPS_MASK       0x200u
#define  TMR0_2_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR0_2_SCTRL_IEFIE_MASK     0x400u
#define  TMR0_2_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR0_2_SCTRL_IEF_MASK       0x800u
#define  TMR0_2_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR0_2_SCTRL_TOFIE_MASK     0x1000u
#define  TMR0_2_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR0_2_SCTRL_TOF_MASK       0x2000u
#define  TMR0_2_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR0_2_SCTRL_TCFIE_MASK     0x4000u
#define  TMR0_2_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR0_2_SCTRL_TCF_MASK       0x8000u
#define  TMR0_2_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR0_2_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR0_2_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR0_2_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR0_2_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR0_2_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR0_2_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR0_2_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR0_2_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR0_2_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR0_2_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR0_2_CSCTRL_CL1_MASK      0x2u
#define  TMR0_2_CSCTRL_CL1_SHIFT     0
#define  TMR0_2_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR0_2_CSCTRL_CL1_SHIFT))&TMR0_2_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR0_2_CSCTRL_CL2_MASK      0x8u
#define  TMR0_2_CSCTRL_CL2_SHIFT     2
#define  TMR0_2_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR0_2_CSCTRL_CL2_SHIFT))&TMR0_2_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR0_2_CSCTRL_TCF1_MASK     0x10u
#define  TMR0_2_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR0_2_CSCTRL_TCF2_MASK     0x20u
#define  TMR0_2_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR0_2_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR0_2_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR0_2_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR0_2_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR0_2_CSCTRL_UP_MASK       0x200u
#define  TMR0_2_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR0_2_CSCTRL_TCI_MASK      0x400u
#define  TMR0_2_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR0_2_CSCTRL_ROC_MASK      0x800u
#define  TMR0_2_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR0_2_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR0_2_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR0_2_CSCTRL_FAULT_MASK    0x2000u
#define  TMR0_2_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR0_2_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR0_2_CSCTRL_DBG_EN_SHIFT  14
#define  TMR0_2_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR0_2_CSCTRL_DBG_EN_SHIFT))&TMR0_2_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR0_2_FILT_FILT_PER_MASK   0x8u
#define  TMR0_2_FILT_FILT_PER_SHIFT  0
#define  TMR0_2_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR0_2_FILT_FILT_PER_SHIFT))&TMR0_2_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR0_2_FILT_FILT_CNT_MASK   0x300u
#define  TMR0_2_FILT_FILT_CNT_SHIFT  8
#define  TMR0_2_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR0_2_FILT_FILT_CNT_SHIFT))&TMR0_2_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR0_2_DMA_IEFDE_MASK       0x1u
#define  TMR0_2_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR0_2_DMA_CMPLD1DE_MASK    0x2u
#define  TMR0_2_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR0_2_DMA_CMPLD2DE_MASK    0x4u
#define  TMR0_2_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  TMR0_3_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR0_3_COMP1_COMPARISON_1_SHIFT 0
#define  TMR0_3_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR0_3_COMP1_COMPARISON_1_SHIFT))&TMR0_3_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR0_3_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR0_3_COMP2_COMPARISON_2_SHIFT 0
#define  TMR0_3_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR0_3_COMP2_COMPARISON_2_SHIFT))&TMR0_3_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR0_3_CAPT_CAPTURE_MASK    0x10u
#define  TMR0_3_CAPT_CAPTURE_SHIFT   0
#define  TMR0_3_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR0_3_CAPT_CAPTURE_SHIFT))&TMR0_3_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR0_3_LOAD_LOAD_MASK       0x10u
#define  TMR0_3_LOAD_LOAD_SHIFT      0
#define  TMR0_3_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR0_3_LOAD_LOAD_SHIFT))&TMR0_3_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR0_3_HOLD_HOLD_MASK       0x10u
#define  TMR0_3_HOLD_HOLD_SHIFT      0
#define  TMR0_3_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR0_3_HOLD_HOLD_SHIFT))&TMR0_3_HOLD_HOLD_MASK)

#define  TMR0_3_CNTR_COUNTER_MASK    0x10u
#define  TMR0_3_CNTR_COUNTER_SHIFT   0
#define  TMR0_3_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR0_3_CNTR_COUNTER_SHIFT))&TMR0_3_CNTR_COUNTER_MASK)

#define  TMR0_3_CTRL_OUTMODE_MASK    0x3u
#define  TMR0_3_CTRL_OUTMODE_SHIFT   0
#define  TMR0_3_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR0_3_CTRL_OUTMODE_SHIFT))&TMR0_3_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR0_3_CTRL_COINIT_MASK     0x8u
#define  TMR0_3_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR0_3_CTRL_DIR_MASK        0x10u
#define  TMR0_3_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR0_3_CTRL_LENGTH_MASK     0x20u
#define  TMR0_3_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR0_3_CTRL_ONCE_MASK       0x40u
#define  TMR0_3_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR0_3_CTRL_SCS_MASK        0x100u
#define  TMR0_3_CTRL_SCS_SHIFT       7
#define  TMR0_3_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR0_3_CTRL_SCS_SHIFT))&TMR0_3_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR0_3_CTRL_PCS_MASK        0x800u
#define  TMR0_3_CTRL_PCS_SHIFT       9
#define  TMR0_3_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR0_3_CTRL_PCS_SHIFT))&TMR0_3_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR0_3_CTRL_CM_MASK         0x6000u
#define  TMR0_3_CTRL_CM_SHIFT        13
#define  TMR0_3_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR0_3_CTRL_CM_SHIFT))&TMR0_3_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR0_3_SCTRL_OEN_MASK       0x1u
#define  TMR0_3_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR0_3_SCTRL_OPS_MASK       0x2u
#define  TMR0_3_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR0_3_SCTRL_FORCE_MASK     0x4u
#define  TMR0_3_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR0_3_SCTRL_VAL_MASK       0x8u
#define  TMR0_3_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR0_3_SCTRL_EEOF_MASK      0x10u
#define  TMR0_3_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR0_3_SCTRL_MSTR_MASK      0x20u
#define  TMR0_3_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR0_3_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR0_3_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR0_3_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR0_3_SCTRL_CAPTURE_MODE_SHIFT))&TMR0_3_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR0_3_SCTRL_INPUT_MASK     0x100u
#define  TMR0_3_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR0_3_SCTRL_IPS_MASK       0x200u
#define  TMR0_3_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR0_3_SCTRL_IEFIE_MASK     0x400u
#define  TMR0_3_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR0_3_SCTRL_IEF_MASK       0x800u
#define  TMR0_3_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR0_3_SCTRL_TOFIE_MASK     0x1000u
#define  TMR0_3_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR0_3_SCTRL_TOF_MASK       0x2000u
#define  TMR0_3_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR0_3_SCTRL_TCFIE_MASK     0x4000u
#define  TMR0_3_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR0_3_SCTRL_TCF_MASK       0x8000u
#define  TMR0_3_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR0_3_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR0_3_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR0_3_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR0_3_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR0_3_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR0_3_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR0_3_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR0_3_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR0_3_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR0_3_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR0_3_CSCTRL_CL1_MASK      0x2u
#define  TMR0_3_CSCTRL_CL1_SHIFT     0
#define  TMR0_3_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR0_3_CSCTRL_CL1_SHIFT))&TMR0_3_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR0_3_CSCTRL_CL2_MASK      0x8u
#define  TMR0_3_CSCTRL_CL2_SHIFT     2
#define  TMR0_3_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR0_3_CSCTRL_CL2_SHIFT))&TMR0_3_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR0_3_CSCTRL_TCF1_MASK     0x10u
#define  TMR0_3_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR0_3_CSCTRL_TCF2_MASK     0x20u
#define  TMR0_3_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR0_3_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR0_3_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR0_3_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR0_3_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR0_3_CSCTRL_UP_MASK       0x200u
#define  TMR0_3_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR0_3_CSCTRL_TCI_MASK      0x400u
#define  TMR0_3_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR0_3_CSCTRL_ROC_MASK      0x800u
#define  TMR0_3_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR0_3_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR0_3_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR0_3_CSCTRL_FAULT_MASK    0x2000u
#define  TMR0_3_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR0_3_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR0_3_CSCTRL_DBG_EN_SHIFT  14
#define  TMR0_3_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR0_3_CSCTRL_DBG_EN_SHIFT))&TMR0_3_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR0_3_FILT_FILT_PER_MASK   0x8u
#define  TMR0_3_FILT_FILT_PER_SHIFT  0
#define  TMR0_3_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR0_3_FILT_FILT_PER_SHIFT))&TMR0_3_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR0_3_FILT_FILT_CNT_MASK   0x300u
#define  TMR0_3_FILT_FILT_CNT_SHIFT  8
#define  TMR0_3_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR0_3_FILT_FILT_CNT_SHIFT))&TMR0_3_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR0_3_DMA_IEFDE_MASK       0x1u
#define  TMR0_3_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR0_3_DMA_CMPLD1DE_MASK    0x2u
#define  TMR0_3_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR0_3_DMA_CMPLD2DE_MASK    0x4u
#define  TMR0_3_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  TMR1_0_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR1_0_COMP1_COMPARISON_1_SHIFT 0
#define  TMR1_0_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR1_0_COMP1_COMPARISON_1_SHIFT))&TMR1_0_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR1_0_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR1_0_COMP2_COMPARISON_2_SHIFT 0
#define  TMR1_0_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR1_0_COMP2_COMPARISON_2_SHIFT))&TMR1_0_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR1_0_CAPT_CAPTURE_MASK    0x10u
#define  TMR1_0_CAPT_CAPTURE_SHIFT   0
#define  TMR1_0_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR1_0_CAPT_CAPTURE_SHIFT))&TMR1_0_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR1_0_LOAD_LOAD_MASK       0x10u
#define  TMR1_0_LOAD_LOAD_SHIFT      0
#define  TMR1_0_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR1_0_LOAD_LOAD_SHIFT))&TMR1_0_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR1_0_HOLD_HOLD_MASK       0x10u
#define  TMR1_0_HOLD_HOLD_SHIFT      0
#define  TMR1_0_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR1_0_HOLD_HOLD_SHIFT))&TMR1_0_HOLD_HOLD_MASK)

#define  TMR1_0_CNTR_COUNTER_MASK    0x10u
#define  TMR1_0_CNTR_COUNTER_SHIFT   0
#define  TMR1_0_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR1_0_CNTR_COUNTER_SHIFT))&TMR1_0_CNTR_COUNTER_MASK)

#define  TMR1_0_CTRL_OUTMODE_MASK    0x3u
#define  TMR1_0_CTRL_OUTMODE_SHIFT   0
#define  TMR1_0_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR1_0_CTRL_OUTMODE_SHIFT))&TMR1_0_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR1_0_CTRL_COINIT_MASK     0x8u
#define  TMR1_0_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR1_0_CTRL_DIR_MASK        0x10u
#define  TMR1_0_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR1_0_CTRL_LENGTH_MASK     0x20u
#define  TMR1_0_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR1_0_CTRL_ONCE_MASK       0x40u
#define  TMR1_0_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR1_0_CTRL_SCS_MASK        0x100u
#define  TMR1_0_CTRL_SCS_SHIFT       7
#define  TMR1_0_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR1_0_CTRL_SCS_SHIFT))&TMR1_0_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR1_0_CTRL_PCS_MASK        0x800u
#define  TMR1_0_CTRL_PCS_SHIFT       9
#define  TMR1_0_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR1_0_CTRL_PCS_SHIFT))&TMR1_0_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR1_0_CTRL_CM_MASK         0x6000u
#define  TMR1_0_CTRL_CM_SHIFT        13
#define  TMR1_0_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR1_0_CTRL_CM_SHIFT))&TMR1_0_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR1_0_SCTRL_OEN_MASK       0x1u
#define  TMR1_0_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR1_0_SCTRL_OPS_MASK       0x2u
#define  TMR1_0_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR1_0_SCTRL_FORCE_MASK     0x4u
#define  TMR1_0_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR1_0_SCTRL_VAL_MASK       0x8u
#define  TMR1_0_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR1_0_SCTRL_EEOF_MASK      0x10u
#define  TMR1_0_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR1_0_SCTRL_MSTR_MASK      0x20u
#define  TMR1_0_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR1_0_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR1_0_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR1_0_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR1_0_SCTRL_CAPTURE_MODE_SHIFT))&TMR1_0_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR1_0_SCTRL_INPUT_MASK     0x100u
#define  TMR1_0_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR1_0_SCTRL_IPS_MASK       0x200u
#define  TMR1_0_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR1_0_SCTRL_IEFIE_MASK     0x400u
#define  TMR1_0_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR1_0_SCTRL_IEF_MASK       0x800u
#define  TMR1_0_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR1_0_SCTRL_TOFIE_MASK     0x1000u
#define  TMR1_0_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR1_0_SCTRL_TOF_MASK       0x2000u
#define  TMR1_0_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR1_0_SCTRL_TCFIE_MASK     0x4000u
#define  TMR1_0_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR1_0_SCTRL_TCF_MASK       0x8000u
#define  TMR1_0_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR1_0_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR1_0_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR1_0_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR1_0_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR1_0_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR1_0_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR1_0_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR1_0_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR1_0_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR1_0_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR1_0_CSCTRL_CL1_MASK      0x2u
#define  TMR1_0_CSCTRL_CL1_SHIFT     0
#define  TMR1_0_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR1_0_CSCTRL_CL1_SHIFT))&TMR1_0_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR1_0_CSCTRL_CL2_MASK      0x8u
#define  TMR1_0_CSCTRL_CL2_SHIFT     2
#define  TMR1_0_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR1_0_CSCTRL_CL2_SHIFT))&TMR1_0_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR1_0_CSCTRL_TCF1_MASK     0x10u
#define  TMR1_0_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR1_0_CSCTRL_TCF2_MASK     0x20u
#define  TMR1_0_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR1_0_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR1_0_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR1_0_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR1_0_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR1_0_CSCTRL_UP_MASK       0x200u
#define  TMR1_0_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR1_0_CSCTRL_TCI_MASK      0x400u
#define  TMR1_0_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR1_0_CSCTRL_ROC_MASK      0x800u
#define  TMR1_0_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR1_0_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR1_0_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR1_0_CSCTRL_FAULT_MASK    0x2000u
#define  TMR1_0_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR1_0_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR1_0_CSCTRL_DBG_EN_SHIFT  14
#define  TMR1_0_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR1_0_CSCTRL_DBG_EN_SHIFT))&TMR1_0_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR1_0_FILT_FILT_PER_MASK   0x8u
#define  TMR1_0_FILT_FILT_PER_SHIFT  0
#define  TMR1_0_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR1_0_FILT_FILT_PER_SHIFT))&TMR1_0_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR1_0_FILT_FILT_CNT_MASK   0x300u
#define  TMR1_0_FILT_FILT_CNT_SHIFT  8
#define  TMR1_0_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR1_0_FILT_FILT_CNT_SHIFT))&TMR1_0_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR1_0_DMA_IEFDE_MASK       0x1u
#define  TMR1_0_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR1_0_DMA_CMPLD1DE_MASK    0x2u
#define  TMR1_0_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR1_0_DMA_CMPLD2DE_MASK    0x4u
#define  TMR1_0_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  TMR1_0_ENBL_ENBL_MASK       0x4u
#define  TMR1_0_ENBL_ENBL_SHIFT      0
#define  TMR1_0_ENBL_ENBL(x)         (((uint16_t)(((uint16_t)(x))<<TMR1_0_ENBL_ENBL_SHIFT))&TMR1_0_ENBL_ENBL_MASK)
//   Timer Channel Enable
//     0b0 ==> Timer channel is disabled.
//     0b1 ==> Timer channel is enabled. (default)

#define  TMR1_1_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR1_1_COMP1_COMPARISON_1_SHIFT 0
#define  TMR1_1_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR1_1_COMP1_COMPARISON_1_SHIFT))&TMR1_1_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR1_1_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR1_1_COMP2_COMPARISON_2_SHIFT 0
#define  TMR1_1_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR1_1_COMP2_COMPARISON_2_SHIFT))&TMR1_1_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR1_1_CAPT_CAPTURE_MASK    0x10u
#define  TMR1_1_CAPT_CAPTURE_SHIFT   0
#define  TMR1_1_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR1_1_CAPT_CAPTURE_SHIFT))&TMR1_1_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR1_1_LOAD_LOAD_MASK       0x10u
#define  TMR1_1_LOAD_LOAD_SHIFT      0
#define  TMR1_1_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR1_1_LOAD_LOAD_SHIFT))&TMR1_1_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR1_1_HOLD_HOLD_MASK       0x10u
#define  TMR1_1_HOLD_HOLD_SHIFT      0
#define  TMR1_1_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR1_1_HOLD_HOLD_SHIFT))&TMR1_1_HOLD_HOLD_MASK)

#define  TMR1_1_CNTR_COUNTER_MASK    0x10u
#define  TMR1_1_CNTR_COUNTER_SHIFT   0
#define  TMR1_1_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR1_1_CNTR_COUNTER_SHIFT))&TMR1_1_CNTR_COUNTER_MASK)

#define  TMR1_1_CTRL_OUTMODE_MASK    0x3u
#define  TMR1_1_CTRL_OUTMODE_SHIFT   0
#define  TMR1_1_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR1_1_CTRL_OUTMODE_SHIFT))&TMR1_1_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR1_1_CTRL_COINIT_MASK     0x8u
#define  TMR1_1_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR1_1_CTRL_DIR_MASK        0x10u
#define  TMR1_1_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR1_1_CTRL_LENGTH_MASK     0x20u
#define  TMR1_1_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR1_1_CTRL_ONCE_MASK       0x40u
#define  TMR1_1_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR1_1_CTRL_SCS_MASK        0x100u
#define  TMR1_1_CTRL_SCS_SHIFT       7
#define  TMR1_1_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR1_1_CTRL_SCS_SHIFT))&TMR1_1_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR1_1_CTRL_PCS_MASK        0x800u
#define  TMR1_1_CTRL_PCS_SHIFT       9
#define  TMR1_1_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR1_1_CTRL_PCS_SHIFT))&TMR1_1_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR1_1_CTRL_CM_MASK         0x6000u
#define  TMR1_1_CTRL_CM_SHIFT        13
#define  TMR1_1_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR1_1_CTRL_CM_SHIFT))&TMR1_1_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR1_1_SCTRL_OEN_MASK       0x1u
#define  TMR1_1_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR1_1_SCTRL_OPS_MASK       0x2u
#define  TMR1_1_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR1_1_SCTRL_FORCE_MASK     0x4u
#define  TMR1_1_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR1_1_SCTRL_VAL_MASK       0x8u
#define  TMR1_1_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR1_1_SCTRL_EEOF_MASK      0x10u
#define  TMR1_1_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR1_1_SCTRL_MSTR_MASK      0x20u
#define  TMR1_1_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR1_1_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR1_1_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR1_1_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR1_1_SCTRL_CAPTURE_MODE_SHIFT))&TMR1_1_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR1_1_SCTRL_INPUT_MASK     0x100u
#define  TMR1_1_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR1_1_SCTRL_IPS_MASK       0x200u
#define  TMR1_1_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR1_1_SCTRL_IEFIE_MASK     0x400u
#define  TMR1_1_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR1_1_SCTRL_IEF_MASK       0x800u
#define  TMR1_1_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR1_1_SCTRL_TOFIE_MASK     0x1000u
#define  TMR1_1_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR1_1_SCTRL_TOF_MASK       0x2000u
#define  TMR1_1_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR1_1_SCTRL_TCFIE_MASK     0x4000u
#define  TMR1_1_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR1_1_SCTRL_TCF_MASK       0x8000u
#define  TMR1_1_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR1_1_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR1_1_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR1_1_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR1_1_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR1_1_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR1_1_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR1_1_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR1_1_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR1_1_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR1_1_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR1_1_CSCTRL_CL1_MASK      0x2u
#define  TMR1_1_CSCTRL_CL1_SHIFT     0
#define  TMR1_1_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR1_1_CSCTRL_CL1_SHIFT))&TMR1_1_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR1_1_CSCTRL_CL2_MASK      0x8u
#define  TMR1_1_CSCTRL_CL2_SHIFT     2
#define  TMR1_1_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR1_1_CSCTRL_CL2_SHIFT))&TMR1_1_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR1_1_CSCTRL_TCF1_MASK     0x10u
#define  TMR1_1_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR1_1_CSCTRL_TCF2_MASK     0x20u
#define  TMR1_1_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR1_1_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR1_1_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR1_1_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR1_1_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR1_1_CSCTRL_UP_MASK       0x200u
#define  TMR1_1_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR1_1_CSCTRL_TCI_MASK      0x400u
#define  TMR1_1_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR1_1_CSCTRL_ROC_MASK      0x800u
#define  TMR1_1_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR1_1_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR1_1_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR1_1_CSCTRL_FAULT_MASK    0x2000u
#define  TMR1_1_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR1_1_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR1_1_CSCTRL_DBG_EN_SHIFT  14
#define  TMR1_1_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR1_1_CSCTRL_DBG_EN_SHIFT))&TMR1_1_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR1_1_FILT_FILT_PER_MASK   0x8u
#define  TMR1_1_FILT_FILT_PER_SHIFT  0
#define  TMR1_1_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR1_1_FILT_FILT_PER_SHIFT))&TMR1_1_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR1_1_FILT_FILT_CNT_MASK   0x300u
#define  TMR1_1_FILT_FILT_CNT_SHIFT  8
#define  TMR1_1_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR1_1_FILT_FILT_CNT_SHIFT))&TMR1_1_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR1_1_DMA_IEFDE_MASK       0x1u
#define  TMR1_1_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR1_1_DMA_CMPLD1DE_MASK    0x2u
#define  TMR1_1_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR1_1_DMA_CMPLD2DE_MASK    0x4u
#define  TMR1_1_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  TMR1_2_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR1_2_COMP1_COMPARISON_1_SHIFT 0
#define  TMR1_2_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR1_2_COMP1_COMPARISON_1_SHIFT))&TMR1_2_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR1_2_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR1_2_COMP2_COMPARISON_2_SHIFT 0
#define  TMR1_2_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR1_2_COMP2_COMPARISON_2_SHIFT))&TMR1_2_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR1_2_CAPT_CAPTURE_MASK    0x10u
#define  TMR1_2_CAPT_CAPTURE_SHIFT   0
#define  TMR1_2_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR1_2_CAPT_CAPTURE_SHIFT))&TMR1_2_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR1_2_LOAD_LOAD_MASK       0x10u
#define  TMR1_2_LOAD_LOAD_SHIFT      0
#define  TMR1_2_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR1_2_LOAD_LOAD_SHIFT))&TMR1_2_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR1_2_HOLD_HOLD_MASK       0x10u
#define  TMR1_2_HOLD_HOLD_SHIFT      0
#define  TMR1_2_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR1_2_HOLD_HOLD_SHIFT))&TMR1_2_HOLD_HOLD_MASK)

#define  TMR1_2_CNTR_COUNTER_MASK    0x10u
#define  TMR1_2_CNTR_COUNTER_SHIFT   0
#define  TMR1_2_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR1_2_CNTR_COUNTER_SHIFT))&TMR1_2_CNTR_COUNTER_MASK)

#define  TMR1_2_CTRL_OUTMODE_MASK    0x3u
#define  TMR1_2_CTRL_OUTMODE_SHIFT   0
#define  TMR1_2_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR1_2_CTRL_OUTMODE_SHIFT))&TMR1_2_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR1_2_CTRL_COINIT_MASK     0x8u
#define  TMR1_2_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR1_2_CTRL_DIR_MASK        0x10u
#define  TMR1_2_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR1_2_CTRL_LENGTH_MASK     0x20u
#define  TMR1_2_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR1_2_CTRL_ONCE_MASK       0x40u
#define  TMR1_2_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR1_2_CTRL_SCS_MASK        0x100u
#define  TMR1_2_CTRL_SCS_SHIFT       7
#define  TMR1_2_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR1_2_CTRL_SCS_SHIFT))&TMR1_2_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR1_2_CTRL_PCS_MASK        0x800u
#define  TMR1_2_CTRL_PCS_SHIFT       9
#define  TMR1_2_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR1_2_CTRL_PCS_SHIFT))&TMR1_2_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR1_2_CTRL_CM_MASK         0x6000u
#define  TMR1_2_CTRL_CM_SHIFT        13
#define  TMR1_2_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR1_2_CTRL_CM_SHIFT))&TMR1_2_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR1_2_SCTRL_OEN_MASK       0x1u
#define  TMR1_2_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR1_2_SCTRL_OPS_MASK       0x2u
#define  TMR1_2_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR1_2_SCTRL_FORCE_MASK     0x4u
#define  TMR1_2_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR1_2_SCTRL_VAL_MASK       0x8u
#define  TMR1_2_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR1_2_SCTRL_EEOF_MASK      0x10u
#define  TMR1_2_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR1_2_SCTRL_MSTR_MASK      0x20u
#define  TMR1_2_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR1_2_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR1_2_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR1_2_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR1_2_SCTRL_CAPTURE_MODE_SHIFT))&TMR1_2_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR1_2_SCTRL_INPUT_MASK     0x100u
#define  TMR1_2_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR1_2_SCTRL_IPS_MASK       0x200u
#define  TMR1_2_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR1_2_SCTRL_IEFIE_MASK     0x400u
#define  TMR1_2_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR1_2_SCTRL_IEF_MASK       0x800u
#define  TMR1_2_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR1_2_SCTRL_TOFIE_MASK     0x1000u
#define  TMR1_2_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR1_2_SCTRL_TOF_MASK       0x2000u
#define  TMR1_2_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR1_2_SCTRL_TCFIE_MASK     0x4000u
#define  TMR1_2_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR1_2_SCTRL_TCF_MASK       0x8000u
#define  TMR1_2_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR1_2_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR1_2_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR1_2_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR1_2_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR1_2_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR1_2_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR1_2_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR1_2_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR1_2_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR1_2_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR1_2_CSCTRL_CL1_MASK      0x2u
#define  TMR1_2_CSCTRL_CL1_SHIFT     0
#define  TMR1_2_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR1_2_CSCTRL_CL1_SHIFT))&TMR1_2_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR1_2_CSCTRL_CL2_MASK      0x8u
#define  TMR1_2_CSCTRL_CL2_SHIFT     2
#define  TMR1_2_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR1_2_CSCTRL_CL2_SHIFT))&TMR1_2_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR1_2_CSCTRL_TCF1_MASK     0x10u
#define  TMR1_2_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR1_2_CSCTRL_TCF2_MASK     0x20u
#define  TMR1_2_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR1_2_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR1_2_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR1_2_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR1_2_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR1_2_CSCTRL_UP_MASK       0x200u
#define  TMR1_2_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR1_2_CSCTRL_TCI_MASK      0x400u
#define  TMR1_2_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR1_2_CSCTRL_ROC_MASK      0x800u
#define  TMR1_2_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR1_2_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR1_2_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR1_2_CSCTRL_FAULT_MASK    0x2000u
#define  TMR1_2_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR1_2_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR1_2_CSCTRL_DBG_EN_SHIFT  14
#define  TMR1_2_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR1_2_CSCTRL_DBG_EN_SHIFT))&TMR1_2_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR1_2_FILT_FILT_PER_MASK   0x8u
#define  TMR1_2_FILT_FILT_PER_SHIFT  0
#define  TMR1_2_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR1_2_FILT_FILT_PER_SHIFT))&TMR1_2_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR1_2_FILT_FILT_CNT_MASK   0x300u
#define  TMR1_2_FILT_FILT_CNT_SHIFT  8
#define  TMR1_2_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR1_2_FILT_FILT_CNT_SHIFT))&TMR1_2_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR1_2_DMA_IEFDE_MASK       0x1u
#define  TMR1_2_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR1_2_DMA_CMPLD1DE_MASK    0x2u
#define  TMR1_2_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR1_2_DMA_CMPLD2DE_MASK    0x4u
#define  TMR1_2_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  TMR1_3_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR1_3_COMP1_COMPARISON_1_SHIFT 0
#define  TMR1_3_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR1_3_COMP1_COMPARISON_1_SHIFT))&TMR1_3_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR1_3_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR1_3_COMP2_COMPARISON_2_SHIFT 0
#define  TMR1_3_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR1_3_COMP2_COMPARISON_2_SHIFT))&TMR1_3_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR1_3_CAPT_CAPTURE_MASK    0x10u
#define  TMR1_3_CAPT_CAPTURE_SHIFT   0
#define  TMR1_3_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR1_3_CAPT_CAPTURE_SHIFT))&TMR1_3_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR1_3_LOAD_LOAD_MASK       0x10u
#define  TMR1_3_LOAD_LOAD_SHIFT      0
#define  TMR1_3_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR1_3_LOAD_LOAD_SHIFT))&TMR1_3_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR1_3_HOLD_HOLD_MASK       0x10u
#define  TMR1_3_HOLD_HOLD_SHIFT      0
#define  TMR1_3_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR1_3_HOLD_HOLD_SHIFT))&TMR1_3_HOLD_HOLD_MASK)

#define  TMR1_3_CNTR_COUNTER_MASK    0x10u
#define  TMR1_3_CNTR_COUNTER_SHIFT   0
#define  TMR1_3_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR1_3_CNTR_COUNTER_SHIFT))&TMR1_3_CNTR_COUNTER_MASK)

#define  TMR1_3_CTRL_OUTMODE_MASK    0x3u
#define  TMR1_3_CTRL_OUTMODE_SHIFT   0
#define  TMR1_3_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR1_3_CTRL_OUTMODE_SHIFT))&TMR1_3_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR1_3_CTRL_COINIT_MASK     0x8u
#define  TMR1_3_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR1_3_CTRL_DIR_MASK        0x10u
#define  TMR1_3_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR1_3_CTRL_LENGTH_MASK     0x20u
#define  TMR1_3_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR1_3_CTRL_ONCE_MASK       0x40u
#define  TMR1_3_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR1_3_CTRL_SCS_MASK        0x100u
#define  TMR1_3_CTRL_SCS_SHIFT       7
#define  TMR1_3_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR1_3_CTRL_SCS_SHIFT))&TMR1_3_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR1_3_CTRL_PCS_MASK        0x800u
#define  TMR1_3_CTRL_PCS_SHIFT       9
#define  TMR1_3_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR1_3_CTRL_PCS_SHIFT))&TMR1_3_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR1_3_CTRL_CM_MASK         0x6000u
#define  TMR1_3_CTRL_CM_SHIFT        13
#define  TMR1_3_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR1_3_CTRL_CM_SHIFT))&TMR1_3_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR1_3_SCTRL_OEN_MASK       0x1u
#define  TMR1_3_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR1_3_SCTRL_OPS_MASK       0x2u
#define  TMR1_3_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR1_3_SCTRL_FORCE_MASK     0x4u
#define  TMR1_3_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR1_3_SCTRL_VAL_MASK       0x8u
#define  TMR1_3_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR1_3_SCTRL_EEOF_MASK      0x10u
#define  TMR1_3_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR1_3_SCTRL_MSTR_MASK      0x20u
#define  TMR1_3_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR1_3_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR1_3_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR1_3_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR1_3_SCTRL_CAPTURE_MODE_SHIFT))&TMR1_3_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR1_3_SCTRL_INPUT_MASK     0x100u
#define  TMR1_3_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR1_3_SCTRL_IPS_MASK       0x200u
#define  TMR1_3_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR1_3_SCTRL_IEFIE_MASK     0x400u
#define  TMR1_3_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR1_3_SCTRL_IEF_MASK       0x800u
#define  TMR1_3_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR1_3_SCTRL_TOFIE_MASK     0x1000u
#define  TMR1_3_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR1_3_SCTRL_TOF_MASK       0x2000u
#define  TMR1_3_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR1_3_SCTRL_TCFIE_MASK     0x4000u
#define  TMR1_3_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR1_3_SCTRL_TCF_MASK       0x8000u
#define  TMR1_3_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR1_3_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR1_3_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR1_3_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR1_3_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR1_3_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR1_3_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR1_3_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR1_3_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR1_3_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR1_3_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR1_3_CSCTRL_CL1_MASK      0x2u
#define  TMR1_3_CSCTRL_CL1_SHIFT     0
#define  TMR1_3_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR1_3_CSCTRL_CL1_SHIFT))&TMR1_3_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR1_3_CSCTRL_CL2_MASK      0x8u
#define  TMR1_3_CSCTRL_CL2_SHIFT     2
#define  TMR1_3_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR1_3_CSCTRL_CL2_SHIFT))&TMR1_3_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR1_3_CSCTRL_TCF1_MASK     0x10u
#define  TMR1_3_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR1_3_CSCTRL_TCF2_MASK     0x20u
#define  TMR1_3_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR1_3_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR1_3_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR1_3_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR1_3_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR1_3_CSCTRL_UP_MASK       0x200u
#define  TMR1_3_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR1_3_CSCTRL_TCI_MASK      0x400u
#define  TMR1_3_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR1_3_CSCTRL_ROC_MASK      0x800u
#define  TMR1_3_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR1_3_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR1_3_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR1_3_CSCTRL_FAULT_MASK    0x2000u
#define  TMR1_3_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR1_3_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR1_3_CSCTRL_DBG_EN_SHIFT  14
#define  TMR1_3_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR1_3_CSCTRL_DBG_EN_SHIFT))&TMR1_3_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR1_3_FILT_FILT_PER_MASK   0x8u
#define  TMR1_3_FILT_FILT_PER_SHIFT  0
#define  TMR1_3_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR1_3_FILT_FILT_PER_SHIFT))&TMR1_3_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR1_3_FILT_FILT_CNT_MASK   0x300u
#define  TMR1_3_FILT_FILT_CNT_SHIFT  8
#define  TMR1_3_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR1_3_FILT_FILT_CNT_SHIFT))&TMR1_3_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR1_3_DMA_IEFDE_MASK       0x1u
#define  TMR1_3_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR1_3_DMA_CMPLD1DE_MASK    0x2u
#define  TMR1_3_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR1_3_DMA_CMPLD2DE_MASK    0x4u
#define  TMR1_3_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  TMR2_0_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR2_0_COMP1_COMPARISON_1_SHIFT 0
#define  TMR2_0_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR2_0_COMP1_COMPARISON_1_SHIFT))&TMR2_0_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR2_0_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR2_0_COMP2_COMPARISON_2_SHIFT 0
#define  TMR2_0_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR2_0_COMP2_COMPARISON_2_SHIFT))&TMR2_0_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR2_0_CAPT_CAPTURE_MASK    0x10u
#define  TMR2_0_CAPT_CAPTURE_SHIFT   0
#define  TMR2_0_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR2_0_CAPT_CAPTURE_SHIFT))&TMR2_0_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR2_0_LOAD_LOAD_MASK       0x10u
#define  TMR2_0_LOAD_LOAD_SHIFT      0
#define  TMR2_0_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR2_0_LOAD_LOAD_SHIFT))&TMR2_0_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR2_0_HOLD_HOLD_MASK       0x10u
#define  TMR2_0_HOLD_HOLD_SHIFT      0
#define  TMR2_0_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR2_0_HOLD_HOLD_SHIFT))&TMR2_0_HOLD_HOLD_MASK)

#define  TMR2_0_CNTR_COUNTER_MASK    0x10u
#define  TMR2_0_CNTR_COUNTER_SHIFT   0
#define  TMR2_0_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR2_0_CNTR_COUNTER_SHIFT))&TMR2_0_CNTR_COUNTER_MASK)

#define  TMR2_0_CTRL_OUTMODE_MASK    0x3u
#define  TMR2_0_CTRL_OUTMODE_SHIFT   0
#define  TMR2_0_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR2_0_CTRL_OUTMODE_SHIFT))&TMR2_0_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR2_0_CTRL_COINIT_MASK     0x8u
#define  TMR2_0_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR2_0_CTRL_DIR_MASK        0x10u
#define  TMR2_0_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR2_0_CTRL_LENGTH_MASK     0x20u
#define  TMR2_0_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR2_0_CTRL_ONCE_MASK       0x40u
#define  TMR2_0_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR2_0_CTRL_SCS_MASK        0x100u
#define  TMR2_0_CTRL_SCS_SHIFT       7
#define  TMR2_0_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR2_0_CTRL_SCS_SHIFT))&TMR2_0_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR2_0_CTRL_PCS_MASK        0x800u
#define  TMR2_0_CTRL_PCS_SHIFT       9
#define  TMR2_0_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR2_0_CTRL_PCS_SHIFT))&TMR2_0_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR2_0_CTRL_CM_MASK         0x6000u
#define  TMR2_0_CTRL_CM_SHIFT        13
#define  TMR2_0_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR2_0_CTRL_CM_SHIFT))&TMR2_0_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR2_0_SCTRL_OEN_MASK       0x1u
#define  TMR2_0_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR2_0_SCTRL_OPS_MASK       0x2u
#define  TMR2_0_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR2_0_SCTRL_FORCE_MASK     0x4u
#define  TMR2_0_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR2_0_SCTRL_VAL_MASK       0x8u
#define  TMR2_0_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR2_0_SCTRL_EEOF_MASK      0x10u
#define  TMR2_0_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR2_0_SCTRL_MSTR_MASK      0x20u
#define  TMR2_0_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR2_0_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR2_0_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR2_0_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR2_0_SCTRL_CAPTURE_MODE_SHIFT))&TMR2_0_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR2_0_SCTRL_INPUT_MASK     0x100u
#define  TMR2_0_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR2_0_SCTRL_IPS_MASK       0x200u
#define  TMR2_0_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR2_0_SCTRL_IEFIE_MASK     0x400u
#define  TMR2_0_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR2_0_SCTRL_IEF_MASK       0x800u
#define  TMR2_0_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR2_0_SCTRL_TOFIE_MASK     0x1000u
#define  TMR2_0_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR2_0_SCTRL_TOF_MASK       0x2000u
#define  TMR2_0_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR2_0_SCTRL_TCFIE_MASK     0x4000u
#define  TMR2_0_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR2_0_SCTRL_TCF_MASK       0x8000u
#define  TMR2_0_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR2_0_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR2_0_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR2_0_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR2_0_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR2_0_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR2_0_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR2_0_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR2_0_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR2_0_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR2_0_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR2_0_CSCTRL_CL1_MASK      0x2u
#define  TMR2_0_CSCTRL_CL1_SHIFT     0
#define  TMR2_0_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR2_0_CSCTRL_CL1_SHIFT))&TMR2_0_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR2_0_CSCTRL_CL2_MASK      0x8u
#define  TMR2_0_CSCTRL_CL2_SHIFT     2
#define  TMR2_0_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR2_0_CSCTRL_CL2_SHIFT))&TMR2_0_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR2_0_CSCTRL_TCF1_MASK     0x10u
#define  TMR2_0_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR2_0_CSCTRL_TCF2_MASK     0x20u
#define  TMR2_0_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR2_0_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR2_0_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR2_0_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR2_0_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR2_0_CSCTRL_UP_MASK       0x200u
#define  TMR2_0_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR2_0_CSCTRL_TCI_MASK      0x400u
#define  TMR2_0_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR2_0_CSCTRL_ROC_MASK      0x800u
#define  TMR2_0_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR2_0_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR2_0_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR2_0_CSCTRL_FAULT_MASK    0x2000u
#define  TMR2_0_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR2_0_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR2_0_CSCTRL_DBG_EN_SHIFT  14
#define  TMR2_0_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR2_0_CSCTRL_DBG_EN_SHIFT))&TMR2_0_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR2_0_FILT_FILT_PER_MASK   0x8u
#define  TMR2_0_FILT_FILT_PER_SHIFT  0
#define  TMR2_0_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR2_0_FILT_FILT_PER_SHIFT))&TMR2_0_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR2_0_FILT_FILT_CNT_MASK   0x300u
#define  TMR2_0_FILT_FILT_CNT_SHIFT  8
#define  TMR2_0_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR2_0_FILT_FILT_CNT_SHIFT))&TMR2_0_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR2_0_DMA_IEFDE_MASK       0x1u
#define  TMR2_0_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR2_0_DMA_CMPLD1DE_MASK    0x2u
#define  TMR2_0_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR2_0_DMA_CMPLD2DE_MASK    0x4u
#define  TMR2_0_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  TMR2_0_ENBL_ENBL_MASK       0x4u
#define  TMR2_0_ENBL_ENBL_SHIFT      0
#define  TMR2_0_ENBL_ENBL(x)         (((uint16_t)(((uint16_t)(x))<<TMR2_0_ENBL_ENBL_SHIFT))&TMR2_0_ENBL_ENBL_MASK)
//   Timer Channel Enable
//     0b0 ==> Timer channel is disabled.
//     0b1 ==> Timer channel is enabled. (default)

#define  TMR2_1_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR2_1_COMP1_COMPARISON_1_SHIFT 0
#define  TMR2_1_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR2_1_COMP1_COMPARISON_1_SHIFT))&TMR2_1_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR2_1_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR2_1_COMP2_COMPARISON_2_SHIFT 0
#define  TMR2_1_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR2_1_COMP2_COMPARISON_2_SHIFT))&TMR2_1_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR2_1_CAPT_CAPTURE_MASK    0x10u
#define  TMR2_1_CAPT_CAPTURE_SHIFT   0
#define  TMR2_1_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR2_1_CAPT_CAPTURE_SHIFT))&TMR2_1_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR2_1_LOAD_LOAD_MASK       0x10u
#define  TMR2_1_LOAD_LOAD_SHIFT      0
#define  TMR2_1_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR2_1_LOAD_LOAD_SHIFT))&TMR2_1_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR2_1_HOLD_HOLD_MASK       0x10u
#define  TMR2_1_HOLD_HOLD_SHIFT      0
#define  TMR2_1_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR2_1_HOLD_HOLD_SHIFT))&TMR2_1_HOLD_HOLD_MASK)

#define  TMR2_1_CNTR_COUNTER_MASK    0x10u
#define  TMR2_1_CNTR_COUNTER_SHIFT   0
#define  TMR2_1_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR2_1_CNTR_COUNTER_SHIFT))&TMR2_1_CNTR_COUNTER_MASK)

#define  TMR2_1_CTRL_OUTMODE_MASK    0x3u
#define  TMR2_1_CTRL_OUTMODE_SHIFT   0
#define  TMR2_1_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR2_1_CTRL_OUTMODE_SHIFT))&TMR2_1_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR2_1_CTRL_COINIT_MASK     0x8u
#define  TMR2_1_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR2_1_CTRL_DIR_MASK        0x10u
#define  TMR2_1_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR2_1_CTRL_LENGTH_MASK     0x20u
#define  TMR2_1_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR2_1_CTRL_ONCE_MASK       0x40u
#define  TMR2_1_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR2_1_CTRL_SCS_MASK        0x100u
#define  TMR2_1_CTRL_SCS_SHIFT       7
#define  TMR2_1_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR2_1_CTRL_SCS_SHIFT))&TMR2_1_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR2_1_CTRL_PCS_MASK        0x800u
#define  TMR2_1_CTRL_PCS_SHIFT       9
#define  TMR2_1_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR2_1_CTRL_PCS_SHIFT))&TMR2_1_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR2_1_CTRL_CM_MASK         0x6000u
#define  TMR2_1_CTRL_CM_SHIFT        13
#define  TMR2_1_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR2_1_CTRL_CM_SHIFT))&TMR2_1_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR2_1_SCTRL_OEN_MASK       0x1u
#define  TMR2_1_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR2_1_SCTRL_OPS_MASK       0x2u
#define  TMR2_1_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR2_1_SCTRL_FORCE_MASK     0x4u
#define  TMR2_1_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR2_1_SCTRL_VAL_MASK       0x8u
#define  TMR2_1_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR2_1_SCTRL_EEOF_MASK      0x10u
#define  TMR2_1_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR2_1_SCTRL_MSTR_MASK      0x20u
#define  TMR2_1_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR2_1_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR2_1_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR2_1_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR2_1_SCTRL_CAPTURE_MODE_SHIFT))&TMR2_1_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR2_1_SCTRL_INPUT_MASK     0x100u
#define  TMR2_1_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR2_1_SCTRL_IPS_MASK       0x200u
#define  TMR2_1_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR2_1_SCTRL_IEFIE_MASK     0x400u
#define  TMR2_1_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR2_1_SCTRL_IEF_MASK       0x800u
#define  TMR2_1_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR2_1_SCTRL_TOFIE_MASK     0x1000u
#define  TMR2_1_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR2_1_SCTRL_TOF_MASK       0x2000u
#define  TMR2_1_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR2_1_SCTRL_TCFIE_MASK     0x4000u
#define  TMR2_1_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR2_1_SCTRL_TCF_MASK       0x8000u
#define  TMR2_1_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR2_1_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR2_1_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR2_1_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR2_1_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR2_1_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR2_1_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR2_1_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR2_1_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR2_1_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR2_1_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR2_1_CSCTRL_CL1_MASK      0x2u
#define  TMR2_1_CSCTRL_CL1_SHIFT     0
#define  TMR2_1_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR2_1_CSCTRL_CL1_SHIFT))&TMR2_1_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR2_1_CSCTRL_CL2_MASK      0x8u
#define  TMR2_1_CSCTRL_CL2_SHIFT     2
#define  TMR2_1_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR2_1_CSCTRL_CL2_SHIFT))&TMR2_1_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR2_1_CSCTRL_TCF1_MASK     0x10u
#define  TMR2_1_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR2_1_CSCTRL_TCF2_MASK     0x20u
#define  TMR2_1_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR2_1_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR2_1_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR2_1_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR2_1_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR2_1_CSCTRL_UP_MASK       0x200u
#define  TMR2_1_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR2_1_CSCTRL_TCI_MASK      0x400u
#define  TMR2_1_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR2_1_CSCTRL_ROC_MASK      0x800u
#define  TMR2_1_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR2_1_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR2_1_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR2_1_CSCTRL_FAULT_MASK    0x2000u
#define  TMR2_1_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR2_1_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR2_1_CSCTRL_DBG_EN_SHIFT  14
#define  TMR2_1_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR2_1_CSCTRL_DBG_EN_SHIFT))&TMR2_1_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR2_1_FILT_FILT_PER_MASK   0x8u
#define  TMR2_1_FILT_FILT_PER_SHIFT  0
#define  TMR2_1_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR2_1_FILT_FILT_PER_SHIFT))&TMR2_1_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR2_1_FILT_FILT_CNT_MASK   0x300u
#define  TMR2_1_FILT_FILT_CNT_SHIFT  8
#define  TMR2_1_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR2_1_FILT_FILT_CNT_SHIFT))&TMR2_1_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR2_1_DMA_IEFDE_MASK       0x1u
#define  TMR2_1_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR2_1_DMA_CMPLD1DE_MASK    0x2u
#define  TMR2_1_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR2_1_DMA_CMPLD2DE_MASK    0x4u
#define  TMR2_1_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  TMR2_2_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR2_2_COMP1_COMPARISON_1_SHIFT 0
#define  TMR2_2_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR2_2_COMP1_COMPARISON_1_SHIFT))&TMR2_2_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR2_2_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR2_2_COMP2_COMPARISON_2_SHIFT 0
#define  TMR2_2_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR2_2_COMP2_COMPARISON_2_SHIFT))&TMR2_2_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR2_2_CAPT_CAPTURE_MASK    0x10u
#define  TMR2_2_CAPT_CAPTURE_SHIFT   0
#define  TMR2_2_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR2_2_CAPT_CAPTURE_SHIFT))&TMR2_2_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR2_2_LOAD_LOAD_MASK       0x10u
#define  TMR2_2_LOAD_LOAD_SHIFT      0
#define  TMR2_2_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR2_2_LOAD_LOAD_SHIFT))&TMR2_2_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR2_2_HOLD_HOLD_MASK       0x10u
#define  TMR2_2_HOLD_HOLD_SHIFT      0
#define  TMR2_2_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR2_2_HOLD_HOLD_SHIFT))&TMR2_2_HOLD_HOLD_MASK)

#define  TMR2_2_CNTR_COUNTER_MASK    0x10u
#define  TMR2_2_CNTR_COUNTER_SHIFT   0
#define  TMR2_2_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR2_2_CNTR_COUNTER_SHIFT))&TMR2_2_CNTR_COUNTER_MASK)

#define  TMR2_2_CTRL_OUTMODE_MASK    0x3u
#define  TMR2_2_CTRL_OUTMODE_SHIFT   0
#define  TMR2_2_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR2_2_CTRL_OUTMODE_SHIFT))&TMR2_2_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR2_2_CTRL_COINIT_MASK     0x8u
#define  TMR2_2_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR2_2_CTRL_DIR_MASK        0x10u
#define  TMR2_2_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR2_2_CTRL_LENGTH_MASK     0x20u
#define  TMR2_2_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR2_2_CTRL_ONCE_MASK       0x40u
#define  TMR2_2_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR2_2_CTRL_SCS_MASK        0x100u
#define  TMR2_2_CTRL_SCS_SHIFT       7
#define  TMR2_2_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR2_2_CTRL_SCS_SHIFT))&TMR2_2_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR2_2_CTRL_PCS_MASK        0x800u
#define  TMR2_2_CTRL_PCS_SHIFT       9
#define  TMR2_2_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR2_2_CTRL_PCS_SHIFT))&TMR2_2_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR2_2_CTRL_CM_MASK         0x6000u
#define  TMR2_2_CTRL_CM_SHIFT        13
#define  TMR2_2_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR2_2_CTRL_CM_SHIFT))&TMR2_2_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR2_2_SCTRL_OEN_MASK       0x1u
#define  TMR2_2_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR2_2_SCTRL_OPS_MASK       0x2u
#define  TMR2_2_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR2_2_SCTRL_FORCE_MASK     0x4u
#define  TMR2_2_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR2_2_SCTRL_VAL_MASK       0x8u
#define  TMR2_2_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR2_2_SCTRL_EEOF_MASK      0x10u
#define  TMR2_2_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR2_2_SCTRL_MSTR_MASK      0x20u
#define  TMR2_2_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR2_2_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR2_2_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR2_2_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR2_2_SCTRL_CAPTURE_MODE_SHIFT))&TMR2_2_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR2_2_SCTRL_INPUT_MASK     0x100u
#define  TMR2_2_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR2_2_SCTRL_IPS_MASK       0x200u
#define  TMR2_2_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR2_2_SCTRL_IEFIE_MASK     0x400u
#define  TMR2_2_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR2_2_SCTRL_IEF_MASK       0x800u
#define  TMR2_2_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR2_2_SCTRL_TOFIE_MASK     0x1000u
#define  TMR2_2_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR2_2_SCTRL_TOF_MASK       0x2000u
#define  TMR2_2_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR2_2_SCTRL_TCFIE_MASK     0x4000u
#define  TMR2_2_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR2_2_SCTRL_TCF_MASK       0x8000u
#define  TMR2_2_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR2_2_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR2_2_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR2_2_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR2_2_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR2_2_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR2_2_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR2_2_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR2_2_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR2_2_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR2_2_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR2_2_CSCTRL_CL1_MASK      0x2u
#define  TMR2_2_CSCTRL_CL1_SHIFT     0
#define  TMR2_2_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR2_2_CSCTRL_CL1_SHIFT))&TMR2_2_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR2_2_CSCTRL_CL2_MASK      0x8u
#define  TMR2_2_CSCTRL_CL2_SHIFT     2
#define  TMR2_2_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR2_2_CSCTRL_CL2_SHIFT))&TMR2_2_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR2_2_CSCTRL_TCF1_MASK     0x10u
#define  TMR2_2_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR2_2_CSCTRL_TCF2_MASK     0x20u
#define  TMR2_2_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR2_2_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR2_2_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR2_2_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR2_2_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR2_2_CSCTRL_UP_MASK       0x200u
#define  TMR2_2_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR2_2_CSCTRL_TCI_MASK      0x400u
#define  TMR2_2_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR2_2_CSCTRL_ROC_MASK      0x800u
#define  TMR2_2_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR2_2_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR2_2_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR2_2_CSCTRL_FAULT_MASK    0x2000u
#define  TMR2_2_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR2_2_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR2_2_CSCTRL_DBG_EN_SHIFT  14
#define  TMR2_2_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR2_2_CSCTRL_DBG_EN_SHIFT))&TMR2_2_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR2_2_FILT_FILT_PER_MASK   0x8u
#define  TMR2_2_FILT_FILT_PER_SHIFT  0
#define  TMR2_2_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR2_2_FILT_FILT_PER_SHIFT))&TMR2_2_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR2_2_FILT_FILT_CNT_MASK   0x300u
#define  TMR2_2_FILT_FILT_CNT_SHIFT  8
#define  TMR2_2_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR2_2_FILT_FILT_CNT_SHIFT))&TMR2_2_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR2_2_DMA_IEFDE_MASK       0x1u
#define  TMR2_2_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR2_2_DMA_CMPLD1DE_MASK    0x2u
#define  TMR2_2_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR2_2_DMA_CMPLD2DE_MASK    0x4u
#define  TMR2_2_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  TMR2_3_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR2_3_COMP1_COMPARISON_1_SHIFT 0
#define  TMR2_3_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR2_3_COMP1_COMPARISON_1_SHIFT))&TMR2_3_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR2_3_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR2_3_COMP2_COMPARISON_2_SHIFT 0
#define  TMR2_3_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR2_3_COMP2_COMPARISON_2_SHIFT))&TMR2_3_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR2_3_CAPT_CAPTURE_MASK    0x10u
#define  TMR2_3_CAPT_CAPTURE_SHIFT   0
#define  TMR2_3_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR2_3_CAPT_CAPTURE_SHIFT))&TMR2_3_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR2_3_LOAD_LOAD_MASK       0x10u
#define  TMR2_3_LOAD_LOAD_SHIFT      0
#define  TMR2_3_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR2_3_LOAD_LOAD_SHIFT))&TMR2_3_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR2_3_HOLD_HOLD_MASK       0x10u
#define  TMR2_3_HOLD_HOLD_SHIFT      0
#define  TMR2_3_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR2_3_HOLD_HOLD_SHIFT))&TMR2_3_HOLD_HOLD_MASK)

#define  TMR2_3_CNTR_COUNTER_MASK    0x10u
#define  TMR2_3_CNTR_COUNTER_SHIFT   0
#define  TMR2_3_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR2_3_CNTR_COUNTER_SHIFT))&TMR2_3_CNTR_COUNTER_MASK)

#define  TMR2_3_CTRL_OUTMODE_MASK    0x3u
#define  TMR2_3_CTRL_OUTMODE_SHIFT   0
#define  TMR2_3_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR2_3_CTRL_OUTMODE_SHIFT))&TMR2_3_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR2_3_CTRL_COINIT_MASK     0x8u
#define  TMR2_3_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR2_3_CTRL_DIR_MASK        0x10u
#define  TMR2_3_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR2_3_CTRL_LENGTH_MASK     0x20u
#define  TMR2_3_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR2_3_CTRL_ONCE_MASK       0x40u
#define  TMR2_3_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR2_3_CTRL_SCS_MASK        0x100u
#define  TMR2_3_CTRL_SCS_SHIFT       7
#define  TMR2_3_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR2_3_CTRL_SCS_SHIFT))&TMR2_3_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR2_3_CTRL_PCS_MASK        0x800u
#define  TMR2_3_CTRL_PCS_SHIFT       9
#define  TMR2_3_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR2_3_CTRL_PCS_SHIFT))&TMR2_3_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR2_3_CTRL_CM_MASK         0x6000u
#define  TMR2_3_CTRL_CM_SHIFT        13
#define  TMR2_3_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR2_3_CTRL_CM_SHIFT))&TMR2_3_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR2_3_SCTRL_OEN_MASK       0x1u
#define  TMR2_3_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR2_3_SCTRL_OPS_MASK       0x2u
#define  TMR2_3_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR2_3_SCTRL_FORCE_MASK     0x4u
#define  TMR2_3_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR2_3_SCTRL_VAL_MASK       0x8u
#define  TMR2_3_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR2_3_SCTRL_EEOF_MASK      0x10u
#define  TMR2_3_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR2_3_SCTRL_MSTR_MASK      0x20u
#define  TMR2_3_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR2_3_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR2_3_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR2_3_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR2_3_SCTRL_CAPTURE_MODE_SHIFT))&TMR2_3_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR2_3_SCTRL_INPUT_MASK     0x100u
#define  TMR2_3_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR2_3_SCTRL_IPS_MASK       0x200u
#define  TMR2_3_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR2_3_SCTRL_IEFIE_MASK     0x400u
#define  TMR2_3_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR2_3_SCTRL_IEF_MASK       0x800u
#define  TMR2_3_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR2_3_SCTRL_TOFIE_MASK     0x1000u
#define  TMR2_3_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR2_3_SCTRL_TOF_MASK       0x2000u
#define  TMR2_3_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR2_3_SCTRL_TCFIE_MASK     0x4000u
#define  TMR2_3_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR2_3_SCTRL_TCF_MASK       0x8000u
#define  TMR2_3_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR2_3_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR2_3_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR2_3_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR2_3_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR2_3_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR2_3_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR2_3_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR2_3_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR2_3_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR2_3_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR2_3_CSCTRL_CL1_MASK      0x2u
#define  TMR2_3_CSCTRL_CL1_SHIFT     0
#define  TMR2_3_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR2_3_CSCTRL_CL1_SHIFT))&TMR2_3_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR2_3_CSCTRL_CL2_MASK      0x8u
#define  TMR2_3_CSCTRL_CL2_SHIFT     2
#define  TMR2_3_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR2_3_CSCTRL_CL2_SHIFT))&TMR2_3_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR2_3_CSCTRL_TCF1_MASK     0x10u
#define  TMR2_3_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR2_3_CSCTRL_TCF2_MASK     0x20u
#define  TMR2_3_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR2_3_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR2_3_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR2_3_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR2_3_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR2_3_CSCTRL_UP_MASK       0x200u
#define  TMR2_3_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR2_3_CSCTRL_TCI_MASK      0x400u
#define  TMR2_3_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR2_3_CSCTRL_ROC_MASK      0x800u
#define  TMR2_3_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR2_3_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR2_3_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR2_3_CSCTRL_FAULT_MASK    0x2000u
#define  TMR2_3_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR2_3_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR2_3_CSCTRL_DBG_EN_SHIFT  14
#define  TMR2_3_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR2_3_CSCTRL_DBG_EN_SHIFT))&TMR2_3_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR2_3_FILT_FILT_PER_MASK   0x8u
#define  TMR2_3_FILT_FILT_PER_SHIFT  0
#define  TMR2_3_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR2_3_FILT_FILT_PER_SHIFT))&TMR2_3_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR2_3_FILT_FILT_CNT_MASK   0x300u
#define  TMR2_3_FILT_FILT_CNT_SHIFT  8
#define  TMR2_3_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR2_3_FILT_FILT_CNT_SHIFT))&TMR2_3_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR2_3_DMA_IEFDE_MASK       0x1u
#define  TMR2_3_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR2_3_DMA_CMPLD1DE_MASK    0x2u
#define  TMR2_3_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR2_3_DMA_CMPLD2DE_MASK    0x4u
#define  TMR2_3_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  TMR3_0_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR3_0_COMP1_COMPARISON_1_SHIFT 0
#define  TMR3_0_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR3_0_COMP1_COMPARISON_1_SHIFT))&TMR3_0_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR3_0_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR3_0_COMP2_COMPARISON_2_SHIFT 0
#define  TMR3_0_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR3_0_COMP2_COMPARISON_2_SHIFT))&TMR3_0_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR3_0_CAPT_CAPTURE_MASK    0x10u
#define  TMR3_0_CAPT_CAPTURE_SHIFT   0
#define  TMR3_0_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR3_0_CAPT_CAPTURE_SHIFT))&TMR3_0_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR3_0_LOAD_LOAD_MASK       0x10u
#define  TMR3_0_LOAD_LOAD_SHIFT      0
#define  TMR3_0_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR3_0_LOAD_LOAD_SHIFT))&TMR3_0_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR3_0_HOLD_HOLD_MASK       0x10u
#define  TMR3_0_HOLD_HOLD_SHIFT      0
#define  TMR3_0_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR3_0_HOLD_HOLD_SHIFT))&TMR3_0_HOLD_HOLD_MASK)

#define  TMR3_0_CNTR_COUNTER_MASK    0x10u
#define  TMR3_0_CNTR_COUNTER_SHIFT   0
#define  TMR3_0_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR3_0_CNTR_COUNTER_SHIFT))&TMR3_0_CNTR_COUNTER_MASK)

#define  TMR3_0_CTRL_OUTMODE_MASK    0x3u
#define  TMR3_0_CTRL_OUTMODE_SHIFT   0
#define  TMR3_0_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR3_0_CTRL_OUTMODE_SHIFT))&TMR3_0_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR3_0_CTRL_COINIT_MASK     0x8u
#define  TMR3_0_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR3_0_CTRL_DIR_MASK        0x10u
#define  TMR3_0_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR3_0_CTRL_LENGTH_MASK     0x20u
#define  TMR3_0_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR3_0_CTRL_ONCE_MASK       0x40u
#define  TMR3_0_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR3_0_CTRL_SCS_MASK        0x100u
#define  TMR3_0_CTRL_SCS_SHIFT       7
#define  TMR3_0_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR3_0_CTRL_SCS_SHIFT))&TMR3_0_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR3_0_CTRL_PCS_MASK        0x800u
#define  TMR3_0_CTRL_PCS_SHIFT       9
#define  TMR3_0_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR3_0_CTRL_PCS_SHIFT))&TMR3_0_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR3_0_CTRL_CM_MASK         0x6000u
#define  TMR3_0_CTRL_CM_SHIFT        13
#define  TMR3_0_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR3_0_CTRL_CM_SHIFT))&TMR3_0_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR3_0_SCTRL_OEN_MASK       0x1u
#define  TMR3_0_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR3_0_SCTRL_OPS_MASK       0x2u
#define  TMR3_0_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR3_0_SCTRL_FORCE_MASK     0x4u
#define  TMR3_0_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR3_0_SCTRL_VAL_MASK       0x8u
#define  TMR3_0_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR3_0_SCTRL_EEOF_MASK      0x10u
#define  TMR3_0_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR3_0_SCTRL_MSTR_MASK      0x20u
#define  TMR3_0_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR3_0_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR3_0_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR3_0_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR3_0_SCTRL_CAPTURE_MODE_SHIFT))&TMR3_0_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR3_0_SCTRL_INPUT_MASK     0x100u
#define  TMR3_0_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR3_0_SCTRL_IPS_MASK       0x200u
#define  TMR3_0_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR3_0_SCTRL_IEFIE_MASK     0x400u
#define  TMR3_0_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR3_0_SCTRL_IEF_MASK       0x800u
#define  TMR3_0_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR3_0_SCTRL_TOFIE_MASK     0x1000u
#define  TMR3_0_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR3_0_SCTRL_TOF_MASK       0x2000u
#define  TMR3_0_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR3_0_SCTRL_TCFIE_MASK     0x4000u
#define  TMR3_0_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR3_0_SCTRL_TCF_MASK       0x8000u
#define  TMR3_0_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR3_0_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR3_0_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR3_0_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR3_0_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR3_0_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR3_0_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR3_0_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR3_0_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR3_0_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR3_0_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR3_0_CSCTRL_CL1_MASK      0x2u
#define  TMR3_0_CSCTRL_CL1_SHIFT     0
#define  TMR3_0_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR3_0_CSCTRL_CL1_SHIFT))&TMR3_0_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR3_0_CSCTRL_CL2_MASK      0x8u
#define  TMR3_0_CSCTRL_CL2_SHIFT     2
#define  TMR3_0_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR3_0_CSCTRL_CL2_SHIFT))&TMR3_0_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR3_0_CSCTRL_TCF1_MASK     0x10u
#define  TMR3_0_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR3_0_CSCTRL_TCF2_MASK     0x20u
#define  TMR3_0_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR3_0_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR3_0_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR3_0_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR3_0_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR3_0_CSCTRL_UP_MASK       0x200u
#define  TMR3_0_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR3_0_CSCTRL_TCI_MASK      0x400u
#define  TMR3_0_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR3_0_CSCTRL_ROC_MASK      0x800u
#define  TMR3_0_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR3_0_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR3_0_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR3_0_CSCTRL_FAULT_MASK    0x2000u
#define  TMR3_0_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR3_0_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR3_0_CSCTRL_DBG_EN_SHIFT  14
#define  TMR3_0_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR3_0_CSCTRL_DBG_EN_SHIFT))&TMR3_0_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR3_0_FILT_FILT_PER_MASK   0x8u
#define  TMR3_0_FILT_FILT_PER_SHIFT  0
#define  TMR3_0_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR3_0_FILT_FILT_PER_SHIFT))&TMR3_0_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR3_0_FILT_FILT_CNT_MASK   0x300u
#define  TMR3_0_FILT_FILT_CNT_SHIFT  8
#define  TMR3_0_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR3_0_FILT_FILT_CNT_SHIFT))&TMR3_0_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR3_0_DMA_IEFDE_MASK       0x1u
#define  TMR3_0_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR3_0_DMA_CMPLD1DE_MASK    0x2u
#define  TMR3_0_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR3_0_DMA_CMPLD2DE_MASK    0x4u
#define  TMR3_0_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  TMR3_0_ENBL_ENBL_MASK       0x4u
#define  TMR3_0_ENBL_ENBL_SHIFT      0
#define  TMR3_0_ENBL_ENBL(x)         (((uint16_t)(((uint16_t)(x))<<TMR3_0_ENBL_ENBL_SHIFT))&TMR3_0_ENBL_ENBL_MASK)
//   Timer Channel Enable
//     0b0 ==> Timer channel is disabled.
//     0b1 ==> Timer channel is enabled. (default)

#define  TMR3_1_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR3_1_COMP1_COMPARISON_1_SHIFT 0
#define  TMR3_1_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR3_1_COMP1_COMPARISON_1_SHIFT))&TMR3_1_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR3_1_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR3_1_COMP2_COMPARISON_2_SHIFT 0
#define  TMR3_1_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR3_1_COMP2_COMPARISON_2_SHIFT))&TMR3_1_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR3_1_CAPT_CAPTURE_MASK    0x10u
#define  TMR3_1_CAPT_CAPTURE_SHIFT   0
#define  TMR3_1_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR3_1_CAPT_CAPTURE_SHIFT))&TMR3_1_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR3_1_LOAD_LOAD_MASK       0x10u
#define  TMR3_1_LOAD_LOAD_SHIFT      0
#define  TMR3_1_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR3_1_LOAD_LOAD_SHIFT))&TMR3_1_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR3_1_HOLD_HOLD_MASK       0x10u
#define  TMR3_1_HOLD_HOLD_SHIFT      0
#define  TMR3_1_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR3_1_HOLD_HOLD_SHIFT))&TMR3_1_HOLD_HOLD_MASK)

#define  TMR3_1_CNTR_COUNTER_MASK    0x10u
#define  TMR3_1_CNTR_COUNTER_SHIFT   0
#define  TMR3_1_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR3_1_CNTR_COUNTER_SHIFT))&TMR3_1_CNTR_COUNTER_MASK)

#define  TMR3_1_CTRL_OUTMODE_MASK    0x3u
#define  TMR3_1_CTRL_OUTMODE_SHIFT   0
#define  TMR3_1_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR3_1_CTRL_OUTMODE_SHIFT))&TMR3_1_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR3_1_CTRL_COINIT_MASK     0x8u
#define  TMR3_1_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR3_1_CTRL_DIR_MASK        0x10u
#define  TMR3_1_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR3_1_CTRL_LENGTH_MASK     0x20u
#define  TMR3_1_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR3_1_CTRL_ONCE_MASK       0x40u
#define  TMR3_1_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR3_1_CTRL_SCS_MASK        0x100u
#define  TMR3_1_CTRL_SCS_SHIFT       7
#define  TMR3_1_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR3_1_CTRL_SCS_SHIFT))&TMR3_1_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR3_1_CTRL_PCS_MASK        0x800u
#define  TMR3_1_CTRL_PCS_SHIFT       9
#define  TMR3_1_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR3_1_CTRL_PCS_SHIFT))&TMR3_1_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR3_1_CTRL_CM_MASK         0x6000u
#define  TMR3_1_CTRL_CM_SHIFT        13
#define  TMR3_1_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR3_1_CTRL_CM_SHIFT))&TMR3_1_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR3_1_SCTRL_OEN_MASK       0x1u
#define  TMR3_1_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR3_1_SCTRL_OPS_MASK       0x2u
#define  TMR3_1_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR3_1_SCTRL_FORCE_MASK     0x4u
#define  TMR3_1_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR3_1_SCTRL_VAL_MASK       0x8u
#define  TMR3_1_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR3_1_SCTRL_EEOF_MASK      0x10u
#define  TMR3_1_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR3_1_SCTRL_MSTR_MASK      0x20u
#define  TMR3_1_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR3_1_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR3_1_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR3_1_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR3_1_SCTRL_CAPTURE_MODE_SHIFT))&TMR3_1_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR3_1_SCTRL_INPUT_MASK     0x100u
#define  TMR3_1_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR3_1_SCTRL_IPS_MASK       0x200u
#define  TMR3_1_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR3_1_SCTRL_IEFIE_MASK     0x400u
#define  TMR3_1_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR3_1_SCTRL_IEF_MASK       0x800u
#define  TMR3_1_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR3_1_SCTRL_TOFIE_MASK     0x1000u
#define  TMR3_1_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR3_1_SCTRL_TOF_MASK       0x2000u
#define  TMR3_1_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR3_1_SCTRL_TCFIE_MASK     0x4000u
#define  TMR3_1_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR3_1_SCTRL_TCF_MASK       0x8000u
#define  TMR3_1_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR3_1_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR3_1_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR3_1_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR3_1_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR3_1_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR3_1_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR3_1_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR3_1_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR3_1_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR3_1_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR3_1_CSCTRL_CL1_MASK      0x2u
#define  TMR3_1_CSCTRL_CL1_SHIFT     0
#define  TMR3_1_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR3_1_CSCTRL_CL1_SHIFT))&TMR3_1_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR3_1_CSCTRL_CL2_MASK      0x8u
#define  TMR3_1_CSCTRL_CL2_SHIFT     2
#define  TMR3_1_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR3_1_CSCTRL_CL2_SHIFT))&TMR3_1_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR3_1_CSCTRL_TCF1_MASK     0x10u
#define  TMR3_1_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR3_1_CSCTRL_TCF2_MASK     0x20u
#define  TMR3_1_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR3_1_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR3_1_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR3_1_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR3_1_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR3_1_CSCTRL_UP_MASK       0x200u
#define  TMR3_1_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR3_1_CSCTRL_TCI_MASK      0x400u
#define  TMR3_1_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR3_1_CSCTRL_ROC_MASK      0x800u
#define  TMR3_1_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR3_1_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR3_1_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR3_1_CSCTRL_FAULT_MASK    0x2000u
#define  TMR3_1_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR3_1_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR3_1_CSCTRL_DBG_EN_SHIFT  14
#define  TMR3_1_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR3_1_CSCTRL_DBG_EN_SHIFT))&TMR3_1_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR3_1_FILT_FILT_PER_MASK   0x8u
#define  TMR3_1_FILT_FILT_PER_SHIFT  0
#define  TMR3_1_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR3_1_FILT_FILT_PER_SHIFT))&TMR3_1_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR3_1_FILT_FILT_CNT_MASK   0x300u
#define  TMR3_1_FILT_FILT_CNT_SHIFT  8
#define  TMR3_1_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR3_1_FILT_FILT_CNT_SHIFT))&TMR3_1_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR3_1_DMA_IEFDE_MASK       0x1u
#define  TMR3_1_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR3_1_DMA_CMPLD1DE_MASK    0x2u
#define  TMR3_1_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR3_1_DMA_CMPLD2DE_MASK    0x4u
#define  TMR3_1_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  TMR3_2_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR3_2_COMP1_COMPARISON_1_SHIFT 0
#define  TMR3_2_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR3_2_COMP1_COMPARISON_1_SHIFT))&TMR3_2_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR3_2_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR3_2_COMP2_COMPARISON_2_SHIFT 0
#define  TMR3_2_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR3_2_COMP2_COMPARISON_2_SHIFT))&TMR3_2_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR3_2_CAPT_CAPTURE_MASK    0x10u
#define  TMR3_2_CAPT_CAPTURE_SHIFT   0
#define  TMR3_2_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR3_2_CAPT_CAPTURE_SHIFT))&TMR3_2_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR3_2_LOAD_LOAD_MASK       0x10u
#define  TMR3_2_LOAD_LOAD_SHIFT      0
#define  TMR3_2_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR3_2_LOAD_LOAD_SHIFT))&TMR3_2_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR3_2_HOLD_HOLD_MASK       0x10u
#define  TMR3_2_HOLD_HOLD_SHIFT      0
#define  TMR3_2_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR3_2_HOLD_HOLD_SHIFT))&TMR3_2_HOLD_HOLD_MASK)

#define  TMR3_2_CNTR_COUNTER_MASK    0x10u
#define  TMR3_2_CNTR_COUNTER_SHIFT   0
#define  TMR3_2_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR3_2_CNTR_COUNTER_SHIFT))&TMR3_2_CNTR_COUNTER_MASK)

#define  TMR3_2_CTRL_OUTMODE_MASK    0x3u
#define  TMR3_2_CTRL_OUTMODE_SHIFT   0
#define  TMR3_2_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR3_2_CTRL_OUTMODE_SHIFT))&TMR3_2_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR3_2_CTRL_COINIT_MASK     0x8u
#define  TMR3_2_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR3_2_CTRL_DIR_MASK        0x10u
#define  TMR3_2_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR3_2_CTRL_LENGTH_MASK     0x20u
#define  TMR3_2_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR3_2_CTRL_ONCE_MASK       0x40u
#define  TMR3_2_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR3_2_CTRL_SCS_MASK        0x100u
#define  TMR3_2_CTRL_SCS_SHIFT       7
#define  TMR3_2_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR3_2_CTRL_SCS_SHIFT))&TMR3_2_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR3_2_CTRL_PCS_MASK        0x800u
#define  TMR3_2_CTRL_PCS_SHIFT       9
#define  TMR3_2_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR3_2_CTRL_PCS_SHIFT))&TMR3_2_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR3_2_CTRL_CM_MASK         0x6000u
#define  TMR3_2_CTRL_CM_SHIFT        13
#define  TMR3_2_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR3_2_CTRL_CM_SHIFT))&TMR3_2_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR3_2_SCTRL_OEN_MASK       0x1u
#define  TMR3_2_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR3_2_SCTRL_OPS_MASK       0x2u
#define  TMR3_2_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR3_2_SCTRL_FORCE_MASK     0x4u
#define  TMR3_2_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR3_2_SCTRL_VAL_MASK       0x8u
#define  TMR3_2_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR3_2_SCTRL_EEOF_MASK      0x10u
#define  TMR3_2_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR3_2_SCTRL_MSTR_MASK      0x20u
#define  TMR3_2_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR3_2_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR3_2_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR3_2_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR3_2_SCTRL_CAPTURE_MODE_SHIFT))&TMR3_2_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR3_2_SCTRL_INPUT_MASK     0x100u
#define  TMR3_2_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR3_2_SCTRL_IPS_MASK       0x200u
#define  TMR3_2_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR3_2_SCTRL_IEFIE_MASK     0x400u
#define  TMR3_2_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR3_2_SCTRL_IEF_MASK       0x800u
#define  TMR3_2_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR3_2_SCTRL_TOFIE_MASK     0x1000u
#define  TMR3_2_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR3_2_SCTRL_TOF_MASK       0x2000u
#define  TMR3_2_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR3_2_SCTRL_TCFIE_MASK     0x4000u
#define  TMR3_2_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR3_2_SCTRL_TCF_MASK       0x8000u
#define  TMR3_2_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR3_2_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR3_2_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR3_2_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR3_2_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR3_2_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR3_2_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR3_2_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR3_2_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR3_2_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR3_2_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR3_2_CSCTRL_CL1_MASK      0x2u
#define  TMR3_2_CSCTRL_CL1_SHIFT     0
#define  TMR3_2_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR3_2_CSCTRL_CL1_SHIFT))&TMR3_2_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR3_2_CSCTRL_CL2_MASK      0x8u
#define  TMR3_2_CSCTRL_CL2_SHIFT     2
#define  TMR3_2_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR3_2_CSCTRL_CL2_SHIFT))&TMR3_2_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR3_2_CSCTRL_TCF1_MASK     0x10u
#define  TMR3_2_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR3_2_CSCTRL_TCF2_MASK     0x20u
#define  TMR3_2_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR3_2_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR3_2_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR3_2_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR3_2_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR3_2_CSCTRL_UP_MASK       0x200u
#define  TMR3_2_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR3_2_CSCTRL_TCI_MASK      0x400u
#define  TMR3_2_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR3_2_CSCTRL_ROC_MASK      0x800u
#define  TMR3_2_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR3_2_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR3_2_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR3_2_CSCTRL_FAULT_MASK    0x2000u
#define  TMR3_2_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR3_2_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR3_2_CSCTRL_DBG_EN_SHIFT  14
#define  TMR3_2_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR3_2_CSCTRL_DBG_EN_SHIFT))&TMR3_2_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR3_2_FILT_FILT_PER_MASK   0x8u
#define  TMR3_2_FILT_FILT_PER_SHIFT  0
#define  TMR3_2_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR3_2_FILT_FILT_PER_SHIFT))&TMR3_2_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR3_2_FILT_FILT_CNT_MASK   0x300u
#define  TMR3_2_FILT_FILT_CNT_SHIFT  8
#define  TMR3_2_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR3_2_FILT_FILT_CNT_SHIFT))&TMR3_2_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR3_2_DMA_IEFDE_MASK       0x1u
#define  TMR3_2_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR3_2_DMA_CMPLD1DE_MASK    0x2u
#define  TMR3_2_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR3_2_DMA_CMPLD2DE_MASK    0x4u
#define  TMR3_2_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  TMR3_3_COMP1_COMPARISON_1_MASK 0x10u
#define  TMR3_3_COMP1_COMPARISON_1_SHIFT 0
#define  TMR3_3_COMP1_COMPARISON_1(x) (((uint16_t)(((uint16_t)(x))<<TMR3_3_COMP1_COMPARISON_1_SHIFT))&TMR3_3_COMP1_COMPARISON_1_MASK)
//   Comparison Value 1

#define  TMR3_3_COMP2_COMPARISON_2_MASK 0x10u
#define  TMR3_3_COMP2_COMPARISON_2_SHIFT 0
#define  TMR3_3_COMP2_COMPARISON_2(x) (((uint16_t)(((uint16_t)(x))<<TMR3_3_COMP2_COMPARISON_2_SHIFT))&TMR3_3_COMP2_COMPARISON_2_MASK)
//   Comparison Value 2

#define  TMR3_3_CAPT_CAPTURE_MASK    0x10u
#define  TMR3_3_CAPT_CAPTURE_SHIFT   0
#define  TMR3_3_CAPT_CAPTURE(x)      (((uint16_t)(((uint16_t)(x))<<TMR3_3_CAPT_CAPTURE_SHIFT))&TMR3_3_CAPT_CAPTURE_MASK)
//   Capture Value

#define  TMR3_3_LOAD_LOAD_MASK       0x10u
#define  TMR3_3_LOAD_LOAD_SHIFT      0
#define  TMR3_3_LOAD_LOAD(x)         (((uint16_t)(((uint16_t)(x))<<TMR3_3_LOAD_LOAD_SHIFT))&TMR3_3_LOAD_LOAD_MASK)
//   Timer Load Register

#define  TMR3_3_HOLD_HOLD_MASK       0x10u
#define  TMR3_3_HOLD_HOLD_SHIFT      0
#define  TMR3_3_HOLD_HOLD(x)         (((uint16_t)(((uint16_t)(x))<<TMR3_3_HOLD_HOLD_SHIFT))&TMR3_3_HOLD_HOLD_MASK)

#define  TMR3_3_CNTR_COUNTER_MASK    0x10u
#define  TMR3_3_CNTR_COUNTER_SHIFT   0
#define  TMR3_3_CNTR_COUNTER(x)      (((uint16_t)(((uint16_t)(x))<<TMR3_3_CNTR_COUNTER_SHIFT))&TMR3_3_CNTR_COUNTER_MASK)

#define  TMR3_3_CTRL_OUTMODE_MASK    0x3u
#define  TMR3_3_CTRL_OUTMODE_SHIFT   0
#define  TMR3_3_CTRL_OUTMODE(x)      (((uint16_t)(((uint16_t)(x))<<TMR3_3_CTRL_OUTMODE_SHIFT))&TMR3_3_CTRL_OUTMODE_MASK)
//   Output Mode
//     0b000 ==> Asserted while counter is active
//     0b001 ==> Clear OFLAG output on successful compare
//     0b010 ==> Set OFLAG output on successful compare
//     0b011 ==> Toggle OFLAG output on successful compare
//     0b100 ==> Toggle OFLAG output using alternating compare registers
//     0b101 ==> Set on compare, cleared on secondary source input edge
//     0b110 ==> Set on compare, cleared on counter rollover
//     0b111 ==> Enable gated clock output while counter is active

#define  TMR3_3_CTRL_COINIT_MASK     0x8u
#define  TMR3_3_CTRL_COINIT_SHIFT    3
//   Co-Channel Initialization
//     0b0 ==> Co-channel counter/timers cannot force a re-initialization of this counter/timer
//     0b1 ==> Co-channel counter/timers may force a re-initialization of this counter/timer

#define  TMR3_3_CTRL_DIR_MASK        0x10u
#define  TMR3_3_CTRL_DIR_SHIFT       4
//   Count Direction
//     0b0 ==> Count up.
//     0b1 ==> Count down.

#define  TMR3_3_CTRL_LENGTH_MASK     0x20u
#define  TMR3_3_CTRL_LENGTH_SHIFT    5
//   Count Length
//     0b0 ==> Roll over.
//     0b1 ==> Count until compare, then re-initialize. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, alternating values of COMP1 and COMP2 are used to generate successful comparisons. For example, the counter counts until a COMP1 value is reached, re-initializes, counts until COMP2 value is reached, re-initializes, counts until COMP1 value
//             is reached, and so on.

#define  TMR3_3_CTRL_ONCE_MASK       0x40u
#define  TMR3_3_CTRL_ONCE_SHIFT      6
//   Count Once
//     0b0 ==> Count repeatedly.
//     0b1 ==> Count until compare and then stop. If counting up, a successful compare occurs when the counter reaches a COMP1 value. If counting down, a successful compare occurs when the counter reaches a COMP2 value. When output mode $4 is used, the counter re-initializes after reaching the COMP1 value, continues to count to the COMP2 value, and then stops.

#define  TMR3_3_CTRL_SCS_MASK        0x100u
#define  TMR3_3_CTRL_SCS_SHIFT       7
#define  TMR3_3_CTRL_SCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR3_3_CTRL_SCS_SHIFT))&TMR3_3_CTRL_SCS_MASK)
//   Secondary Count Source
//     0b00 ==> Counter 0 input pin
//     0b01 ==> Counter 1 input pin
//     0b10 ==> Counter 2 input pin
//     0b11 ==> Counter 3 input pin

#define  TMR3_3_CTRL_PCS_MASK        0x800u
#define  TMR3_3_CTRL_PCS_SHIFT       9
#define  TMR3_3_CTRL_PCS(x)          (((uint16_t)(((uint16_t)(x))<<TMR3_3_CTRL_PCS_SHIFT))&TMR3_3_CTRL_PCS_MASK)
//   Primary Count Source
//     0b0000 ==> Counter 0 input pin
//     0b0001 ==> Counter 1 input pin
//     0b0010 ==> Counter 2 input pin
//     0b0011 ==> Counter 3 input pin
//     0b0100 ==> Counter 0 output
//     0b0101 ==> Counter 1 output
//     0b0110 ==> Counter 2 output
//     0b0111 ==> Counter 3 output
//     0b1000 ==> IP bus clock divide by 1 prescaler
//     0b1001 ==> IP bus clock divide by 2 prescaler
//     0b1010 ==> IP bus clock divide by 4 prescaler
//     0b1011 ==> IP bus clock divide by 8 prescaler
//     0b1100 ==> IP bus clock divide by 16 prescaler
//     0b1101 ==> IP bus clock divide by 32 prescaler
//     0b1110 ==> IP bus clock divide by 64 prescaler
//     0b1111 ==> IP bus clock divide by 128 prescaler

#define  TMR3_3_CTRL_CM_MASK         0x6000u
#define  TMR3_3_CTRL_CM_SHIFT        13
#define  TMR3_3_CTRL_CM(x)           (((uint16_t)(((uint16_t)(x))<<TMR3_3_CTRL_CM_SHIFT))&TMR3_3_CTRL_CM_MASK)
//   Count Mode
//     0b000 ==> No operation
//     0b001 ==> Count rising edges of primary sourceRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1. If the primary count source is IP bus clock divide by 1, only rising edges are counted regardless of the value of SCTRL[IPS].
//     0b010 ==> Count rising and falling edges of primary sourceIP bus clock divide by 1 cannot be used as a primary count source in edge count mode.
//     0b011 ==> Count rising edges of primary source while secondary input high active
//     0b100 ==> Quadrature count mode, uses primary and secondary sources
//     0b101 ==> Count rising edges of primary source; secondary source specifies directionRising edges are counted only when SCTRL[IPS] = 0. Falling edges are counted when SCTRL[IPS] = 1.
//     0b110 ==> Edge of secondary source triggers primary count until compare
//     0b111 ==> Cascaded counter mode (up/down)The primary count source must be set to one of the counter outputs.

#define  TMR3_3_SCTRL_OEN_MASK       0x1u
#define  TMR3_3_SCTRL_OEN_SHIFT      0
//   Output Enable
//     0b0 ==> The external pin is configured as an input.
//     0b1 ==> The OFLAG output signal is driven on the external pin. Other timer groups using this external pin as their input see the driven value. The polarity of the signal is determined by OPS.

#define  TMR3_3_SCTRL_OPS_MASK       0x2u
#define  TMR3_3_SCTRL_OPS_SHIFT      1
//   Output Polarity Select
//     0b0 ==> True polarity.
//     0b1 ==> Inverted polarity.

#define  TMR3_3_SCTRL_FORCE_MASK     0x4u
#define  TMR3_3_SCTRL_FORCE_SHIFT    2
//   Force OFLAG Output

#define  TMR3_3_SCTRL_VAL_MASK       0x8u
#define  TMR3_3_SCTRL_VAL_SHIFT      3
//   Forced OFLAG Value

#define  TMR3_3_SCTRL_EEOF_MASK      0x10u
#define  TMR3_3_SCTRL_EEOF_SHIFT     4
//   Enable External OFLAG Force

#define  TMR3_3_SCTRL_MSTR_MASK      0x20u
#define  TMR3_3_SCTRL_MSTR_SHIFT     5
//   Master Mode

#define  TMR3_3_SCTRL_CAPTURE_MODE_MASK 0x80u
#define  TMR3_3_SCTRL_CAPTURE_MODE_SHIFT 6
#define  TMR3_3_SCTRL_CAPTURE_MODE(x) (((uint16_t)(((uint16_t)(x))<<TMR3_3_SCTRL_CAPTURE_MODE_SHIFT))&TMR3_3_SCTRL_CAPTURE_MODE_MASK)
//   Input Capture Mode
//     0b00 ==> Capture function is disabled
//     0b01 ==> Load capture register on rising edge (when IPS=0) or falling edge (when IPS=1) of input
//     0b10 ==> Load capture register on falling edge (when IPS=0) or rising edge (when IPS=1) of input
//     0b11 ==> Load capture register on both edges of input

#define  TMR3_3_SCTRL_INPUT_MASK     0x100u
#define  TMR3_3_SCTRL_INPUT_SHIFT    8
//   External Input Signal

#define  TMR3_3_SCTRL_IPS_MASK       0x200u
#define  TMR3_3_SCTRL_IPS_SHIFT      9
//   Input Polarity Select

#define  TMR3_3_SCTRL_IEFIE_MASK     0x400u
#define  TMR3_3_SCTRL_IEFIE_SHIFT    10
//   Input Edge Flag Interrupt Enable

#define  TMR3_3_SCTRL_IEF_MASK       0x800u
#define  TMR3_3_SCTRL_IEF_SHIFT      11
//   Input Edge Flag

#define  TMR3_3_SCTRL_TOFIE_MASK     0x1000u
#define  TMR3_3_SCTRL_TOFIE_SHIFT    12
//   Timer Overflow Flag Interrupt Enable

#define  TMR3_3_SCTRL_TOF_MASK       0x2000u
#define  TMR3_3_SCTRL_TOF_SHIFT      13
//   Timer Overflow Flag

#define  TMR3_3_SCTRL_TCFIE_MASK     0x4000u
#define  TMR3_3_SCTRL_TCFIE_SHIFT    14
//   Timer Compare Flag Interrupt Enable

#define  TMR3_3_SCTRL_TCF_MASK       0x8000u
#define  TMR3_3_SCTRL_TCF_SHIFT      15
//   Timer Compare Flag

#define  TMR3_3_CMPLD1_COMPARATOR_LOAD_1_MASK 0x10u
#define  TMR3_3_CMPLD1_COMPARATOR_LOAD_1_SHIFT 0
#define  TMR3_3_CMPLD1_COMPARATOR_LOAD_1(x) (((uint16_t)(((uint16_t)(x))<<TMR3_3_CMPLD1_COMPARATOR_LOAD_1_SHIFT))&TMR3_3_CMPLD1_COMPARATOR_LOAD_1_MASK)

#define  TMR3_3_CMPLD2_COMPARATOR_LOAD_2_MASK 0x10u
#define  TMR3_3_CMPLD2_COMPARATOR_LOAD_2_SHIFT 0
#define  TMR3_3_CMPLD2_COMPARATOR_LOAD_2(x) (((uint16_t)(((uint16_t)(x))<<TMR3_3_CMPLD2_COMPARATOR_LOAD_2_SHIFT))&TMR3_3_CMPLD2_COMPARATOR_LOAD_2_MASK)

#define  TMR3_3_CSCTRL_CL1_MASK      0x2u
#define  TMR3_3_CSCTRL_CL1_SHIFT     0
#define  TMR3_3_CSCTRL_CL1(x)        (((uint16_t)(((uint16_t)(x))<<TMR3_3_CSCTRL_CL1_SHIFT))&TMR3_3_CSCTRL_CL1_MASK)
//   Compare Load Control 1
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR3_3_CSCTRL_CL2_MASK      0x8u
#define  TMR3_3_CSCTRL_CL2_SHIFT     2
#define  TMR3_3_CSCTRL_CL2(x)        (((uint16_t)(((uint16_t)(x))<<TMR3_3_CSCTRL_CL2_SHIFT))&TMR3_3_CSCTRL_CL2_MASK)
//   Compare Load Control 2
//     0b00 ==> Never preload
//     0b01 ==> Load upon successful compare with the value in COMP1
//     0b10 ==> Load upon successful compare with the value in COMP2
//     0b11 ==> Reserved

#define  TMR3_3_CSCTRL_TCF1_MASK     0x10u
#define  TMR3_3_CSCTRL_TCF1_SHIFT    4
//   Timer Compare 1 Interrupt Flag

#define  TMR3_3_CSCTRL_TCF2_MASK     0x20u
#define  TMR3_3_CSCTRL_TCF2_SHIFT    5
//   Timer Compare 2 Interrupt Flag

#define  TMR3_3_CSCTRL_TCF1EN_MASK   0x40u
#define  TMR3_3_CSCTRL_TCF1EN_SHIFT  6
//   Timer Compare 1 Interrupt Enable

#define  TMR3_3_CSCTRL_TCF2EN_MASK   0x80u
#define  TMR3_3_CSCTRL_TCF2EN_SHIFT  7
//   Timer Compare 2 Interrupt Enable

#define  TMR3_3_CSCTRL_UP_MASK       0x200u
#define  TMR3_3_CSCTRL_UP_SHIFT      9
//   Counting Direction Indicator
//     0b0 ==> The last count was in the DOWN direction.
//     0b1 ==> The last count was in the UP direction.

#define  TMR3_3_CSCTRL_TCI_MASK      0x400u
#define  TMR3_3_CSCTRL_TCI_SHIFT     10
//   Triggered Count Initialization Control
//     0b0 ==> Stop counter upon receiving a second trigger event while still counting from the first trigger event.
//     0b1 ==> Reload the counter upon receiving a second trigger event while still counting from the first trigger event.

#define  TMR3_3_CSCTRL_ROC_MASK      0x800u
#define  TMR3_3_CSCTRL_ROC_SHIFT     11
//   Reload on Capture
//     0b0 ==> Do not reload the counter on a capture event.
//     0b1 ==> Reload the counter on a capture event.

#define  TMR3_3_CSCTRL_ALT_LOAD_MASK 0x1000u
#define  TMR3_3_CSCTRL_ALT_LOAD_SHIFT 12
//   Alternative Load Enable
//     0b0 ==> Counter can be re-initialized only with the LOAD register.
//     0b1 ==> Counter can be re-initialized with the LOAD or CMPLD2 registers depending on count direction.

#define  TMR3_3_CSCTRL_FAULT_MASK    0x2000u
#define  TMR3_3_CSCTRL_FAULT_SHIFT   13
//   Fault Enable
//     0b0 ==> Fault function disabled.
//     0b1 ==> Fault function enabled.

#define  TMR3_3_CSCTRL_DBG_EN_MASK   0x8000u
#define  TMR3_3_CSCTRL_DBG_EN_SHIFT  14
#define  TMR3_3_CSCTRL_DBG_EN(x)     (((uint16_t)(((uint16_t)(x))<<TMR3_3_CSCTRL_DBG_EN_SHIFT))&TMR3_3_CSCTRL_DBG_EN_MASK)
//   Debug Actions Enable
//     0b00 ==> Continue with normal operation during debug mode. (default)
//     0b01 ==> Halt TMR counter during debug mode.
//     0b10 ==> Force TMR output to logic 0 (prior to consideration of SCTRL[OPS]).
//     0b11 ==> Both halt counter and force output to 0 during debug mode.

#define  TMR3_3_FILT_FILT_PER_MASK   0x8u
#define  TMR3_3_FILT_FILT_PER_SHIFT  0
#define  TMR3_3_FILT_FILT_PER(x)     (((uint16_t)(((uint16_t)(x))<<TMR3_3_FILT_FILT_PER_SHIFT))&TMR3_3_FILT_FILT_PER_MASK)
//   Input Filter Sample Period

#define  TMR3_3_FILT_FILT_CNT_MASK   0x300u
#define  TMR3_3_FILT_FILT_CNT_SHIFT  8
#define  TMR3_3_FILT_FILT_CNT(x)     (((uint16_t)(((uint16_t)(x))<<TMR3_3_FILT_FILT_CNT_SHIFT))&TMR3_3_FILT_FILT_CNT_MASK)
//   Input Filter Sample Count

#define  TMR3_3_DMA_IEFDE_MASK       0x1u
#define  TMR3_3_DMA_IEFDE_SHIFT      0
//   Input Edge Flag DMA Enable

#define  TMR3_3_DMA_CMPLD1DE_MASK    0x2u
#define  TMR3_3_DMA_CMPLD1DE_SHIFT   1
//   Comparator Preload Register 1 DMA Enable

#define  TMR3_3_DMA_CMPLD2DE_MASK    0x4u
#define  TMR3_3_DMA_CMPLD2DE_SHIFT   2
//   Comparator Preload Register 2 DMA Enable

#define  UART0_BDH_SBR_MASK          0x5u
#define  UART0_BDH_SBR_SHIFT         0
#define  UART0_BDH_SBR(x)            (((uint8_t)(((uint8_t)(x))<<UART0_BDH_SBR_SHIFT))&UART0_BDH_SBR_MASK)
//   UART Baud Rate Bits

#define  UART0_BDH_RXEDGIE_MASK      0x40u
#define  UART0_BDH_RXEDGIE_SHIFT     6
//   RxD Input Active Edge Interrupt Enable
//     0b0 ==> Hardware interrupts from RXEDGIF disabled using polling.
//     0b1 ==> RXEDGIF interrupt request enabled.

#define  UART0_BDH_LBKDIE_MASK       0x80u
#define  UART0_BDH_LBKDIE_SHIFT      7
//   LIN Break Detect Interrupt Enable
//     0b0 ==> LBKDIF interrupt requests disabled.
//     0b1 ==> LBKDIF interrupt requests enabled.

#define  UART0_BDL_SBR_MASK          0x8u
#define  UART0_BDL_SBR_SHIFT         0
#define  UART0_BDL_SBR(x)            (((uint8_t)(((uint8_t)(x))<<UART0_BDL_SBR_SHIFT))&UART0_BDL_SBR_MASK)
//   UART Baud Rate Bits

#define  UART0_C1_PT_MASK            0x1u
#define  UART0_C1_PT_SHIFT           0
//   Parity Type
//     0b0 ==> Even parity.
//     0b1 ==> Odd parity.

#define  UART0_C1_PE_MASK            0x2u
#define  UART0_C1_PE_SHIFT           1
//   Parity Enable
//     0b0 ==> Parity function disabled.
//     0b1 ==> Parity function enabled.

#define  UART0_C1_ILT_MASK           0x4u
#define  UART0_C1_ILT_SHIFT          2
//   Idle Line Type Select
//     0b0 ==> Idle character bit count starts after start bit.
//     0b1 ==> Idle character bit count starts after stop bit.

#define  UART0_C1_WAKE_MASK          0x8u
#define  UART0_C1_WAKE_SHIFT         3
//   Receiver Wakeup Method Select
//     0b0 ==> Idle line wakeup.
//     0b1 ==> Address mark wakeup.

#define  UART0_C1_M_MASK             0x10u
#define  UART0_C1_M_SHIFT            4
//   9-bit or 8-bit Mode Select
//     0b0 ==> Normal-start + 8 data bits (MSB/LSB first as determined by MSBF) + stop.
//     0b1 ==> Use-start + 9 data bits (MSB/LSB first as determined by MSBF) + stop.

#define  UART0_C1_RSRC_MASK          0x20u
#define  UART0_C1_RSRC_SHIFT         5
//   Receiver Source Select
//     0b0 ==> Selects internal loop back mode. The receiver input is internally connected to transmitter output.
//     0b1 ==> Single wire UART mode where the receiver input is connected to the transmit pin input signal.

#define  UART0_C1_LOOPS_MASK         0x80u
#define  UART0_C1_LOOPS_SHIFT        7
//   Loop Mode Select
//     0b0 ==> Normal operation.
//     0b1 ==> Loop mode where transmitter output is internally connected to receiver input. The receiver input is determined by RSRC.

#define  UART0_C2_SBK_MASK           0x1u
#define  UART0_C2_SBK_SHIFT          0
//   Send Break
//     0b0 ==> Normal transmitter operation.
//     0b1 ==> Queue break characters to be sent.

#define  UART0_C2_RWU_MASK           0x2u
#define  UART0_C2_RWU_SHIFT          1
//   Receiver Wakeup Control
//     0b0 ==> Normal operation.
//     0b1 ==> RWU enables the wakeup function and inhibits further receiver interrupt requests. Normally, hardware wakes the receiver by automatically clearing RWU.

#define  UART0_C2_RE_MASK            0x4u
#define  UART0_C2_RE_SHIFT           2
//   Receiver Enable
//     0b0 ==> Receiver off.
//     0b1 ==> Receiver on.

#define  UART0_C2_TE_MASK            0x8u
#define  UART0_C2_TE_SHIFT           3
//   Transmitter Enable
//     0b0 ==> Transmitter off.
//     0b1 ==> Transmitter on.

#define  UART0_C2_ILIE_MASK          0x10u
#define  UART0_C2_ILIE_SHIFT         4
//   Idle Line Interrupt Enable
//     0b0 ==> IDLE interrupt requests disabled.
//     0b1 ==> IDLE interrupt requests enabled.

#define  UART0_C2_RIE_MASK           0x20u
#define  UART0_C2_RIE_SHIFT          5
//   Receiver Full Interrupt or DMA Transfer Enable
//     0b0 ==> RDRF interrupt and DMA transfer requests disabled.
//     0b1 ==> RDRF interrupt or DMA transfer requests enabled.

#define  UART0_C2_TCIE_MASK          0x40u
#define  UART0_C2_TCIE_SHIFT         6
//   Transmission Complete Interrupt Enable
//     0b0 ==> TC interrupt requests disabled.
//     0b1 ==> TC interrupt requests enabled.

#define  UART0_C2_TIE_MASK           0x80u
#define  UART0_C2_TIE_SHIFT          7
//   Transmitter Interrupt or DMA Transfer Enable.
//     0b0 ==> TDRE interrupt and DMA transfer requests disabled.
//     0b1 ==> TDRE interrupt or DMA transfer requests enabled.

#define  UART0_S1_PF_MASK            0x1u
#define  UART0_S1_PF_SHIFT           0
//   Parity Error Flag
//     0b0 ==> No parity error detected since the last time this flag was cleared. If the receive buffer has a depth greater than 1, then there may be data in the receive buffer what was received with a parity error.
//     0b1 ==> At least one dataword was received with a parity error since the last time this flag was cleared.

#define  UART0_S1_FE_MASK            0x2u
#define  UART0_S1_FE_SHIFT           1
//   Framing Error Flag
//     0b0 ==> No framing error detected.
//     0b1 ==> Framing error.

#define  UART0_S1_NF_MASK            0x4u
#define  UART0_S1_NF_SHIFT           2
//   Noise Flag
//     0b0 ==> No noise detected since the last time this flag was cleared. If the receive buffer has a depth greater than 1 then there may be data in the receiver buffer that was received with noise.
//     0b1 ==> At least one dataword was received with noise detected since the last time the flag was cleared.

#define  UART0_S1_OR_MASK            0x8u
#define  UART0_S1_OR_SHIFT           3
//   Receiver Overrun Flag
//     0b0 ==> No overrun has occurred since the last time the flag was cleared.
//     0b1 ==> Overrun has occurred or the overrun flag has not been cleared since the last overrun occured.

#define  UART0_S1_IDLE_MASK          0x10u
#define  UART0_S1_IDLE_SHIFT         4
//   Idle Line Flag
//     0b0 ==> Receiver input is either active now or has never become active since the IDLE flag was last cleared.
//     0b1 ==> Receiver input has become idle or the flag has not been cleared since it last asserted.

#define  UART0_S1_RDRF_MASK          0x20u
#define  UART0_S1_RDRF_SHIFT         5
//   Receive Data Register Full Flag
//     0b0 ==> The number of datawords in the receive buffer is less than the number indicated by RXWATER.
//     0b1 ==> The number of datawords in the receive buffer is equal to or greater than the number indicated by RXWATER at some point in time since this flag was last cleared.

#define  UART0_S1_TC_MASK            0x40u
#define  UART0_S1_TC_SHIFT           6
//   Transmit Complete Flag
//     0b0 ==> Transmitter active (sending data, a preamble, or a break).
//     0b1 ==> Transmitter idle (transmission activity complete).

#define  UART0_S1_TDRE_MASK          0x80u
#define  UART0_S1_TDRE_SHIFT         7
//   Transmit Data Register Empty Flag
//     0b0 ==> The amount of data in the transmit buffer is greater than the value indicated by TWFIFO[TXWATER].
//     0b1 ==> The amount of data in the transmit buffer is less than or equal to the value indicated by TWFIFO[TXWATER] at some point in time since the flag has been cleared.

#define  UART0_S2_RAF_MASK           0x1u
#define  UART0_S2_RAF_SHIFT          0
//   Receiver Active Flag
//     0b0 ==> UART receiver idle/inactive waiting for a start bit.
//     0b1 ==> UART receiver active, RxD input not idle.

#define  UART0_S2_LBKDE_MASK         0x2u
#define  UART0_S2_LBKDE_SHIFT        1
//   LIN Break Detection Enable
//     0b0 ==> Break character is detected at one of the following lengths: 10 bit times if C1[M] = 0 11 bit times if C1[M] = 1 and C4[M10] = 0 12 bit times if C1[M] = 1, C4[M10] = 1, and S1[PE] = 1
//     0b1 ==> Break character is detected at length of 11 bit times if C1[M] = 0 or 12 bits time if C1[M] = 1.

#define  UART0_S2_BRK13_MASK         0x4u
#define  UART0_S2_BRK13_SHIFT        2
//   Break Transmit Character Length
//     0b0 ==> Break character is 10, 11, or 12 bits long.
//     0b1 ==> Break character is 13 or 14 bits long.

#define  UART0_S2_RWUID_MASK         0x8u
#define  UART0_S2_RWUID_SHIFT        3
//   Receive Wakeup Idle Detect
//     0b0 ==> S1[IDLE] is not set upon detection of an idle character.
//     0b1 ==> S1[IDLE] is set upon detection of an idle character.

#define  UART0_S2_RXINV_MASK         0x10u
#define  UART0_S2_RXINV_SHIFT        4
//   Receive Data Inversion
//     0b0 ==> Receive data is not inverted.
//     0b1 ==> Receive data is inverted.

#define  UART0_S2_MSBF_MASK          0x20u
#define  UART0_S2_MSBF_SHIFT         5
//   Most Significant Bit First
//     0b0 ==> LSB (bit0) is the first bit that is transmitted following the start bit. Further, the first bit received after the start bit is identified as bit0.
//     0b1 ==> MSB (bit8, bit7 or bit6) is the first bit that is transmitted following the start bit, depending on the setting of C1[M] and C1[PE]. Further, the first bit received after the start bit is identified as bit8, bit7, or bit6, depending on the setting of C1[M] and C1[PE].

#define  UART0_S2_RXEDGIF_MASK       0x40u
#define  UART0_S2_RXEDGIF_SHIFT      6
//   RxD Pin Active Edge Interrupt Flag
//     0b0 ==> No active edge on the receive pin has occurred.
//     0b1 ==> An active edge on the receive pin has occurred.

#define  UART0_S2_LBKDIF_MASK        0x80u
#define  UART0_S2_LBKDIF_SHIFT       7
//   LIN Break Detect Interrupt Flag
//     0b0 ==> No LIN break character detected.
//     0b1 ==> LIN break character detected.

#define  UART0_C3_PEIE_MASK          0x1u
#define  UART0_C3_PEIE_SHIFT         0
//   Parity Error Interrupt Enable
//     0b0 ==> PF interrupt requests are disabled.
//     0b1 ==> PF interrupt requests are enabled.

#define  UART0_C3_FEIE_MASK          0x2u
#define  UART0_C3_FEIE_SHIFT         1
//   Framing Error Interrupt Enable
//     0b0 ==> FE interrupt requests are disabled.
//     0b1 ==> FE interrupt requests are enabled.

#define  UART0_C3_NEIE_MASK          0x4u
#define  UART0_C3_NEIE_SHIFT         2
//   Noise Error Interrupt Enable
//     0b0 ==> NF interrupt requests are disabled.
//     0b1 ==> NF interrupt requests are enabled.

#define  UART0_C3_ORIE_MASK          0x8u
#define  UART0_C3_ORIE_SHIFT         3
//   Overrun Error Interrupt Enable
//     0b0 ==> OR interrupts are disabled.
//     0b1 ==> OR interrupt requests are enabled.

#define  UART0_C3_TXINV_MASK         0x10u
#define  UART0_C3_TXINV_SHIFT        4
//   Transmit Data Inversion.
//     0b0 ==> Transmit data is not inverted.
//     0b1 ==> Transmit data is inverted.

#define  UART0_C3_TXDIR_MASK         0x20u
#define  UART0_C3_TXDIR_SHIFT        5
//   Transmitter Pin Data Direction in Single-Wire mode
//     0b0 ==> TXD pin is an input in single wire mode.
//     0b1 ==> TXD pin is an output in single wire mode.

#define  UART0_C3_T8_MASK            0x40u
#define  UART0_C3_T8_SHIFT           6
//   Transmit Bit 8

#define  UART0_C3_R8_MASK            0x80u
#define  UART0_C3_R8_SHIFT           7
//   Received Bit 8

#define  UART0_D_RT_MASK             0x8u
#define  UART0_D_RT_SHIFT            0
#define  UART0_D_RT(x)               (((uint8_t)(((uint8_t)(x))<<UART0_D_RT_SHIFT))&UART0_D_RT_MASK)

#define  UART0_MA1_MA_MASK           0x8u
#define  UART0_MA1_MA_SHIFT          0
#define  UART0_MA1_MA(x)             (((uint8_t)(((uint8_t)(x))<<UART0_MA1_MA_SHIFT))&UART0_MA1_MA_MASK)
//   Match Address

#define  UART0_MA2_MA_MASK           0x8u
#define  UART0_MA2_MA_SHIFT          0
#define  UART0_MA2_MA(x)             (((uint8_t)(((uint8_t)(x))<<UART0_MA2_MA_SHIFT))&UART0_MA2_MA_MASK)
//   Match Address

#define  UART0_C4_BRFA_MASK          0x5u
#define  UART0_C4_BRFA_SHIFT         0
#define  UART0_C4_BRFA(x)            (((uint8_t)(((uint8_t)(x))<<UART0_C4_BRFA_SHIFT))&UART0_C4_BRFA_MASK)
//   Baud Rate Fine Adjust

#define  UART0_C4_M10_MASK           0x20u
#define  UART0_C4_M10_SHIFT          5
//   10-bit Mode select
//     0b0 ==> The parity bit is the ninth bit in the serial transmission.
//     0b1 ==> The parity bit is the tenth bit in the serial transmission.

#define  UART0_C4_MAEN2_MASK         0x40u
#define  UART0_C4_MAEN2_SHIFT        6
//   Match Address Mode Enable 2
//     0b0 ==> All data received is transferred to the data buffer if MAEN1 is cleared.
//     0b1 ==> All data received with the most significant bit cleared, is discarded. All data received with the most significant bit set, is compared with contents of MA2 register. If no match occurs, the data is discarded. If a match occurs, data is transferred to the data buffer. This field must be cleared when C7816[ISO7816E] is set/enabled.

#define  UART0_C4_MAEN1_MASK         0x80u
#define  UART0_C4_MAEN1_SHIFT        7
//   Match Address Mode Enable 1
//     0b0 ==> All data received is transferred to the data buffer if MAEN2 is cleared.
//     0b1 ==> All data received with the most significant bit cleared, is discarded. All data received with the most significant bit set, is compared with contents of MA1 register. If no match occurs, the data is discarded. If match occurs, data is transferred to the data buffer. This field must be cleared when C7816[ISO7816E] is set/enabled.

#define  UART0_C5_RDMAS_MASK         0x20u
#define  UART0_C5_RDMAS_SHIFT        5
//   Receiver Full DMA Select
//     0b0 ==> If C2[RIE] and S1[RDRF] are set, the RDFR interrupt request signal is asserted to request an interrupt service.
//     0b1 ==> If C2[RIE] and S1[RDRF] are set, the RDRF DMA request signal is asserted to request a DMA transfer.

#define  UART0_C5_TDMAS_MASK         0x80u
#define  UART0_C5_TDMAS_SHIFT        7
//   Transmitter DMA Select
//     0b0 ==> If C2[TIE] is set and the S1[TDRE] flag is set, the TDRE interrupt request signal is asserted to request interrupt service.
//     0b1 ==> If C2[TIE] is set and the S1[TDRE] flag is set, the TDRE DMA request signal is asserted to request a DMA transfer.

#define  UART0_IR_TNP_MASK           0x2u
#define  UART0_IR_TNP_SHIFT          0
#define  UART0_IR_TNP(x)             (((uint8_t)(((uint8_t)(x))<<UART0_IR_TNP_SHIFT))&UART0_IR_TNP_MASK)
//   Transmitter narrow pulse
//     0b00 ==> 3/16.
//     0b01 ==> 1/16.
//     0b10 ==> 1/32.
//     0b11 ==> 1/4.

#define  UART0_IR_IREN_MASK          0x4u
#define  UART0_IR_IREN_SHIFT         2
//   Infrared enable
//     0b0 ==> IR disabled.
//     0b1 ==> IR enabled.

#define  UART0_C7816_ISO_7816E_MASK  0x1u
#define  UART0_C7816_ISO_7816E_SHIFT 0
//   ISO-7816 Functionality Enabled
//     0b0 ==> ISO-7816 functionality is turned off/not enabled.
//     0b1 ==> ISO-7816 functionality is turned on/enabled.

#define  UART0_C7816_TTYPE_MASK      0x2u
#define  UART0_C7816_TTYPE_SHIFT     1
//   Transfer Type
//     0b0 ==> T = 0 per the ISO-7816 specification.
//     0b1 ==> T = 1 per the ISO-7816 specification.

#define  UART0_C7816_INIT_MASK       0x4u
#define  UART0_C7816_INIT_SHIFT      2
//   Detect Initial Character
//     0b0 ==> Normal operating mode. Receiver does not seek to identify initial character.
//     0b1 ==> Receiver searches for initial character.

#define  UART0_C7816_ANACK_MASK      0x8u
#define  UART0_C7816_ANACK_SHIFT     3
//   Generate NACK on Error
//     0b0 ==> No NACK is automatically generated.
//     0b1 ==> A NACK is automatically generated if a parity error is detected or if an invalid initial character is detected.

#define  UART0_C7816_ONACK_MASK      0x10u
#define  UART0_C7816_ONACK_SHIFT     4
//   Generate NACK on Overflow
//     0b0 ==> The received data does not generate a NACK when the receipt of the data results in an overflow event.
//     0b1 ==> If the receiver buffer overflows, a NACK is automatically sent on a received character.

#define  UART0_IE7816_RXTE_MASK      0x1u
#define  UART0_IE7816_RXTE_SHIFT     0
//   Receive Threshold Exceeded Interrupt Enable
//     0b0 ==> The assertion of IS7816[RXT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[RXT] results in the generation of an interrupt.

#define  UART0_IE7816_TXTE_MASK      0x2u
#define  UART0_IE7816_TXTE_SHIFT     1
//   Transmit Threshold Exceeded Interrupt Enable
//     0b0 ==> The assertion of IS7816[TXT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[TXT] results in the generation of an interrupt.

#define  UART0_IE7816_GTVE_MASK      0x4u
#define  UART0_IE7816_GTVE_SHIFT     2
//   Guard Timer Violated Interrupt Enable
//     0b0 ==> The assertion of IS7816[GTV] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[GTV] results in the generation of an interrupt.

#define  UART0_IE7816_INITDE_MASK    0x10u
#define  UART0_IE7816_INITDE_SHIFT   4
//   Initial Character Detected Interrupt Enable
//     0b0 ==> The assertion of IS7816[INITD] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[INITD] results in the generation of an interrupt.

#define  UART0_IE7816_BWTE_MASK      0x20u
#define  UART0_IE7816_BWTE_SHIFT     5
//   Block Wait Timer Interrupt Enable
//     0b0 ==> The assertion of IS7816[BWT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[BWT] results in the generation of an interrupt.

#define  UART0_IE7816_CWTE_MASK      0x40u
#define  UART0_IE7816_CWTE_SHIFT     6
//   Character Wait Timer Interrupt Enable
//     0b0 ==> The assertion of IS7816[CWT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[CWT] results in the generation of an interrupt.

#define  UART0_IE7816_WTE_MASK       0x80u
#define  UART0_IE7816_WTE_SHIFT      7
//   Wait Timer Interrupt Enable
//     0b0 ==> The assertion of IS7816[WT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[WT] results in the generation of an interrupt.

#define  UART0_IS7816_RXT_MASK       0x1u
#define  UART0_IS7816_RXT_SHIFT      0
//   Receive Threshold Exceeded Interrupt
//     0b0 ==> The number of consecutive NACKS generated as a result of parity errors and buffer overruns is less than or equal to the value in ET7816[RXTHRESHOLD].
//     0b1 ==> The number of consecutive NACKS generated as a result of parity errors and buffer overruns is greater than the value in ET7816[RXTHRESHOLD].

#define  UART0_IS7816_TXT_MASK       0x2u
#define  UART0_IS7816_TXT_SHIFT      1
//   Transmit Threshold Exceeded Interrupt
//     0b0 ==> The number of retries and corresponding NACKS does not exceed the value in ET7816[TXTHRESHOLD].
//     0b1 ==> The number of retries and corresponding NACKS exceeds the value in ET7816[TXTHRESHOLD].

#define  UART0_IS7816_GTV_MASK       0x4u
#define  UART0_IS7816_GTV_SHIFT      2
//   Guard Timer Violated Interrupt
//     0b0 ==> A guard time (GT, CGT, or BGT) has not been violated.
//     0b1 ==> A guard time (GT, CGT, or BGT) has been violated.

#define  UART0_IS7816_INITD_MASK     0x10u
#define  UART0_IS7816_INITD_SHIFT    4
//   Initial Character Detected Interrupt
//     0b0 ==> A valid initial character has not been received.
//     0b1 ==> A valid initial character has been received.

#define  UART0_IS7816_BWT_MASK       0x20u
#define  UART0_IS7816_BWT_SHIFT      5
//   Block Wait Timer Interrupt
//     0b0 ==> Block wait time (BWT) has not been violated.
//     0b1 ==> Block wait time (BWT) has been violated.

#define  UART0_IS7816_CWT_MASK       0x40u
#define  UART0_IS7816_CWT_SHIFT      6
//   Character Wait Timer Interrupt
//     0b0 ==> Character wait time (CWT) has not been violated.
//     0b1 ==> Character wait time (CWT) has been violated.

#define  UART0_IS7816_WT_MASK        0x80u
#define  UART0_IS7816_WT_SHIFT       7
//   Wait Timer Interrupt
//     0b0 ==> Wait time (WT) has not been violated.
//     0b1 ==> Wait time (WT) has been violated.

#define  UART0_WP7816T0_WI_MASK      0x8u
#define  UART0_WP7816T0_WI_SHIFT     0
#define  UART0_WP7816T0_WI(x)        (((uint8_t)(((uint8_t)(x))<<UART0_WP7816T0_WI_SHIFT))&UART0_WP7816T0_WI_MASK)
//   Wait Timer Interrupt (C7816[TTYPE] = 0)

#define  UART0_WP7816T1_BWI_MASK     0x4u
#define  UART0_WP7816T1_BWI_SHIFT    0
#define  UART0_WP7816T1_BWI(x)       (((uint8_t)(((uint8_t)(x))<<UART0_WP7816T1_BWI_SHIFT))&UART0_WP7816T1_BWI_MASK)
//   Block Wait Time Integer(C7816[TTYPE] = 1)

#define  UART0_WP7816T1_CWI_MASK     0x40u
#define  UART0_WP7816T1_CWI_SHIFT    4
#define  UART0_WP7816T1_CWI(x)       (((uint8_t)(((uint8_t)(x))<<UART0_WP7816T1_CWI_SHIFT))&UART0_WP7816T1_CWI_MASK)
//   Character Wait Time Integer (C7816[TTYPE] = 1)

#define  UART0_WN7816_GTN_MASK       0x8u
#define  UART0_WN7816_GTN_SHIFT      0
#define  UART0_WN7816_GTN(x)         (((uint8_t)(((uint8_t)(x))<<UART0_WN7816_GTN_SHIFT))&UART0_WN7816_GTN_MASK)
//   Guard Band N

#define  UART0_WF7816_GTFD_MASK      0x8u
#define  UART0_WF7816_GTFD_SHIFT     0
#define  UART0_WF7816_GTFD(x)        (((uint8_t)(((uint8_t)(x))<<UART0_WF7816_GTFD_SHIFT))&UART0_WF7816_GTFD_MASK)
//   FD Multiplier

#define  UART0_ET7816_RXTHRESHOLD_MASK 0x4u
#define  UART0_ET7816_RXTHRESHOLD_SHIFT 0
#define  UART0_ET7816_RXTHRESHOLD(x) (((uint8_t)(((uint8_t)(x))<<UART0_ET7816_RXTHRESHOLD_SHIFT))&UART0_ET7816_RXTHRESHOLD_MASK)
//   Receive NACK Threshold

#define  UART0_ET7816_TXTHRESHOLD_MASK 0x40u
#define  UART0_ET7816_TXTHRESHOLD_SHIFT 4
#define  UART0_ET7816_TXTHRESHOLD(x) (((uint8_t)(((uint8_t)(x))<<UART0_ET7816_TXTHRESHOLD_SHIFT))&UART0_ET7816_TXTHRESHOLD_MASK)
//   Transmit NACK Threshold
//     0b0 ==> TXT asserts on the first NACK that is received.
//     0b1 ==> TXT asserts on the second NACK that is received.

#define  UART0_TL7816_TLEN_MASK      0x8u
#define  UART0_TL7816_TLEN_SHIFT     0
#define  UART0_TL7816_TLEN(x)        (((uint8_t)(((uint8_t)(x))<<UART0_TL7816_TLEN_SHIFT))&UART0_TL7816_TLEN_MASK)
//   Transmit Length

#define  UART0_WB_WBASE_MASK         0x8u
#define  UART0_WB_WBASE_SHIFT        0
#define  UART0_WB_WBASE(x)           (((uint8_t)(((uint8_t)(x))<<UART0_WB_WBASE_SHIFT))&UART0_WB_WBASE_MASK)
//   CEA709.1-B WBASE register

#define  UART1_BDH_SBR_MASK          0x5u
#define  UART1_BDH_SBR_SHIFT         0
#define  UART1_BDH_SBR(x)            (((uint8_t)(((uint8_t)(x))<<UART1_BDH_SBR_SHIFT))&UART1_BDH_SBR_MASK)
//   UART Baud Rate Bits

#define  UART1_BDH_RXEDGIE_MASK      0x40u
#define  UART1_BDH_RXEDGIE_SHIFT     6
//   RxD Input Active Edge Interrupt Enable
//     0b0 ==> Hardware interrupts from RXEDGIF disabled using polling.
//     0b1 ==> RXEDGIF interrupt request enabled.

#define  UART1_BDH_LBKDIE_MASK       0x80u
#define  UART1_BDH_LBKDIE_SHIFT      7
//   LIN Break Detect Interrupt Enable
//     0b0 ==> LBKDIF interrupt requests disabled.
//     0b1 ==> LBKDIF interrupt requests enabled.

#define  UART1_BDL_SBR_MASK          0x8u
#define  UART1_BDL_SBR_SHIFT         0
#define  UART1_BDL_SBR(x)            (((uint8_t)(((uint8_t)(x))<<UART1_BDL_SBR_SHIFT))&UART1_BDL_SBR_MASK)
//   UART Baud Rate Bits

#define  UART1_C1_PT_MASK            0x1u
#define  UART1_C1_PT_SHIFT           0
//   Parity Type
//     0b0 ==> Even parity.
//     0b1 ==> Odd parity.

#define  UART1_C1_PE_MASK            0x2u
#define  UART1_C1_PE_SHIFT           1
//   Parity Enable
//     0b0 ==> Parity function disabled.
//     0b1 ==> Parity function enabled.

#define  UART1_C1_ILT_MASK           0x4u
#define  UART1_C1_ILT_SHIFT          2
//   Idle Line Type Select
//     0b0 ==> Idle character bit count starts after start bit.
//     0b1 ==> Idle character bit count starts after stop bit.

#define  UART1_C1_WAKE_MASK          0x8u
#define  UART1_C1_WAKE_SHIFT         3
//   Receiver Wakeup Method Select
//     0b0 ==> Idle line wakeup.
//     0b1 ==> Address mark wakeup.

#define  UART1_C1_M_MASK             0x10u
#define  UART1_C1_M_SHIFT            4
//   9-bit or 8-bit Mode Select
//     0b0 ==> Normal-start + 8 data bits (MSB/LSB first as determined by MSBF) + stop.
//     0b1 ==> Use-start + 9 data bits (MSB/LSB first as determined by MSBF) + stop.

#define  UART1_C1_RSRC_MASK          0x20u
#define  UART1_C1_RSRC_SHIFT         5
//   Receiver Source Select
//     0b0 ==> Selects internal loop back mode. The receiver input is internally connected to transmitter output.
//     0b1 ==> Single wire UART mode where the receiver input is connected to the transmit pin input signal.

#define  UART1_C1_LOOPS_MASK         0x80u
#define  UART1_C1_LOOPS_SHIFT        7
//   Loop Mode Select
//     0b0 ==> Normal operation.
//     0b1 ==> Loop mode where transmitter output is internally connected to receiver input. The receiver input is determined by RSRC.

#define  UART1_C2_SBK_MASK           0x1u
#define  UART1_C2_SBK_SHIFT          0
//   Send Break
//     0b0 ==> Normal transmitter operation.
//     0b1 ==> Queue break characters to be sent.

#define  UART1_C2_RWU_MASK           0x2u
#define  UART1_C2_RWU_SHIFT          1
//   Receiver Wakeup Control
//     0b0 ==> Normal operation.
//     0b1 ==> RWU enables the wakeup function and inhibits further receiver interrupt requests. Normally, hardware wakes the receiver by automatically clearing RWU.

#define  UART1_C2_RE_MASK            0x4u
#define  UART1_C2_RE_SHIFT           2
//   Receiver Enable
//     0b0 ==> Receiver off.
//     0b1 ==> Receiver on.

#define  UART1_C2_TE_MASK            0x8u
#define  UART1_C2_TE_SHIFT           3
//   Transmitter Enable
//     0b0 ==> Transmitter off.
//     0b1 ==> Transmitter on.

#define  UART1_C2_ILIE_MASK          0x10u
#define  UART1_C2_ILIE_SHIFT         4
//   Idle Line Interrupt Enable
//     0b0 ==> IDLE interrupt requests disabled.
//     0b1 ==> IDLE interrupt requests enabled.

#define  UART1_C2_RIE_MASK           0x20u
#define  UART1_C2_RIE_SHIFT          5
//   Receiver Full Interrupt or DMA Transfer Enable
//     0b0 ==> RDRF interrupt and DMA transfer requests disabled.
//     0b1 ==> RDRF interrupt or DMA transfer requests enabled.

#define  UART1_C2_TCIE_MASK          0x40u
#define  UART1_C2_TCIE_SHIFT         6
//   Transmission Complete Interrupt Enable
//     0b0 ==> TC interrupt requests disabled.
//     0b1 ==> TC interrupt requests enabled.

#define  UART1_C2_TIE_MASK           0x80u
#define  UART1_C2_TIE_SHIFT          7
//   Transmitter Interrupt or DMA Transfer Enable.
//     0b0 ==> TDRE interrupt and DMA transfer requests disabled.
//     0b1 ==> TDRE interrupt or DMA transfer requests enabled.

#define  UART1_S1_PF_MASK            0x1u
#define  UART1_S1_PF_SHIFT           0
//   Parity Error Flag
//     0b0 ==> No parity error detected since the last time this flag was cleared. If the receive buffer has a depth greater than 1, then there may be data in the receive buffer what was received with a parity error.
//     0b1 ==> At least one dataword was received with a parity error since the last time this flag was cleared.

#define  UART1_S1_FE_MASK            0x2u
#define  UART1_S1_FE_SHIFT           1
//   Framing Error Flag
//     0b0 ==> No framing error detected.
//     0b1 ==> Framing error.

#define  UART1_S1_NF_MASK            0x4u
#define  UART1_S1_NF_SHIFT           2
//   Noise Flag
//     0b0 ==> No noise detected since the last time this flag was cleared. If the receive buffer has a depth greater than 1 then there may be data in the receiver buffer that was received with noise.
//     0b1 ==> At least one dataword was received with noise detected since the last time the flag was cleared.

#define  UART1_S1_OR_MASK            0x8u
#define  UART1_S1_OR_SHIFT           3
//   Receiver Overrun Flag
//     0b0 ==> No overrun has occurred since the last time the flag was cleared.
//     0b1 ==> Overrun has occurred or the overrun flag has not been cleared since the last overrun occured.

#define  UART1_S1_IDLE_MASK          0x10u
#define  UART1_S1_IDLE_SHIFT         4
//   Idle Line Flag
//     0b0 ==> Receiver input is either active now or has never become active since the IDLE flag was last cleared.
//     0b1 ==> Receiver input has become idle or the flag has not been cleared since it last asserted.

#define  UART1_S1_RDRF_MASK          0x20u
#define  UART1_S1_RDRF_SHIFT         5
//   Receive Data Register Full Flag
//     0b0 ==> The number of datawords in the receive buffer is less than the number indicated by RXWATER.
//     0b1 ==> The number of datawords in the receive buffer is equal to or greater than the number indicated by RXWATER at some point in time since this flag was last cleared.

#define  UART1_S1_TC_MASK            0x40u
#define  UART1_S1_TC_SHIFT           6
//   Transmit Complete Flag
//     0b0 ==> Transmitter active (sending data, a preamble, or a break).
//     0b1 ==> Transmitter idle (transmission activity complete).

#define  UART1_S1_TDRE_MASK          0x80u
#define  UART1_S1_TDRE_SHIFT         7
//   Transmit Data Register Empty Flag
//     0b0 ==> The amount of data in the transmit buffer is greater than the value indicated by TWFIFO[TXWATER].
//     0b1 ==> The amount of data in the transmit buffer is less than or equal to the value indicated by TWFIFO[TXWATER] at some point in time since the flag has been cleared.

#define  UART1_S2_RAF_MASK           0x1u
#define  UART1_S2_RAF_SHIFT          0
//   Receiver Active Flag
//     0b0 ==> UART receiver idle/inactive waiting for a start bit.
//     0b1 ==> UART receiver active, RxD input not idle.

#define  UART1_S2_LBKDE_MASK         0x2u
#define  UART1_S2_LBKDE_SHIFT        1
//   LIN Break Detection Enable
//     0b0 ==> Break character is detected at one of the following lengths: 10 bit times if C1[M] = 0 11 bit times if C1[M] = 1 and C4[M10] = 0 12 bit times if C1[M] = 1, C4[M10] = 1, and S1[PE] = 1
//     0b1 ==> Break character is detected at length of 11 bit times if C1[M] = 0 or 12 bits time if C1[M] = 1.

#define  UART1_S2_BRK13_MASK         0x4u
#define  UART1_S2_BRK13_SHIFT        2
//   Break Transmit Character Length
//     0b0 ==> Break character is 10, 11, or 12 bits long.
//     0b1 ==> Break character is 13 or 14 bits long.

#define  UART1_S2_RWUID_MASK         0x8u
#define  UART1_S2_RWUID_SHIFT        3
//   Receive Wakeup Idle Detect
//     0b0 ==> S1[IDLE] is not set upon detection of an idle character.
//     0b1 ==> S1[IDLE] is set upon detection of an idle character.

#define  UART1_S2_RXINV_MASK         0x10u
#define  UART1_S2_RXINV_SHIFT        4
//   Receive Data Inversion
//     0b0 ==> Receive data is not inverted.
//     0b1 ==> Receive data is inverted.

#define  UART1_S2_MSBF_MASK          0x20u
#define  UART1_S2_MSBF_SHIFT         5
//   Most Significant Bit First
//     0b0 ==> LSB (bit0) is the first bit that is transmitted following the start bit. Further, the first bit received after the start bit is identified as bit0.
//     0b1 ==> MSB (bit8, bit7 or bit6) is the first bit that is transmitted following the start bit, depending on the setting of C1[M] and C1[PE]. Further, the first bit received after the start bit is identified as bit8, bit7, or bit6, depending on the setting of C1[M] and C1[PE].

#define  UART1_S2_RXEDGIF_MASK       0x40u
#define  UART1_S2_RXEDGIF_SHIFT      6
//   RxD Pin Active Edge Interrupt Flag
//     0b0 ==> No active edge on the receive pin has occurred.
//     0b1 ==> An active edge on the receive pin has occurred.

#define  UART1_S2_LBKDIF_MASK        0x80u
#define  UART1_S2_LBKDIF_SHIFT       7
//   LIN Break Detect Interrupt Flag
//     0b0 ==> No LIN break character detected.
//     0b1 ==> LIN break character detected.

#define  UART1_C3_PEIE_MASK          0x1u
#define  UART1_C3_PEIE_SHIFT         0
//   Parity Error Interrupt Enable
//     0b0 ==> PF interrupt requests are disabled.
//     0b1 ==> PF interrupt requests are enabled.

#define  UART1_C3_FEIE_MASK          0x2u
#define  UART1_C3_FEIE_SHIFT         1
//   Framing Error Interrupt Enable
//     0b0 ==> FE interrupt requests are disabled.
//     0b1 ==> FE interrupt requests are enabled.

#define  UART1_C3_NEIE_MASK          0x4u
#define  UART1_C3_NEIE_SHIFT         2
//   Noise Error Interrupt Enable
//     0b0 ==> NF interrupt requests are disabled.
//     0b1 ==> NF interrupt requests are enabled.

#define  UART1_C3_ORIE_MASK          0x8u
#define  UART1_C3_ORIE_SHIFT         3
//   Overrun Error Interrupt Enable
//     0b0 ==> OR interrupts are disabled.
//     0b1 ==> OR interrupt requests are enabled.

#define  UART1_C3_TXINV_MASK         0x10u
#define  UART1_C3_TXINV_SHIFT        4
//   Transmit Data Inversion.
//     0b0 ==> Transmit data is not inverted.
//     0b1 ==> Transmit data is inverted.

#define  UART1_C3_TXDIR_MASK         0x20u
#define  UART1_C3_TXDIR_SHIFT        5
//   Transmitter Pin Data Direction in Single-Wire mode
//     0b0 ==> TXD pin is an input in single wire mode.
//     0b1 ==> TXD pin is an output in single wire mode.

#define  UART1_C3_T8_MASK            0x40u
#define  UART1_C3_T8_SHIFT           6
//   Transmit Bit 8

#define  UART1_C3_R8_MASK            0x80u
#define  UART1_C3_R8_SHIFT           7
//   Received Bit 8

#define  UART1_D_RT_MASK             0x8u
#define  UART1_D_RT_SHIFT            0
#define  UART1_D_RT(x)               (((uint8_t)(((uint8_t)(x))<<UART1_D_RT_SHIFT))&UART1_D_RT_MASK)

#define  UART1_MA1_MA_MASK           0x8u
#define  UART1_MA1_MA_SHIFT          0
#define  UART1_MA1_MA(x)             (((uint8_t)(((uint8_t)(x))<<UART1_MA1_MA_SHIFT))&UART1_MA1_MA_MASK)
//   Match Address

#define  UART1_MA2_MA_MASK           0x8u
#define  UART1_MA2_MA_SHIFT          0
#define  UART1_MA2_MA(x)             (((uint8_t)(((uint8_t)(x))<<UART1_MA2_MA_SHIFT))&UART1_MA2_MA_MASK)
//   Match Address

#define  UART1_C4_BRFA_MASK          0x5u
#define  UART1_C4_BRFA_SHIFT         0
#define  UART1_C4_BRFA(x)            (((uint8_t)(((uint8_t)(x))<<UART1_C4_BRFA_SHIFT))&UART1_C4_BRFA_MASK)
//   Baud Rate Fine Adjust

#define  UART1_C4_M10_MASK           0x20u
#define  UART1_C4_M10_SHIFT          5
//   10-bit Mode select
//     0b0 ==> The parity bit is the ninth bit in the serial transmission.
//     0b1 ==> The parity bit is the tenth bit in the serial transmission.

#define  UART1_C4_MAEN2_MASK         0x40u
#define  UART1_C4_MAEN2_SHIFT        6
//   Match Address Mode Enable 2
//     0b0 ==> All data received is transferred to the data buffer if MAEN1 is cleared.
//     0b1 ==> All data received with the most significant bit cleared, is discarded. All data received with the most significant bit set, is compared with contents of MA2 register. If no match occurs, the data is discarded. If a match occurs, data is transferred to the data buffer. This field must be cleared when C7816[ISO7816E] is set/enabled.

#define  UART1_C4_MAEN1_MASK         0x80u
#define  UART1_C4_MAEN1_SHIFT        7
//   Match Address Mode Enable 1
//     0b0 ==> All data received is transferred to the data buffer if MAEN2 is cleared.
//     0b1 ==> All data received with the most significant bit cleared, is discarded. All data received with the most significant bit set, is compared with contents of MA1 register. If no match occurs, the data is discarded. If match occurs, data is transferred to the data buffer. This field must be cleared when C7816[ISO7816E] is set/enabled.

#define  UART1_C5_RDMAS_MASK         0x20u
#define  UART1_C5_RDMAS_SHIFT        5
//   Receiver Full DMA Select
//     0b0 ==> If C2[RIE] and S1[RDRF] are set, the RDFR interrupt request signal is asserted to request an interrupt service.
//     0b1 ==> If C2[RIE] and S1[RDRF] are set, the RDRF DMA request signal is asserted to request a DMA transfer.

#define  UART1_C5_TDMAS_MASK         0x80u
#define  UART1_C5_TDMAS_SHIFT        7
//   Transmitter DMA Select
//     0b0 ==> If C2[TIE] is set and the S1[TDRE] flag is set, the TDRE interrupt request signal is asserted to request interrupt service.
//     0b1 ==> If C2[TIE] is set and the S1[TDRE] flag is set, the TDRE DMA request signal is asserted to request a DMA transfer.

#define  UART1_IR_TNP_MASK           0x2u
#define  UART1_IR_TNP_SHIFT          0
#define  UART1_IR_TNP(x)             (((uint8_t)(((uint8_t)(x))<<UART1_IR_TNP_SHIFT))&UART1_IR_TNP_MASK)
//   Transmitter narrow pulse
//     0b00 ==> 3/16.
//     0b01 ==> 1/16.
//     0b10 ==> 1/32.
//     0b11 ==> 1/4.

#define  UART1_IR_IREN_MASK          0x4u
#define  UART1_IR_IREN_SHIFT         2
//   Infrared enable
//     0b0 ==> IR disabled.
//     0b1 ==> IR enabled.

#define  UART1_C7816_ISO_7816E_MASK  0x1u
#define  UART1_C7816_ISO_7816E_SHIFT 0
//   ISO-7816 Functionality Enabled
//     0b0 ==> ISO-7816 functionality is turned off/not enabled.
//     0b1 ==> ISO-7816 functionality is turned on/enabled.

#define  UART1_C7816_TTYPE_MASK      0x2u
#define  UART1_C7816_TTYPE_SHIFT     1
//   Transfer Type
//     0b0 ==> T = 0 per the ISO-7816 specification.
//     0b1 ==> T = 1 per the ISO-7816 specification.

#define  UART1_C7816_INIT_MASK       0x4u
#define  UART1_C7816_INIT_SHIFT      2
//   Detect Initial Character
//     0b0 ==> Normal operating mode. Receiver does not seek to identify initial character.
//     0b1 ==> Receiver searches for initial character.

#define  UART1_C7816_ANACK_MASK      0x8u
#define  UART1_C7816_ANACK_SHIFT     3
//   Generate NACK on Error
//     0b0 ==> No NACK is automatically generated.
//     0b1 ==> A NACK is automatically generated if a parity error is detected or if an invalid initial character is detected.

#define  UART1_C7816_ONACK_MASK      0x10u
#define  UART1_C7816_ONACK_SHIFT     4
//   Generate NACK on Overflow
//     0b0 ==> The received data does not generate a NACK when the receipt of the data results in an overflow event.
//     0b1 ==> If the receiver buffer overflows, a NACK is automatically sent on a received character.

#define  UART1_IE7816_RXTE_MASK      0x1u
#define  UART1_IE7816_RXTE_SHIFT     0
//   Receive Threshold Exceeded Interrupt Enable
//     0b0 ==> The assertion of IS7816[RXT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[RXT] results in the generation of an interrupt.

#define  UART1_IE7816_TXTE_MASK      0x2u
#define  UART1_IE7816_TXTE_SHIFT     1
//   Transmit Threshold Exceeded Interrupt Enable
//     0b0 ==> The assertion of IS7816[TXT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[TXT] results in the generation of an interrupt.

#define  UART1_IE7816_GTVE_MASK      0x4u
#define  UART1_IE7816_GTVE_SHIFT     2
//   Guard Timer Violated Interrupt Enable
//     0b0 ==> The assertion of IS7816[GTV] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[GTV] results in the generation of an interrupt.

#define  UART1_IE7816_INITDE_MASK    0x10u
#define  UART1_IE7816_INITDE_SHIFT   4
//   Initial Character Detected Interrupt Enable
//     0b0 ==> The assertion of IS7816[INITD] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[INITD] results in the generation of an interrupt.

#define  UART1_IE7816_BWTE_MASK      0x20u
#define  UART1_IE7816_BWTE_SHIFT     5
//   Block Wait Timer Interrupt Enable
//     0b0 ==> The assertion of IS7816[BWT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[BWT] results in the generation of an interrupt.

#define  UART1_IE7816_CWTE_MASK      0x40u
#define  UART1_IE7816_CWTE_SHIFT     6
//   Character Wait Timer Interrupt Enable
//     0b0 ==> The assertion of IS7816[CWT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[CWT] results in the generation of an interrupt.

#define  UART1_IE7816_WTE_MASK       0x80u
#define  UART1_IE7816_WTE_SHIFT      7
//   Wait Timer Interrupt Enable
//     0b0 ==> The assertion of IS7816[WT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[WT] results in the generation of an interrupt.

#define  UART1_IS7816_RXT_MASK       0x1u
#define  UART1_IS7816_RXT_SHIFT      0
//   Receive Threshold Exceeded Interrupt
//     0b0 ==> The number of consecutive NACKS generated as a result of parity errors and buffer overruns is less than or equal to the value in ET7816[RXTHRESHOLD].
//     0b1 ==> The number of consecutive NACKS generated as a result of parity errors and buffer overruns is greater than the value in ET7816[RXTHRESHOLD].

#define  UART1_IS7816_TXT_MASK       0x2u
#define  UART1_IS7816_TXT_SHIFT      1
//   Transmit Threshold Exceeded Interrupt
//     0b0 ==> The number of retries and corresponding NACKS does not exceed the value in ET7816[TXTHRESHOLD].
//     0b1 ==> The number of retries and corresponding NACKS exceeds the value in ET7816[TXTHRESHOLD].

#define  UART1_IS7816_GTV_MASK       0x4u
#define  UART1_IS7816_GTV_SHIFT      2
//   Guard Timer Violated Interrupt
//     0b0 ==> A guard time (GT, CGT, or BGT) has not been violated.
//     0b1 ==> A guard time (GT, CGT, or BGT) has been violated.

#define  UART1_IS7816_INITD_MASK     0x10u
#define  UART1_IS7816_INITD_SHIFT    4
//   Initial Character Detected Interrupt
//     0b0 ==> A valid initial character has not been received.
//     0b1 ==> A valid initial character has been received.

#define  UART1_IS7816_BWT_MASK       0x20u
#define  UART1_IS7816_BWT_SHIFT      5
//   Block Wait Timer Interrupt
//     0b0 ==> Block wait time (BWT) has not been violated.
//     0b1 ==> Block wait time (BWT) has been violated.

#define  UART1_IS7816_CWT_MASK       0x40u
#define  UART1_IS7816_CWT_SHIFT      6
//   Character Wait Timer Interrupt
//     0b0 ==> Character wait time (CWT) has not been violated.
//     0b1 ==> Character wait time (CWT) has been violated.

#define  UART1_IS7816_WT_MASK        0x80u
#define  UART1_IS7816_WT_SHIFT       7
//   Wait Timer Interrupt
//     0b0 ==> Wait time (WT) has not been violated.
//     0b1 ==> Wait time (WT) has been violated.

#define  UART1_WP7816T0_WI_MASK      0x8u
#define  UART1_WP7816T0_WI_SHIFT     0
#define  UART1_WP7816T0_WI(x)        (((uint8_t)(((uint8_t)(x))<<UART1_WP7816T0_WI_SHIFT))&UART1_WP7816T0_WI_MASK)
//   Wait Timer Interrupt (C7816[TTYPE] = 0)

#define  UART1_WP7816T1_BWI_MASK     0x4u
#define  UART1_WP7816T1_BWI_SHIFT    0
#define  UART1_WP7816T1_BWI(x)       (((uint8_t)(((uint8_t)(x))<<UART1_WP7816T1_BWI_SHIFT))&UART1_WP7816T1_BWI_MASK)
//   Block Wait Time Integer(C7816[TTYPE] = 1)

#define  UART1_WP7816T1_CWI_MASK     0x40u
#define  UART1_WP7816T1_CWI_SHIFT    4
#define  UART1_WP7816T1_CWI(x)       (((uint8_t)(((uint8_t)(x))<<UART1_WP7816T1_CWI_SHIFT))&UART1_WP7816T1_CWI_MASK)
//   Character Wait Time Integer (C7816[TTYPE] = 1)

#define  UART1_WN7816_GTN_MASK       0x8u
#define  UART1_WN7816_GTN_SHIFT      0
#define  UART1_WN7816_GTN(x)         (((uint8_t)(((uint8_t)(x))<<UART1_WN7816_GTN_SHIFT))&UART1_WN7816_GTN_MASK)
//   Guard Band N

#define  UART1_WF7816_GTFD_MASK      0x8u
#define  UART1_WF7816_GTFD_SHIFT     0
#define  UART1_WF7816_GTFD(x)        (((uint8_t)(((uint8_t)(x))<<UART1_WF7816_GTFD_SHIFT))&UART1_WF7816_GTFD_MASK)
//   FD Multiplier

#define  UART1_ET7816_RXTHRESHOLD_MASK 0x4u
#define  UART1_ET7816_RXTHRESHOLD_SHIFT 0
#define  UART1_ET7816_RXTHRESHOLD(x) (((uint8_t)(((uint8_t)(x))<<UART1_ET7816_RXTHRESHOLD_SHIFT))&UART1_ET7816_RXTHRESHOLD_MASK)
//   Receive NACK Threshold

#define  UART1_ET7816_TXTHRESHOLD_MASK 0x40u
#define  UART1_ET7816_TXTHRESHOLD_SHIFT 4
#define  UART1_ET7816_TXTHRESHOLD(x) (((uint8_t)(((uint8_t)(x))<<UART1_ET7816_TXTHRESHOLD_SHIFT))&UART1_ET7816_TXTHRESHOLD_MASK)
//   Transmit NACK Threshold
//     0b0 ==> TXT asserts on the first NACK that is received.
//     0b1 ==> TXT asserts on the second NACK that is received.

#define  UART1_TL7816_TLEN_MASK      0x8u
#define  UART1_TL7816_TLEN_SHIFT     0
#define  UART1_TL7816_TLEN(x)        (((uint8_t)(((uint8_t)(x))<<UART1_TL7816_TLEN_SHIFT))&UART1_TL7816_TLEN_MASK)
//   Transmit Length

#define  UART1_WB_WBASE_MASK         0x8u
#define  UART1_WB_WBASE_SHIFT        0
#define  UART1_WB_WBASE(x)           (((uint8_t)(((uint8_t)(x))<<UART1_WB_WBASE_SHIFT))&UART1_WB_WBASE_MASK)
//   CEA709.1-B WBASE register

#define  UART2_BDH_SBR_MASK          0x5u
#define  UART2_BDH_SBR_SHIFT         0
#define  UART2_BDH_SBR(x)            (((uint8_t)(((uint8_t)(x))<<UART2_BDH_SBR_SHIFT))&UART2_BDH_SBR_MASK)
//   UART Baud Rate Bits

#define  UART2_BDH_RXEDGIE_MASK      0x40u
#define  UART2_BDH_RXEDGIE_SHIFT     6
//   RxD Input Active Edge Interrupt Enable
//     0b0 ==> Hardware interrupts from RXEDGIF disabled using polling.
//     0b1 ==> RXEDGIF interrupt request enabled.

#define  UART2_BDH_LBKDIE_MASK       0x80u
#define  UART2_BDH_LBKDIE_SHIFT      7
//   LIN Break Detect Interrupt Enable
//     0b0 ==> LBKDIF interrupt requests disabled.
//     0b1 ==> LBKDIF interrupt requests enabled.

#define  UART2_BDL_SBR_MASK          0x8u
#define  UART2_BDL_SBR_SHIFT         0
#define  UART2_BDL_SBR(x)            (((uint8_t)(((uint8_t)(x))<<UART2_BDL_SBR_SHIFT))&UART2_BDL_SBR_MASK)
//   UART Baud Rate Bits

#define  UART2_C1_PT_MASK            0x1u
#define  UART2_C1_PT_SHIFT           0
//   Parity Type
//     0b0 ==> Even parity.
//     0b1 ==> Odd parity.

#define  UART2_C1_PE_MASK            0x2u
#define  UART2_C1_PE_SHIFT           1
//   Parity Enable
//     0b0 ==> Parity function disabled.
//     0b1 ==> Parity function enabled.

#define  UART2_C1_ILT_MASK           0x4u
#define  UART2_C1_ILT_SHIFT          2
//   Idle Line Type Select
//     0b0 ==> Idle character bit count starts after start bit.
//     0b1 ==> Idle character bit count starts after stop bit.

#define  UART2_C1_WAKE_MASK          0x8u
#define  UART2_C1_WAKE_SHIFT         3
//   Receiver Wakeup Method Select
//     0b0 ==> Idle line wakeup.
//     0b1 ==> Address mark wakeup.

#define  UART2_C1_M_MASK             0x10u
#define  UART2_C1_M_SHIFT            4
//   9-bit or 8-bit Mode Select
//     0b0 ==> Normal-start + 8 data bits (MSB/LSB first as determined by MSBF) + stop.
//     0b1 ==> Use-start + 9 data bits (MSB/LSB first as determined by MSBF) + stop.

#define  UART2_C1_RSRC_MASK          0x20u
#define  UART2_C1_RSRC_SHIFT         5
//   Receiver Source Select
//     0b0 ==> Selects internal loop back mode. The receiver input is internally connected to transmitter output.
//     0b1 ==> Single wire UART mode where the receiver input is connected to the transmit pin input signal.

#define  UART2_C1_LOOPS_MASK         0x80u
#define  UART2_C1_LOOPS_SHIFT        7
//   Loop Mode Select
//     0b0 ==> Normal operation.
//     0b1 ==> Loop mode where transmitter output is internally connected to receiver input. The receiver input is determined by RSRC.

#define  UART2_C2_SBK_MASK           0x1u
#define  UART2_C2_SBK_SHIFT          0
//   Send Break
//     0b0 ==> Normal transmitter operation.
//     0b1 ==> Queue break characters to be sent.

#define  UART2_C2_RWU_MASK           0x2u
#define  UART2_C2_RWU_SHIFT          1
//   Receiver Wakeup Control
//     0b0 ==> Normal operation.
//     0b1 ==> RWU enables the wakeup function and inhibits further receiver interrupt requests. Normally, hardware wakes the receiver by automatically clearing RWU.

#define  UART2_C2_RE_MASK            0x4u
#define  UART2_C2_RE_SHIFT           2
//   Receiver Enable
//     0b0 ==> Receiver off.
//     0b1 ==> Receiver on.

#define  UART2_C2_TE_MASK            0x8u
#define  UART2_C2_TE_SHIFT           3
//   Transmitter Enable
//     0b0 ==> Transmitter off.
//     0b1 ==> Transmitter on.

#define  UART2_C2_ILIE_MASK          0x10u
#define  UART2_C2_ILIE_SHIFT         4
//   Idle Line Interrupt Enable
//     0b0 ==> IDLE interrupt requests disabled.
//     0b1 ==> IDLE interrupt requests enabled.

#define  UART2_C2_RIE_MASK           0x20u
#define  UART2_C2_RIE_SHIFT          5
//   Receiver Full Interrupt or DMA Transfer Enable
//     0b0 ==> RDRF interrupt and DMA transfer requests disabled.
//     0b1 ==> RDRF interrupt or DMA transfer requests enabled.

#define  UART2_C2_TCIE_MASK          0x40u
#define  UART2_C2_TCIE_SHIFT         6
//   Transmission Complete Interrupt Enable
//     0b0 ==> TC interrupt requests disabled.
//     0b1 ==> TC interrupt requests enabled.

#define  UART2_C2_TIE_MASK           0x80u
#define  UART2_C2_TIE_SHIFT          7
//   Transmitter Interrupt or DMA Transfer Enable.
//     0b0 ==> TDRE interrupt and DMA transfer requests disabled.
//     0b1 ==> TDRE interrupt or DMA transfer requests enabled.

#define  UART2_S1_PF_MASK            0x1u
#define  UART2_S1_PF_SHIFT           0
//   Parity Error Flag
//     0b0 ==> No parity error detected since the last time this flag was cleared. If the receive buffer has a depth greater than 1, then there may be data in the receive buffer what was received with a parity error.
//     0b1 ==> At least one dataword was received with a parity error since the last time this flag was cleared.

#define  UART2_S1_FE_MASK            0x2u
#define  UART2_S1_FE_SHIFT           1
//   Framing Error Flag
//     0b0 ==> No framing error detected.
//     0b1 ==> Framing error.

#define  UART2_S1_NF_MASK            0x4u
#define  UART2_S1_NF_SHIFT           2
//   Noise Flag
//     0b0 ==> No noise detected since the last time this flag was cleared. If the receive buffer has a depth greater than 1 then there may be data in the receiver buffer that was received with noise.
//     0b1 ==> At least one dataword was received with noise detected since the last time the flag was cleared.

#define  UART2_S1_OR_MASK            0x8u
#define  UART2_S1_OR_SHIFT           3
//   Receiver Overrun Flag
//     0b0 ==> No overrun has occurred since the last time the flag was cleared.
//     0b1 ==> Overrun has occurred or the overrun flag has not been cleared since the last overrun occured.

#define  UART2_S1_IDLE_MASK          0x10u
#define  UART2_S1_IDLE_SHIFT         4
//   Idle Line Flag
//     0b0 ==> Receiver input is either active now or has never become active since the IDLE flag was last cleared.
//     0b1 ==> Receiver input has become idle or the flag has not been cleared since it last asserted.

#define  UART2_S1_RDRF_MASK          0x20u
#define  UART2_S1_RDRF_SHIFT         5
//   Receive Data Register Full Flag
//     0b0 ==> The number of datawords in the receive buffer is less than the number indicated by RXWATER.
//     0b1 ==> The number of datawords in the receive buffer is equal to or greater than the number indicated by RXWATER at some point in time since this flag was last cleared.

#define  UART2_S1_TC_MASK            0x40u
#define  UART2_S1_TC_SHIFT           6
//   Transmit Complete Flag
//     0b0 ==> Transmitter active (sending data, a preamble, or a break).
//     0b1 ==> Transmitter idle (transmission activity complete).

#define  UART2_S1_TDRE_MASK          0x80u
#define  UART2_S1_TDRE_SHIFT         7
//   Transmit Data Register Empty Flag
//     0b0 ==> The amount of data in the transmit buffer is greater than the value indicated by TWFIFO[TXWATER].
//     0b1 ==> The amount of data in the transmit buffer is less than or equal to the value indicated by TWFIFO[TXWATER] at some point in time since the flag has been cleared.

#define  UART2_S2_RAF_MASK           0x1u
#define  UART2_S2_RAF_SHIFT          0
//   Receiver Active Flag
//     0b0 ==> UART receiver idle/inactive waiting for a start bit.
//     0b1 ==> UART receiver active, RxD input not idle.

#define  UART2_S2_LBKDE_MASK         0x2u
#define  UART2_S2_LBKDE_SHIFT        1
//   LIN Break Detection Enable
//     0b0 ==> Break character is detected at one of the following lengths: 10 bit times if C1[M] = 0 11 bit times if C1[M] = 1 and C4[M10] = 0 12 bit times if C1[M] = 1, C4[M10] = 1, and S1[PE] = 1
//     0b1 ==> Break character is detected at length of 11 bit times if C1[M] = 0 or 12 bits time if C1[M] = 1.

#define  UART2_S2_BRK13_MASK         0x4u
#define  UART2_S2_BRK13_SHIFT        2
//   Break Transmit Character Length
//     0b0 ==> Break character is 10, 11, or 12 bits long.
//     0b1 ==> Break character is 13 or 14 bits long.

#define  UART2_S2_RWUID_MASK         0x8u
#define  UART2_S2_RWUID_SHIFT        3
//   Receive Wakeup Idle Detect
//     0b0 ==> S1[IDLE] is not set upon detection of an idle character.
//     0b1 ==> S1[IDLE] is set upon detection of an idle character.

#define  UART2_S2_RXINV_MASK         0x10u
#define  UART2_S2_RXINV_SHIFT        4
//   Receive Data Inversion
//     0b0 ==> Receive data is not inverted.
//     0b1 ==> Receive data is inverted.

#define  UART2_S2_MSBF_MASK          0x20u
#define  UART2_S2_MSBF_SHIFT         5
//   Most Significant Bit First
//     0b0 ==> LSB (bit0) is the first bit that is transmitted following the start bit. Further, the first bit received after the start bit is identified as bit0.
//     0b1 ==> MSB (bit8, bit7 or bit6) is the first bit that is transmitted following the start bit, depending on the setting of C1[M] and C1[PE]. Further, the first bit received after the start bit is identified as bit8, bit7, or bit6, depending on the setting of C1[M] and C1[PE].

#define  UART2_S2_RXEDGIF_MASK       0x40u
#define  UART2_S2_RXEDGIF_SHIFT      6
//   RxD Pin Active Edge Interrupt Flag
//     0b0 ==> No active edge on the receive pin has occurred.
//     0b1 ==> An active edge on the receive pin has occurred.

#define  UART2_S2_LBKDIF_MASK        0x80u
#define  UART2_S2_LBKDIF_SHIFT       7
//   LIN Break Detect Interrupt Flag
//     0b0 ==> No LIN break character detected.
//     0b1 ==> LIN break character detected.

#define  UART2_C3_PEIE_MASK          0x1u
#define  UART2_C3_PEIE_SHIFT         0
//   Parity Error Interrupt Enable
//     0b0 ==> PF interrupt requests are disabled.
//     0b1 ==> PF interrupt requests are enabled.

#define  UART2_C3_FEIE_MASK          0x2u
#define  UART2_C3_FEIE_SHIFT         1
//   Framing Error Interrupt Enable
//     0b0 ==> FE interrupt requests are disabled.
//     0b1 ==> FE interrupt requests are enabled.

#define  UART2_C3_NEIE_MASK          0x4u
#define  UART2_C3_NEIE_SHIFT         2
//   Noise Error Interrupt Enable
//     0b0 ==> NF interrupt requests are disabled.
//     0b1 ==> NF interrupt requests are enabled.

#define  UART2_C3_ORIE_MASK          0x8u
#define  UART2_C3_ORIE_SHIFT         3
//   Overrun Error Interrupt Enable
//     0b0 ==> OR interrupts are disabled.
//     0b1 ==> OR interrupt requests are enabled.

#define  UART2_C3_TXINV_MASK         0x10u
#define  UART2_C3_TXINV_SHIFT        4
//   Transmit Data Inversion.
//     0b0 ==> Transmit data is not inverted.
//     0b1 ==> Transmit data is inverted.

#define  UART2_C3_TXDIR_MASK         0x20u
#define  UART2_C3_TXDIR_SHIFT        5
//   Transmitter Pin Data Direction in Single-Wire mode
//     0b0 ==> TXD pin is an input in single wire mode.
//     0b1 ==> TXD pin is an output in single wire mode.

#define  UART2_C3_T8_MASK            0x40u
#define  UART2_C3_T8_SHIFT           6
//   Transmit Bit 8

#define  UART2_C3_R8_MASK            0x80u
#define  UART2_C3_R8_SHIFT           7
//   Received Bit 8

#define  UART2_D_RT_MASK             0x8u
#define  UART2_D_RT_SHIFT            0
#define  UART2_D_RT(x)               (((uint8_t)(((uint8_t)(x))<<UART2_D_RT_SHIFT))&UART2_D_RT_MASK)

#define  UART2_MA1_MA_MASK           0x8u
#define  UART2_MA1_MA_SHIFT          0
#define  UART2_MA1_MA(x)             (((uint8_t)(((uint8_t)(x))<<UART2_MA1_MA_SHIFT))&UART2_MA1_MA_MASK)
//   Match Address

#define  UART2_MA2_MA_MASK           0x8u
#define  UART2_MA2_MA_SHIFT          0
#define  UART2_MA2_MA(x)             (((uint8_t)(((uint8_t)(x))<<UART2_MA2_MA_SHIFT))&UART2_MA2_MA_MASK)
//   Match Address

#define  UART2_C4_BRFA_MASK          0x5u
#define  UART2_C4_BRFA_SHIFT         0
#define  UART2_C4_BRFA(x)            (((uint8_t)(((uint8_t)(x))<<UART2_C4_BRFA_SHIFT))&UART2_C4_BRFA_MASK)
//   Baud Rate Fine Adjust

#define  UART2_C4_M10_MASK           0x20u
#define  UART2_C4_M10_SHIFT          5
//   10-bit Mode select
//     0b0 ==> The parity bit is the ninth bit in the serial transmission.
//     0b1 ==> The parity bit is the tenth bit in the serial transmission.

#define  UART2_C4_MAEN2_MASK         0x40u
#define  UART2_C4_MAEN2_SHIFT        6
//   Match Address Mode Enable 2
//     0b0 ==> All data received is transferred to the data buffer if MAEN1 is cleared.
//     0b1 ==> All data received with the most significant bit cleared, is discarded. All data received with the most significant bit set, is compared with contents of MA2 register. If no match occurs, the data is discarded. If a match occurs, data is transferred to the data buffer. This field must be cleared when C7816[ISO7816E] is set/enabled.

#define  UART2_C4_MAEN1_MASK         0x80u
#define  UART2_C4_MAEN1_SHIFT        7
//   Match Address Mode Enable 1
//     0b0 ==> All data received is transferred to the data buffer if MAEN2 is cleared.
//     0b1 ==> All data received with the most significant bit cleared, is discarded. All data received with the most significant bit set, is compared with contents of MA1 register. If no match occurs, the data is discarded. If match occurs, data is transferred to the data buffer. This field must be cleared when C7816[ISO7816E] is set/enabled.

#define  UART2_C5_RDMAS_MASK         0x20u
#define  UART2_C5_RDMAS_SHIFT        5
//   Receiver Full DMA Select
//     0b0 ==> If C2[RIE] and S1[RDRF] are set, the RDFR interrupt request signal is asserted to request an interrupt service.
//     0b1 ==> If C2[RIE] and S1[RDRF] are set, the RDRF DMA request signal is asserted to request a DMA transfer.

#define  UART2_C5_TDMAS_MASK         0x80u
#define  UART2_C5_TDMAS_SHIFT        7
//   Transmitter DMA Select
//     0b0 ==> If C2[TIE] is set and the S1[TDRE] flag is set, the TDRE interrupt request signal is asserted to request interrupt service.
//     0b1 ==> If C2[TIE] is set and the S1[TDRE] flag is set, the TDRE DMA request signal is asserted to request a DMA transfer.

#define  UART2_IR_TNP_MASK           0x2u
#define  UART2_IR_TNP_SHIFT          0
#define  UART2_IR_TNP(x)             (((uint8_t)(((uint8_t)(x))<<UART2_IR_TNP_SHIFT))&UART2_IR_TNP_MASK)
//   Transmitter narrow pulse
//     0b00 ==> 3/16.
//     0b01 ==> 1/16.
//     0b10 ==> 1/32.
//     0b11 ==> 1/4.

#define  UART2_IR_IREN_MASK          0x4u
#define  UART2_IR_IREN_SHIFT         2
//   Infrared enable
//     0b0 ==> IR disabled.
//     0b1 ==> IR enabled.

#define  UART2_C7816_ISO_7816E_MASK  0x1u
#define  UART2_C7816_ISO_7816E_SHIFT 0
//   ISO-7816 Functionality Enabled
//     0b0 ==> ISO-7816 functionality is turned off/not enabled.
//     0b1 ==> ISO-7816 functionality is turned on/enabled.

#define  UART2_C7816_TTYPE_MASK      0x2u
#define  UART2_C7816_TTYPE_SHIFT     1
//   Transfer Type
//     0b0 ==> T = 0 per the ISO-7816 specification.
//     0b1 ==> T = 1 per the ISO-7816 specification.

#define  UART2_C7816_INIT_MASK       0x4u
#define  UART2_C7816_INIT_SHIFT      2
//   Detect Initial Character
//     0b0 ==> Normal operating mode. Receiver does not seek to identify initial character.
//     0b1 ==> Receiver searches for initial character.

#define  UART2_C7816_ANACK_MASK      0x8u
#define  UART2_C7816_ANACK_SHIFT     3
//   Generate NACK on Error
//     0b0 ==> No NACK is automatically generated.
//     0b1 ==> A NACK is automatically generated if a parity error is detected or if an invalid initial character is detected.

#define  UART2_C7816_ONACK_MASK      0x10u
#define  UART2_C7816_ONACK_SHIFT     4
//   Generate NACK on Overflow
//     0b0 ==> The received data does not generate a NACK when the receipt of the data results in an overflow event.
//     0b1 ==> If the receiver buffer overflows, a NACK is automatically sent on a received character.

#define  UART2_IE7816_RXTE_MASK      0x1u
#define  UART2_IE7816_RXTE_SHIFT     0
//   Receive Threshold Exceeded Interrupt Enable
//     0b0 ==> The assertion of IS7816[RXT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[RXT] results in the generation of an interrupt.

#define  UART2_IE7816_TXTE_MASK      0x2u
#define  UART2_IE7816_TXTE_SHIFT     1
//   Transmit Threshold Exceeded Interrupt Enable
//     0b0 ==> The assertion of IS7816[TXT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[TXT] results in the generation of an interrupt.

#define  UART2_IE7816_GTVE_MASK      0x4u
#define  UART2_IE7816_GTVE_SHIFT     2
//   Guard Timer Violated Interrupt Enable
//     0b0 ==> The assertion of IS7816[GTV] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[GTV] results in the generation of an interrupt.

#define  UART2_IE7816_INITDE_MASK    0x10u
#define  UART2_IE7816_INITDE_SHIFT   4
//   Initial Character Detected Interrupt Enable
//     0b0 ==> The assertion of IS7816[INITD] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[INITD] results in the generation of an interrupt.

#define  UART2_IE7816_BWTE_MASK      0x20u
#define  UART2_IE7816_BWTE_SHIFT     5
//   Block Wait Timer Interrupt Enable
//     0b0 ==> The assertion of IS7816[BWT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[BWT] results in the generation of an interrupt.

#define  UART2_IE7816_CWTE_MASK      0x40u
#define  UART2_IE7816_CWTE_SHIFT     6
//   Character Wait Timer Interrupt Enable
//     0b0 ==> The assertion of IS7816[CWT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[CWT] results in the generation of an interrupt.

#define  UART2_IE7816_WTE_MASK       0x80u
#define  UART2_IE7816_WTE_SHIFT      7
//   Wait Timer Interrupt Enable
//     0b0 ==> The assertion of IS7816[WT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[WT] results in the generation of an interrupt.

#define  UART2_IS7816_RXT_MASK       0x1u
#define  UART2_IS7816_RXT_SHIFT      0
//   Receive Threshold Exceeded Interrupt
//     0b0 ==> The number of consecutive NACKS generated as a result of parity errors and buffer overruns is less than or equal to the value in ET7816[RXTHRESHOLD].
//     0b1 ==> The number of consecutive NACKS generated as a result of parity errors and buffer overruns is greater than the value in ET7816[RXTHRESHOLD].

#define  UART2_IS7816_TXT_MASK       0x2u
#define  UART2_IS7816_TXT_SHIFT      1
//   Transmit Threshold Exceeded Interrupt
//     0b0 ==> The number of retries and corresponding NACKS does not exceed the value in ET7816[TXTHRESHOLD].
//     0b1 ==> The number of retries and corresponding NACKS exceeds the value in ET7816[TXTHRESHOLD].

#define  UART2_IS7816_GTV_MASK       0x4u
#define  UART2_IS7816_GTV_SHIFT      2
//   Guard Timer Violated Interrupt
//     0b0 ==> A guard time (GT, CGT, or BGT) has not been violated.
//     0b1 ==> A guard time (GT, CGT, or BGT) has been violated.

#define  UART2_IS7816_INITD_MASK     0x10u
#define  UART2_IS7816_INITD_SHIFT    4
//   Initial Character Detected Interrupt
//     0b0 ==> A valid initial character has not been received.
//     0b1 ==> A valid initial character has been received.

#define  UART2_IS7816_BWT_MASK       0x20u
#define  UART2_IS7816_BWT_SHIFT      5
//   Block Wait Timer Interrupt
//     0b0 ==> Block wait time (BWT) has not been violated.
//     0b1 ==> Block wait time (BWT) has been violated.

#define  UART2_IS7816_CWT_MASK       0x40u
#define  UART2_IS7816_CWT_SHIFT      6
//   Character Wait Timer Interrupt
//     0b0 ==> Character wait time (CWT) has not been violated.
//     0b1 ==> Character wait time (CWT) has been violated.

#define  UART2_IS7816_WT_MASK        0x80u
#define  UART2_IS7816_WT_SHIFT       7
//   Wait Timer Interrupt
//     0b0 ==> Wait time (WT) has not been violated.
//     0b1 ==> Wait time (WT) has been violated.

#define  UART2_WP7816T0_WI_MASK      0x8u
#define  UART2_WP7816T0_WI_SHIFT     0
#define  UART2_WP7816T0_WI(x)        (((uint8_t)(((uint8_t)(x))<<UART2_WP7816T0_WI_SHIFT))&UART2_WP7816T0_WI_MASK)
//   Wait Timer Interrupt (C7816[TTYPE] = 0)

#define  UART2_WP7816T1_BWI_MASK     0x4u
#define  UART2_WP7816T1_BWI_SHIFT    0
#define  UART2_WP7816T1_BWI(x)       (((uint8_t)(((uint8_t)(x))<<UART2_WP7816T1_BWI_SHIFT))&UART2_WP7816T1_BWI_MASK)
//   Block Wait Time Integer(C7816[TTYPE] = 1)

#define  UART2_WP7816T1_CWI_MASK     0x40u
#define  UART2_WP7816T1_CWI_SHIFT    4
#define  UART2_WP7816T1_CWI(x)       (((uint8_t)(((uint8_t)(x))<<UART2_WP7816T1_CWI_SHIFT))&UART2_WP7816T1_CWI_MASK)
//   Character Wait Time Integer (C7816[TTYPE] = 1)

#define  UART2_WN7816_GTN_MASK       0x8u
#define  UART2_WN7816_GTN_SHIFT      0
#define  UART2_WN7816_GTN(x)         (((uint8_t)(((uint8_t)(x))<<UART2_WN7816_GTN_SHIFT))&UART2_WN7816_GTN_MASK)
//   Guard Band N

#define  UART2_WF7816_GTFD_MASK      0x8u
#define  UART2_WF7816_GTFD_SHIFT     0
#define  UART2_WF7816_GTFD(x)        (((uint8_t)(((uint8_t)(x))<<UART2_WF7816_GTFD_SHIFT))&UART2_WF7816_GTFD_MASK)
//   FD Multiplier

#define  UART2_ET7816_RXTHRESHOLD_MASK 0x4u
#define  UART2_ET7816_RXTHRESHOLD_SHIFT 0
#define  UART2_ET7816_RXTHRESHOLD(x) (((uint8_t)(((uint8_t)(x))<<UART2_ET7816_RXTHRESHOLD_SHIFT))&UART2_ET7816_RXTHRESHOLD_MASK)
//   Receive NACK Threshold

#define  UART2_ET7816_TXTHRESHOLD_MASK 0x40u
#define  UART2_ET7816_TXTHRESHOLD_SHIFT 4
#define  UART2_ET7816_TXTHRESHOLD(x) (((uint8_t)(((uint8_t)(x))<<UART2_ET7816_TXTHRESHOLD_SHIFT))&UART2_ET7816_TXTHRESHOLD_MASK)
//   Transmit NACK Threshold
//     0b0 ==> TXT asserts on the first NACK that is received.
//     0b1 ==> TXT asserts on the second NACK that is received.

#define  UART2_TL7816_TLEN_MASK      0x8u
#define  UART2_TL7816_TLEN_SHIFT     0
#define  UART2_TL7816_TLEN(x)        (((uint8_t)(((uint8_t)(x))<<UART2_TL7816_TLEN_SHIFT))&UART2_TL7816_TLEN_MASK)
//   Transmit Length

#define  UART2_WB_WBASE_MASK         0x8u
#define  UART2_WB_WBASE_SHIFT        0
#define  UART2_WB_WBASE(x)           (((uint8_t)(((uint8_t)(x))<<UART2_WB_WBASE_SHIFT))&UART2_WB_WBASE_MASK)
//   CEA709.1-B WBASE register

#define  UART3_BDH_SBR_MASK          0x5u
#define  UART3_BDH_SBR_SHIFT         0
#define  UART3_BDH_SBR(x)            (((uint8_t)(((uint8_t)(x))<<UART3_BDH_SBR_SHIFT))&UART3_BDH_SBR_MASK)
//   UART Baud Rate Bits

#define  UART3_BDH_RXEDGIE_MASK      0x40u
#define  UART3_BDH_RXEDGIE_SHIFT     6
//   RxD Input Active Edge Interrupt Enable
//     0b0 ==> Hardware interrupts from RXEDGIF disabled using polling.
//     0b1 ==> RXEDGIF interrupt request enabled.

#define  UART3_BDH_LBKDIE_MASK       0x80u
#define  UART3_BDH_LBKDIE_SHIFT      7
//   LIN Break Detect Interrupt Enable
//     0b0 ==> LBKDIF interrupt requests disabled.
//     0b1 ==> LBKDIF interrupt requests enabled.

#define  UART3_BDL_SBR_MASK          0x8u
#define  UART3_BDL_SBR_SHIFT         0
#define  UART3_BDL_SBR(x)            (((uint8_t)(((uint8_t)(x))<<UART3_BDL_SBR_SHIFT))&UART3_BDL_SBR_MASK)
//   UART Baud Rate Bits

#define  UART3_C1_PT_MASK            0x1u
#define  UART3_C1_PT_SHIFT           0
//   Parity Type
//     0b0 ==> Even parity.
//     0b1 ==> Odd parity.

#define  UART3_C1_PE_MASK            0x2u
#define  UART3_C1_PE_SHIFT           1
//   Parity Enable
//     0b0 ==> Parity function disabled.
//     0b1 ==> Parity function enabled.

#define  UART3_C1_ILT_MASK           0x4u
#define  UART3_C1_ILT_SHIFT          2
//   Idle Line Type Select
//     0b0 ==> Idle character bit count starts after start bit.
//     0b1 ==> Idle character bit count starts after stop bit.

#define  UART3_C1_WAKE_MASK          0x8u
#define  UART3_C1_WAKE_SHIFT         3
//   Receiver Wakeup Method Select
//     0b0 ==> Idle line wakeup.
//     0b1 ==> Address mark wakeup.

#define  UART3_C1_M_MASK             0x10u
#define  UART3_C1_M_SHIFT            4
//   9-bit or 8-bit Mode Select
//     0b0 ==> Normal-start + 8 data bits (MSB/LSB first as determined by MSBF) + stop.
//     0b1 ==> Use-start + 9 data bits (MSB/LSB first as determined by MSBF) + stop.

#define  UART3_C1_RSRC_MASK          0x20u
#define  UART3_C1_RSRC_SHIFT         5
//   Receiver Source Select
//     0b0 ==> Selects internal loop back mode. The receiver input is internally connected to transmitter output.
//     0b1 ==> Single wire UART mode where the receiver input is connected to the transmit pin input signal.

#define  UART3_C1_LOOPS_MASK         0x80u
#define  UART3_C1_LOOPS_SHIFT        7
//   Loop Mode Select
//     0b0 ==> Normal operation.
//     0b1 ==> Loop mode where transmitter output is internally connected to receiver input. The receiver input is determined by RSRC.

#define  UART3_C2_SBK_MASK           0x1u
#define  UART3_C2_SBK_SHIFT          0
//   Send Break
//     0b0 ==> Normal transmitter operation.
//     0b1 ==> Queue break characters to be sent.

#define  UART3_C2_RWU_MASK           0x2u
#define  UART3_C2_RWU_SHIFT          1
//   Receiver Wakeup Control
//     0b0 ==> Normal operation.
//     0b1 ==> RWU enables the wakeup function and inhibits further receiver interrupt requests. Normally, hardware wakes the receiver by automatically clearing RWU.

#define  UART3_C2_RE_MASK            0x4u
#define  UART3_C2_RE_SHIFT           2
//   Receiver Enable
//     0b0 ==> Receiver off.
//     0b1 ==> Receiver on.

#define  UART3_C2_TE_MASK            0x8u
#define  UART3_C2_TE_SHIFT           3
//   Transmitter Enable
//     0b0 ==> Transmitter off.
//     0b1 ==> Transmitter on.

#define  UART3_C2_ILIE_MASK          0x10u
#define  UART3_C2_ILIE_SHIFT         4
//   Idle Line Interrupt Enable
//     0b0 ==> IDLE interrupt requests disabled.
//     0b1 ==> IDLE interrupt requests enabled.

#define  UART3_C2_RIE_MASK           0x20u
#define  UART3_C2_RIE_SHIFT          5
//   Receiver Full Interrupt or DMA Transfer Enable
//     0b0 ==> RDRF interrupt and DMA transfer requests disabled.
//     0b1 ==> RDRF interrupt or DMA transfer requests enabled.

#define  UART3_C2_TCIE_MASK          0x40u
#define  UART3_C2_TCIE_SHIFT         6
//   Transmission Complete Interrupt Enable
//     0b0 ==> TC interrupt requests disabled.
//     0b1 ==> TC interrupt requests enabled.

#define  UART3_C2_TIE_MASK           0x80u
#define  UART3_C2_TIE_SHIFT          7
//   Transmitter Interrupt or DMA Transfer Enable.
//     0b0 ==> TDRE interrupt and DMA transfer requests disabled.
//     0b1 ==> TDRE interrupt or DMA transfer requests enabled.

#define  UART3_S1_PF_MASK            0x1u
#define  UART3_S1_PF_SHIFT           0
//   Parity Error Flag
//     0b0 ==> No parity error detected since the last time this flag was cleared. If the receive buffer has a depth greater than 1, then there may be data in the receive buffer what was received with a parity error.
//     0b1 ==> At least one dataword was received with a parity error since the last time this flag was cleared.

#define  UART3_S1_FE_MASK            0x2u
#define  UART3_S1_FE_SHIFT           1
//   Framing Error Flag
//     0b0 ==> No framing error detected.
//     0b1 ==> Framing error.

#define  UART3_S1_NF_MASK            0x4u
#define  UART3_S1_NF_SHIFT           2
//   Noise Flag
//     0b0 ==> No noise detected since the last time this flag was cleared. If the receive buffer has a depth greater than 1 then there may be data in the receiver buffer that was received with noise.
//     0b1 ==> At least one dataword was received with noise detected since the last time the flag was cleared.

#define  UART3_S1_OR_MASK            0x8u
#define  UART3_S1_OR_SHIFT           3
//   Receiver Overrun Flag
//     0b0 ==> No overrun has occurred since the last time the flag was cleared.
//     0b1 ==> Overrun has occurred or the overrun flag has not been cleared since the last overrun occured.

#define  UART3_S1_IDLE_MASK          0x10u
#define  UART3_S1_IDLE_SHIFT         4
//   Idle Line Flag
//     0b0 ==> Receiver input is either active now or has never become active since the IDLE flag was last cleared.
//     0b1 ==> Receiver input has become idle or the flag has not been cleared since it last asserted.

#define  UART3_S1_RDRF_MASK          0x20u
#define  UART3_S1_RDRF_SHIFT         5
//   Receive Data Register Full Flag
//     0b0 ==> The number of datawords in the receive buffer is less than the number indicated by RXWATER.
//     0b1 ==> The number of datawords in the receive buffer is equal to or greater than the number indicated by RXWATER at some point in time since this flag was last cleared.

#define  UART3_S1_TC_MASK            0x40u
#define  UART3_S1_TC_SHIFT           6
//   Transmit Complete Flag
//     0b0 ==> Transmitter active (sending data, a preamble, or a break).
//     0b1 ==> Transmitter idle (transmission activity complete).

#define  UART3_S1_TDRE_MASK          0x80u
#define  UART3_S1_TDRE_SHIFT         7
//   Transmit Data Register Empty Flag
//     0b0 ==> The amount of data in the transmit buffer is greater than the value indicated by TWFIFO[TXWATER].
//     0b1 ==> The amount of data in the transmit buffer is less than or equal to the value indicated by TWFIFO[TXWATER] at some point in time since the flag has been cleared.

#define  UART3_S2_RAF_MASK           0x1u
#define  UART3_S2_RAF_SHIFT          0
//   Receiver Active Flag
//     0b0 ==> UART receiver idle/inactive waiting for a start bit.
//     0b1 ==> UART receiver active, RxD input not idle.

#define  UART3_S2_LBKDE_MASK         0x2u
#define  UART3_S2_LBKDE_SHIFT        1
//   LIN Break Detection Enable
//     0b0 ==> Break character is detected at one of the following lengths: 10 bit times if C1[M] = 0 11 bit times if C1[M] = 1 and C4[M10] = 0 12 bit times if C1[M] = 1, C4[M10] = 1, and S1[PE] = 1
//     0b1 ==> Break character is detected at length of 11 bit times if C1[M] = 0 or 12 bits time if C1[M] = 1.

#define  UART3_S2_BRK13_MASK         0x4u
#define  UART3_S2_BRK13_SHIFT        2
//   Break Transmit Character Length
//     0b0 ==> Break character is 10, 11, or 12 bits long.
//     0b1 ==> Break character is 13 or 14 bits long.

#define  UART3_S2_RWUID_MASK         0x8u
#define  UART3_S2_RWUID_SHIFT        3
//   Receive Wakeup Idle Detect
//     0b0 ==> S1[IDLE] is not set upon detection of an idle character.
//     0b1 ==> S1[IDLE] is set upon detection of an idle character.

#define  UART3_S2_RXINV_MASK         0x10u
#define  UART3_S2_RXINV_SHIFT        4
//   Receive Data Inversion
//     0b0 ==> Receive data is not inverted.
//     0b1 ==> Receive data is inverted.

#define  UART3_S2_MSBF_MASK          0x20u
#define  UART3_S2_MSBF_SHIFT         5
//   Most Significant Bit First
//     0b0 ==> LSB (bit0) is the first bit that is transmitted following the start bit. Further, the first bit received after the start bit is identified as bit0.
//     0b1 ==> MSB (bit8, bit7 or bit6) is the first bit that is transmitted following the start bit, depending on the setting of C1[M] and C1[PE]. Further, the first bit received after the start bit is identified as bit8, bit7, or bit6, depending on the setting of C1[M] and C1[PE].

#define  UART3_S2_RXEDGIF_MASK       0x40u
#define  UART3_S2_RXEDGIF_SHIFT      6
//   RxD Pin Active Edge Interrupt Flag
//     0b0 ==> No active edge on the receive pin has occurred.
//     0b1 ==> An active edge on the receive pin has occurred.

#define  UART3_S2_LBKDIF_MASK        0x80u
#define  UART3_S2_LBKDIF_SHIFT       7
//   LIN Break Detect Interrupt Flag
//     0b0 ==> No LIN break character detected.
//     0b1 ==> LIN break character detected.

#define  UART3_C3_PEIE_MASK          0x1u
#define  UART3_C3_PEIE_SHIFT         0
//   Parity Error Interrupt Enable
//     0b0 ==> PF interrupt requests are disabled.
//     0b1 ==> PF interrupt requests are enabled.

#define  UART3_C3_FEIE_MASK          0x2u
#define  UART3_C3_FEIE_SHIFT         1
//   Framing Error Interrupt Enable
//     0b0 ==> FE interrupt requests are disabled.
//     0b1 ==> FE interrupt requests are enabled.

#define  UART3_C3_NEIE_MASK          0x4u
#define  UART3_C3_NEIE_SHIFT         2
//   Noise Error Interrupt Enable
//     0b0 ==> NF interrupt requests are disabled.
//     0b1 ==> NF interrupt requests are enabled.

#define  UART3_C3_ORIE_MASK          0x8u
#define  UART3_C3_ORIE_SHIFT         3
//   Overrun Error Interrupt Enable
//     0b0 ==> OR interrupts are disabled.
//     0b1 ==> OR interrupt requests are enabled.

#define  UART3_C3_TXINV_MASK         0x10u
#define  UART3_C3_TXINV_SHIFT        4
//   Transmit Data Inversion.
//     0b0 ==> Transmit data is not inverted.
//     0b1 ==> Transmit data is inverted.

#define  UART3_C3_TXDIR_MASK         0x20u
#define  UART3_C3_TXDIR_SHIFT        5
//   Transmitter Pin Data Direction in Single-Wire mode
//     0b0 ==> TXD pin is an input in single wire mode.
//     0b1 ==> TXD pin is an output in single wire mode.

#define  UART3_C3_T8_MASK            0x40u
#define  UART3_C3_T8_SHIFT           6
//   Transmit Bit 8

#define  UART3_C3_R8_MASK            0x80u
#define  UART3_C3_R8_SHIFT           7
//   Received Bit 8

#define  UART3_D_RT_MASK             0x8u
#define  UART3_D_RT_SHIFT            0
#define  UART3_D_RT(x)               (((uint8_t)(((uint8_t)(x))<<UART3_D_RT_SHIFT))&UART3_D_RT_MASK)

#define  UART3_MA1_MA_MASK           0x8u
#define  UART3_MA1_MA_SHIFT          0
#define  UART3_MA1_MA(x)             (((uint8_t)(((uint8_t)(x))<<UART3_MA1_MA_SHIFT))&UART3_MA1_MA_MASK)
//   Match Address

#define  UART3_MA2_MA_MASK           0x8u
#define  UART3_MA2_MA_SHIFT          0
#define  UART3_MA2_MA(x)             (((uint8_t)(((uint8_t)(x))<<UART3_MA2_MA_SHIFT))&UART3_MA2_MA_MASK)
//   Match Address

#define  UART3_C4_BRFA_MASK          0x5u
#define  UART3_C4_BRFA_SHIFT         0
#define  UART3_C4_BRFA(x)            (((uint8_t)(((uint8_t)(x))<<UART3_C4_BRFA_SHIFT))&UART3_C4_BRFA_MASK)
//   Baud Rate Fine Adjust

#define  UART3_C4_M10_MASK           0x20u
#define  UART3_C4_M10_SHIFT          5
//   10-bit Mode select
//     0b0 ==> The parity bit is the ninth bit in the serial transmission.
//     0b1 ==> The parity bit is the tenth bit in the serial transmission.

#define  UART3_C4_MAEN2_MASK         0x40u
#define  UART3_C4_MAEN2_SHIFT        6
//   Match Address Mode Enable 2
//     0b0 ==> All data received is transferred to the data buffer if MAEN1 is cleared.
//     0b1 ==> All data received with the most significant bit cleared, is discarded. All data received with the most significant bit set, is compared with contents of MA2 register. If no match occurs, the data is discarded. If a match occurs, data is transferred to the data buffer. This field must be cleared when C7816[ISO7816E] is set/enabled.

#define  UART3_C4_MAEN1_MASK         0x80u
#define  UART3_C4_MAEN1_SHIFT        7
//   Match Address Mode Enable 1
//     0b0 ==> All data received is transferred to the data buffer if MAEN2 is cleared.
//     0b1 ==> All data received with the most significant bit cleared, is discarded. All data received with the most significant bit set, is compared with contents of MA1 register. If no match occurs, the data is discarded. If match occurs, data is transferred to the data buffer. This field must be cleared when C7816[ISO7816E] is set/enabled.

#define  UART3_C5_RDMAS_MASK         0x20u
#define  UART3_C5_RDMAS_SHIFT        5
//   Receiver Full DMA Select
//     0b0 ==> If C2[RIE] and S1[RDRF] are set, the RDFR interrupt request signal is asserted to request an interrupt service.
//     0b1 ==> If C2[RIE] and S1[RDRF] are set, the RDRF DMA request signal is asserted to request a DMA transfer.

#define  UART3_C5_TDMAS_MASK         0x80u
#define  UART3_C5_TDMAS_SHIFT        7
//   Transmitter DMA Select
//     0b0 ==> If C2[TIE] is set and the S1[TDRE] flag is set, the TDRE interrupt request signal is asserted to request interrupt service.
//     0b1 ==> If C2[TIE] is set and the S1[TDRE] flag is set, the TDRE DMA request signal is asserted to request a DMA transfer.

#define  UART3_IR_TNP_MASK           0x2u
#define  UART3_IR_TNP_SHIFT          0
#define  UART3_IR_TNP(x)             (((uint8_t)(((uint8_t)(x))<<UART3_IR_TNP_SHIFT))&UART3_IR_TNP_MASK)
//   Transmitter narrow pulse
//     0b00 ==> 3/16.
//     0b01 ==> 1/16.
//     0b10 ==> 1/32.
//     0b11 ==> 1/4.

#define  UART3_IR_IREN_MASK          0x4u
#define  UART3_IR_IREN_SHIFT         2
//   Infrared enable
//     0b0 ==> IR disabled.
//     0b1 ==> IR enabled.

#define  UART3_C7816_ISO_7816E_MASK  0x1u
#define  UART3_C7816_ISO_7816E_SHIFT 0
//   ISO-7816 Functionality Enabled
//     0b0 ==> ISO-7816 functionality is turned off/not enabled.
//     0b1 ==> ISO-7816 functionality is turned on/enabled.

#define  UART3_C7816_TTYPE_MASK      0x2u
#define  UART3_C7816_TTYPE_SHIFT     1
//   Transfer Type
//     0b0 ==> T = 0 per the ISO-7816 specification.
//     0b1 ==> T = 1 per the ISO-7816 specification.

#define  UART3_C7816_INIT_MASK       0x4u
#define  UART3_C7816_INIT_SHIFT      2
//   Detect Initial Character
//     0b0 ==> Normal operating mode. Receiver does not seek to identify initial character.
//     0b1 ==> Receiver searches for initial character.

#define  UART3_C7816_ANACK_MASK      0x8u
#define  UART3_C7816_ANACK_SHIFT     3
//   Generate NACK on Error
//     0b0 ==> No NACK is automatically generated.
//     0b1 ==> A NACK is automatically generated if a parity error is detected or if an invalid initial character is detected.

#define  UART3_C7816_ONACK_MASK      0x10u
#define  UART3_C7816_ONACK_SHIFT     4
//   Generate NACK on Overflow
//     0b0 ==> The received data does not generate a NACK when the receipt of the data results in an overflow event.
//     0b1 ==> If the receiver buffer overflows, a NACK is automatically sent on a received character.

#define  UART3_IE7816_RXTE_MASK      0x1u
#define  UART3_IE7816_RXTE_SHIFT     0
//   Receive Threshold Exceeded Interrupt Enable
//     0b0 ==> The assertion of IS7816[RXT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[RXT] results in the generation of an interrupt.

#define  UART3_IE7816_TXTE_MASK      0x2u
#define  UART3_IE7816_TXTE_SHIFT     1
//   Transmit Threshold Exceeded Interrupt Enable
//     0b0 ==> The assertion of IS7816[TXT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[TXT] results in the generation of an interrupt.

#define  UART3_IE7816_GTVE_MASK      0x4u
#define  UART3_IE7816_GTVE_SHIFT     2
//   Guard Timer Violated Interrupt Enable
//     0b0 ==> The assertion of IS7816[GTV] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[GTV] results in the generation of an interrupt.

#define  UART3_IE7816_INITDE_MASK    0x10u
#define  UART3_IE7816_INITDE_SHIFT   4
//   Initial Character Detected Interrupt Enable
//     0b0 ==> The assertion of IS7816[INITD] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[INITD] results in the generation of an interrupt.

#define  UART3_IE7816_BWTE_MASK      0x20u
#define  UART3_IE7816_BWTE_SHIFT     5
//   Block Wait Timer Interrupt Enable
//     0b0 ==> The assertion of IS7816[BWT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[BWT] results in the generation of an interrupt.

#define  UART3_IE7816_CWTE_MASK      0x40u
#define  UART3_IE7816_CWTE_SHIFT     6
//   Character Wait Timer Interrupt Enable
//     0b0 ==> The assertion of IS7816[CWT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[CWT] results in the generation of an interrupt.

#define  UART3_IE7816_WTE_MASK       0x80u
#define  UART3_IE7816_WTE_SHIFT      7
//   Wait Timer Interrupt Enable
//     0b0 ==> The assertion of IS7816[WT] does not result in the generation of an interrupt.
//     0b1 ==> The assertion of IS7816[WT] results in the generation of an interrupt.

#define  UART3_IS7816_RXT_MASK       0x1u
#define  UART3_IS7816_RXT_SHIFT      0
//   Receive Threshold Exceeded Interrupt
//     0b0 ==> The number of consecutive NACKS generated as a result of parity errors and buffer overruns is less than or equal to the value in ET7816[RXTHRESHOLD].
//     0b1 ==> The number of consecutive NACKS generated as a result of parity errors and buffer overruns is greater than the value in ET7816[RXTHRESHOLD].

#define  UART3_IS7816_TXT_MASK       0x2u
#define  UART3_IS7816_TXT_SHIFT      1
//   Transmit Threshold Exceeded Interrupt
//     0b0 ==> The number of retries and corresponding NACKS does not exceed the value in ET7816[TXTHRESHOLD].
//     0b1 ==> The number of retries and corresponding NACKS exceeds the value in ET7816[TXTHRESHOLD].

#define  UART3_IS7816_GTV_MASK       0x4u
#define  UART3_IS7816_GTV_SHIFT      2
//   Guard Timer Violated Interrupt
//     0b0 ==> A guard time (GT, CGT, or BGT) has not been violated.
//     0b1 ==> A guard time (GT, CGT, or BGT) has been violated.

#define  UART3_IS7816_INITD_MASK     0x10u
#define  UART3_IS7816_INITD_SHIFT    4
//   Initial Character Detected Interrupt
//     0b0 ==> A valid initial character has not been received.
//     0b1 ==> A valid initial character has been received.

#define  UART3_IS7816_BWT_MASK       0x20u
#define  UART3_IS7816_BWT_SHIFT      5
//   Block Wait Timer Interrupt
//     0b0 ==> Block wait time (BWT) has not been violated.
//     0b1 ==> Block wait time (BWT) has been violated.

#define  UART3_IS7816_CWT_MASK       0x40u
#define  UART3_IS7816_CWT_SHIFT      6
//   Character Wait Timer Interrupt
//     0b0 ==> Character wait time (CWT) has not been violated.
//     0b1 ==> Character wait time (CWT) has been violated.

#define  UART3_IS7816_WT_MASK        0x80u
#define  UART3_IS7816_WT_SHIFT       7
//   Wait Timer Interrupt
//     0b0 ==> Wait time (WT) has not been violated.
//     0b1 ==> Wait time (WT) has been violated.

#define  UART3_WP7816T0_WI_MASK      0x8u
#define  UART3_WP7816T0_WI_SHIFT     0
#define  UART3_WP7816T0_WI(x)        (((uint8_t)(((uint8_t)(x))<<UART3_WP7816T0_WI_SHIFT))&UART3_WP7816T0_WI_MASK)
//   Wait Timer Interrupt (C7816[TTYPE] = 0)

#define  UART3_WP7816T1_BWI_MASK     0x4u
#define  UART3_WP7816T1_BWI_SHIFT    0
#define  UART3_WP7816T1_BWI(x)       (((uint8_t)(((uint8_t)(x))<<UART3_WP7816T1_BWI_SHIFT))&UART3_WP7816T1_BWI_MASK)
//   Block Wait Time Integer(C7816[TTYPE] = 1)

#define  UART3_WP7816T1_CWI_MASK     0x40u
#define  UART3_WP7816T1_CWI_SHIFT    4
#define  UART3_WP7816T1_CWI(x)       (((uint8_t)(((uint8_t)(x))<<UART3_WP7816T1_CWI_SHIFT))&UART3_WP7816T1_CWI_MASK)
//   Character Wait Time Integer (C7816[TTYPE] = 1)

#define  UART3_WN7816_GTN_MASK       0x8u
#define  UART3_WN7816_GTN_SHIFT      0
#define  UART3_WN7816_GTN(x)         (((uint8_t)(((uint8_t)(x))<<UART3_WN7816_GTN_SHIFT))&UART3_WN7816_GTN_MASK)
//   Guard Band N

#define  UART3_WF7816_GTFD_MASK      0x8u
#define  UART3_WF7816_GTFD_SHIFT     0
#define  UART3_WF7816_GTFD(x)        (((uint8_t)(((uint8_t)(x))<<UART3_WF7816_GTFD_SHIFT))&UART3_WF7816_GTFD_MASK)
//   FD Multiplier

#define  UART3_ET7816_RXTHRESHOLD_MASK 0x4u
#define  UART3_ET7816_RXTHRESHOLD_SHIFT 0
#define  UART3_ET7816_RXTHRESHOLD(x) (((uint8_t)(((uint8_t)(x))<<UART3_ET7816_RXTHRESHOLD_SHIFT))&UART3_ET7816_RXTHRESHOLD_MASK)
//   Receive NACK Threshold

#define  UART3_ET7816_TXTHRESHOLD_MASK 0x40u
#define  UART3_ET7816_TXTHRESHOLD_SHIFT 4
#define  UART3_ET7816_TXTHRESHOLD(x) (((uint8_t)(((uint8_t)(x))<<UART3_ET7816_TXTHRESHOLD_SHIFT))&UART3_ET7816_TXTHRESHOLD_MASK)
//   Transmit NACK Threshold
//     0b0 ==> TXT asserts on the first NACK that is received.
//     0b1 ==> TXT asserts on the second NACK that is received.

#define  UART3_TL7816_TLEN_MASK      0x8u
#define  UART3_TL7816_TLEN_SHIFT     0
#define  UART3_TL7816_TLEN(x)        (((uint8_t)(((uint8_t)(x))<<UART3_TL7816_TLEN_SHIFT))&UART3_TL7816_TLEN_MASK)
//   Transmit Length

#define  UART3_WB_WBASE_MASK         0x8u
#define  UART3_WB_WBASE_SHIFT        0
#define  UART3_WB_WBASE(x)           (((uint8_t)(((uint8_t)(x))<<UART3_WB_WBASE_SHIFT))&UART3_WB_WBASE_MASK)
//   CEA709.1-B WBASE register

#define  VREF_VREFH_TRM_TRIM_MASK    0x6u
#define  VREF_VREFH_TRM_TRIM_SHIFT   0
#define  VREF_VREFH_TRM_TRIM(x)      (((uint8_t)(((uint8_t)(x))<<VREF_VREFH_TRM_TRIM_SHIFT))&VREF_VREFH_TRM_TRIM_MASK)
//   Trim bits
//     0b000000 ==> Min
//     0b111111 ==> Max

#define  VREF_VREFH_TRM_CHOPEN_MASK  0x40u
#define  VREF_VREFH_TRM_CHOPEN_SHIFT 6
//   Chop oscillator enable. When set, internal chopping operation is enabled and the internal analog offset will be minimized.
//     0b0 ==> Chop oscillator is disabled.
//     0b1 ==> Chop oscillator is enabled.

#define  VREF_VREFH_SC_MODE_LV_MASK  0x2u
#define  VREF_VREFH_SC_MODE_LV_SHIFT 0
#define  VREF_VREFH_SC_MODE_LV(x)    (((uint8_t)(((uint8_t)(x))<<VREF_VREFH_SC_MODE_LV_SHIFT))&VREF_VREFH_SC_MODE_LV_MASK)
//   Buffer Mode selection
//     0b00 ==> Bandgap on only, for stabilization and startup
//     0b01 ==> High power buffer mode enabled
//     0b10 ==> Low-power buffer mode enabled
//     0b11 ==> Reserved

#define  VREF_VREFH_SC_VREFST_MASK   0x4u
#define  VREF_VREFH_SC_VREFST_SHIFT  2
//   Internal Voltage Reference stable
//     0b0 ==> The module is disabled or not stable.
//     0b1 ==> The module is stable.

#define  VREF_VREFH_SC_ICOMPEN_MASK  0x20u
#define  VREF_VREFH_SC_ICOMPEN_SHIFT 5
//   Second order curvature compensation enable
//     0b0 ==> Disabled
//     0b1 ==> Enabled

#define  VREF_VREFH_SC_REGEN_MASK    0x40u
#define  VREF_VREFH_SC_REGEN_SHIFT   6
//   Regulator enable
//     0b0 ==> Internal 1.75 V regulator is disabled.
//     0b1 ==> Internal 1.75 V regulator is enabled.

#define  VREF_VREFH_SC_VREFEN_MASK   0x80u
#define  VREF_VREFH_SC_VREFEN_SHIFT  7
//   Internal Voltage Reference enable
//     0b0 ==> The module is disabled.
//     0b1 ==> The module is enabled.

#define  VREF_VREFL_TRM_VREFL_TRIM_MASK 0x3u
#define  VREF_VREFL_TRM_VREFL_TRIM_SHIFT 0
#define  VREF_VREFL_TRM_VREFL_TRIM(x) (((uint8_t)(((uint8_t)(x))<<VREF_VREFL_TRM_VREFL_TRIM_SHIFT))&VREF_VREFL_TRM_VREFL_TRIM_MASK)

#define  VREF_VREFL_TRM_VREFL_EN_MASK 0x8u
#define  VREF_VREFL_TRM_VREFL_EN_SHIFT 3
//     0b0 ==> Disable
//     0b1 ==> Enable

#define  VREF_VREFL_TRM_VREFL_SEL_MASK 0x10u
#define  VREF_VREFL_TRM_VREFL_SEL_SHIFT 4
//     0b0 ==> Internal reference
//     0b1 ==> External reference

#define  WDOG_STCTRLH_WDOGEN_MASK    0x1u
#define  WDOG_STCTRLH_WDOGEN_SHIFT   0
//     0b0 ==> WDOG is disabled.
//     0b1 ==> WDOG is enabled.

#define  WDOG_STCTRLH_CLKSRC_MASK    0x2u
#define  WDOG_STCTRLH_CLKSRC_SHIFT   1
//     0b0 ==> WDOG clock sourced from LPO (also known as the RC oscillator).
//     0b1 ==> WDOG clock sourced from alternate clock source.

#define  WDOG_STCTRLH_IRQRSTEN_MASK  0x4u
#define  WDOG_STCTRLH_IRQRSTEN_SHIFT 2
//     0b0 ==> WDOG time-out generates reset only.
//     0b1 ==> WDOG time-out initially generates an interrupt. After WCT, it generates a reset.

#define  WDOG_STCTRLH_WINEN_MASK     0x8u
#define  WDOG_STCTRLH_WINEN_SHIFT    3
//     0b0 ==> Windowing mode is disabled.
//     0b1 ==> Windowing mode is enabled.

#define  WDOG_STCTRLH_ALLOWUPDATE_MASK 0x10u
#define  WDOG_STCTRLH_ALLOWUPDATE_SHIFT 4
//     0b0 ==> No further updates allowed to WDOG write-once registers.
//     0b1 ==> WDOG write-once registers can be unlocked for updating.

#define  WDOG_STCTRLH_DBGEN_MASK     0x20u
#define  WDOG_STCTRLH_DBGEN_SHIFT    5
//     0b0 ==> WDOG is disabled in CPU Debug mode.
//     0b1 ==> WDOG is enabled in CPU Debug mode.

#define  WDOG_STCTRLH_STOPEN_MASK    0x40u
#define  WDOG_STCTRLH_STOPEN_SHIFT   6
//     0b0 ==> WDOG is disabled in CPU Stop mode.
//     0b1 ==> WDOG is enabled in CPU Stop mode.

#define  WDOG_STCTRLH_WAITEN_MASK    0x80u
#define  WDOG_STCTRLH_WAITEN_SHIFT   7
//     0b0 ==> WDOG is disabled in CPU Wait mode.
//     0b1 ==> WDOG is enabled in CPU Wait mode.

#define  WDOG_STCTRLH_STNDBYEN_MASK  0x100u
#define  WDOG_STCTRLH_STNDBYEN_SHIFT 8
//     0b0 ==> WDOG is disabled in system Standby mode.
//     0b1 ==> WDOG is enabled in system Standby mode.

#define  WDOG_STCTRLH_TESTWDOG_MASK  0x400u
#define  WDOG_STCTRLH_TESTWDOG_SHIFT 10

#define  WDOG_STCTRLH_TESTSEL_MASK   0x800u
#define  WDOG_STCTRLH_TESTSEL_SHIFT  11
//     0b0 ==> Quick test. The timer runs in normal operation. You can load a small time-out value to do a quick test.
//     0b1 ==> Byte test. Puts the timer in the byte test mode where individual bytes of the timer are enabled for operation and are compared for time-out against the corresponding byte of the programmed time-out value. Select the byte through BYTESEL[1:0] for testing.

#define  WDOG_STCTRLH_BYTESEL_MASK   0x2000u
#define  WDOG_STCTRLH_BYTESEL_SHIFT  12
#define  WDOG_STCTRLH_BYTESEL(x)     (((uint16_t)(((uint16_t)(x))<<WDOG_STCTRLH_BYTESEL_SHIFT))&WDOG_STCTRLH_BYTESEL_MASK)
//     0b00 ==> Byte 0 selected
//     0b01 ==> Byte 1 selected
//     0b10 ==> Byte 2 selected
//     0b11 ==> Byte 3 selected

#define  WDOG_STCTRLH_DISTESTWDOG_MASK 0x4000u
#define  WDOG_STCTRLH_DISTESTWDOG_SHIFT 14
//     0b0 ==> WDOG functional test mode is not disabled.
//     0b1 ==> WDOG functional test mode is disabled permanently until reset.

#define  WDOG_STCTRLL_MAP_n_MASK     0x8u
#define  WDOG_STCTRLL_MAP_n_SHIFT    0
#define  WDOG_STCTRLL_MAP_n(x)       (((uint16_t)(((uint16_t)(x))<<WDOG_STCTRLL_MAP_n_SHIFT))&WDOG_STCTRLL_MAP_n_MASK)
//     0b0 ==> Access for master n is disabled.
//     0b1 ==> Access for master n is enabled.

#define  WDOG_STCTRLL_INTFLG_MASK    0x8000u
#define  WDOG_STCTRLL_INTFLG_SHIFT   15

#define  WDOG_TOVALH_TOVALHIGH_MASK  0x10u
#define  WDOG_TOVALH_TOVALHIGH_SHIFT 0
#define  WDOG_TOVALH_TOVALHIGH(x)    (((uint16_t)(((uint16_t)(x))<<WDOG_TOVALH_TOVALHIGH_SHIFT))&WDOG_TOVALH_TOVALHIGH_MASK)

#define  WDOG_WINH_WINHIGH_MASK      0x10u
#define  WDOG_WINH_WINHIGH_SHIFT     0
#define  WDOG_WINH_WINHIGH(x)        (((uint16_t)(((uint16_t)(x))<<WDOG_WINH_WINHIGH_SHIFT))&WDOG_WINH_WINHIGH_MASK)

#define  WDOG_WINL_WINLOW_MASK       0x10u
#define  WDOG_WINL_WINLOW_SHIFT      0
#define  WDOG_WINL_WINLOW(x)         (((uint16_t)(((uint16_t)(x))<<WDOG_WINL_WINLOW_SHIFT))&WDOG_WINL_WINLOW_MASK)

#define  WDOG_REFRESH_WDOGREFRESH_MASK 0x10u
#define  WDOG_REFRESH_WDOGREFRESH_SHIFT 0
#define  WDOG_REFRESH_WDOGREFRESH(x) (((uint16_t)(((uint16_t)(x))<<WDOG_REFRESH_WDOGREFRESH_SHIFT))&WDOG_REFRESH_WDOGREFRESH_MASK)

#define  WDOG_UNLOCK_WDOGUNLOCK_MASK 0x10u
#define  WDOG_UNLOCK_WDOGUNLOCK_SHIFT 0
#define  WDOG_UNLOCK_WDOGUNLOCK(x)   (((uint16_t)(((uint16_t)(x))<<WDOG_UNLOCK_WDOGUNLOCK_SHIFT))&WDOG_UNLOCK_WDOGUNLOCK_MASK)

#define  WDOG_TMROUTH_TIMEROUTHIGH_MASK 0x10u
#define  WDOG_TMROUTH_TIMEROUTHIGH_SHIFT 0
#define  WDOG_TMROUTH_TIMEROUTHIGH(x) (((uint16_t)(((uint16_t)(x))<<WDOG_TMROUTH_TIMEROUTHIGH_SHIFT))&WDOG_TMROUTH_TIMEROUTHIGH_MASK)

#define  WDOG_TMROUTL_TIMEROUTLOW_MASK 0x10u
#define  WDOG_TMROUTL_TIMEROUTLOW_SHIFT 0
#define  WDOG_TMROUTL_TIMEROUTLOW(x) (((uint16_t)(((uint16_t)(x))<<WDOG_TMROUTL_TIMEROUTLOW_SHIFT))&WDOG_TMROUTL_TIMEROUTLOW_MASK)

#define  WDOG_RSTCNT_RSTCNT_MASK     0x10u
#define  WDOG_RSTCNT_RSTCNT_SHIFT    0
#define  WDOG_RSTCNT_RSTCNT(x)       (((uint16_t)(((uint16_t)(x))<<WDOG_RSTCNT_RSTCNT_SHIFT))&WDOG_RSTCNT_RSTCNT_MASK)

#define  WDOG_PRESC_PRESCVAL_MASK    0x300u
#define  WDOG_PRESC_PRESCVAL_SHIFT   8
#define  WDOG_PRESC_PRESCVAL(x)      (((uint16_t)(((uint16_t)(x))<<WDOG_PRESC_PRESCVAL_SHIFT))&WDOG_PRESC_PRESCVAL_MASK)

#define  XBAR_SEL0_SEL0_MASK         0x5u
#define  XBAR_SEL0_SEL0_SHIFT        0
#define  XBAR_SEL0_SEL0(x)           (((uint16_t)(((uint16_t)(x))<<XBAR_SEL0_SEL0_SHIFT))&XBAR_SEL0_SEL0_MASK)

#define  XBAR_SEL0_SEL1_MASK         0x500u
#define  XBAR_SEL0_SEL1_SHIFT        8
#define  XBAR_SEL0_SEL1(x)           (((uint16_t)(((uint16_t)(x))<<XBAR_SEL0_SEL1_SHIFT))&XBAR_SEL0_SEL1_MASK)

#define  XBAR_SEL1_SEL2_MASK         0x6u
#define  XBAR_SEL1_SEL2_SHIFT        0
#define  XBAR_SEL1_SEL2(x)           (((uint16_t)(((uint16_t)(x))<<XBAR_SEL1_SEL2_SHIFT))&XBAR_SEL1_SEL2_MASK)

#define  XBAR_SEL1_SEL3_MASK         0x600u
#define  XBAR_SEL1_SEL3_SHIFT        8
#define  XBAR_SEL1_SEL3(x)           (((uint16_t)(((uint16_t)(x))<<XBAR_SEL1_SEL3_SHIFT))&XBAR_SEL1_SEL3_MASK)

#define  XBAR_SEL2_SEL4_MASK         0x5u
#define  XBAR_SEL2_SEL4_SHIFT        0
#define  XBAR_SEL2_SEL4(x)           (((uint16_t)(((uint16_t)(x))<<XBAR_SEL2_SEL4_SHIFT))&XBAR_SEL2_SEL4_MASK)

#define  XBAR_SEL2_SEL5_MASK         0x500u
#define  XBAR_SEL2_SEL5_SHIFT        8
#define  XBAR_SEL2_SEL5(x)           (((uint16_t)(((uint16_t)(x))<<XBAR_SEL2_SEL5_SHIFT))&XBAR_SEL2_SEL5_MASK)

#define  XBAR_SEL3_SEL6_MASK         0x5u
#define  XBAR_SEL3_SEL6_SHIFT        0
#define  XBAR_SEL3_SEL6(x)           (((uint16_t)(((uint16_t)(x))<<XBAR_SEL3_SEL6_SHIFT))&XBAR_SEL3_SEL6_MASK)

#define  XBAR_SEL3_SEL7_MASK         0x500u
#define  XBAR_SEL3_SEL7_SHIFT        8
#define  XBAR_SEL3_SEL7(x)           (((uint16_t)(((uint16_t)(x))<<XBAR_SEL3_SEL7_SHIFT))&XBAR_SEL3_SEL7_MASK)

#define  XBAR_SEL4_SEL8_MASK         0x5u
#define  XBAR_SEL4_SEL8_SHIFT        0
#define  XBAR_SEL4_SEL8(x)           (((uint16_t)(((uint16_t)(x))<<XBAR_SEL4_SEL8_SHIFT))&XBAR_SEL4_SEL8_MASK)

#define  XBAR_SEL4_SEL9_MASK         0x500u
#define  XBAR_SEL4_SEL9_SHIFT        8
#define  XBAR_SEL4_SEL9(x)           (((uint16_t)(((uint16_t)(x))<<XBAR_SEL4_SEL9_SHIFT))&XBAR_SEL4_SEL9_MASK)

#define  XBAR_SEL5_SEL10_MASK        0x5u
#define  XBAR_SEL5_SEL10_SHIFT       0
#define  XBAR_SEL5_SEL10(x)          (((uint16_t)(((uint16_t)(x))<<XBAR_SEL5_SEL10_SHIFT))&XBAR_SEL5_SEL10_MASK)

#define  XBAR_SEL5_SEL11_MASK        0x500u
#define  XBAR_SEL5_SEL11_SHIFT       8
#define  XBAR_SEL5_SEL11(x)          (((uint16_t)(((uint16_t)(x))<<XBAR_SEL5_SEL11_SHIFT))&XBAR_SEL5_SEL11_MASK)

#define  XBAR_SEL6_SEL12_MASK        0x5u
#define  XBAR_SEL6_SEL12_SHIFT       0
#define  XBAR_SEL6_SEL12(x)          (((uint16_t)(((uint16_t)(x))<<XBAR_SEL6_SEL12_SHIFT))&XBAR_SEL6_SEL12_MASK)

#define  XBAR_SEL6_SEL13_MASK        0x500u
#define  XBAR_SEL6_SEL13_SHIFT       8
#define  XBAR_SEL6_SEL13(x)          (((uint16_t)(((uint16_t)(x))<<XBAR_SEL6_SEL13_SHIFT))&XBAR_SEL6_SEL13_MASK)

#define  XBAR_SEL7_SEL14_MASK        0x6u
#define  XBAR_SEL7_SEL14_SHIFT       0
#define  XBAR_SEL7_SEL14(x)          (((uint16_t)(((uint16_t)(x))<<XBAR_SEL7_SEL14_SHIFT))&XBAR_SEL7_SEL14_MASK)

#define  XBAR_SEL7_SEL15_MASK        0x600u
#define  XBAR_SEL7_SEL15_SHIFT       8
#define  XBAR_SEL7_SEL15(x)          (((uint16_t)(((uint16_t)(x))<<XBAR_SEL7_SEL15_SHIFT))&XBAR_SEL7_SEL15_MASK)

#define  XBAR_SEL8_SEL16_MASK        0x5u
#define  XBAR_SEL8_SEL16_SHIFT       0
#define  XBAR_SEL8_SEL16(x)          (((uint16_t)(((uint16_t)(x))<<XBAR_SEL8_SEL16_SHIFT))&XBAR_SEL8_SEL16_MASK)

#define  XBAR_SEL8_SEL17_MASK        0x500u
#define  XBAR_SEL8_SEL17_SHIFT       8
#define  XBAR_SEL8_SEL17(x)          (((uint16_t)(((uint16_t)(x))<<XBAR_SEL8_SEL17_SHIFT))&XBAR_SEL8_SEL17_MASK)

#define  XBAR_SEL9_SEL18_MASK        0x5u
#define  XBAR_SEL9_SEL18_SHIFT       0
#define  XBAR_SEL9_SEL18(x)          (((uint16_t)(((uint16_t)(x))<<XBAR_SEL9_SEL18_SHIFT))&XBAR_SEL9_SEL18_MASK)

#define  XBAR_SEL9_SEL19_MASK        0x500u
#define  XBAR_SEL9_SEL19_SHIFT       8
#define  XBAR_SEL9_SEL19(x)          (((uint16_t)(((uint16_t)(x))<<XBAR_SEL9_SEL19_SHIFT))&XBAR_SEL9_SEL19_MASK)

#define  XBAR_SEL10_SEL20_MASK       0x5u
#define  XBAR_SEL10_SEL20_SHIFT      0
#define  XBAR_SEL10_SEL20(x)         (((uint16_t)(((uint16_t)(x))<<XBAR_SEL10_SEL20_SHIFT))&XBAR_SEL10_SEL20_MASK)

#define  XBAR_SEL10_SEL21_MASK       0x500u
#define  XBAR_SEL10_SEL21_SHIFT      8
#define  XBAR_SEL10_SEL21(x)         (((uint16_t)(((uint16_t)(x))<<XBAR_SEL10_SEL21_SHIFT))&XBAR_SEL10_SEL21_MASK)

#define  XBAR_SEL11_SEL22_MASK       0x5u
#define  XBAR_SEL11_SEL22_SHIFT      0
#define  XBAR_SEL11_SEL22(x)         (((uint16_t)(((uint16_t)(x))<<XBAR_SEL11_SEL22_SHIFT))&XBAR_SEL11_SEL22_MASK)

#define  XBAR_SEL11_SEL23_MASK       0x500u
#define  XBAR_SEL11_SEL23_SHIFT      8
#define  XBAR_SEL11_SEL23(x)         (((uint16_t)(((uint16_t)(x))<<XBAR_SEL11_SEL23_SHIFT))&XBAR_SEL11_SEL23_MASK)

#define  XBAR_SEL12_SEL24_MASK       0x5u
#define  XBAR_SEL12_SEL24_SHIFT      0
#define  XBAR_SEL12_SEL24(x)         (((uint16_t)(((uint16_t)(x))<<XBAR_SEL12_SEL24_SHIFT))&XBAR_SEL12_SEL24_MASK)

#define  XBAR_CTRL0_DEN0_MASK        0x1u
#define  XBAR_CTRL0_DEN0_SHIFT       0
//   DMA Enable for XBAR_OUT0
//     0b0 ==> DMA disabled
//     0b1 ==> DMA enabled

#define  XBAR_CTRL0_IEN0_MASK        0x2u
#define  XBAR_CTRL0_IEN0_SHIFT       1
//   Interrupt Enable for XBAR_OUT0
//     0b0 ==> Interrupt disabled
//     0b1 ==> Interrupt enabled

#define  XBAR_CTRL0_EDGE0_MASK       0x8u
#define  XBAR_CTRL0_EDGE0_SHIFT      2
#define  XBAR_CTRL0_EDGE0(x)         (((uint16_t)(((uint16_t)(x))<<XBAR_CTRL0_EDGE0_SHIFT))&XBAR_CTRL0_EDGE0_MASK)
//   Active edge for edge detection on XBAR_OUT0
//     0b00 ==> STS0 never asserts
//     0b01 ==> STS0 asserts on rising edges of XBAR_OUT0
//     0b10 ==> STS0 asserts on falling edges of XBAR_OUT0
//     0b11 ==> STS0 asserts on rising and falling edges of XBAR_OUT0

#define  XBAR_CTRL0_STS0_MASK        0x10u
#define  XBAR_CTRL0_STS0_SHIFT       4
//   Edge detection status for XBAR_OUT0
//     0b0 ==> Active edge not yet detected on XBAR_OUT0
//     0b1 ==> Active edge detected on XBAR_OUT0


//////////////////////////////////////////////////////////////////////////////////////
// Backward compatibility.

// No backward compatibility issues.
