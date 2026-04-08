/*
 * File:		main.c
 * Purpose:		TSI Example
 *
 *                      Toggle the LED's on the tower board by pressing the TSI
 *                        touch pads
 *
 */

#include "common.h"
#include "TSI.h"
#include "misc.h"

void GPIO_Init(void);

/********************************************************************/
void main (void)
{

    RTC_Init();
    TSI_Init();
    GPIO_Init();
    TSI_SelfCalibration();
    llwu_configure(0,0,LLWU_ME_WUME4_MASK);
    enable_irq(21);  //LLWUI Vector is 37. IRQ# is 37-16=21
    
    START_SCANNING;
    ENABLE_EOS_INT;
    ENABLE_TSI;

    while(1)
    {
        /* LLS mode */
        printf("\r\n Send any character to go into low power mode LLS \r\n");
        LEDs_Off();
        in_char();
        enable_lpwui();
        TSI_GoToLP();
        delay(800000);
        pee_pbe(CLK0_FREQ_HZ);
        pbe_fbe(CLK0_FREQ_HZ);
        fbe_fbi(32000,0);
        fbi_blpi(32000,0);
        enter_lls(); 
        blpi_fbi(32000,0);
        fbi_fei(32000);
        fei_fbe(CLK0_FREQ_HZ,1,1);
        fbe_pbe(0,CLK0_FREQ_HZ,0,PLL0_PRDIV,PLL0_VDIV);
        pbe_pee(CLK0_FREQ_HZ,PLL_0);
        TSI_GoToActive();
        printf("\r\n Woke up from LLS !!! \r\n");
        LEDs_On();

        /* VLLS3 mode */
        printf("\r\n Send any character to go into low power mode VLLS3 \r\n");
        in_char();
        enable_lpwui();
        TSI_GoToLP();
        delay(800000);
        pee_pbe(CLK0_FREQ_HZ);
        pbe_fbe(CLK0_FREQ_HZ);
        fbe_fbi(32000,0);
        fbi_blpi(32000,0);
        enter_vlls3(); 
        blpi_fbi(32000,0);
        fbi_fei(32000);
        fei_fbe(CLK0_FREQ_HZ,1,1);
        fbe_pbe(0,CLK0_FREQ_HZ,0,PLL0_PRDIV,PLL0_VDIV);
        pbe_pee(CLK0_FREQ_HZ,PLL_0);
        TSI_GoToActive();
        printf("\r\n Woke up from VLLS3 !!!  - Nice!!! \r\n");
        LEDs_On();

        /* VLLS2 mode */
        printf("\r\n Send any character to go into low power mode VLLS2 \r\n");
        in_char();
        enable_lpwui();
        TSI_GoToLP();
        delay(800000);
        pee_pbe(CLK0_FREQ_HZ);
        pbe_fbe(CLK0_FREQ_HZ);
        fbe_fbi(32000,0);
        fbi_blpi(32000,0);
        enter_vlls2(); 
        blpi_fbi(32000,0);
        fbi_fei(32000);
        fei_fbe(CLK0_FREQ_HZ,1,1);
        fbe_pbe(0,CLK0_FREQ_HZ,0,PLL0_PRDIV,PLL0_VDIV);
        pbe_pee(CLK0_FREQ_HZ,PLL_0);
        TSI_GoToActive();
        printf("\r\n Woke up from VLLS2 !!!  - Lower power!!! \r\n");
        LEDs_On();

        /* VLLS1 mode */
        printf("\r\n Send any character to go into low power mode VLLS1 \r\n");
        in_char();
        enable_lpwui();
        TSI_GoToLP();
        delay(800000);
        pee_pbe(CLK0_FREQ_HZ);
        pbe_fbe(CLK0_FREQ_HZ);
        fbe_fbi(32000,0);
        fbi_blpi(32000,0);
        enter_vlls1(); 
        blpi_fbi(32000,0);
        fbi_fei(32000);
        fei_fbe(CLK0_FREQ_HZ,1,1);
        fbe_pbe(0,CLK0_FREQ_HZ,0,PLL0_PRDIV,PLL0_VDIV);
        pbe_pee(CLK0_FREQ_HZ,PLL_0);
        TSI_GoToActive();
        printf("\r\n You will never see this message VLLS1 wakes up in the reset vector!!! \r\n");
        LEDs_On();        

    }

}
/********************************************************************/

