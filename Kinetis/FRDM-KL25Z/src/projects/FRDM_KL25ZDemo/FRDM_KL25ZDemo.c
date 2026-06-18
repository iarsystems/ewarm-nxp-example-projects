/*
 * File:		fredom_test.c
 * Purpose:		Main process
 *
 */

#include "common.h"
#include "freedom_gpio.h"
#include "pit.h"
#include "hal_i2c.h"
#include "hal_dev_mma8451.h"
#include "angle_cal.h"
#include "task_mgr.h"
#include "global.h"
#include "TSIDrv.h"
#include "global.h"
#include "mcg.h"


#include "usb_cdc.h"
#include "usb_reg.h"

/* Testing Suite */
#define USB_DEVICE_XCVR_TEST
   
uint8 gu8USB_Buffer[64];

extern uint8 gu8USB_Flags; 
extern uint8 gu8EP3_OUT_ODD_Buffer[];
extern tBDT tBDTtable[];
extern uint8 gu8USB_Flags; 
extern uint8 gu8EP3_OUT_ODD_Buffer[];
extern tBDT tBDTtable[];



uint8 str1[] = "hola";
/*
*/


#define TPM_Cn_MODE  (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK)
#define TPM_MODULE    1000
#define TPM_INIT_VAL    25
#define SET_LED_GREEN(x)   TPM2_C1V = (x)
#define SET_LED_RED(x)     TPM2_C0V = (x)
#define SET_LED_BLUE(x)    TPM0_C1V = (x)


void accel_demo(void);
void accel_read(void);
void TPM_init(void);


/*****************************************************************************************************
* Definition of module wide VARIABLEs - NOT for use in other modules
*****************************************************************************************************/
signed short accel_x, accel_y, accel_z;
signed short resultx, resulty, resultz;





#define RED_MASK   0x01
#define GREEN_MASK 0x02
#define BLUE_MASK  0x04



void usb_init(void)
{
      printf("\n********** USB XCVR Module Testing **********\n");     
      USB_REG_SET_ENABLE;
      CDC_Init();
      // Pull up enable
      FLAG_SET(USB_CONTROL_DPPULLUPNONOTG_SHIFT,USB0_CONTROL);
}


void usb_service(void)
{
      // If data transfer arrives
        if(FLAG_CHK(EP_OUT,gu8USB_Flags))
        {
            (void)USB_EP_OUT_SizeCheck(EP_OUT);         
            usbEP_Reset(EP_OUT);
            usbSIE_CONTROL(EP_OUT);
            FLAG_CLR(EP_OUT,gu8USB_Flags);

            // Send it back to the PC
            EP_IN_Transfer(EP2,CDC_OUTPointer,1);
            
        }
        
           if (!ti_print)
            {
             ti_print = 300;
             sprintf(gu8USB_Buffer,"\rtsi %%= %03i ", AbsolutePercentegePosition);
             EP_IN_Transfer(EP2,gu8USB_Buffer,15);
            }
            
        
}    
        

void accel_init(void)
{
    unsigned char tmp;
    printf("****Accelerometor init ****\n");
   //Configure MMA8451 sensor //MMA8451Q
    hal_dev_mma8451_init();      //Initialize I2C modules
    tmp = hal_dev_mma8451_read_reg(0x2a);
    hal_dev_mma8451_write_reg(0x2a,tmp|0x01);
}



void accel_read(void)
{
  
        if((hal_dev_mma8451_read_reg(0x00)&0xf) != 0)
        {

            accel_x   = hal_dev_mma8451_read_reg(0x01)<<8;
            accel_x  |= hal_dev_mma8451_read_reg(0x02);
            accel_x >>= 2;

            accel_y   = hal_dev_mma8451_read_reg(0x03)<<8;
            accel_y  |= hal_dev_mma8451_read_reg(0x04);
            accel_y >>= 2;

            accel_z   = hal_dev_mma8451_read_reg(0x05)<<8;
            accel_z  |= hal_dev_mma8451_read_reg(0x06);
            accel_z >>= 2;

            resultx   = hal_dev_mma8451_read_reg(0x01)<<8;
            resultx  |= hal_dev_mma8451_read_reg(0x02);
            resultx >>= 8;

            resulty   = hal_dev_mma8451_read_reg(0x03)<<8;
            resulty  |= hal_dev_mma8451_read_reg(0x04);
            resulty >>= 8;

            resultz   = hal_dev_mma8451_read_reg(0x05)<<8;
            resultz  |= hal_dev_mma8451_read_reg(0x06);
            resultz >>= 8;
            
            
            angle_calculation(); //-900  to  900            
            detect_fall_detection();
            
            accel_demo();
            
            
        }
        
    
}

#define T_DELAY_LED  10


void accel_demo(void)
{
        static int note,notez;
        static int note1,notez1;
        static int note2,notez2;
        
        if (ti_accel_sampling==0)
       {
         accel_read();              
         ti_accel_sampling = 10;            
       } 
 
  
        
        static int cont=0x1;
        note  = yz_angle/100;  //9-9
        note1 = xy_angle/100;  //9-9
        note2 = xz_angle/100;  //9-9
        
        
        if (note != notez && xz_mag > 20 && !ti_delay)   //yz
        {
         if (note <-9) note=-9;
         if (note >9)  note =9;
         if (notez < note) notez++;
         if (notez > note) notez--;
        SET_LED_GREEN(notez*55+500);
        ti_delay = T_DELAY_LED;
        } 
         
        if (note1 != notez1 && xz_mag > 20 && !ti_delay)
        {
         if (note1 <-9) note1=-9;
         if (note1 >9)  note1 =9;
         if (notez1 < note1) notez1++;
         if (notez1 > note1) notez1--;
         SET_LED_RED(notez1*55+500);
         ti_delay = T_DELAY_LED;
        } 


       if (note2 != notez2 && xy_mag > 20 && !ti_delay)
        {
         if (note2 <-9) note2=-9;
         if (note2 >9)  note2 =9;
         if (notez2 < note2) notez2++;
         if (notez2 > note2) notez2--;
         SET_LED_BLUE(notez2*55+500);
         ti_delay = T_DELAY_LED;
        } 

  if (xy_mag < 20 &&  AbsolutePercentegePosition>0 ) 
      { 
  
         SET_LED_BLUE(AbsolutePercentegePosition*10);
         SET_LED_RED(AbsolutePercentegePosition*10);
         SET_LED_GREEN(AbsolutePercentegePosition*10);
      }
         
         
        }
        



/**   buzzer_TPM_init
 * \brief    Initialize LPTPM for RGB led control   
 * \brief    FTM2 and FTM  in PWM edge aligned mode 
 * \author   b01252
 * \param    none
 * \return   none
 */  


void TPM_init(void)
  { 
 
   SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTD_MASK;
   
   SIM_SCGC6|=( SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK);   
   
   SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); //
   
      
   PORTB_PCR18 = (0|PORT_PCR_MUX(3)); /* TPM2_CH0 enable on PTB18 */
   PORTB_PCR19 = (0|PORT_PCR_MUX(3)); /* TPM2_CH1 enable on PTB19 */
   PORTD_PCR1  = (0|PORT_PCR_MUX(4)); /* TPM0_CH1 enable on PTD1 */
   

   TPM0_MOD  = TPM_MODULE;  /* 0x0063 / 25MHz = 4uS PWM period */
   TPM0_C1SC = TPM_Cn_MODE;   /* No Interrupts; High True pulses on Edge Aligned PWM */
   TPM0_C1V  = TPM_INIT_VAL;  /* 90% pulse width */
   
   TPM2_MOD  = TPM_MODULE;  /* 0x0063 / 25MHz = 4uS PWM period */
   TPM2_C0SC = TPM_Cn_MODE;   /* No Interrupts; Low True pulses on Edge Aligned PWM */
   TPM2_C0V  = TPM_INIT_VAL;  /* 90% pulse width */
   TPM2_C1SC = TPM_Cn_MODE;   /* No Interrupts; Low True pulses on Edge Aligned PWM */
   TPM2_C1V  = TPM_INIT_VAL;  /* 90% pulse width */

   TPM2_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(0);     /* Edge Aligned PWM running from BUSCLK / 1 */
   TPM0_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(0);     /* Edge Aligned PWM running from BUSCLK / 1 */
  
  }





void vfn_test_accel(void)
{
     if (ti_accel_sampling==0)
       {
         accel_read();              
         ti_accel_sampling = 10;            
       } 
 
  
}


/*

*/

void vfn_rgb_test(void)
 { 
  static char seq_count=0;
   
  if (!ti_task)
  {  
   seq_count++;
   ti_task = 500;
   switch (seq_count)  
   {
    case 1: 
       SET_LED_GREEN(200);
       SET_LED_RED(0);
       SET_LED_BLUE(0);
         
     break;
    case 2: 
       SET_LED_GREEN(0);
       SET_LED_RED(200);
       SET_LED_BLUE(0);
     break;
   case 3: 
       SET_LED_GREEN(0);
       SET_LED_RED(0);
       SET_LED_BLUE(200);
     break;
    case 4: 
       SET_LED_GREEN(100);
       SET_LED_RED(100);
       SET_LED_BLUE(100);
      
     break;

   default:
     seq_count=0;
     next_task(accel_demo);
   };
   
   
  
  
  }
    
     
   
    
 }


const int Led_sequence[] =
   {
     0x0,
     RED_MASK,
     GREEN_MASK,
     BLUE_MASK,
     RED_MASK  | GREEN_MASK,
     RED_MASK  | BLUE_MASK,
     BLUE_MASK | GREEN_MASK,
     GREEN_MASK,
     BLUE_MASK,
     RED_MASK,
     0x0,
   };
  


/*
 This function assumes that system comes form PEE mode at 48 MHz
 it will change to PEE mode at 96 MHZ to ensure operation of USB 


*/
int pll_96MHz(void)
{ 
  int mcg_clk_hz;
  int temp_reg,i;
  int prdiv = PLL0_PRDIV;
  int vdiv = 48;
  
 
        
        

   mcg_clk_hz = pbe_pee(CLK0_FREQ_HZ);
   
                  /* Configuration for PLL freq = 96MHz */
        SIM_CLKDIV1 = ( 0
                        | SIM_CLKDIV1_OUTDIV1(1)
                        | SIM_CLKDIV1_OUTDIV4(1) );
   
  // Configure MCG_C6
  // The PLLS bit is set to enable the PLL, MCGOUT still sourced from ext ref clk
  // The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE bit in MCG_C6
  temp_reg = MCG_C6; // store present C6 value
  temp_reg &= ~MCG_C6_VDIV0_MASK; // clear VDIV settings
  temp_reg |= MCG_C6_PLLS_MASK | MCG_C6_VDIV0(prdiv - 24); // write new VDIV and enable PLL
  MCG_C6 = temp_reg; // update MCG_C6

  // wait for PLLST status bit to set
  for (i = 0 ; i < 2000 ; i++)
  {
    if (MCG_S & MCG_S_PLLST_MASK) break; // jump out early if PLLST sets before loop finishes
  }
  if (!(MCG_S & MCG_S_PLLST_MASK)) return 0x16; // check bit is really set and return with error if not set

  // Wait for LOCK bit to set
  for (i = 0 ; i < 4000 ; i++)
  {
    if (MCG_S & MCG_S_LOCK0_MASK) break; // jump out early if LOCK sets before loop finishes
  }
  if (!(MCG_S & MCG_S_LOCK0_MASK)) return 0x44; // check bit is really set and return with error if not set

  // Use actual PLL settings to calculate PLL frequency
  prdiv = ((MCG_C5 & MCG_C5_PRDIV0_MASK) + 1);
  vdiv = ((MCG_C6 & MCG_C6_VDIV0_MASK) + 24);

  // now in PBE

  MCG_C1 &= ~MCG_C1_CLKS_MASK; // clear CLKS to switch CLKS mux to select PLL as MCG_OUT

  // Wait for clock status bits to update
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x3) break; // jump out early if CLKST = 3 before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) return 0x1B; // check CLKST is set correctly and return with error if not

  // Now in PEE
  
//  return ((crystal_val / prdiv) * vdiv); //MCGOUT equals PLL output frequency
}

/********************************************************************/
        char aux1;
int main (void)
{
	char ch;
        
         
        

  //     pll_96MHz();
  
        gpio_init();
        Pit_init();
        TSI_Init();    
        TPM_init();
        usb_init();
        
        LED1_ON;
        LED2_ON;
        LED3_ON;
           
        LED1_OFF;
        LED2_OFF;
        LED3_OFF;
        
        next_task(vfn_rgb_test);
  	printf("\nRunning the fredom_test project.\n");
        accel_init();
        

	while(1)
	{
          CDC_Engine();
          ptr_next_task();  // do the actual function 
          TSI_SliderRead();
          usb_service();
    
	} 
}
/********************************************************************/