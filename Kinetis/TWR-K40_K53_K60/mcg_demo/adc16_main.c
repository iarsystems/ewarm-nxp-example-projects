/*
 * File:		adc16_main.c
 * Purpose:		Main process for adc16
 *
 */

#include "common.h"
#include "adc16.h"
#include "sysinit.h"
#include "mcg_twr_demo.h"


#include "vectors.h"
#include "uif.h"
#include "uart.h"


tADC_Config Master_Adc_Config;  
tADC_Cal_Blk CalibrationStore[2];

extern int core_clk_khz;
extern int core_clk_mhz;
extern int periph_clk_khz;

/********************************************************************/
void adc_test (void)
{
	char ch;
        uint8_t cal_ok;
        unsigned short mult_factor;
        unsigned char temp_MCG_C6;  
        

   /* Turn on the ADCo and ADC1 clocks */
  SIM_SCGC6 |= (SIM_SCGC6_ADC0_MASK );
  SIM_SCGC3 |= (SIM_SCGC3_ADC1_MASK );  
  
  /* setup the initial configuration */
  Master_Adc_Config.CONFIG1  = ADLPC_NORMAL | ADC_CFG1_ADIV(ADIV_4) | ADLSMP_LONG | ADC_CFG1_MODE(MODE_16)| ADC_CFG1_ADICLK(ADICLK_BUS);  
  Master_Adc_Config.CONFIG2  = MUXSEL_ADCA | ADACKEN_DISABLED | ADHSC_HISPEED | ADC_CFG2_ADLSTS(ADLSTS_20) ;
  Master_Adc_Config.COMPARE1 = 0x1234u ; 
  Master_Adc_Config.COMPARE2 = 0x5678u ;
  Master_Adc_Config.STATUS2  = ADTRG_SW | ACFE_DISABLED | ACFGT_GREATER | ACREN_ENABLED | DMAEN_DISABLED | ADC_SC2_REFSEL(REFSEL_EXT);
  Master_Adc_Config.STATUS3  = CAL_OFF | ADCO_SINGLE | AVGE_DISABLED | ADC_SC3_AVGS(AVGS_32);
  Master_Adc_Config.PGA      = PGAEN_DISABLED | PGACHP_NOCHOP | PGALP_NORMAL | ADC_PGA_PGAG(PGAG_64);
  Master_Adc_Config.STATUS1A = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(31);       
  Master_Adc_Config.STATUS1B = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(31);  
  
  ADC_Config_Alt(ADC1_BASE_PTR, &Master_Adc_Config);  // config ADC
  cal_ok = ADC_Cal(ADC1_BASE_PTR);                    // do the calibration
  ADC_Read_Cal(ADC1_BASE_PTR,&CalibrationStore[1]);   // store the cal
  
  ADC_Config_Alt(ADC0_BASE_PTR, &Master_Adc_Config);  // config the ADC again to default conditions 
 
            
  
	while(1)
	{
           ADC1_SC1A = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(20);  // Start conversion on POT channel of TWR_K60
           while ( (ADC1_SC1A & ADC_SC1_COCO_MASK) == COCO_NOT );  // Wait conversion end          
           printf("\nPot = %d\r\n",ADC1_RA);                       // this will also clear COCO bit
           
           mult_factor = ((ADC1_RA) >> 11);     // capture upper 5 bits of ADC reading
           
           if (mult_factor != (MCG_C6 & MCG_C6_VDIV_MASK)) // check if PLL VDIV needs to be changed
           {
             temp_MCG_C6 = MCG_C6;
             temp_MCG_C6 &= ~MCG_C6_VDIV_MASK;    // clear present VDIV setting
             temp_MCG_C6 |= mult_factor;          // replace with new value 
             // move to PBE mode while PLL is reprogrammed and re-locks
             MCG_C1 |= MCG_C1_CLKS(2); // select external reference clock as MCG_OUT
             // Wait for clock status bits to update 
             while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){};
             MCG_C6 = temp_MCG_C6;                // store new VDIV setting
             while (!(MCG_S & MCG_S_LOCK_MASK)){} //wait for re-lock
             // move back to PEE mode now that PLL is re-locked at new frequency
             MCG_C1 &= ~MCG_C1_CLKS_MASK; // select PLL as MCG_OUT
             // Wait for clock status bits to update 
             while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){};
           }
           time_delay_ms(400);                  // slow loop down
           core_clk_mhz = ((24 + mult_factor) * 2);
           core_clk_khz = (core_clk_mhz * 1000);
           
           // re-calculate baud rate setting
           periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
           if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
             uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
             else
           uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
           
           printf("\nPLL Freq = %d MHz\r\n",core_clk_mhz);
           //if character has been received exit loop//
           if ((UART_S1_REG(TERM_PORT) & UART_S1_RDRF_MASK)) {
             temp_MCG_C6 = MCG_C6;
             temp_MCG_C6 &= ~MCG_C6_VDIV_MASK;    // clear present VDIV setting
             temp_MCG_C6 |= MCG_C6_VDIV(24);      // replace with default value for 96MHz 
             // move to PBE mode while PLL is reprogrammed and re-locks
             MCG_C1 |= MCG_C1_CLKS(2); // select external reference clock as MCG_OUT
             // Wait for clock status bits to update 
             while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){};
             MCG_C6 = temp_MCG_C6;                // store new VDIV setting
             while (!(MCG_S & MCG_S_LOCK_MASK)){} //wait for re-lock
             // move back to PEE mode now that PLL is re-locked at new frequency
             MCG_C1 &= ~MCG_C1_CLKS_MASK; // select PLL as MCG_OUT
             // Wait for clock status bits to update 
             while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){};
             core_clk_mhz = 96;
             core_clk_khz = (core_clk_mhz * 1000);
             // re-calculate baud rate setting
             periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
             if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
               uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
             else
               uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
             break;   
           }

	} 
}
