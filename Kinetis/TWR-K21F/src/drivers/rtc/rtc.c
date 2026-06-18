/*
 * File:        rtc.c
 * Purpose:     Provide common RTC routines
 *
 * Notes:       
 *              
 */


#include "common.h"
#include "rtc.h"
#include "arm_cm4.h"

/********************************************************************/
/*
 * Initialize the RTC
 *
 *
 * Parameters:
 *  seconds         Start value of seconds register
 *  alarm           Time in seconds of first alarm. Set to 0xFFFFFFFF to effectively disable alarm
 *  c_interval      Interval at which to apply time compensation can range from 1 second (0x0) to 256 (0xFF)
 *  c_value         Compensation value ranges from -127 32kHz cycles to +128 32 kHz cycles
 *                  80h Time prescaler register overflows every 32896 clock cycles.
 *                  FFh Time prescaler register overflows every 32769 clock cycles.
 *                  00h Time prescaler register overflows every 32768 clock cycles.
 *                  01h Time prescaler register overflows every 32767 clock cycles.
 *                  7Fh Time prescaler register overflows every 32641 clock cycles.
 *  interrupt       TRUE or FALSE
 */

//void rtc_init(uint32 seconds, uint32 alarm, uint8 c_interval, uint8 c_value, uint8 interrupt) 
//{
//  int i;
//  
//  /*enable the clock to SRTC module register space*/
//  SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;
//  
//  /*Only VBAT_POR has an effect on the SRTC, RESET to the part does not, so you must manually reset the SRTC to make sure everything is in a known state*/
//  /*clear the software reset bit*/
//    printf("Generating SoftWare reset to SRTC\n");
//    disable_irq(interrupt);
//    disable_irq(interrupt+1);
//    RTC_CR  = RTC_CR_SWR_MASK;
//    RTC_CR  &= ~RTC_CR_SWR_MASK;  
//  
//    if (RTC_SR & RTC_SR_TIF_MASK){
//        RTC_TSR = 0x00000000;   //  this action clears the TIF
//        printf("RTC Invalid flag was set - Write to TSR done to clears RTC_SR =  %#02X \n",  (RTC_SR) )  ;
//    }
//  /*Set time compensation parameters*/
//  RTC_TCR = RTC_TCR_CIR(c_interval) | RTC_TCR_TCR(c_value);
//  
//  /*Enable the counter*/
//  if (seconds >0) {
//     /*Enable the interrupt*/
//     if(interrupt >1){
//        enable_irq(interrupt);
//     }
//     /*Enable the oscillator*/
//    RTC_CR |= RTC_CR_OSCE_MASK;
//  
//    /*Wait to all the 32 kHz to stabilize, refer to the crystal startup time in the crystal datasheet*/
//    for(i=0;i<0x600000;i++);
// 
//    RTC_IER |= RTC_IER_TSIE_MASK;
//    RTC_SR |= RTC_SR_TCE_MASK;
//    /*Configure the timer seconds and alarm registers*/
//    RTC_TSR = seconds;
//
//  } else {
//    RTC_IER &= ~RTC_IER_TSIE_MASK;
//  }
//  if (alarm >0) {
//    RTC_IER |= RTC_IER_TAIE_MASK;
//    RTC_SR |= RTC_SR_TCE_MASK;
//    /*Configure the timer seconds and alarm registers*/
//    RTC_TAR = alarm;
//     /*Enable the interrupt*/
//     if(interrupt >1){
//        enable_irq(interrupt);
//     }
//     /*Enable the oscillator*/
//    RTC_CR |= RTC_CR_OSCE_MASK;
//  
//    /*Wait to all the 32 kHz to stabilize, refer to the crystal startup time in the crystal datasheet*/
//    for(i=0;i<0x600000;i++);
//  } else {
//    RTC_IER &= ~RTC_IER_TAIE_MASK;
//  }
//  rtc_reg_report();
//}

void rtc_reg_report (void) {
   printf("RTC_TSR    = 0x%02X,    ",    (RTC_TSR)) ;
  printf("RTC_TPR    = 0x%02X\n",       (RTC_TPR)) ;
  printf("RTC_TAR    = 0x%02X,    ",    (RTC_TAR)) ;
  printf("RTC_TCR    = 0x%02X\n",       (RTC_TCR)) ;
  printf("RTC_CR     = 0x%02X,    ",    (RTC_CR)) ;
  printf("RTC_SR     = 0x%02X\n",       (RTC_SR)) ;
  printf("RTC_LR     = 0x%02X,    ",    (RTC_LR)) ;
  printf("RTC_IER    = 0x%02X\n",       (RTC_IER)) ;
  printf("RTC_WAR    = 0x%02X,    ",    (RTC_WAR)) ;
  printf("RTC_RAR    = 0x%02X\n",       (RTC_RAR)) ;
}

void rtc_isr(void) 
{
    uint32 rtc_sr = RTC_SR;
    //rtc_isrv_count++;
    if(rtc_sr & RTC_SR_TAF_MASK) // RTC timer alarm flag is set
    {
      RTC_TAR = 0;     // write new value to TAR to clear TAF
    }
    
    if(rtc_sr & RTC_SR_TOF_MASK) // RTC timer Overlow flag is set
    {
     RTC_SR |= RTC_SR_TOF_MASK;
    }

    
    if (rtc_sr & RTC_SR_TIF_MASK) // Timer Invalid flag
    {
        RTC_SR &= ~RTC_SR_TCE_MASK;  //Disable timer
        RTC_TSR = 0x00; // write to clear TOF or TIF
        RTC_SR |= RTC_SR_TCE_MASK;   //re-enable timer
    }
}
