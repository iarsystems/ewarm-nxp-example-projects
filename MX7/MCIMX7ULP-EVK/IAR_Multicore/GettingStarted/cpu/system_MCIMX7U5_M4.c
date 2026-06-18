/*
** ###################################################################
**     Processors:          MCIMX7U5DVK08
**                          MCIMX7U5DVP08
**
**     Compilers:           Keil ARM C/C++ Compiler
**                          GNU C Compiler
**                          GNU C Compiler - CodeSourcery Sourcery G++
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    i.MX7ULPRM, Rev. D, Nov. 2016
**     Version:             rev. 2.0.2, 2017-07
**     Build:
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2017 NXP
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of the copyright holder nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 1.0 (2016-04-13)
**         Initial version.
**     - rev. 2.0 (2016-07-19)
**         RevC Header ER
**     - rev. 2.0.1 (2017-02)
**         Edited by Atanas Uzunov @ IAR Systems. Added basic clock & DDR init.
**     - rev. 2.0.2 (2017-07)
**         Edited By Dimitar Nikolov @ IAR Systems. Merged rev.2.0.1 with 3.0 into rev 2.0.2
**                                                  and minor edits
**     - rev. 2.0.3 (2017-02)
**         Edited by Atanas Uzunov @ IAR Systems. Stop SPLL before configuring. Use rev 5.0 WdogDisable asm code.
**         Updated DDR configration code, make compatible with IMX7ULP Rev.B0 devices.
**
** ###################################################################
*/

/*!
 * @file MCIMX7U5_M4
 * @version 2.0.2
 * @date 2017-07
 * @brief Device specific configuration file for MCIMX7U5_M4 (implementation
 *        file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#include <stdint.h>
#include "device_registers.h"


typedef void (*WdogFuncPtr)(WDOG_MemMapPtr wdog);
#pragma section=".a7app"

void SystemCoreClockUpdate(void);

extern uint32_t __vector_table[];

#if OCRAM_DDR_AMP_MULTICORE
static void Init_DDR(void);
#endif

/* ----------------------------------------------------------------------------
   -- Core clock
   ---------------------------------------------------------------------------- */

uint32_t SystemCoreClock = DEFAULT_SYSTEM_CLOCK;

/* ----------------------------------------------------------------------------
   -- SystemInit()
   ---------------------------------------------------------------------------- */
void SystemInit (void) {
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
  SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));    /* set CP10, CP11 Full Access */
#endif /* ((__FPU_PRESENT == 1) && (__FPU_USED == 1)) */

  /* i.MX7ULP systemInit */
  WdogFuncPtr WdogDisable;
  uint16_t WdogDisable32[] = {
    0xF64D,   /* MOV R1, #0xD928  */
    0x1128,
    0xF24C,   /* MOV R2, #0xC520  */
    0x5220,
    0xEA4F,   /* LSL R1, R1, #16  */
    0x4101,
    0x4411,   /* ADD R1, R1, R2   */
    0x6041,   /* STR R1, [R0, #4] */
    0xF64F,   /* MOV R1, #0xFFFF  */
    0x71FF,
    0x6081,   /* STR R1, [R0, #8] */
    0x6801,   /* LDR R1, [R0, #0] */
    0xF031,   /* BICS R1, R1, #0x80 */
    0x0180,
    0xF051,   /* ORRS R1, R1, #0x20 */
    0x0120,
    0x6001,   /* STR R1, [R0]     */
    0x4770    /* BX  LR           */
  };
  uint16_t WdogDisable16[] = {
    0xF24C,   /* MOV R1, #0xC520  */
    0x5120,
    0x6041,   /* STR R1, [R0, #4] */
    0xF64D,   /* MOV R1, #0xD928  */
    0x1128,
    0x6041,   /* STR R1, [R0, #4] */
    0xF64F,   /* MOV R1, #0xFFFF  */
    0x71FF,
    0x6081,   /* STR R1, [R0, #8] */
    0x6801,   /* LDR R1, [R0, #0] */
    0xF031,   /* BICS R1, R1, #0x80 */
    0x0180,
    0xF051,   /* ORRS R1, R1, #0x20 */
    0x0120,
    0x6001,   /* STR R1, [R0]     */
    0x4770    /* BX  LR           */
  };

  /* M4 resume entry point */
  SIM_BASE_PTR->SIM_DGO_GP1 = (uint32_t)__vector_table;

#if (DISABLE_WDOG)
  if (WDOG0_BASE_PTR->CS & WDOG_CS_EN_MASK)
  {
    /* WDOG has timing requirement to unlock the operation window.
       When running in QSPI flash, it's possible to violate that timing
       requirement. So we put the WDOG operation in RAM */
    /* Is WDOG 32bit access enabled? */
    if (WDOG0_BASE_PTR->CS & WDOG_CS_CMD32EN_MASK)
    {
      WdogDisable = (WdogFuncPtr)(((uint32_t)WdogDisable32) | 1U); /* thumb code */
    }
    else
    {
      WdogDisable = (WdogFuncPtr)(((uint32_t)WdogDisable16) | 1U); /* thumb code */
    }
    WdogDisable(WDOG0_BASE_PTR);
  }
#endif /* (DISABLE_WDOG) */

#if (RESET_CORE_B)
  /* Assert reset to Core B */
  PCC0_BASE_PTR->PCC_MU_A |= PCC0_PCC_MU_A_CGC_MASK;
  /* A7 entry point */
  SIM_BASE_PTR->SIM_DGO_GP3 =(unsigned int)__section_begin(".a7app");
  MUA_BASE_PTR->CR |= (MU_CR_BHR_MASK | MU_CR_RSTH_MASK);
  while(0 == (MUA_BASE_PTR->SR & MU_SR_RS_MASK));
  MUA_BASE_PTR->CR &= ~MU_CR_BHR_MASK;
  while(1 == (MUA_BASE_PTR->SR & MU_SR_RS_MASK));
#endif

  /*  Set FIRC as clock source for RUN and HSRUN mode */
  /*  Set SIRC as clock source for VLPR mode */
  SCG0_RCCR = 0x03000001;
  SCG0_VCCR = 0x02000001;
  SCG0_HCCR = 0x03000001;

  /* Start System OSC */
  SCG0_SOSCCFG = 0x4;      // Use internal crystal oscillator
  SCG0_SOSCCSR = 0x1;      // Set enable bit

  /* Wait until OSC has a valid clock */
  while(0 == (SCG0_SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK));

  /* Stop SysPLL if started */
  SCG0_SPLLCSR = 0x0;

  /* Start SysPLL @ 480MHz */
  SCG0_SPLLCFG = (((SCG_SPLLCFG_PLLS_VAL<<SCG_SPLLCFG_PLLS_SHIFT)&SCG_SPLLCFG_PLLS_MASK) | 3<<SCG_SPLLCFG_MULT_SHIFT);
  SCG0_SPLLCSR = 0x1;      // Set enable bit

  /* Wait until SysPLL has a valid clock */
  while(0 == (SCG0_SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK));

  SCG0_SPLLPFD = 0x80808080;                                           // Gate all PFD outputs
  SCG0_SPLLPFD = 0x80808000 | SCG_SPLLPFD_PFD0(SCG_SPLLPFD_PFD0_VAL);  // Set pfd0_frac

  /* Wait until PFD0 has a valid clock */
  while(0 == (SCG0_SPLLPFD & SCG_SPLLPFD_PFD0_VALID_MASK));

  /* Set RUN clocks */
  SCG0_RCCR =  SCG_RCCR_SCS(SCG_RCCR_SCS_VAL)
             | SCG_RCCR_DIVCORE(SCG_RCCR_DIVCORE_VAL)
             | SCG_RCCR_DIVBUS(SCG_RCCR_DIVBUS_VAL)
             | SCG_RCCR_DIVSLOW(SCG_RCCR_DIVSLOW_VAL);

  /* Set SysPLL dividers */
  SCG0_SPLLDIV =  SCG_SPLLDIV_SPLLDIV1(SCG_SPLLDIV_SPLLDIV1_VAL)
                | SCG_SPLLDIV_SPLLDIV2(SCG_SPLLDIV_SPLLDIV2_VAL)
                | SCG_SPLLDIV_SPLLDIV3(SCG_SPLLDIV_SPLLDIV3_VAL);

#if OCRAM_DDR_AMP_MULTICORE
  /* Init DDR - NT6CL256T32BQ-H2 */
  Init_DDR();
#endif

  /* Enable Code Bus Cache */
  /* set command to invalidate all ways and write GO bit
     to initiate command */
  LMEM_BASE_PTR->PCCCR = LMEM_PCCCR_INVW1_MASK | LMEM_PCCCR_INVW0_MASK;
  LMEM_BASE_PTR->PCCCR |= LMEM_PCCCR_GO_MASK;
  /* Wait until the command completes */
  while (LMEM_BASE_PTR->PCCCR & LMEM_PCCCR_GO_MASK){}
  /* Enable code bus cache, enable write buffer */
  LMEM_BASE_PTR->PCCCR = (LMEM_PCCCR_ENWRBUF_MASK | LMEM_PCCCR_ENCACHE_MASK);
  __ISB();
  __DSB();

  SystemCoreClockUpdate();
}

/* ----------------------------------------------------------------------------
   -- SystemCoreClockUpdate()
   ---------------------------------------------------------------------------- */

void SystemCoreClockUpdate (void) {
  uint8_t spllMulti[] = {0U, 15U, 16U, 20U, 22U, 25U, 30U, 0U};

  /* i.MX7ULP systemCoreClockUpdate */
  uint64_t SCGOUTClock,apllTmp;
  uint32_t apllNum, apllDenom;

  /* Identify current system clock source. */
  switch (SCG0_BASE_PTR->CSR & SCG_CSR_SCS_MASK)
  {
    /* System OSC */
    case SCG_CSR_SCS(1):
      SCGOUTClock = CPU_XTAL_SOSC_CLK_HZ;
      break;
    /* Slow IRC */
    case SCG_CSR_SCS(2):
      SCGOUTClock = ((0 == (SCG0_BASE_PTR->SIRCCFG & SCG_SIRCCFG_RANGE_MASK)) ? 4000000u : 16000000u);
      break;
    /* Fast IRC */
    case SCG_CSR_SCS(3):
      SCGOUTClock = 48000000u + ((SCG0_BASE_PTR->FIRCCFG & SCG_FIRCCFG_RANGE_MASK) >> SCG_FIRCCFG_RANGE_SHIFT) * 4000000u;
      break;
    /* RTC OSC */
    case SCG_CSR_SCS(4):
      SCGOUTClock = 32768u;
      break;
    /* System PLL */
    case SCG_CSR_SCS(6):
      /* System clock from SPLL. */
      SCGOUTClock = (0 == (SCG0_BASE_PTR->SPLLCFG & SCG_SPLLCFG_SOURCE_MASK)) ? CPU_XTAL_SOSC_CLK_HZ :
                    (48000000u + ((SCG0_BASE_PTR->FIRCCFG & SCG_FIRCCFG_RANGE_MASK) >> SCG_FIRCCFG_RANGE_SHIFT) * 4000000u);
      SCGOUTClock /= ((SCG0_BASE_PTR->SPLLCFG & SCG_SPLLCFG_PREDIV_MASK) >> SCG_SPLLCFG_PREDIV_SHIFT) + 1;
      SCGOUTClock *= spllMulti[((SCG0_BASE_PTR->SPLLCFG & SCG_SPLLCFG_MULT_MASK) >> SCG_SPLLCFG_MULT_SHIFT)];
      /* Is Core clock from PLL PFD? */
      if (0 != (SCG0_BASE_PTR->SPLLCFG & SCG_SPLLCFG_PLLS_MASK))
      {
        /* System clock from SPLL PFD. */
        /* Is SPLL PFD bypassed? */
        if (0 == (SCG0_BASE_PTR->SPLLCFG & SCG_SPLLCFG_BYPASS_MASK))
        {
          /* SPLL PFD output clock. */
          switch (SCG0_BASE_PTR->SPLLCFG & SCG_SPLLCFG_PFDSEL_MASK)
          {
            case SCG_SPLLCFG_PFDSEL(0):
              SCGOUTClock = ((uint64_t)SCGOUTClock * 18) / ((SCG0_BASE_PTR->SPLLPFD & SCG_SPLLPFD_PFD0_MASK) >> SCG_SPLLPFD_PFD0_SHIFT);
              break;
            case SCG_SPLLCFG_PFDSEL(1):
              SCGOUTClock = ((uint64_t)SCGOUTClock * 18) / ((SCG0_BASE_PTR->SPLLPFD & SCG_SPLLPFD_PFD1_MASK) >> SCG_SPLLPFD_PFD1_SHIFT);
              break;
            case SCG_SPLLCFG_PFDSEL(2):
              SCGOUTClock = ((uint64_t)SCGOUTClock * 18) / ((SCG0_BASE_PTR->SPLLPFD & SCG_SPLLPFD_PFD2_MASK) >> SCG_SPLLPFD_PFD2_SHIFT);
              break;
            case SCG_SPLLCFG_PFDSEL(3):
              SCGOUTClock = ((uint64_t)SCGOUTClock * 18) / ((SCG0_BASE_PTR->SPLLPFD & SCG_SPLLPFD_PFD3_MASK) >> SCG_SPLLPFD_PFD3_SHIFT);
              break;
            default:
              SCGOUTClock = 0;
              break;
          }
        }
        else
        {
          /* SPLL PFD is bypassed. */
          SCGOUTClock = CPU_XTAL_LVDS_CLK_HZ;
        }
      }
      break;
    /* Auxiliary PLL */
    case SCG_CSR_SCS(5):
      /* System clock from APLL. */
      SCGOUTClock = (0 == (SCG0_BASE_PTR->APLLCFG & SCG_APLLCFG_SOURCE_MASK)) ? CPU_XTAL_SOSC_CLK_HZ :
                    (48000000u + ((SCG0_BASE_PTR->FIRCCFG & SCG_FIRCCFG_RANGE_MASK) >> SCG_FIRCCFG_RANGE_SHIFT) * 4000000u);
      SCGOUTClock /= ((SCG0_BASE_PTR->APLLCFG & SCG_APLLCFG_PREDIV_MASK) >> SCG_APLLCFG_PREDIV_SHIFT) + 1;
      apllNum = SCG0_BASE_PTR->APLLNUM;
      apllDenom = SCG0_BASE_PTR->APLLDENOM;
      apllTmp = (uint64_t)SCGOUTClock * ((uint64_t)apllNum) / ((uint64_t)apllDenom);
      SCGOUTClock = SCGOUTClock * ((SCG0_BASE_PTR->APLLCFG & SCG_APLLCFG_MULT_MASK) >> SCG_APLLCFG_MULT_SHIFT) + apllTmp;
      /* Is Core clock from PLL directly? */
      if (0 == (SCG0_BASE_PTR->APLLCFG & SCG_APLLCFG_PLLS_MASK))
      {
        /* System clock from APLL directly. */
        SCGOUTClock /= (((SCG0_BASE_PTR->APLLCFG & SCG_APLLCFG_PLLPOSTDIV1_MASK) >> SCG_APLLCFG_PLLPOSTDIV1_SHIFT) + 1);
        SCGOUTClock /= (((SCG0_BASE_PTR->APLLCFG & SCG_APLLCFG_PLLPOSTDIV2_MASK) >> SCG_APLLCFG_PLLPOSTDIV2_SHIFT) + 1);
      }
      else
      {
        /* System clock from APLL PFD. */
        /* Is APLL PFD bypassed? */
        if (0 == (SCG0_BASE_PTR->APLLCFG & SCG_APLLCFG_BYPASS_MASK))
        {
          /* APLL PFD output clock. */
          switch (SCG0_BASE_PTR->APLLCFG & SCG_APLLCFG_PFDSEL_MASK)
          {
            case SCG_APLLCFG_PFDSEL(0):
              SCGOUTClock = ((uint64_t)SCGOUTClock * 18) / ((SCG0_BASE_PTR->APLLPFD & SCG_APLLPFD_PFD0_MASK) >> SCG_APLLPFD_PFD0_SHIFT);
              break;
            case SCG_APLLCFG_PFDSEL(1):
              SCGOUTClock = ((uint64_t)SCGOUTClock * 18) / ((SCG0_BASE_PTR->APLLPFD & SCG_APLLPFD_PFD1_MASK) >> SCG_APLLPFD_PFD1_SHIFT);
              break;
            case SCG_APLLCFG_PFDSEL(2):
              SCGOUTClock = ((uint64_t)SCGOUTClock * 18) / ((SCG0_BASE_PTR->APLLPFD & SCG_APLLPFD_PFD2_MASK) >> SCG_APLLPFD_PFD2_SHIFT);
              break;
            case SCG_APLLCFG_PFDSEL(3):
              SCGOUTClock = ((uint64_t)SCGOUTClock * 18) / ((SCG0_BASE_PTR->APLLPFD & SCG_APLLPFD_PFD3_MASK) >> SCG_APLLPFD_PFD3_SHIFT);
              break;
            default:
              SCGOUTClock = 0;
              break;
          }
        }
        else
        {
          /* APLL PFD is bypassed. */
          SCGOUTClock = CPU_XTAL_LVDS_CLK_HZ;
        }
      }
      break;
    /* Can not identify core clock source. */
    default:
      SCGOUTClock = 0u;
      break;
  }
  /* Divide the SCG output clock to get the M4 Core clock. */
  SCGOUTClock /= ((SCG0_BASE_PTR->CSR & SCG_CSR_DIVCORE_MASK) >> SCG_CSR_DIVCORE_SHIFT) + 1;
  /* Update System Core Clock. */
  SystemCoreClock = SCGOUTClock;
}

#if OCRAM_DDR_AMP_MULTICORE
/* ----------------------------------------------------------------------------
   -- Init_DDR()   Memory: Nanya NT6CL256T32BQ-H2 1GByte
   ---------------------------------------------------------------------------- */
static void Init_DDR(void)
{
  // Check if DDR is already configured (when do a software reset from the debugger)
  if (PCC3_PCC_MMDC & 0x40000000)
  {
    // If DDR is configured then exit
    if (MMDC0_MDCTL & (MMDC_MDCTL_SDE_0_MASK | MMDC_MDCTL_SDE_1_MASK)) return;
    // Stop clock to MMDC and continue with initialization
    PCC3_PCC_MMDC = 0;
  }

  // Configure clocks
  SCG1_NICCCR    = 0x01000020;
  SCG1_APLLCSR   = 0x00000000;
  SCG1_APLLPFD   = 0x80808080;
  SCG1_APLLCFG   = 0x00140000; // mult = 20, input = sys_osc
  SCG1_APLLNUM   = 0x00000004; // num = 4
  SCG1_APLLDENOM = 0x00000002; // denom = 2
  SCG1_APLLCSR   = 0x00000001; // apll_freq = input * ( mult + num / denom )
                               // apll_freq = 528MHz

  // Wait for valid APLL clock
  while(0 == (SCG1_APLLCSR & SCG_APLLCSR_APLLVLD_MASK));

  SCG1_APLLPFD = 0x8080801e; //1e // pfd0_clk = apll_freq * 18/30
                             // pfd0_clk = 316.8MHz

  // Wait for valid PFD0 clock
  while(0 == (SCG1_APLLPFD & SCG_APLLPFD_PFD0_VALID_MASK));

  // Setup DDR/NIC clocks
  SCG1_DDRCCR = 0x00000001; // ddr_clk = pfd0_clk/1
  SCG1_NICCCR = 0x11000020; // NICCS = 1(ddr_clk),
                            // scg_nic0_clk = pfd0_clk/2
                            // scg_nic1_bus_clk = pfd0_clk/4

  // Turn-on clock for MMDC
  PCC3_PCC_MMDC = 0x40000000;

  // Configure IOMUXC
  IOMUXC1_DDR_SW_PAD_CTL_GRP_DDRTYPE  = 0x00040000;
  IOMUXC1_DDR_SW_PAD_CTL_GRP_PKE      = 0x00000000;
  IOMUXC1_DDR_SW_PAD_CTL_PAD_DDR_CLK0 = 0x00000180;
  IOMUXC1_DDR_SW_PAD_CTL_GRP_DS_CTRL  = 0x00000180;
  IOMUXC1_DDR_SW_PAD_CTL_GRP_DS_ADDR  = 0x00000180;
  IOMUXC1_DDR_SW_PAD_CTL_GRP_INSEL_DQS  = 0x00010000;
  IOMUXC1_DDR_SW_PAD_CTL_PAD_DDR_DQS0 = 0x0000018c;
  IOMUXC1_DDR_SW_PAD_CTL_PAD_DDR_DQS1 = 0x0000018c;
  IOMUXC1_DDR_SW_PAD_CTL_PAD_DDR_DQS2 = 0x0000018c;
  IOMUXC1_DDR_SW_PAD_CTL_PAD_DDR_DQS3 = 0x0000018c;
  IOMUXC1_DDR_SW_PAD_CTL_GRP_INSEL_DAT = 0x00010000;
  IOMUXC1_DDR_SW_PAD_CTL_GRP_DS_DAT0  = 0x00000180;
  IOMUXC1_DDR_SW_PAD_CTL_GRP_DS_DAT1  = 0x00000180;
  IOMUXC1_DDR_SW_PAD_CTL_GRP_DS_DAT2  = 0x00000180;
  IOMUXC1_DDR_SW_PAD_CTL_GRP_DS_DAT3  = 0x00000180;
  IOMUXC1_DDR_SW_PAD_CTL_PAD_DDR_DQM0 = 0x00000180;
  IOMUXC1_DDR_SW_PAD_CTL_PAD_DDR_DQM1 = 0x00000180;
  IOMUXC1_DDR_SW_PAD_CTL_PAD_DDR_DQM2 = 0x00000180;
  IOMUXC1_DDR_SW_PAD_CTL_PAD_DDR_DQM3 = 0x00000180;
  IOMUXC1_DDR_SW_PAD_CTL_PAD_DDR_ZQ0  = 0x00040000;
  IOMUXC1_DDR_SW_PAD_CTL_PAD_DDR_ZQ1  = 0x00040000;

  // Configure MMDC
  MMDC0_MDCTL &= ~(MMDC_MDCTL_SDE_0_MASK | MMDC_MDCTL_SDE_1_MASK);
  MMDC0_MDSCR = MMDC_MDSCR_CON_REQ_MASK;
  MMDC0_MPZQHWCTRL = 0xA1390003;
  while(MMDC0_MPZQHWCTRL & MMDC_MPZQHWCTRL_ZQ_HW_FOR_MASK); // wait for HW ZQ calib end
  MMDC0_MPZQLP2CTL = 0x0D3900A0;
  MMDC0_MPPDCMPR2 = 0x00400000;
  MMDC0_MPRDDLCTL = 0x40404040;
  MMDC0_MPWRDLCTL = 0x40404040;
  MMDC0_MPRDDQBY0DL = 0x33333333;
  MMDC0_MPRDDQBY1DL = 0x33333333;
  MMDC0_MPRDDQBY2DL = 0x33333333;
  MMDC0_MPRDDQBY3DL = 0x33333333;
  MMDC0_MPDCCR = 0x24922492;
  MMDC0_MPMUR0 = 0x00000800;
  MMDC0_MDPDC = 0x00020052;
  MMDC0_MDCFG0 = 0x292C42F3;
  MMDC0_MDCFG1 = 0x00100A22;
  MMDC0_MDCFG3LP = 0x00130556;
  MMDC0_MDCFG2 = 0x00C700DB;
  MMDC0_MDMISC = 0x00211718;
  MMDC0_MDRWD = 0x0F9F26D2;
  MMDC0_MDOR = 0x009F0E10;
  MMDC0_MDASP = 0x0000003F;
  MMDC0_MDCTL = 0xC3190000;

  MMDC0_MDSCR = 0x00008010;
  MMDC0_MDSCR = 0x00008018;
  MMDC0_MDSCR = 0x003F8030;
  MMDC0_MDSCR = 0x003F8038;
  MMDC0_MDSCR = 0xFF0A8030;
  MMDC0_MDSCR = 0xFF0A8038;
  MMDC0_MDSCR = 0x04028030;
  MMDC0_MDSCR = 0x04028038;
  MMDC0_MDSCR = 0x83018030;
  MMDC0_MDSCR = 0x83018038;
  MMDC0_MDSCR = 0x01038030;
  MMDC0_MDSCR = 0x01038038;

  MMDC0_MPDGCTRL0 = 0x20000000;
  MMDC0_MDREF = 0x00001800;
  //MMDC0_MPZQHWCTRL = 0xA1310000;
  MMDC0_MDPDC = 0x00020052;
  MMDC0_MAPSR = 0x00001006;
  MMDC0_MDSCR = 0x00000000;
}
#endif
