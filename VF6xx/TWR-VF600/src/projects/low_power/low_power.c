#include "common.h"
#include "sysinit.h"
#include "low_power.h"
#include "lpt.h"
#include "leds.h"

uint32_t  new_ipg_freq;

int main()
{

  printf("****** Welcome to Vybrid Low Power Example! ******\n\n");
  
  /* print previous and current power modes */
  //print_pwr_mode();

  /* setup wakeup sources and isr's, etc */
  low_power_setup();

  /************************************/
  /* Test the various low power modes */
  /************************************/

  /**************************************************************************
   *  First run potentiometer demo in full speed RUN mode
   */
  /* Message that we are back running at full frequency */
  printf("***** Current mode: RUN mode - 396 MHz CA5 Target Frequency! *****\n");

  /* Read potentiometer - will stay here until SW2 is pushed */
  read_potentiometer();


  /***********************************************************************
   * 1. 	LPRUN
   *
   * --> Turn off PLL's, continue to run code
   *
   ***********************************************************************/
  printf("---> Entering Low Power Run Mode (LPRUN)...\n\n");
  //printf("---> Entering Low Power Run Mode (LPRUN), hit any key to continue...\n\n");
  //uart_getchar();
  low_power_mode_entry(ENABLE_LPRUN_MODE, WAKE_FROM_INTERRUPT_DONT_CARE, USE_INTERNAL_CLOCK);
  printf("**** Now In Low Power Run Mode (LPRUN) - 24 MHz CA5 Target Frequency! ***** \n");

  /* Read potentiometer - will stay here until SW2 is pushed */
  read_potentiometer();


  /***********************************************************************
   * 3.		LPSTOPx mode
   *
   * 	--> SW1 push button (PTB16) can be used as wake-up
   *
   ***********************************************************************/
  /* Setup Wakeup Unit for SW1 (PTB16) wake up out of LPSTOPx modes */
  enable_wkpu(WKUP_SOURCE_11, FALLING_EDGE_ENABLED);	//button push grounds pin

  //printf("----> Entering LPSTOP3 mode, hit any key to continue...[ SW2 = Exit LPSTOPx ]\n");
  printf("----> Entering LPSTOP3 mode...[ SW2 = Exit LPSTOPx ]\n");
  printf("----> Debugger connection will be lost...\n");
  //uart_getchar();

  low_power_mode_entry(ENABLE_LPSTOP3_MODE, WAKE_FROM_INTERRUPT_TRUE, USE_INTERNAL_CLOCK);

  /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  /* !!! Note:  To recover from LPSTOP3, run from SD card or other external interface 	*/
  /* !!! Then the reset procedure will execute and the program will be restarted 		*/
  /* !!! If running debugger session, the debug connection will be lost 				*/
  /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
  /* Re-initialize clocks to original clock setup */
  printf("---- Now in LPRUN Mode!!! ---- \n");

  /* Light LEDs */
  led_blink_tower();
}


/*******************************************************************
*
*   enter low power modes
*
*   Inputs:  	Low Power Mode to enterWAIT, STOP, LPSTOP1-3
*				wake from interrupt
*	Return:		None
*	Desc:		Use 24MHz as clock source in LPRUN mode, PLLs OFF
*				Use 32KHz as clock source in ULPRUN mode, PLLs OFF
*
*
*	Notes:		In WAIT or STOP mode, GPC module can gate the wakeup
*				since module interrupts allow exit from WAIT or STOP
*
*
*   #define ENABLE_WAIT_MODE         0
*   #define ENABLE_STOP_MODE         1
*   #define ENABLE_LPSTOP1_MODE      2
*   #define ENABLE_LPSTOP2_MODE      3
*   #define ENABLE_LPSTOP3_MODE      4
*
*
********************************************************************/
void low_power_mode_entry(uint32_t low_power_mode, uint32_t wake_from_interrupt, uint32_t clk_int_ext)
{
  uint32 new_clk_freq, arm_div, bus_div, ipg_div, temp;
  uint8_t dummy_char;

  /**************************************************************************/
  /* Set GPC interrupt flags to allow to wake from interrupt...or not       */
  /**************************************************************************/
  if ((low_power_mode == ENABLE_WAIT_MODE) || (low_power_mode == ENABLE_STOP_MODE))
  {
    if (wake_from_interrupt == WAKE_FROM_INTERRUPT_TRUE)
    {
    	/* Allow all module interrupts to wake from STOP mode */
    	GPC->IMR1 = 0x0;
    	GPC->IMR2 = 0x0;
    	GPC->IMR3 = 0x0;
    	GPC->IMR4 = 0x0;

    	/* specific module examples below */
    	/* Allow UART1 interrupt to exit from STOP mode */
    	//GPC->IMR2 &= ~GPC_IMR2_UART1_MASK;	//0=Enable

    	/* Allow GPIO1 to wake from STOP (push button) */
    	//GPC->IMR4 &= ~GPC_IMR4_GPIO1_MASK;	//0=Enable
    }
    else if (wake_from_interrupt == WAKE_FROM_INTERRUPT_FALSE)
    {
    	/* Module interrupts do NOT exit from STOP modes */
    	GPC->IMR1 = 0xFFFFFFFF;
    	GPC->IMR2 = 0xFFFFFFFF;
    	GPC->IMR3 = 0xFFFFFFFF;
    	GPC->IMR4 = 0xFFFFFFFF;

    	/* specific module examples below */
    	/* Mask UART1 interrupt from exiting from STOP mode */
    	//GPC->IMR2 |= GPC_IMR2_UART1_MASK;	//1=Disable

    	/* Mask GPIO1 interrupt from exiting STOP (push button) */
    	//GPC->IMR4 |= GPC_IMR4_GPIO1_MASK;	//1=Disable
    }
  }

  /**************************************************************/
  /* System Clock selection in low power mode     				*/
  /**************************************************************/
  /* For all modes except Ultra Low Power -- use 24MHz fast clock */
  if (low_power_mode != ENABLE_ULPRUN_MODE)
  {
	/* Choose which 24MHz fast clock source to use, but don't switch system yet */
	if (clk_int_ext == USE_INTERNAL_CLOCK)
	{
		CCM->CCR |= CCM_CCR_FIRC_EN_MASK;			//enable FIRC
		CCM->CCSR &= ~CCM_CCSR_FAST_CLK_SEL_MASK;	//CCM.CCSR.B.FAST_CLK_SEL = 0;      	//24MHz IRC clock.
	}
	else
	{
		CCM->CCR |= CCM_CCR_FXOSC_EN_MASK;			//enable FXOSC
		CCM->CCSR |= CCM_CCSR_FAST_CLK_SEL_MASK;	//CCM.CCSR.B.FAST_CLK_SEL = 1;    	//24 MHz XOSC clock
	}

	/* Switch System Clock */
	CCM->CCSR &= ~CCM_CCSR_SYS_CLK_SEL_MASK;	//CCM.CCSR.B.SYS_CLK_SEL = 0x0;    	//Select Fast clock.  WAS: CCM.CCSR.R &= ~0x0F;
	new_clk_freq = 24000;				//24 MHz

	if (clk_int_ext == USE_EXTERNAL_CLOCK)
	{
		/* save current draw by disabling IRC */
		CCM->CCR &= ~CCM_CCR_FIRC_EN_MASK;
	}
  }
  else
  {
	/* Choose One slow clock source, but don't switch system yet */
	if (clk_int_ext == USE_INTERNAL_CLOCK)
	{
		CCM->CCSR &= ~CCM_CCSR_SLOW_CLK_SEL_MASK;	//CCM.CCSR.B.SLOW_CLK_SEL = 0;      	//32KHz, divided 128kHz IRC clock
	}
	else
	{
		CCM->CCSR |= CCM_CCSR_SLOW_CLK_SEL_MASK; 	//CCM.CCSR.B.SLOW_CLK_SEL = 1;    	//32KHz OSC clock
	}

	/********************************************************************/
	/********************************************************************/
	/* If using J-Link, Disconnect debugger and use terminal to run 	*/
	/* Debugger gets lost when slow IRC is selected 					*/
	/********************************************************************/
	/********************************************************************/
	printf("Disconnect debugger for ULPRUN mode...\n");
	//printf("Disconnect debugger for ULPRUN mode, push any key to continue...\n");
    //uart_getchar();

    /* Switch system clock */
    CCM->CCSR |= CCM_CCSR_SYS_CLK_SEL(1); 	//CCM.CCSR.B.SYS_CLK_SEL = 0x1;    	//Select Slow clock.
    new_clk_freq = 32;						//32 kHz
  }

  /*****************************************************************/
  /* Read the current divide values to calculate new run frequency */
  /*****************************************************************/
  arm_div = ((CCM->CACRR & CCM_CACRR_ARM_CLK_DIV_MASK) >> CCM_CACRR_ARM_CLK_DIV_SHIFT);	//CCM.CACRR.B.ARM_CLK_DIV;
  bus_div = ((CCM->CACRR & CCM_CACRR_BUS_CLK_DIV_MASK) >> CCM_CACRR_BUS_CLK_DIV_SHIFT);	//CCM.CACRR.B.BUS_CLK_DIV;
  ipg_div = ((CCM->CACRR & CCM_CACRR_IPG_CLK_DIV_MASK) >> CCM_CACRR_IPG_CLK_DIV_SHIFT);	//CCM.CACRR.B.IPG_CLK_DIV;
  new_ipg_freq = (new_clk_freq / (arm_div + 1));
  new_ipg_freq = (new_ipg_freq / (bus_div + 1));
  new_ipg_freq = (new_ipg_freq / (ipg_div + 1));


  /*************************************************/
  /* Disable all PLLs through the Anadig registers */
  /*************************************************/
  ANADIG->PLL3_CTRL &= ~ANADIG_PLL3_CTRL_ENABLE_MASK;	 //ANADIG.USB1_PLL_CTRL.B.ENABLE  = 0;
  ANADIG->PLL7_CTRL &= ~ANADIG_PLL7_CTRL_ENABLE_MASK;	 //ANADIG.USB2_PLL_CTRL.B.ENABLE  = 0;
  ANADIG->PLL2_CTRL &= ~ANADIG_PLL2_CTRL_ENABLE_MASK;	 //ANADIG.PLL_528_CTRL.B.ENABLE   = 0;
  ANADIG->PLL4_CTRL &= ~ANADIG_PLL4_CTRL_ENABLE_MASK;	 //ANADIG.PLL_AUDIO_CTRL.B.ENABLE = 0;
  ANADIG->PLL6_CTRL &= ~ANADIG_PLL6_CTRL_ENABLE_MASK;	 //ANADIG.PLL_VIDEO_CTRL.B.ENABLE = 0;
  ANADIG->PLL5_CTRL &= ~ANADIG_PLL5_CTRL_ENABLE_MASK;	 //ANADIG.PLL_ENET_CTRL.B.ENABLE  = 0;
  ANADIG->PLL1_CTRL &= ~ANADIG_PLL1_CTRL_ENABLE_MASK;	 //ANADIG.PLL_SYS_CTRL.B.ENABLE   = 0;

  /******************************************************************
   *
   * Mask or un-mask core from entering low power mode
   * Use this with GPC.LMR setting to allow masking of one core
   * from entering low power mode
   *
   ******************************************************************/
  /* Can wake up CA5 to WKPU ISR from LPSTOPx mode */
  /* M4 keeps toggling LEDs, showing it is still executing */
  /* LPSTOPx recovery to U-Boot, config for 115200 8-N-1 */
  /* <1 mA */
  CCM->CLPCR &= ~CCM_CLPCR_M_CORE1_WFI_MASK;		/* M_CORE1_WFI = 0, not masked */
  CCM->CLPCR &= ~CCM_CLPCR_M_CORE0_WFI_MASK;		/* M_CORE0_WFI = 0, not masked */


  /* Reinitialize the UART when in modes using 24 MHz clock */
  if (low_power_mode != ENABLE_ULPRUN_MODE)
  {
	  uart_init((int)new_ipg_freq, TERMINAL_BAUD);
	  //uart_init((int)new_ipg_freq, TERMINAL_BAUD_REDUCED);
	  //printf("====UART_re-init using new clock frequency! =======\n");
	  //printf("=====Hit_any_key_to_enter_low_power_mode=========================\n");
	  //printf("New Clock Freq - UART_re-init,_hit_any_key_to_continue...\n\n");
	  //dummy_char = uart_getchar();
  }

  if ((low_power_mode == ENABLE_ULPRUN_MODE) || (low_power_mode == ENABLE_LPRUN_MODE))
  {
	/***********************************************************************/
    /* We are done if mode is run mode, no need to execute WFI instruction */
	/***********************************************************************/
    return;
  }
  else if (low_power_mode == ENABLE_WAIT_MODE)
  {
    /**************/
    /* WAIT Mode  */
    /**************/
    printf("\nMoving to WAIT MODE!!!\n\n");

    /* Configure GPC for WAIT mode */
    GPC->LPMR = 0x1;

    /* ARM clock - 0=enabled on wait mode, 1=disabled, for debugging */
    CCM->CLPCR &= ~CCM_CLPCR_ARM_CLK_LPM_MASK;

    // Execute WFI instruction
    __asm("WFI");
  }
  else
  {
    /**************/
    /* STOP Modes */
    /**************/

    /* Setup whether PMIC VSTBY REQ pin is asserted in STOP mode */
	/* This pin notifies external power management IC to use stdby or normal functional voltage */
	/* 1=Assert, 0=Do not Assert */
    CCM->CLPCR |= CCM_CLPCR_ANATOP_STOP_MODE_MASK;

    if (low_power_mode == ENABLE_STOP_MODE)
    {
      /**********/
      /* STOP   */
      /**********/
      printf("\nMoving to STOP MODE!!!\n\n");

      /* Ensure Power Domain 1 will remain on */
      GPC->PGCR &= ~0x00000001;		/* PG_PD1 = 0 - Leave ON */

      /* Deep Sleep STOP mode - periphery power down, maintain memory contents, outputs pulled low */
      GPC->PGCR |= 0x00000080;	/* DS_STOP = 1 */

      /****************************************************************
       * Additional Power Savings options for STOP mode !
       ****************************************************************/
      /* Enable Well Bias - This allows extra power savings because it reduces static leakage */
      GPC->PGCR |= 0x00000010;		/* WB_STOP = 1 */
      /* HPREG Power down in STOP mode */
      GPC->PGCR |= 0x00000008;		/* HP_OFF = 1 */

    }
    else if (low_power_mode == ENABLE_LPSTOP1_MODE)
    {
      /***********/
      /* LPSTOP1 */
      /***********/
      printf("\nMoving to LPSTOP1 MODE!!! GPC->PGCR: 0x%X\n\n", GPC->PGCR);

      /* Deep Sleep LPSTOP mode - periphery power down, maintain memory contents, outputs pulled low */
      GPC->PGCR |= 0x00000040;	/* DS_STBY = 1 */

      /* Setup LPS1 mode by turning off all SRAM blocks */
      GPC->PGCR |= GPC_PGCR_PG_48K_MASK;	//1=Turn OFF
      GPC->PGCR |= GPC_PGCR_PG_16K_MASK;	//1=Turn OFF

      /* Ensure Power Domain 1 will turn off in LPSTOPx mode */
      GPC->PGCR |= 0x00000001;		/* PG_PD1 = 1 - turn OFF */

    }
    else if (low_power_mode == ENABLE_LPSTOP2_MODE)
    {
      /***********/
      /* LPSTOP2 */
      /***********/
      printf("\nMoving to LPSTOP2 MODE!!! GPC->PGCR: 0x%X\n\n", GPC->PGCR);

      /* Deep Sleep LPSTOP mode - periphery power down, maintain memory contents, outputs pulled low */
      GPC->PGCR |= 0x00000040;	/* DS_STBY = 1 */

      // Setup LPS2 mode
      GPC->PGCR |= GPC_PGCR_PG_48K_MASK;	//1=Turn OFF
      GPC->PGCR &= ~GPC_PGCR_PG_16K_MASK;	//0=Leave ON

      /* Ensure Power Domain 1 will turn off in LPSTOPx mode */
      GPC->PGCR |= 0x00000001;		/* PG_PD1 = 1 - turn OFF */

    }
    else if (low_power_mode == ENABLE_LPSTOP3_MODE)
    {
      /***********/
      /* LPSTOP3 */
      /***********/
      printf("\nMoving to LPSTOP3 MODE (64K SRAM only)!!! GPC->PGCR: 0x%X\n\n", GPC->PGCR);

      /* Deep Sleep LPSTOP mode - periphery power down, maintain memory contents, outputs pulled low */
      GPC->PGCR |= 0x00000040;	/* DS_STBY = 1 */

      // Setup LPS3 mode
      GPC->PGCR &= ~GPC_PGCR_PG_48K_MASK;	//0=Leave ON
      GPC->PGCR &= ~GPC_PGCR_PG_16K_MASK;	//0=Leave ON

      /* Ensure Power Domain 1 will turn OFF in LPSTOPx mode */
      GPC->PGCR |= 0x00000001;		/* PG_PD1 = 1 - turn OFF */
    }

    /*******************************
     * Configure GPC for STOP mode
     ********************************/
    GPC->LPMR = 0x02;

    /* dummy wait - debug printf stopping */
    for (temp = 0; temp < 0xf000; temp++){};

    /**********************************************************
     * Enter Low Power mode here with WFI instruction
     **********************************************************/
    __asm("WFI");

  }

  /*
   * If Exiting low power mode, continue here
   */
  	  printf("\nExited Low Power MODE (wake from WFI)!!!\n");
  	  print_clk_mode();

}

void print_pwr_mode(void)
{
	uint32_t mode;

	/*********************************************************************/
	/* Check previous power mode */
	/*********************************************************************/
	mode = ((GPC->PGSR & GPC_PGSR_PREV_LPM_MASK) >> GPC_PGSR_PREV_LPM_SHIFT);
	if (mode == 0)
	  printf("Previous PWR Mode: Run\n");
	else if (mode == 1)
	  printf("Previous PWR Mode: WAIT\n");
	else if (mode == 2)
	  printf("Previous PWR Mode: STOP\n");
	else if (mode == 3)
	  printf("Previous PWR Mode: LPSTOPx\n");
	else
	  printf("Previous PWR Mode: Undetermined\n");

	/*********************************************************************/
	/* Check current power mode */
	/*********************************************************************/
	mode = ((GPC->PGSR & GPC_PGSR_CUR_LPM_MASK) >> GPC_PGSR_CUR_LPM_SHIFT);
	if (mode == 0)
	  printf("Current PWR Mode: Run\n");
	else if (mode == 1)
	  printf("Current PWR Mode: WAIT\n");
	else if (mode == 2)
	  printf("Current PWR Mode: STOP\n");
	else if (mode == 3)
	  printf("Previous PWR Mode: LPSTOPx\n");
	else
	  printf("Current PWR Mode: Undetermined\n");

	if (GPC->PGSR & GPC_PGSR_PSR_MASK)
	  GPC->PGSR = GPC_PGSR_PSR_MASK;	//Clear Status bit
}

void print_clk_mode(void)
{
  printf("\n");
  printf("Faraday Clock Mode Settings\n");
  printf("----------------------------------------------------\n");
  printf("CCM CCSR       = 0x%X\n", CCM->CCSR);
  printf("CA5 CLK        = 0x%X\n", ((CCM->CCSR & 0x80) >> 3) /* CCM.CCSR.B.CA5_CLK_SEL */);
  printf("System CLK     = 0x%X\n", ((CCM->CCSR & CCM_CCSR_SYS_CLK_SEL_MASK) >> CCM_CCSR_SYS_CLK_SEL_SHIFT) /* CCM.CCSR.B.SYS_CLK_SEL */);
  printf("Fast CLK SEL   = 0x%X\n", ((CCM->CCSR & CCM_CCSR_FAST_CLK_SEL_MASK) >> CCM_CCSR_FAST_CLK_SEL_SHIFT) /* CCM.CCSR.B.FAST_CLK_SEL */);
  printf("Slow CLK SEL   = 0x%X\n", ((CCM->CCSR & CCM_CCSR_SLOW_CLK_SEL_MASK) >> CCM_CCSR_SLOW_CLK_SEL_SHIFT) /* CCM.CCSR.B.SLOW_CLK_SEL */);
  printf("----------------------------------------------------\n");
  printf("CCM CACRR      = 0x%X\n", CCM->CACRR);
  printf("ARM CLK DIV    = 0x%X\n", ((CCM->CACRR& CCM_CACRR_ARM_CLK_DIV_MASK) >> CCM_CACRR_ARM_CLK_DIV_SHIFT) /* CCM.CACRR.B.ARM_CLK_DIV */);
  printf("Bus CLK DIV    = 0x%X\n", ((CCM->CACRR & CCM_CACRR_BUS_CLK_DIV_MASK) >> CCM_CACRR_BUS_CLK_DIV_SHIFT) /* CCM.CACRR.B.BUS_CLK_DIV */);
  printf("Flex CLK DIV   = 0x%X\n", ((CCM->CACRR & CCM_CACRR_FLEX_CLK_DIV_MASK) >> CCM_CACRR_FLEX_CLK_DIV_SHIFT) /* CCM.CACRR.B.FLEX_CLK_DIV */);
  printf("IPG CLK DIV    = 0x%X\n", ((CCM->CACRR & CCM_CACRR_IPG_CLK_DIV_MASK) >> CCM_CACRR_IPG_CLK_DIV_SHIFT) /* CCM.CACRR.B.IPG_CLK_DIV */);
  printf("----------------------------------------------------\n\n");
}

/* Blink LEDs on TWR-VF600 */
void led_blink_tower()
{
	ENABLE_LEDS;

  //Flash on and off LED's
  while(1)
  {
	//Turn on LED's by driving 0 (active low)
	  LEDS_ON;

    //Delay
    time_delay_ms(200);
    
    //Turn off LED's by driving 1
    LEDS_OFF;

    //Delay
    time_delay_ms(400);
  }
}


/***************************************************************************/
/* Potentiometer test using ADC                                             */
/* PTC30 used for potentiometer input (ADC0SE5)                             */
/* Which is GPIO103, Portd_7                                                */
/***************************************************************************/
void read_potentiometer()
{
  uint32_t i = 0, voltage = 0, temp1;
  uint16_t conversion;

  /* Enable ADC0 clock gate */
  CCM->CCGR1 |= CCM_CCGR1_CG11(CLK_ON_ALL_MODES); 	//CG27 = 1;

  /* Setup button IO to allow push button SW2 to exit test */
  PUSH_BUTTONS_INIT;
  ENABLE_LEDS;

  /* setup pin for ADC function, input */
  IOMUXC->RGPIO[103] = 0x00600001;      /* MUX=0b110(ADC), IBE=1 */

  /* Calibrate the ADC */
  //printf("\nCalibrating ADC...");
  ADC0->GC |= ADC_GC_CAL_MASK;	//ADC0.GC.B.CAL = 1;        /* Set CAL Bit to Start */
  while((ADC0->GC & ADC_GC_CAL_MASK) == 1){};  				/* Wait for it to complete */
  if(ADC0->GS & ADC_GS_CALF_MASK)							/* ADC0.GS.B.CALF) */
  {
    //printf("\nADC Calibration Failure!\n");
  }
  else
  {
    //printf("\nADC Calibration Complete!\n");
  }
  conversion = ADC0->R[0];		//ADC0.R1.R;     /* Read result register to clear COCO */

  /* Setup ADC for continuous conversions on potentiometer input */
  ADC0->CFG = 0x0001020A;    /* OVWREN=1, ADTRG=0 (s/w trig), REFSEL=0b00 (VREFH/L), ADHSC=0b0 (normal speed), ADSTS=0b10 (sample period), MODE=0b10(12-bit), ADIV=0b10 (clk/4) */
  ADC0->GC = 0x00000060;     /* Continuous conversions, averaging enabled */
  ADC0->HC[0] = 0x00000005;    /* channel 5 */
  ADC0->PCTL = 0x00000020;   /* disable I/O functionality on channel 5. Use as ADC input only */

  /* Show results and test for range of input.  Tell user to turn dial */
  //printf("\nReading Potentiometer Values...[ SW2 = ABORT ]\n\n");
  printf("\n>>>> Reading Potentiometer Values, rotate dial to see updates...<<<<\n\n");

  PUSH_BUTTONS_INIT;

  /* Now poll and read results until potentiometer dial is turned to max in both directions */
  do
  {
    /* check conversion complete flag */
    if (ADC0->HS & ADC_HS_COCO0_MASK)		//ADC0.HS.B.COCO1 & 1)
    {
      conversion = ADC0->R[0];				//ADC0.R1.R;  /* Read result register to clear COCO */
      voltage = ((conversion * 3300) / 4096);
    }

    i++; 	/* increment counter */

    /*****************************/
    /* print what we are reading */
    /*****************************/
    if (i % 9000 == 0)
    {
    	printf("\r\tCurrent Potentiometer Reading:  %4d mV....[ Push SW2 to continue ]", voltage);
    	LEDS_TOGGLE;
    }

    /***************/
    /* SW2 = ABORT */
    /***************/
    temp1 = GPIO1->PDIR;
    //temp1 = (temp1 & 0x00000040);	//SW1 button PTB
    temp1 = (temp1 & 0x00000080);	//SW2 button PTB

  } while (temp1); /* button push will ground IO */

  /* Now wait until button is released */
   while((GPIO1->PDIR & 0x00000080) == 0){}; 	//((GPIO1->PDIR & 0x00000040) == 0){} //SW1;

  printf("\n\nExited Potentiometer Reads...\n\n");

}

/*******************************************************************/
/* Smooth LED scrolling like Knight Industries Two Thousand (KITT) */
/*******************************************************************/
void knight_rider(uint32_t count)
{
	uint32_t i, dly = 20;

  /* Setup GPIO output on LED pins  */
	ENABLE_LEDS;

  /* Flash on and off LED's */
  for (i = 0; i < count; i++)
  {
    /* one direction */
    LED0_ON;  time_delay_ms(dly);  LED1_ON; time_delay_ms(dly);
    LED0_OFF; time_delay_ms(dly);  LED2_ON; time_delay_ms(dly);
    LED1_OFF; time_delay_ms(dly);  LED3_ON; time_delay_ms(dly);
    LED2_OFF; time_delay_ms(dly);

    /* other direction */
    LED2_ON;  time_delay_ms(dly);  LED3_OFF;  time_delay_ms(dly);
    LED1_ON;  time_delay_ms(dly);  LED2_OFF;  time_delay_ms(dly);
    LED0_ON;  time_delay_ms(dly);  LED1_OFF;
  }
}

void low_power_setup(void)
{
	/* Enable buttons on Tower */
	PUSH_BUTTONS_INIT;

	/* Setup UART1 for clocking on all modes */
	CCM->CCGR0 |= CCM_CCGR0_CG8(CLK_ON_ALL_MODES);

	/* Setup PTB16 (PORTB[6], GPIO38) for interrupt on either edge for SW1 push */
	//PORT1->PCR[6] |= PORT_PCR_IRQC(0x9);		//0x9=rising, 0xA=falling, 0xB=either edge, 0xC=when logic 1, 0x0=disabled
	PORT1->PCR[6] |= PORT_PCR_IRQC(0x0);		//Disable because we don't want port pin interrupt, we want wakeup unit interrupt

	/* Make sure we are running at the correct baud rate for low power modes */
	//uart_init(BUS_CLK_KHZ, TERMINAL_BAUD_REDUCED);
	uart_init(BUS_CLK_KHZ, TERMINAL_BAUD);

	/* Enable ptb interrupt for wakeup */
	enable_interrupt(WKPU0_IRQn, wkpu_isr);		//GIC=128, NVIC=92

	// Enable the IRQ, FIQ and Aborts in A5 Core
  __asm ("CPSIE IFA");

}

void enable_wkpu(uint32 source, uint32_t rise_fall)
{
	WKPU->IRER = (1 << source);		//i.e. 0x0000800 -> WKPU_P11 PTB16 -- enable wakeup/interrupt pads to interrupt controller
	WKPU->WRER = (1 << source);		//i.e, 0x00000800 -> WKPU_P11 PTB16 -- enable message from wakeup/interrupt pads to the mode entry and power control modules

	if (rise_fall == RISING_EDGE_ENABLED)
		WKPU->WIREER = (1 << source);		//rising edge interrupts
	else
		WKPU->WIFEER = (1 << source);		//enable falling edge interrupts (Tower push buttons are pulled up, grounded with button push) */

}

void ptb_isr(void)
{
	uint32_t data;

	data = PORT1->ISFR;
	if (data)
	{
		/* clear */
		PORT1->ISFR = data;
	}
}

void wkpu_isr(void)
{
	uint32_t data;

	data = WKPU->WISR;
	if (data)
	{
		/* clear */
		WKPU->WISR = data;
	}

	/* debug - stay here */
	led_blink_tower();
}

void low_power_gate(uint32_t A5_gate, uint32_t M4_gate)
{
#if 0 /* debug in progress for additional options */

	/* Can wake up CA5 to WKPU ISR from LPSTOPx mode */
	/* M4 keeps toggling LEDs, showing it is still executing */
	/* LPSTOPx recovery to U-Boot, config for 115200 8-N-1 */
	/* <1 mA */
	CCM->CLPCR &= ~CCM_CLPCR_M_CORE1_WFI_MASK;		/* M_CORE1_WFI = 0, not masked */
	CCM->CLPCR &= ~CCM_CLPCR_M_CORE0_WFI_MASK;		/* M_CORE0_WFI = 0, not masked */

	/* config 1:  */
	/* CA5 and M4 cores go into LSPTOPx modes */
	/* Wakeup is reset, UBoot runs off of sd card */
	/* <1 mA */
	/* ?? M4 Not getting masked - debug more */
	CCM->CLPCR |= CCM_CLPCR_M_CORE1_WFI_MASK;		/* M_CORE1_WFI = 1, masked */
	CCM->CLPCR |= CCM_CLPCR_M_CORE0_WFI_MASK;		/* NOT masked?  M_CORE0_WFI = 1, masked */

	/* config 2: */
	/* CA5 wakes up into wkpu isr */
	/* M4 keeps executing */
	/* 34 mA */
	CCM->CLPCR &= ~CCM_CLPCR_M_CORE1_WFI_MASK;	/* M_CORE1_WFI = 0 */
	CCM->CLPCR |= CCM_CLPCR_M_CORE0_WFI_MASK;		/* M_CORE0_WFI = 0 */

	/* config 3: */
	/* recovery: reset to UBoot */
	/* < 1mA */
	CCM->CLPCR |= CCM_CLPCR_M_CORE1_WFI_MASK;			/* M_CORE1_WFI = 0 */
	CCM->CLPCR &= ~CCM_CLPCR_M_CORE0_WFI_MASK;		/* M_CORE0_WFI = 0 */
#endif
}
