/*
 * LowPowerAppStates.c
 *
 *  Created on: Jul 31, 2012
 *      Author: B23243
 */

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
#include "LowPowerAppStates.h"

/*****************************************************************************************************
* Declaration of module wide FUNCTIONs - NOT for use in other modules
*****************************************************************************************************/

void vfnLowPowerDemoIDLEState(void);
void vfnLowPowerDemoReadState(void);
void vfnLowPowerDemoCheckValueState(void);
void vfnLowPowerDemoGoLowPowerState(void);
void vfnLowPowerAlarmState(void);
void vfnSampleAccel(void);
void vfnEnterLowPower(void);
void uart(uint8 u8state);
void i2c(uint8 u8state);
void vfnlptmrInit(int count, int clock_source);


/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Declaration of module wide TYPEs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide VARIABLEs - NOT for use in other modules
*****************************************************************************************************/
sSM sMLowPowerDemoState;  

uint8 gu8lpTimerEventFlag = 0;

uint16 gu16PotVValue;
uint16 gu16PotmVValue;

uint8 gu8AppSelected;

/* Actual system clock frequency */
extern int mcg_clk_hz;
extern int mcg_clk_khz;
extern int core_clk_khz;
extern int periph_clk_khz;
extern int pll_clk_khz;

extern signed char result[20];

extern uint8 gu8lpTimerEventFlag;

uint32 gu32AccelSampling;

uint16 gu16AlarmDelay = 0;
uint8 gu8AlarmFlag = FALSE;
uint16 gu16SecondsCounter = 0;



tADC_Config Master_Adc_Config; 
/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs - NOT for use in other modules
*****************************************************************************************************/

/*****************************************************************************************************
* Code of project wide FUNCTIONS
*****************************************************************************************************/


/** 
*      @return none
*      
*      <b>Description:</b>  
*      \par
*      Table with the Low Power Demo application states
*      
*/
void ( * const vpfnLowPowerDemoStateMachine[])(void) =
{
		vfnLowPowerDemoIDLEState,
		vfnLowPowerDemoReadState,
		vfnLowPowerDemoCheckValueState,
		vfnLowPowerDemoGoLowPowerState,
		vfnLowPowerAlarmState,	
};

/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Idle state do nothing 
*/   
void vfnLowPowerDemoIDLEState(void)
{
	sMLowPowerDemoState.ActualState = LP_DEMO_READ_STATE;
	sMLowPowerDemoState.PrevState = LP_DEMO_IDLE_STATE;
	
}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Read the Potentiometer value or the Accelerometer position
*/   
void vfnLowPowerDemoReadState(void)
{
	if(gu8AppSelected)
		vfnSampleAccel();
	else
	{
		gu16PotVValue = u16fnADCRead(ADC_POT_CHANNEL);
		gu16PotVValue = (gu16PotVValue*330)/4095;
		gu16PotmVValue = gu16PotVValue%100;
		gu16PotVValue = gu16PotVValue/100;
	}
	sMLowPowerDemoState.ActualState = LP_DEMO_CHECK_VALUE_STATE;
	sMLowPowerDemoState.PrevState = LP_DEMO_READ_STATE;	
}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Depending the obtained value on past state go to sleep or send the alarm
*/ 
void vfnLowPowerDemoCheckValueState(void)
{
	if(gu8AppSelected)
	{
		if(HORIZONTAL_POSITION)   
			sMLowPowerDemoState.ActualState = LP_DEMO_ALARM_STATE;	
		else
		   sMLowPowerDemoState.ActualState = LP_DEMO_GO_LOW_POWER_STATE;
	}else
	{
		if(gu16PotVValue >= TWO_VOLTS_VALUE)	
			sMLowPowerDemoState.ActualState = LP_DEMO_ALARM_STATE;	
		else	
			sMLowPowerDemoState.ActualState = LP_DEMO_GO_LOW_POWER_STATE;
	}
	
	sMLowPowerDemoState.PrevState = LP_DEMO_CHECK_VALUE_STATE;	
	
}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Reset counters and flags and go to LLS mode 
*/ 
void vfnLowPowerDemoGoLowPowerState(void)
{

	gu8AlarmFlag = FALSE;
	gu16SecondsCounter = 0;

	vfnEnterLowPower();
	
	sMLowPowerDemoState.ActualState = LP_DEMO_IDLE_STATE;
	sMLowPowerDemoState.PrevState = LP_DEMO_GO_LOW_POWER_STATE;	
}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Every Second the LED toggle and a message will be send thru UART
*/ 
void vfnLowPowerAlarmState(void)
{	
	if(!gu8AlarmFlag)
	{
		gu8AlarmFlag = TRUE;
		uart(ON);
		vfnLEDsOutPutConfig();
		gu16SecondsCounter = 0;
	}	
        
	if(gu8lpTimerEventFlag)
	{
		gu16AlarmDelay++;
		gu8lpTimerEventFlag = FALSE;
	}
	
   if(gu16AlarmDelay == ALARM_DELAY)
   {
	   gu16SecondsCounter++;
	   LED1_TOGGLE;
	   if(gu8AppSelected)
		   	printf("Horizontal position (For %d Sec)\n\r",gu16SecondsCounter);
	   else
		    printf("Potentiometer value = %d.%2dv)\n\r",gu16PotVValue, gu16PotmVValue);
	   
	   	gu16AlarmDelay = 0;	   
   }
	
	sMLowPowerDemoState.ActualState = LP_DEMO_IDLE_STATE;
	sMLowPowerDemoState.PrevState = LP_DEMO_ALARM_STATE;		
}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	   Change the MCG and UART to use internal clock at 21000MHz
*/ 
void vfnMCGandUARTConfig(void)
{
	uint8 op_Mode;
	
  op_Mode = what_mcg_mode();
	if(op_Mode == PEE)
	{
		mcg_clk_hz = pee_pbe(CLK0_FREQ_HZ);
		mcg_clk_hz = pbe_fbe(CLK0_FREQ_HZ);
		mcg_clk_hz = fbe_fbi(4000000,1);
		mcg_clk_hz = fbi_fei(32000);
	}

	/*
     * Use the value obtained from the pll_init function to define variables
	 * for the core clock in kHz and also the peripheral clock. These
	 * variables can be used by other functions that need awareness of the
	 * system frequency.
	 */
    mcg_clk_khz = mcg_clk_hz / 1000;
	core_clk_khz = mcg_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> 28)+ 1);
  	periph_clk_khz = mcg_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
        
#if (defined(TWR_K21F100M))        
    /* Enable the UART5_TXD function on PTD8 (1) */
    PORTD_PCR8  = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
    /* Enable the UART5_RXD function on PTD9 (2) */
    PORTD_PCR9  = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin  	     
    uart_init(TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
#else
    /* Enable the UART0_TXD function on PTA14 */
    PORTA_PCR14 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin
    /* Enable the UART0_RXD function on PTA15 */
    PORTA_PCR15 = PORT_PCR_MUX(0x3); // UART is alt3 function for this pin	     
    uart_init(TERM_PORT, core_clk_khz, TERMINAL_BAUD);
#endif  	
	
    uart_init(TERM_PORT, core_clk_khz, TERMINAL_BAUD);
}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Configure Accelerometer 
*/ 
void vfnAccelConfig(void)
{
    //Initialize I2C
    init_I2C();
   //Configure accelemoter sensor
    I2CWriteRegister(0x2A,0x01);
}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Take some samples for the Tower position 
*/ 
void vfnSampleAccel(void)
{
	 while(gu32AccelSampling++<=ACCEL_SAMPLING_TIMES)
	 {
		 vfnAccelRead();             		            
	 } 
	 gu32AccelSampling = 0;  

}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Read tower position
*/ 
void vfnAccelRead(void)
{
        //Look at status of accelerometer
        I2CReadMultiRegisters(0x01,6);        
}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Configure LPTMR to interrupt every 100ms
*/ 
void vfnLPTMRConfig(void)
{
     vfnlptmrInit(100, LPTMR_USE_LPOCLK); //100
     enable_irq(INT_LPTimer-16);	 
}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Enable LLWU module and allow LPTMR as wake up module
*/ 
void vfnLowPowerConfig(void)
{
	  /*Enable all operation modes because this is a write once register*/  
           SMC_PMPROT =  SMC_PMPROT_AVLLS_MASK |
	                 SMC_PMPROT_ALLS_MASK  |    
			 SMC_PMPROT_AVLP_MASK;
	   
	   LLWU_ME |= (1<<LLWU_ME_WUME0_SHIFT);  //enable LPTMR to wake up
           
           enable_irq( INT_LLW-16);
		
}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Disable UART and configure GPIO as input to reduce current consumption and go to sleep
*/ 
void vfnEnterLowPower(void)
{
	vfnLEDsInPutConfig(); 
	uart(OFF);
	enter_lls();
}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Disable UART pins  
*/ 
void uart(uint8 u8state)
{
    if(u8state)
    {
        PORTA_PCR14 = PORT_PCR_MUX(3); // UART is alt3 function for this pin PORTE_PCR16 
        PORTA_PCR15 = PORT_PCR_MUX(3); // UART is alt3 function for this pin PORTE_PCR17
       
    }else
    {
       PORTA_PCR14 = PORT_PCR_MUX(0); // Analog is alt0 function for this pin
       PORTA_PCR15 = PORT_PCR_MUX(0); // Analog is alt0 function for this pin
    }
  
 
}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Enable clock gates
*/  
 void vfnEnableClockGates(void)
 {
	SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK;
	SIM_SCGC5 |= (SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTB_MASK);
        SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK; //Turn on clock to I2C` module
 }
 /*****************************************************************************************************************/

 /** 
 *      @return none
 *      
 *      <b>Description:</b>  
 * 	   \par
 * 	    Configure Low Power Timer to use Low Power Oscillator as source
 */ 
void vfnlptmrInit(int count, int clock_source)
{
   SIM_SCGC5|=SIM_SCGC5_LPTIMER_MASK;  
 
    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) // 0000 is div 2
                 | LPTMR_PSR_PBYP_MASK  // LPO feeds directly to LPT
                 | LPTMR_PSR_PCS(clock_source)) ; // use the choice of clock
              
    LPTMR0_CMR = LPTMR_CMR_COMPARE(count);  //Set compare value

    LPTMR0_CSR =(  LPTMR_CSR_TCF_MASK   // Clear any pending interrupt
                 | LPTMR_CSR_TIE_MASK   // LPT interrupt enabled
                 | LPTMR_CSR_TPS(0)     //TMR pin select
                 |!LPTMR_CSR_TPP_MASK   //TMR Pin polarity
                 |!LPTMR_CSR_TFC_MASK   // Timer Free running counter is reset whenever TMR counter equals compare
                 |!LPTMR_CSR_TMS_MASK   //LPTMR0 as Timer
                );
    
    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;   //Turn on LPT and start counting
}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Low power timer interrupt. 
*      Note: Use this function to avoid modify the common one on the driver 
*/ 
void lptmr_app_isr(void)
{
   //LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;  
  LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;   // write 1 to TCF to clear the LPT timer compare flag
  LPTMR0_CSR = ( LPTMR_CSR_TEN_MASK | LPTMR_CSR_TIE_MASK | LPTMR_CSR_TCF_MASK  );
  
  gu8lpTimerEventFlag = TRUE;
}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Configure ADC for single ended 
*/ 
void vfnADCConfig(void)
{
 uint8 cal_ok;
 
  SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
  PORTB_PCR2=(0|PORT_PCR_MUX(1));
  /* Initialize ADC0 */
  /* Do calibration first with 32 h/w averages*/
  Master_Adc_Config.CONFIG1  = ADLPC_NORMAL | ADC_CFG1_ADIV(ADIV_8) | ADLSMP_LONG | ADC_CFG1_MODE(MODE_16)
                              | ADC_CFG1_ADICLK(ADICLK_BUS_2);  
  Master_Adc_Config.CONFIG2  = MUXSEL_ADCA | ADACKEN_ENABLED | ADHSC_HISPEED | ADC_CFG2_ADLSTS(ADLSTS_20) ;
  Master_Adc_Config.COMPARE1 = 0x1234u ; 
  Master_Adc_Config.COMPARE2 = 0x5678u ;
  Master_Adc_Config.STATUS2  = ADTRG_SW | ACFE_DISABLED | ACFGT_GREATER | ACREN_ENABLED | DMAEN_DISABLED | ADC_SC2_REFSEL(REFSEL_EXT);
  Master_Adc_Config.STATUS3  = CAL_OFF | ADCO_SINGLE | AVGE_ENABLED | ADC_SC3_AVGS(AVGS_32);
  //Master_Adc_Config.PGA      = PGAEN_DISABLED | PGACHP_NOCHOP | PGALP_NORMAL | ADC_PGA_PGAG(PGAG_64);
  Master_Adc_Config.STATUS1A = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(12);       
  Master_Adc_Config.STATUS1B = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(12);  
  
  ADC_Config_Alt(ADC0_BASE_PTR, &Master_Adc_Config);  // config ADC
  cal_ok = ADC_Cal(ADC0_BASE_PTR);                    // do the calibration
  
  // Now do normal ADC configuration with 4 h/w averages and h/w trigger from PDB
  Master_Adc_Config.CONFIG1  = ADLPC_NORMAL | ADC_CFG1_ADIV(ADIV_8) | ADLSMP_LONG | ADC_CFG1_MODE(MODE_12)
                              | ADC_CFG1_ADICLK(ADICLK_BUS_2);  
  Master_Adc_Config.CONFIG2  = MUXSEL_ADCA | ADACKEN_ENABLED | ADHSC_HISPEED | ADC_CFG2_ADLSTS(ADLSTS_20) ;
  Master_Adc_Config.COMPARE1 = 0x1234u ; 
  Master_Adc_Config.COMPARE2 = 0x5678u ;
  Master_Adc_Config.STATUS2  = ADTRG_SW | ACFE_DISABLED | ACFGT_GREATER | ACREN_DISABLED | DMAEN_DISABLED | ADC_SC2_REFSEL(REFSEL_EXT);
  Master_Adc_Config.STATUS3  = CAL_OFF | ADCO_SINGLE | AVGE_ENABLED | ADC_SC3_AVGS(AVGS_4);
  //Master_Adc_Config.PGA      = PGAEN_DISABLED | PGACHP_NOCHOP | PGALP_NORMAL | ADC_PGA_PGAG(PGAG_64);
  
  Master_Adc_Config.STATUS1B = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(ADC_POT_CHANNEL); 
  
  ADC_Config_Alt(ADC0_BASE_PTR, &Master_Adc_Config);  // config the ADC again to default conditions    
}
/*****************************************************************************************************************/

/** 
*      @return none
*      
*      <b>Description:</b>  
* 	   \par
* 	    Low power timer interrupt. 
*      Read the value on the selected channel
*/ 
uint16 u16fnADCRead(uint8 channel)
{  
   ADC0_SC1A = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(channel);  // reads POT signal
   while((ADC0_SC1A & ADC_SC1_COCO_MASK)== 0){};
   return ADC0_RA; 
}


