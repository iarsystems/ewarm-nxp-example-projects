/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2011
 *
 *    File name   : __low_level_init.c
 *    Description : __low level init module 
 *
 *    History :
 *    1. Date        : May, 2011
 *       Author      : Stoyan Choynev
 *       Description : Create
 *
 *    $Revision: 16 $
 *
 **************************************************************************/
#include "board.h"
#include <stdint.h>
#include <stdio.h>
/*************************************************************************
 * Function Name: low_level_init
 * Parameters: none
 *
 * Return: none
 *
 * Description: Low level init code
 *
 *************************************************************************/
__arm int __low_level_init (void)
{
  /*Power init*/
  HW_POWER_CTRL_bit.CLKGATE = 0;
  /*Init VDDIO 3.25V*/
  HW_POWER_VDDIOCTRL_bit.DISABLE_STEPPING = 1;
  HW_POWER_VDDIOCTRL_bit.LINREG_OFFSET = 0;
  HW_POWER_VDDIOCTRL_bit.BO_OFFSET = 4;
  HW_POWER_VDDIOCTRL_bit.TRG = 18;
  HW_POWER_VDDIOCTRL_bit.DISABLE_FET = 1;
  /*Init VDDA 1.8V*/
  HW_POWER_VDDACTRL_bit.DISABLE_STEPPING = 1;
  HW_POWER_VDDACTRL_bit.LINREG_OFFSET = 0;
  HW_POWER_VDDACTRL_bit.BO_OFFSET = 4;
  HW_POWER_VDDACTRL_bit.TRG = 12;
  HW_POWER_VDDACTRL_bit.ENABLE_LINREG = 1;
  HW_POWER_VDDACTRL_bit.DISABLE_FET = 1;
  /*Init VDDM 2.45V*/
  HW_POWER_VDDMEMCTRL_bit.TRG = 15;
  HW_POWER_VDDMEMCTRL_bit.PULLDOWN_ACTIVE = 1;
  HW_POWER_VDDMEMCTRL_bit.ENABLE_LINREG = 1;
  for(int i = 0; i < 10000; i++);
  HW_POWER_VDDMEMCTRL_bit.PULLDOWN_ACTIVE = 0;
  HW_POWER_VDDMEMCTRL_bit.ENABLE_ILIMIT = 0;
  /*Init VDDD 1,55V*/
  HW_POWER_VDDDCTRL_bit.DISABLE_STEPPING = 1;
  HW_POWER_VDDDCTRL_bit.LINREG_OFFSET = 0;
  HW_POWER_VDDDCTRL_bit.BO_OFFSET = 4;
  HW_POWER_VDDDCTRL_bit.TRG = 30;
  
  /*Clock init*/
  /*PLL Enable*/
  HW_CLKCTRL_PLLCTRL0_bit.POWER = 1;
  while(!HW_CLKCTRL_PLLCTRL1_bit.LOCK);
  
  /*ref_cpu clock config*/
  HW_CLKCTRL_CPUFRAC_bit.CPUFRAC = 19;    /*480*(18/19) =  454.74*/
  HW_CLKCTRL_CPUFRAC_bit.CLKGATECPU = 0;  /*enable CPU frac divider clock*/
  /*CLK_P divider set*/
  HW_CLKCTRL_CPU_bit.INTERRUPT_WAIT = 0;
  HW_CLKCTRL_CPU_bit.DIV_CPU = 1;         /*CPU runs @ ~454MHz*/
  while(HW_CLKCTRL_CPU_bit.BUSY_REF_CPU);
  /*CLK_H divider set*/
  HW_CLKCTRL_HBUS_bit.AUTO_SLOW_MODE = 0;
  HW_CLKCTRL_HBUS_bit.DIV_FRAC_EN = 0;
  HW_CLKCTRL_HBUS_bit.DIV = 3;            /*HCLK is 151.58MHz*/
  while(HW_CLKCTRL_HBUS_bit.BUSY);
  /*use ref_cpu*/
  HW_CLKCTRL_CLKSEQ_bit.BYPASS_CPU = 0;

  /*ref_emi clock config*/
  HW_CLKCTRL_EMIFRAC_bit.EMIFRAC = 33;    /*480*(18/33) = 261.81MHz*/
  HW_CLKCTRL_EMIFRAC_bit.CLKGATEEMI = 0;  /*enable EMI frac divider clock*/
  /*EMI int divider set*/
  HW_CLKCTRL_EMI_bit.CLKGATE = 0;
  HW_CLKCTRL_EMI_bit.SYNC_MODE_EN = 0;    /*Disable Synch mode*/
  HW_CLKCTRL_EMI_bit.DIV_EMI = 2;         /*EMI runs @  130.91MHz*/
  while(HW_CLKCTRL_EMI_bit.BUSY_REF_EMI);
  /*use ref_emi*/
  HW_CLKCTRL_CLKSEQ_bit.BYPASS_EMI = 0;

  /*EMI int*/
  /*Pin init*/
  HW_PINCTRL_CTRL_bit.CLKGATE = 0;
  HW_PINCTRL_CTRL_bit.SFTRST = 0;
  /*Set Pin drive strength*/
  HW_PINCTRL_DRIVE9_CLR = 0xFFFFFFF0;
  HW_PINCTRL_DRIVE9_SET = 0x22222220;
  HW_PINCTRL_DRIVE10_CLR = 0xFFFFFFFF;
  HW_PINCTRL_DRIVE10_SET = 0x22222222;
  HW_PINCTRL_DRIVE11_CLR = 0xFFF00FFF;
  HW_PINCTRL_DRIVE11_SET = 0x22200222;
  HW_PINCTRL_DRIVE12_CLR = 0xFFFFFFFF;
  HW_PINCTRL_DRIVE12_SET = 0x22222222;
  HW_PINCTRL_DRIVE13_CLR = 0xFFFFFFFF;
  HW_PINCTRL_DRIVE13_SET = 0x22222222;
  HW_PINCTRL_DRIVE14_CLR = 0x00FFFFFF;
  HW_PINCTRL_DRIVE14_SET = 0x00222222;
  /*Pin Mux*/
  HW_PINCTRL_MUXSEL4_CLR = 0xFFFC0000;
  HW_PINCTRL_MUXSEL5_CLR = 0xFC3FFFFF;
  HW_PINCTRL_MUXSEL6_CLR = 0xFFFFFFFF;
  HW_PINCTRL_MUXSEL7_CLR = 0x00000FFF;
  /*SDRAM Init*/
  HW_EMI_CTRL_bit.SFTRST = 0;
  HW_EMI_CTRL_bit.MEM_WIDTH = 1;
  
  HW_DRAM_CTL00 = 0x01010001;
  HW_DRAM_CTL01 = 0x00010100;
  HW_DRAM_CTL02 = 0x01000101;
  HW_DRAM_CTL03 = 0x00000001;
  HW_DRAM_CTL04 = 0x00000101;
  HW_DRAM_CTL05 = 0x00000000;
  HW_DRAM_CTL06 = 0x00010000;
  HW_DRAM_CTL07 = 0x01000001;
  HW_DRAM_CTL09 = 0x00000001;
  HW_DRAM_CTL10 = 0x07000200;
  HW_DRAM_CTL11 = 0x00070202;
  HW_DRAM_CTL12 = 0x02020000;
  HW_DRAM_CTL13 = 0x04040a01;
  HW_DRAM_CTL14 = 0x00000203;
  HW_DRAM_CTL15 = 0x02040000;
  HW_DRAM_CTL16 = 0x02000000;
  HW_DRAM_CTL17 = 0x19000f08;
  HW_DRAM_CTL18 = 0x0d0d0000;
  HW_DRAM_CTL19 = 0x02021313;
  HW_DRAM_CTL20 = 0x02061521;
  HW_DRAM_CTL21 = 0x0000000a;
  HW_DRAM_CTL22 = 0x00080008;
  HW_DRAM_CTL23 = 0x00200020;
  HW_DRAM_CTL24 = 0x00200020;
  HW_DRAM_CTL25 = 0x00200020;
  HW_DRAM_CTL26 = 0x000003f7;
  HW_DRAM_CTL29 = 0x00000020;
  HW_DRAM_CTL30 = 0x00000020;
  HW_DRAM_CTL31 = 0x00c80000;
  HW_DRAM_CTL32 = 0x000a23cd;
  HW_DRAM_CTL33 = 0x000000c8;
  HW_DRAM_CTL34 = 0x00006665;
  HW_DRAM_CTL36 = 0x00000101;
  HW_DRAM_CTL37 = 0x00040001;
  HW_DRAM_CTL38 = 0x00000000;
  HW_DRAM_CTL39 = 0x00000000;
  HW_DRAM_CTL40 = 0x00010000;
  HW_DRAM_CTL08 = 0x01010000;
  
  HW_DRAM_CTL16_bit.LOWPOWER_CONTROL = 0x08;
  HW_DRAM_CTL16_bit.LOWPOWER_AUTO_ENABLE = 0x08;
  
  return 1;
}
