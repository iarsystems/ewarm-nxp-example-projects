/********************************************************************************
*                                                                     
*       Copyright (C) 2010 Freescale Semiconductor, Inc.              
*       All Rights Reserved								              
*														              
* Filename:     mcg_demo.c                
*														              
* Revision:     1.0 	
*
* Author:       Alistair Muir
*														              
* Functions:    Demo code for the K60 Tower board:
*               RD - displays the current MCG register values
*               WRT - allows a new value to be written to an MCG register
*               Autotrim - allows the fast and slow IRCs to be trimmed using the ATM
*               BLPI - moves from PEE mode to BLPI mode
*               PEE - moves from BLPI to PEE mode
*               FEI - moves from PEE to FEI mode
*               FEI2PEE - moves from FEI to PEE mode
*               ADC - uses the ADC along with the on board pot to control the PLL freq
*               CSTM1 - empty function that can be customized to move between modes
*               CSTM2 - empty function that can be customized to move between modes
*               BLPI_PEE - example code from the QRUG to switch to BLPI mode and then 
*                          back to pee mode
*               FEI_PEE - example code from the QRUG to switch from FEI to PEE mode
*               RTC_REF - example code from the QRUG to use the 32 kHz RTC oscillator 
*                         as the reference clock for the FLL.
*         
* Description:  
*
* Notes:        Routines are included to allow the movement from any MCG mode to any
*               other although not necessarily in the shortest way. You may need to go
*               through an indirect path.
*
*
* Instructions:  Uses serial terminal command-line interface scheme established
*                within \common\uif.c.  Default baud is 19200.
*                The auto trim should be run first to trim both IRCs
*                The slow IRC should be trimmed to 32768 Hz (atc 0 32768)
*                The fast IRC shoud be trimmed to 3600000 Hz (atc 1 3600000)
*                Using other frequencies may cause the serial communication to fail due to the baud rate being wrong
*
*                Type 'help' to see a list of commands/descriptions.
*                Type desired command to execute individual validation test 
*                cases, or to read/write registers for manual configuration.
*********************************************************************************/
#include "common.h"
#include "vectors.h"
#include "mcg_twr_demo.h"
#include "uif.h"
#include "uart.h"
#include "isr.h"

/********************************************************************/
/*
 * Setup user interface
 */
const char BANNER[] = "\nMCG Tower Demo\n";
const char PROMPT[] = "Test> ";
unsigned char state;
unsigned char clks_val;
uint32 TestPassesCount;
UIF_CMD UIF_CMDTAB[] =
{
    UIF_CMDS_ALL
    {"rd",0,0,0,cmd_rd, "Register Display",""},
    {"wrt",0,3,0,cmd_wrt, "Write MCG  <register index> <value>"},
    {"atc",0,2,0,cmd_atc,"Autotrim <0 = slow, 1 = fast> <frequency in Hz>"},
    {"blpi",0,0,0,cmd_blpi, "Move to BLPI Mode",""},
    {"pee",0,0,0,cmd_pee, "Move to PEE Mode",""},
    {"fei",0,0,0,cmd_fei, "Move to FEI Mode",""},
    {"fei2pee",0,0,0,cmd_fei_pee, "Move to PEE Mode",""},
    {"adc",0,0,0,cmd_adc, "ADC Test",""},
    {"atc_loop",0,3,0,cmd_atc_loop,"Autotrim <0 = slow, 1 = fast> <start frequency in Hz> <stop frequency in Hz>"},
    {"cstm1",0,0,0,cmd_cstm1, "Custom 1 function",""},
    {"cstm2",0,0,0,cmd_cstm2, "Custom 2 function",""},
    {"blpi_pee",0,0,0,cmd_blpi_pee, "Move to BLPI and back to PEE",""},
    {"fei_pee",0,0,0,cmd_fei_pee_demo, "Move from FEI to PEE Mode",""},
    {"rtc_ref",0,0,0,cmd_rtc_as_refclk, "Use the RTC 32kHz clock as the FLL ref clock",""},

};

UIF_SETCMD UIF_SETCMDTAB[] =
{
    {"sysclk", 0,1,setshow_sysclk,""},
};

const int UIF_NUM_CMD    = UIF_CMDTAB_SIZE;
const int UIF_NUM_SETCMD = UIF_SETCMDTAB_SIZE;

//global variables
int  error_cnt=0;
int *res_ptr;
int  redge_flag=0;    // Flag to indicate when Rising Edge Interrupt has occurred.
int  fedge_flag=0;    // Flag to indicate when Falling Edge Interrupt has occurred.
int  index=0;         // Looping index used to select which of the three CMPs to test.
extern int core_clk_khz;
extern int core_clk_mhz;
extern int periph_clk_khz;

unsigned char sctrim_val;
unsigned char scftrim_val;

char *dummy_argv[10];
char dummyline [10];
CMP_MemMapPtr cmpch;



void main(void)
{
  
//    DisableInterrupts;
  
/* Enable FTM2 Interrupt in NVIC*/
    NVICICPR2|=(1);   //Clear any pending interrupts 
    NVICISER2|=(1);   //Enable interrupts from FTM2 module    
    EnableInterrupts;

    Port_Init(); 
    ftm_enable();
    timer_setup();
//    Int_Vector_Set();
    printf("\n");
    printf("**************************************************\n");
    printf("*                                                *\n");
    printf("* MCG Tower Demo                                 *\n");
    printf("*                                                *\n");
    printf("**************************************************\n");
    printf(HELPMSG);
    printf("\n\n");

//    outSRS();
    mainloop();
}// end main

//******************************************************************************
void mainloop (void)
{

    while (TRUE)
    {
        printf(PROMPT);
        run_cmd();
    }
}// end mainloop
   
//******************************************************************************
//***************************  TEST CASES  *************************************
//******************************************************************************

void cmd_rd (int argc, char **argv)
{

    printf("\n-------------------------------------------\n");
    printf("       MCG Register Read Utility\n");
    printf("-------------------------------------------\n");
    printf("\n");
        printf("MCG_C1   = %#02X \r\n", (MCG_C1));
        printf("MCG_C2   = %#02X \r\n", (MCG_C2));
        printf("MCG_C3   = %#02X \r\n", (MCG_C3));
        printf("MCG_C4   = %#02X \r\n", (MCG_C4)) ;
        printf("MCG_C5   = %#02X \r\n", (MCG_C5));
        printf("MCG_C6   = %#02X \r\n\n", (MCG_C6));
        printf("MCG_S    = %#02X \r\n\n", (MCG_S)) ;
        printf("MCG_ATC   = %#02X \r\n",   (MCG_ATC)) ;
        printf("MCG_ATCVL = %#02X \r\n",   (MCG_ATCVL)) ;
        printf("MCG_ATVCH = %#02X \r\n",   (MCG_ATCVH));
   
}// end cmd_rd

//******************************************************************************

void
cmd_wrt (int argc, char **argv)
{
    uint32 regindex;
    uint32 regvalue;
    
    int result = 0;
    res_ptr = &result;
         
    printf("\n-------------------------------------------\n");
    printf("  MCG Single Register Write Utility\n");
    printf("-------------------------------------------\n");
    printf("\n");
    if (argc != 3)
    {
        printf("Valid 'wrt' syntax:\n");
        printf("MCG> wrt  <register index> <value> \n\n");
        printf("MCG register index and current values are: \n\n");
        printf("MCG_C1     Register  0 = %#02X \r\n",    (MCG_C1))  ;
        printf("MCG_C2     Register  1 = %#02X \r\n",    (MCG_C2))  ;
        printf("MCG_C3     Register  2 = %#02X \r\n",  (MCG_C3) )  ;
        printf("MCG_C4     Register  3 = %#02X \r\n",    (MCG_C4)   )   ;
        printf("MCG_C5     Register  4 = %#02X \r\n",    (MCG_C5)   )   ;
        printf("MCG_C6     Register  5 = %#02X \r\n\n",    (MCG_C6) ) ; 
        printf("MCG_S      Register  6 = %#02X \r\n\n",  (MCG_S) )   ;
        printf("MCG_ATC     Register  7 = %#02X \r\n",    (MCG_ATC)  ) ;
        printf("MCG_ATCVL   Register  8 = %#02X \r\n",    (MCG_ATCVL)  ) ;
        printf("MCG_ATCVH   Register  9 = %#02X \r\n",    (MCG_ATCVH)  ) ;

        return;
    }
    else{
      
        regindex = get_value(argv[1],res_ptr,10);
        regvalue = get_value(argv[2],res_ptr,16);
                
         if(regvalue<256){        
              switch (regindex){
                 case 0: MCG_C1 = regvalue;
                    break;
                 case 1: MCG_C2 = regvalue;
                    break;
                 case 2: MCG_C3 = regvalue;
                    break;
                 case 3: MCG_C4 = regvalue;
                    break;
                 case 4: MCG_C5 = regvalue;
                    break;
                 case 5: MCG_C6 = regvalue;
                    break;
                 case 6: MCG_S = regvalue;
                    break;
                 case 7: MCG_ATC = regvalue;
                    break;                      
                 case 8: MCG_ATCVL = regvalue;
                    break;
                 case 9: MCG_ATCVH = regvalue;
                    break;
                 default: break;
              } // end switch/case regindex
         } // if
        else 
        printf("Register value not in valid range (0x00-0xFF).\r\n\n");
        printf("MCG_C1     Register  0 = %#02X \r\n",    (MCG_C1))  ;
        printf("MCG_C2     Register  1 = %#02X \r\n",    (MCG_C2))  ;
        printf("MCG_C3     Register  2 = %#02X \r\n\n",  (MCG_C3) )  ;
        printf("MCG_C4     Register  3 = %#02X \r\n",    (MCG_C4)   )   ;
        printf("MCG_C5     Register  4 = %#02X \r\n",    (MCG_C5)   )   ;
        printf("MCG_C6     Register  5 = %#02X \r\n",    (MCG_C6) ) ; 
        printf("MCG_S      Register  6 = %#02X \r\n\n",  (MCG_S) )   ;
        printf("MCG_ATC    Register  7 = %#02X \r\n",    (MCG_ATC)  ) ;
        printf("MCG_ATCVL   Register  8 = %#02X \r\n",    (MCG_ATCVL)  ) ;
        printf("MCG_ATCVH   Register  9 = %#02X \r\n",    (MCG_ATCVH)  ) ;
    }// end else
}// end cmd_wrt


void cmd_atc (int argc, char **argv)
{
    unsigned char regindex;
    uint32 regvalue;
    uint32 temp_reg = 0;
    unsigned char temp_reg8 = 0;
    unsigned char irclken_state;
    uint32 orig_SIM_CLKDIV1;
    unsigned short atcv;
    
    int result = 0;
    res_ptr = &result;
         
    printf("\n-------------------------------------------\n");
    printf("  MCG AutoTrim Utility\n");
    printf("-------------------------------------------\n");
    printf("\n");
    if (argc != 3)
    {
        printf("Valid 'atc' syntax:\n");
        printf("MCG> atc  <0 = slow, 1 = fast> <frequency in Hz> \n\n");

        return;
    }
    else{
      
        regindex = get_value(argv[1],res_ptr,10);
        regvalue = get_value(argv[2],res_ptr,10);
        clks_val = ((MCG_C1 & MCG_C1_CLKS_MASK) >> 6);
        
        if (((clks_val == 1) || (clks_val ==3)))
        {
          printf("\nAuto trim error - cannot use the internal clock source.\r\n\n");
          return; //error using IRC as system clock
        }
              
        if ((clks_val == 0) && (!(MCG_C6 & MCG_C6_PLLS_MASK)) && (MCG_C1 & MCG_C1_IREFS_MASK))
        {
          printf("\nAuto trim error - cannot use the FLL with internal clock source.\r\n\n");
          return; // error using FLL with IRC
        }
        if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 3)
        {
          printf("\nWARNING - PLL is not the internal clock source. Auto trim value will not be correct\r\n\n");
        }
                
        if(!regindex) //determine if slow or fast IRC to be trimmed
        {
          // NOTE: IRCLKEN should not be required - 1.0 errata.
          // Remove this when silicon is fixed.         
          MCG_C2 &= ~MCG_C2_IRCS_MASK; //must select the slow IRC to be enabled
          // store MCG_C1 value to maintain IRCLEN state after autotrim is finished
          irclken_state = MCG_C1;
          MCG_C1 |= MCG_C1_IRCLKEN_MASK; // set IRCLKEN
        } // if
        orig_SIM_CLKDIV1 = SIM_CLKDIV1;        // backup current divider value
        temp_reg = SIM_CLKDIV1;                // bus clock needs to be between 8 and 16 MHz
        temp_reg &= ~SIM_CLKDIV1_OUTDIV2_MASK; // clear bus divider field
        temp_reg |= SIM_CLKDIV1_OUTDIV2(5);    //divide 96MHz PLL by 6 = 16MHz bus clock
        SIM_CLKDIV1 = temp_reg; 
// Set up autocal registers, must use floating point calculation
        if (regindex)
          atcv = (unsigned short)(128.0f * (21.0f * (16000000.0f / (float)regvalue))); 
        else
          atcv = (unsigned short)(21.0f * (16000000.0f / (float)regvalue));
        
        MCG_ATCVL = (atcv & 0xFF); //Set ATCVL to lower 8 bits of count value
        MCG_ATCVH = ((atcv & 0xFF00) >> 8); // Set ATCVH to upper 8 bits of count value

// Enable autocal
        MCG_ATC = 0x0; // clear auto trim control register
        temp_reg8 |= (MCG_ATC_ATME_MASK | (regindex << MCG_ATC_ATMS_SHIFT)); //Select IRC to trim and enable trim machine
        MCG_ATC = temp_reg8;
        
        while (MCG_ATC & MCG_ATC_ATME_MASK) {}; //poll for ATME bit to clear
        
        SIM_CLKDIV1 = orig_SIM_CLKDIV1; //restore the divider value
        
        if (MCG_ATC & MCG_ATC_ATMF_MASK) // check if error flag set
        {
          printf("Autotrim error.\r\n\n");
          printf("\n");
          printf("MCG_C1   = %#02X \r\n", (MCG_C1));
          printf("MCG_C2   = %#02X \r\n", (MCG_C2));
          printf("MCG_C3   = %#02X \r\n", (MCG_C3));
          printf("MCG_C4   = %#02X \r\n", (MCG_C4)) ;
          printf("MCG_C5   = %#02X \r\n", (MCG_C5));
          printf("MCG_C6   = %#02X \r\n\n", (MCG_C6));
          printf("MCG_S    = %#02X \r\n\n", (MCG_S)) ;
          printf("MCG_ATC   = %#02X \r\n",   (MCG_ATC)) ;
          printf("MCG_ATCVL = %#02X \r\n",   (MCG_ATCVL)) ;
          printf("MCG_ATVCH = %#02X \r\n",   (MCG_ATCVH));
          MCG_ATC |= MCG_ATC_ATMF_MASK; // clear fail flag
        } 
        else 
        {      
          printf("Autotrim Passed.\r\n\n");
          printf("MCG_C3   = %#02X \r\n", (MCG_C3));
          printf("MCG_C4   = %#02X \r\n", (MCG_C4));
          // Check trim value is not at either extreme of the range
          if (!regindex)
          {
            sctrim_val = MCG_C3; //store slow IRC trim value
            scftrim_val = MCG_C4 & MCG_C4_SCFTRIM_MASK; // store fine trim bit
            if ((MCG_C3 == 0xFF) || (MCG_C3 == 0))
            {
              printf("\nAutotrim result is not valid.\r\n\n");
            }
            // restore MCG_C1 value 
            MCG_C1 = irclken_state;
          }
          else
          {
            if ((((MCG_C4 & MCG_C4_FCTRIM_MASK) >> MCG_C4_FCTRIM_SHIFT) == 0xF) ||
              (((MCG_C4 & MCG_C4_FCTRIM_MASK) >> MCG_C4_FCTRIM_SHIFT) == 0))
            {
              printf("\nAutotrim result is not valid.\r\n\n");
            }
          }
        }
          
    }// end else
}// end cmd_atc



void cmd_atc_loop (int argc, char **argv)
{
    unsigned char regindex;
    uint32 start_freq;
    uint32 stop_freq;
    uint32 step_size;

    uint32 i;
    uint32 temp_reg = 0;
    unsigned char temp_reg8 = 0;
    uint32 orig_SIM_CLKDIV1;
    unsigned short atcv;
    
    int result = 0;
    res_ptr = &result;
         
    printf("\n-------------------------------------------\n");
    printf("  MCG AutoTrim Utility\n");
    printf("-------------------------------------------\n");
    printf("\n");
    if (argc != 4)
    {
        printf("Valid 'atc' syntax:\n");
        printf("MCG> atc  <0 = slow, 1 = fast> <frequency in Hz> \n\n");

        return;
    }
    else{
      
        regindex = get_value(argv[1],res_ptr,10);
        start_freq = get_value(argv[2],res_ptr,10);
        stop_freq = get_value(argv[3],res_ptr,10);
        clks_val = ((MCG_C1 & MCG_C1_CLKS_MASK) >> 6);
        
        if (((clks_val == 1) || (clks_val ==3)))
        {
          printf("\nAuto trim error - cannot use the internal clock source.\r\n\n");
          return; //error using IRC as system clock
        }
              
        if ((clks_val == 0) && (!(MCG_C6 & MCG_C6_PLLS_MASK)) && (MCG_C1 & MCG_C1_IREFS_MASK))
        {
          printf("\nAuto trim error - cannot use the FLL with internal clock source.\r\n\n");
          return; // error using FLL with IRC
        }
        if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 3)
        {
          printf("\nWARNING - PLL is not the internal clock source. Auto trim value will not be correct\r\n\n");
        }
                
        if(!regindex) //determine if slow or fast IRC to be trimmed
        {
          // NOTE: IRCLKEN should not be required - 1.0 errata.
          // Remove this when silicon is fixed.         
          MCG_C2 &= ~MCG_C2_IRCS_MASK; //must select the slow IRC to be enabled
          MCG_C1 |= MCG_C1_IRCLKEN_MASK;
          step_size = 1;
        }
        else
        {
          step_size = 100;
        } // if
        orig_SIM_CLKDIV1 = SIM_CLKDIV1; // bus clock needs to be between 8 and 16 MHz
        temp_reg = SIM_CLKDIV1;
        temp_reg &= ~SIM_CLKDIV1_OUTDIV2_MASK; // clear bus divider field
        temp_reg |= SIM_CLKDIV1_OUTDIV2(5); //divide 96MHz PLL by 6 = 16MHz bus clock
        SIM_CLKDIV1 = temp_reg; 
        
        periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
        if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
           uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
        else
           uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
// Set up autocal registers, must use floating point calculation
        
        for (i = start_freq ; i < stop_freq ; i = i + step_size)
        {
        if (regindex)
          atcv = (unsigned short)(128.0f * (21.0f * (16000000.0f / (float)i))); 
        else
          atcv = (unsigned short)(21.0f * (16000000.0f / (float)i));
        
        MCG_ATCVL = (atcv & 0xFF); //Set ATCVL to lower 8 bits of count value
        MCG_ATCVH = ((atcv & 0xFF00) >> 8); // Set ATCVH to upper 8 bits of count value

// Enable autocal
        MCG_ATC = 0x0; // clear auto trim control register
        temp_reg8 |= (MCG_ATC_ATME_MASK | (regindex << MCG_ATC_ATMS_SHIFT)); //Select IRC to trim and enable trim machine
        MCG_ATC = temp_reg8;
        
        while (MCG_ATC & MCG_ATC_ATME_MASK) {}; //poll for ATME bit to clear
        
       
        if (MCG_ATC & MCG_ATC_ATMF_MASK)
        {
          printf("Autotrim error.\r\n\n");
          MCG_ATC |= MCG_ATC_ATMF_MASK; // clear fail flag
        } 
        else 
        {
          result = 0;
          result |= (MCG_C3 << 1);
          result |= (MCG_C4 & MCG_C4_SCFTRIM_MASK);
          printf("%d %d\r\n", i, result);
          // Check trim value is not at either extreme of the range         
        }
        } //for 
    }// end else
    
    SIM_CLKDIV1 = orig_SIM_CLKDIV1; //restore the divider value
    periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
        if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
           uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
        else
           uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
    
}// end cmd_atc_loop

void cmd_blpi(int argc, char **argv)
{
  pee_pbe();  
  pbe_blpe();
  blpe_fbe();
  fbe_fee();
  fee_fei();
  fei_fbi();
  fbi_blpi();
  
  printf("\n  MCG now running in BLPI mode\r\n\n");
}

void cmd_pee(int argc, char **argv)
{
  blpi_fbi();
  fbi_fbe();
  fbe_pbe();
  pbe_pee();
  
  printf("\n  MCG now running in PEE mode\r\n\n");
}

void cmd_fei(int argc, char **argv)
{
// copy previously stored trim values 
  MCG_C3 = sctrim_val;
  if (scftrim_val)
    MCG_C4 |= MCG_C4_SCFTRIM_MASK;
  else
    MCG_C4 &= ~MCG_C4_SCFTRIM_MASK;
  
  pee_pbe();  
  pbe_blpe();
  blpe_fbe();
  fbe_fee();
  fee_fei();
  
  printf("\n  MCG now running in FEI mode\r\n\n");
}

void cmd_fei_pee(int argc, char **argv)
{
  fei_fbi();
  fbi_fbe();
  fbe_pbe();
  pbe_pee();
  
  printf("\n  MCG now running in PEE mode\r\n\n");
}

void cmd_adc(int argc, char **argv)
{
  adc_test();
  
  printf("  ADC\r\n\n");
}


void cmd_blpi_pee(int argc, char **argv)
{
  unsigned char temp_reg;
// demo version of QRUG code 
// must be in PEE mode before running this
// must trim fast irc to 3600000 Hz before running this to keep the UART working 
// Moving from PEE to BLPI  
// first move from PEE to PBE 
  temp_reg = MCG_C1;
  temp_reg &= ~MCG_C1_FRDIV_MASK; // clear FRDIV field
#if (K60_CLK)  
  temp_reg |= MCG_C1_FRDIV(5);    //set FLL ref divider to 1024
#else  
  temp_reg |= MCG_C1_FRDIV(3);    //set FLL ref divider to 256
#endif  
  temp_reg |= MCG_C1_CLKS(2); // select external reference clock as MCG_OUT
  MCG_C1 = temp_reg;
// Wait for clock status bits to update indicating clock has switched
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){} 
// now move to FBE mode
// make sure the FRDIV is configured to keep the FLL reference within spec.  

  MCG_C6 &= ~MCG_C6_PLLS_MASK; // clear PLLS to select the FLL
  
  while (MCG_S & MCG_S_PLLST_MASK){} // Wait for PLLST status bit to clear to
                                     // indicate switch to FLL output
// now move to FBI mode
  MCG_C2 |= MCG_C2_IRCS_MASK; // set the IRCS bit to select the fast IRC
// set CLKS to 1 to select the internal reference clock
// keep FRDIV at existing value to keep FLL ref clock in spec.
// set IREFS to 1 to select internal reference clock
  temp_reg = MCG_C1;
  temp_reg &= ~MCG_C1_CLKS_MASK;
  temp_reg |= MCG_C1_CLKS(1) | MCG_C1_IREFS_MASK;  
  MCG_C1 = temp_reg;
// wait for internal reference to be selected  
  while (!(MCG_S & MCG_S_IREFST_MASK)){} 
// wait for fast internal reference to be selected  
  while (!(MCG_S & MCG_S_IRCST_MASK)){} 
// wait for clock to switch to IRC 
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x1){} 
// now move to BLPI
  MCG_C2 |= MCG_C2_LP_MASK; // set the LP bit to enter BLPI

// set up the SIM clock dividers BEFORE switching to VLPR to ensure the
// system clock speeds are in spec. MCGCLKOUT = 2 MHz in BLPI mode
// core = 2 MHz, bus = 2 MHz, flexbus = 2 MHz, flash = 1 MHz  
  SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(0) 
              | SIM_CLKDIV1_OUTDIV3(0) | SIM_CLKDIV1_OUTDIV4(1);
// in BLPI
  core_clk_khz = 1850;
  periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
      uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  else
      uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
  
  cmd_rd (0, dummy_argv); // print out registers
  
  printf("\n  MCG now running in BLPI mode\r\n\n");
  printf("\n  Press any key to return to PEE mode\r\n\n");

// would normally enter VLPR now  
// wait for character to be typed  
  while (!((UART_S1_REG(TERM_PORT) & UART_S1_RDRF_MASK))) {}
  
// Moving from BLPI to PEE
// first move to FBI
  MCG_C2 &= ~MCG_C2_LP_MASK; // clear the LP bit to exit BLPI  
// move to FBE
// clear IREFS to select the external ref clock 
// set CLKS = 2 to select the ext ref clock as clk source
#if defined(K60_CLK) 
// K60 tower board uses 50MHz ext clock, do not enable the oscillator  
  MCG_C2 = 0;
#else
// Enable external oscillator, RANGE=1, HGO=1, EREFS=1, LP=0, IRCS=0
// K40 tower board uses 8MHz crystal    
  MCG_C2 = MCG_C2_RANGE(1) | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK;
#endif
  temp_reg = MCG_C1;
  temp_reg &= ~MCG_C1_FRDIV_MASK; // clear FRDIV field
  temp_reg &= ~MCG_C1_CLKS_MASK;  // clear CLKS to select FLL output
  temp_reg |= MCG_C1_CLKS(2);     // select ext ref clk
#if (K60_CLK)  
  temp_reg |= MCG_C1_FRDIV(5);    //set FLL ref divider to 1024
#else  
  temp_reg |= MCG_C1_FRDIV(3);    //set FLL ref divider to 256
#endif
  temp_reg &= ~MCG_C1_IREFS_MASK; // clear IREFS
  MCG_C1 = temp_reg;              // update MCG_C1 
  
  MCG_C4 &= ~MCG_C4_DMX32_MASK;   // clear DMX32 (not req'd but keeps it in consistant state 

/* if we aren't using an osc input we don't need to wait for the osc to init */
#if (!defined(K60_CLK))
  while (!(MCG_S & MCG_S_OSCINIT_MASK)){};  // wait for oscillator to initialize
#endif  
  while (MCG_S & MCG_S_IREFST_MASK){}; // wait for Reference Status bit to update
  
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}; // Wait for clock status bits to update  

// Now configure the PLL and move to PBE mode
// set the PRDIV field to generate a 4MHz reference clock (8MHz /2) for K40
// set the PRDIV field to generate a 2MHz reference clock (50MHz /25) for K60   
#if (K60_CLK)  
  MCG_C5 = MCG_C5_PRDIV(24);    //set PLL ref divider to 25
#else  
  MCG_C5 = MCG_C5_PRDIV(1);    //set PLL ref divider to 2
#endif  

// set the VDIV field to 0, which is x24, giving 4 x 24  = 96 MHz for K40
// set the VDIV field to 24, which is x48, giving 2 x 48  = 96 MHz for K60
// the PLLS bit is set to enable the PLL
// You should enable clock monitor now that external reference is being used
// In this example it is not emabled as changing MCG clock modes may cause an
// unexpected reset if an internal clocking mode is selected with CME set  
// LOLIE can be optionally set to enable the loss of lock interrupt
#if (K60_CLK)  
  MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(24);
#else
  MCG_C6 = MCG_C6_PLLS_MASK;
#endif  

// wait until the source of the PLLS clock has switched to the PLL  
  while (!(MCG_S & MCG_S_PLLST_MASK)){}
// wait until the PLL has achieved lock
  while (!(MCG_S & MCG_S_LOCK_MASK)){}
// set up the SIM clock dividers BEFORE switching to the PLL to ensure the
// system clock speeds are in spec.
// core = PLL, bus = PLL/2, flexbus = PLL/2, flash = PLL/4  
  SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(1) 
              | SIM_CLKDIV1_OUTDIV3(1) | SIM_CLKDIV1_OUTDIV4(3);
  
// Transition into PEE by setting CLKS to 0
// previous MCG_C1 settings remain the same, just need to set CLKS to 0
  MCG_C1 &= ~MCG_C1_CLKS_MASK;

// Wait for MCGOUT to switch over to the PLL
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){}
// set the core clock to adjust the UART baud rate  
 
  core_clk_khz = 96000;
      
  periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
      uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  else
      uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
  
  printf("\n  MCG now running in PEE mode\r\n\n");
} //cmd_blpi_pee


void cmd_fei_pee_demo(int argc, char **argv)
{
  unsigned char temp_reg;
// demo version of QRUG code   
// must be in FEI mode before running this  
// If the internal load capacitors are being used, they should be selected 
// before enabling the oscillator. Application specific. 16pF and 8pF selected
// in this example
  OSC_CR = OSC_CR_SC16P_MASK | OSC_CR_SC8P_MASK;
// Enabling the oscillator for 8 MHz crystal
// RANGE=1, should be set to match the frequency of the crystal being used
// HGO=1, high gain is selected, provides better noise immunity and faster
// oscillator start time but does draw higher current
// EREFS=1, enable the external oscillator
// LP=0, low power mode not selected (not actually part of osc setup)
// IRCS=0, slow internal ref clock selected (not actually part of osc setup)
#if defined(K60_CLK) 
// K60 tower board uses 50MHz ext clock, do not enable the oscillator  
  MCG_C2 = 0;
#else
// Enable external oscillator, RANGE=1, HGO=1, EREFS=1, LP=0, IRCS=0
// K40 tower board uses 8MHz crystal    
  MCG_C2 = MCG_C2_RANGE(1) | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK;
#endif
  
// Select ext oscillator, reference divider and clear IREFS to start ext osc
// CLKS=2, select the external clock source 
// FRDIV=3, set the FLL ref divider to keep the ref clock in range 
//         (even if FLL is not being used) 8 MHz / 256 = 31.25 kHz         
// IREFS=0, select the external clock 
// IRCLKEN=0, disable IRCLK (can enable it if desired)
// IREFSTEN=0, disable IRC in stop mode (can keep it enabled in stop if desired)
  temp_reg = MCG_C1;
  temp_reg &= ~MCG_C1_FRDIV_MASK; // clear FRDIV field
  temp_reg &= ~MCG_C1_CLKS_MASK;  // clear CLKS to select FLL output
  temp_reg |= MCG_C1_CLKS(2);     // select ext ref clk
#if (K60_CLK)  
  temp_reg |= MCG_C1_FRDIV(5);    //set FLL ref divider to 1024
#else  
  temp_reg |= MCG_C1_FRDIV(3);    //set FLL ref divider to 256
#endif
  temp_reg &= ~MCG_C1_IREFS_MASK; // clear IREFS
  MCG_C1 = temp_reg;              // update MCG_C1

// wait for oscillator to initialize
/* if we aren't using an osc input we don't need to wait for the osc to init */
#if (!defined(K60_CLK))
  while (!(MCG_S & MCG_S_OSCINIT_MASK)){};  // wait for oscillator to initialize
#endif  

// wait for Reference clock to switch to external reference 
  while (MCG_S & MCG_S_IREFST_MASK){} 

// Wait for MCGOUT to switch over to the external reference clock 
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}

// Now configure the PLL and move to PBE mode
// set the PRDIV field to generate a 4MHz reference clock (8MHz /2) for K40
// set the PRDIV field to generate a 2MHz reference clock (50MHz /25) for K60   
#if (K60_CLK)  
  MCG_C5 = MCG_C5_PRDIV(24);    //set PLL ref divider to 25
#else  
  MCG_C5 = MCG_C5_PRDIV(1);    //set PLL ref divider to 2
#endif  

// set the VDIV field to 0, which is x24, giving 4 x 24  = 96 MHz for K40
// set the VDIV field to 24, which is x48, giving 2 x 48  = 96 MHz for K60
// the PLLS bit is set to enable the PLL
// You should enable clock monitor now that external reference is being used
// In this example it is not emabled as changing MCG clock modes may cause an
// unexpected reset if an internal clocking mode is selected with CME set  
// LOLIE can be optionally set to enable the loss of lock interrupt
#if (K60_CLK)  
  MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(24);
#else
  MCG_C6 = MCG_C6_PLLS_MASK;
#endif  

// wait until the source of the PLLS clock has switched to the PLL  
  while (!(MCG_S & MCG_S_PLLST_MASK)){}
// wait until the PLL has achieved lock
  while (!(MCG_S & MCG_S_LOCK_MASK)){}
// set up the SIM clock dividers BEFORE switching to the PLL to ensure the
// system clock speeds are in spec.
// core = PLL, bus = PLL/2, flexbus = PLL/2, flash = PLL/4  
  SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(1) 
              | SIM_CLKDIV1_OUTDIV3(1) | SIM_CLKDIV1_OUTDIV4(3);
  
// Transition into PEE by setting CLKS to 0
// previous MCG_C1 settings remain the same, just need to set CLKS to 0
  MCG_C1 &= ~MCG_C1_CLKS_MASK;

// Wait for MCGOUT to switch over to the PLL
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){}
// set the core clock to adjust the UART baud rate  

  core_clk_khz = 96000;
      
  periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
      uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  else
      uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
  
  printf("\n  MCG now running in PEE mode\r\n\n");
  
} //cmd_fei_pee



void pee_pbe(void)
{  
  MCG_C1 |= MCG_C1_CLKS(2); // select external reference clock as MCG_OUT
  // Wait for clock status bits to update 
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}; 

#if (K60_CLK)
  core_clk_khz = 50000;
#else
  core_clk_khz = 8000;
#endif
  
  periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
      uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  else
      uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
  
} // freq = REF_CLK

void pbe_blpe(void)
{
  MCG_C2 |= MCG_C2_LP_MASK; //set LP bit to disable the PLL
  
#if (K60_CLK)
  core_clk_khz = 50000;
#else
  core_clk_khz = 8000;
#endif
  
  periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
      uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  else
      uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
  
} // freq = REF_CLK

void blpe_fbe(void)
{
  unsigned char temp_reg;
 
// Set FRDIV to keep FLL ref clock in range, even though FLL is not being used  
#if (K60_CLK)
  temp_reg = MCG_C1;
  temp_reg &= ~MCG_C1_FRDIV_MASK; // clear FRDIV field
  temp_reg |= MCG_C1_FRDIV(5); //set FLL ref divider to 1024
  MCG_C1 = temp_reg; // update MCG_C1 
#else  
  temp_reg = MCG_C1;
  temp_reg &= ~MCG_C1_FRDIV_MASK; // clear FRDIV field
  temp_reg |= MCG_C1_FRDIV(3); // set FLL ref divider to 256
  MCG_C1 = temp_reg; // update MCG_C1
#endif
  
  MCG_C6 &= ~MCG_C6_PLLS_MASK; // clear PLLS to select the FLL
  MCG_C2 &= ~MCG_C2_LP_MASK; // clear LP bit
  while (MCG_S & MCG_S_PLLST_MASK){}; // Wait for PLL status bit to update
  
#if (K60_CLK)
  core_clk_khz = 50000;
#else
  core_clk_khz = 8000;
#endif
  
  periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
      uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  else
      uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
   
} // freq = REF_CLK

void fbe_fee(void)
{
  unsigned char temp_reg;
  
#if (K60_CLK)
  temp_reg = MCG_C1;
  temp_reg &= ~MCG_C1_FRDIV_MASK; // clear FRDIV field
  temp_reg &= ~MCG_C1_CLKS_MASK; // clear CLKS to select FLL output
  temp_reg |= MCG_C1_FRDIV(5); //set FLL ref divider to 1024
  MCG_C1 = temp_reg; // update MCG_C1 
#else  
  temp_reg = MCG_C4;
  temp_reg &= ~MCG_C4_DRST_DRS_MASK; // clear DRS to select low range
//  temp_reg |= MCG_C4_DMX32_MASK; // set DMX32 Alistair
  MCG_C4 = temp_reg; //update MCG_C4
  
  temp_reg = MCG_C1;
  temp_reg &= ~MCG_C1_FRDIV_MASK; // clear FRDIV field
  temp_reg &= ~MCG_C1_CLKS_MASK; // clear CLKS to select FLL output
  temp_reg |= MCG_C1_FRDIV(3); // set FLL ref divider to 256
  MCG_C1 = temp_reg; // update MCG_C1
#endif
  
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x0){}; // wait for clock to switch
#if (K60_CLK)
  core_clk_khz = 31250;
#else  
  core_clk_khz = 22875;
#endif
  
  periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
      uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  else
      uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
    
} //22.875MHz (may not be ok for all 1.0 devices)

void fee_fei(void)
{
  unsigned char temp_reg;
  int drs_val;
  
  drs_val = 0; // change this value to select different DRS range
               // 0 = 24MHz, 1 = 48MHz, 2 = 72MHz, 3 = 96MHz - assuming DMX32 is set
               // must trim slow IRC to 32768Hz if DMX32 is to be set and to keep UART working
  
  MCG_C1 |= MCG_C1_IREFS_MASK; // select internal reference
  temp_reg = MCG_C4;
  temp_reg &= ~MCG_C4_DRST_DRS_MASK; // clear the DRS field
  temp_reg |= MCG_C4_DRST_DRS(drs_val); // change this value to select different DRS range
  temp_reg |= MCG_C4_DMX32_MASK; // make sure DMX32 is set Alistair
  MCG_C4 = temp_reg;
  
  core_clk_khz = (24000 * (drs_val + 1));
  periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
      uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  else
      uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
    
} //24MHz assuming irc at 32.768kHz and DMX32 set and drs=0  

void fei_fbi(void)
{
  unsigned char temp_reg;
// the fast IRC is selected to keep the UART baud rate constant
// the fast IRC needs to be trimmed to 3600000Hz before calling this routine  
  MCG_C2 |= MCG_C2_IRCS_MASK; // select fast IRCS
  temp_reg = MCG_C1;
  temp_reg &= ~MCG_C1_CLKS_MASK; // clear CLKS to select FLL output
  temp_reg |= MCG_C1_CLKS(1); // update MCG_C1
  MCG_C1 = temp_reg;
  
  while (!(MCG_S & MCG_S_IREFST_MASK)){}; // wait for Reference Status bit to update
  
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x1){}; // Wait for clock status bits to update
  
  core_clk_khz = 1850;
  periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
      uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  else
      uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
    
} // ~1.85MHz

void fbi_blpi(void)
{
// assumes the fast IRC is selected and trimmed to 3600000Hz  
  MCG_C2 |= MCG_C2_LP_MASK; //set LP bit to disable the FLL
  
  core_clk_khz = 1850;
  periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
      uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  else
      uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
  time_delay_ms(100);  
} // ~1.85MHz

void blpi_fbi(void)
{
// assumes the fast IRC is selected and trimmed to 3600000Hz   
  MCG_C2 &= ~MCG_C2_LP_MASK; //clear LP bit 
  
  core_clk_khz = 1850;
  periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
      uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  else
      uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
    
} // ~1.85MHz

void fbi_fbe(void)
{
  unsigned char temp_reg;
// check if oscillator needs to be configured and enabled or if ext clock is supplied  
#if defined(K60_CLK) 
// K60 tower board uses 50MHz ext clock, do not enable the oscillator  
  MCG_C2 = 0;
#else
// Enable external oscillator, RANGE=1, HGO=1, EREFS=1, LP=0, IRCS=0
// K40 tower board uses 8MHz crystal    
  MCG_C2 = MCG_C2_RANGE(1) | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK;
#endif
  temp_reg = MCG_C1;
  temp_reg &= ~MCG_C1_FRDIV_MASK; // clear FRDIV field
  temp_reg &= ~MCG_C1_CLKS_MASK;  // clear CLKS to select FLL output
  temp_reg |= MCG_C1_CLKS(2);     // select ext ref clk
#if (K60_CLK)  
  temp_reg |= MCG_C1_FRDIV(5);    //set FLL ref divider to 1024
#else  
  temp_reg |= MCG_C1_FRDIV(3);    //set FLL ref divider to 256
#endif
  temp_reg &= ~MCG_C1_IREFS_MASK; // clear IREFS
  MCG_C1 = temp_reg;              // update MCG_C1 
  
  MCG_C4 &= ~MCG_C4_DMX32_MASK;   // clear DMX32 (not req'd but keeps it in consistant state 

/* if we aren't using an osc input we don't need to wait for the osc to init */
#if (!defined(K60_CLK))
  while (!(MCG_S & MCG_S_OSCINIT_MASK)){};  // wait for oscillator to initialize
#endif  
  while (MCG_S & MCG_S_IREFST_MASK){}; // wait for Reference Status bit to update
  
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}; // Wait for clock status bits to update  

#if (K60_CLK)
  core_clk_khz = 50000;
#else
  core_clk_khz = 8000;
#endif
  
  periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
      uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  else
      uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
}

void fbe_pbe(void)
{
  MCG_C6 |= MCG_C6_PLLS_MASK; // enable PLL, still clocked from ext ref clk
}

void pbe_pee(void)
{  
  MCG_C1 &= ~MCG_C1_CLKS_MASK; // select PLL as MCG_OUT
  // Wait for clock status bits to update 
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){}; 

  switch (CORE_CLK_MHZ) {
    case PLL50:
      core_clk_khz = 50000;
      break;
    case PLL100:
      core_clk_khz = 100000;
      break;
    case PLL96:
      core_clk_khz = 96000;
      break;  
    case PLL48:
      core_clk_khz = 48000;
      break;  
  }
      
  periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
      uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  else
      uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
  
} //96MHz

void cmd_cstm1(int argc, char **argv)
{ 
  printf("\n  Custom 1 Function\r\n\n");
}

void cmd_cstm2(int argc, char **argv)
{ 
  printf("\n  Custom 2 Function\r\n\n");
}


void cmd_rtc_as_refclk(int argc, char **argv)
{
  unsigned char temp_reg;
  
// Using the RTC OSC as Ref Clk
// enable RTC clock gating
  SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;  
// set RTC in default state using software reset
  RTC_CR |= RTC_CR_SWR_MASK; // set SWR
  RTC_CR &= ~RTC_CR_SWR_MASK; // clear SWR
// Configure and enable the RTC OSC
// select the load caps (application dependent) and the oscillator enable bit
// note that other bits in this register may need to be set depending on the intended use of the RTC
  
  RTC_CR |= RTC_CR_OSCE_MASK;

  time_delay_ms(1000); // wait for the RTC oscillator to intialize
// select the RTC oscillator as the MCG reference clock
  SIM_SOPT2 |= SIM_SOPT2_MCGCLKSEL_MASK;
  
// ensure MCG_C2 is in the reset state, key item is RANGE = 0 to select the correct FRDIV factor
  MCG_C2 = 0x0;
  
// Select the Reference Divider and clear IREFS to select the osc
// CLKS=0, select the FLL as the clock source for MCGOUTCLK
// FRDIV=0, set the FLL ref divider to divide by 1 
// IREFS=0, select the external clock 
// IRCLKEN=0, disable IRCLK (can enable if desired)
// IREFSTEN=0, disable IRC in stop mode (can keep it enabled in stop if desired)
  MCG_C1 = 0x0;

  while (MCG_S & MCG_S_IREFST_MASK){}; // wait for Reference clock to switch to external reference
    
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x0){}; // Wait for clock status bits to update
  
// Can select the FLL operating range/freq by means of the DRS and DMX32 bits
// Must first ensure the system clock dividers are set to keep the core and bus clocks
// within spec.
  SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(0) 
              | SIM_CLKDIV1_OUTDIV3(0) | SIM_CLKDIV1_OUTDIV4(1);
// In this case DMX32 is set and DRS is set to 0 = 24MHz from a 32.768 kHz crystal
  temp_reg = MCG_C4;
  temp_reg &= MCG_C4_DRST_DRS_MASK; // clear DRS value
  temp_reg |= MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0);  
  MCG_C4 = temp_reg;
  
// should enable clock monitor now that external reference is being used
// in this example it is not emabled as changing MCG clock modes may cause an
// unexpected reset if an internal clocking mode is selected with CME set  
//  MCG_C6 |= MCG_C6_CME_MASK;
  
  core_clk_khz = 24000; // FLL at 48MHz since DRS=01
  periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);
  if ((TERM_PORT == UART0_BASE_PTR) | (TERM_PORT == UART1_BASE_PTR))
      uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  else
      uart_init (TERM_PORT, periph_clk_khz, TERMINAL_BAUD);
  
  printf("\n  RTC clock is now the FLL reference clock.\r\n\n");
  
} //end cmd_rtc_as_refclk  


void ftm_enable(void)
{
      // Initialize FTM2 for output compare
    FTM2_MODE |= FTM_MODE_FTMEN_MASK;  // Enable FTM2, non-TPM-compatible registers.
    FTM2_CNT = 0x00000000;  // Clear counter.
    FTM2_MOD = FTM_MOD_MOD(0xFFFF);  // FTM0 Modulo set for counter overflow after 1 system clock cycles 
    FTM2_C0SC = FTM_CnSC_MSA_MASK | FTM_CnSC_ELSA_MASK; // FTM2_CH0 set for Output Compare mode, toggle output on match
    FTM2_C0V = FTM_CnV_VAL(0xFFFF);  // FTM2_CH0 output compare action on counter value of 0.
    FTM2_OUTINIT |= (FTM_OUTINIT_CH0OI_MASK);  // Static output level high for FTM2_CH0 when OC toggle not desired.
    FTM2_SC = FTM_SC_CLKS(1) | FTM_SC_PS(6);  // FTM0 in up-counter mode, TOF interrupt disabled, system clock source, div-by-128 prescaler.
#if (defined(TWR_K40X256))
    FTM2_SC |= FTM_SC_TOIE_MASK; // enable TOF interrupt if K40 tower board
#endif
}

void ftm2_isr(void)
{
  uint32 temp_reg;

  temp_reg = FTM2_SC;              // read FTM2_SC 
  temp_reg &= ~FTM_SC_TOF_MASK;   // Clear TOF
  FTM2_SC = temp_reg;
  GPIOC_PTOR = 0x00000080; // toggle port C 7      
  return;
}


//******************************************************************************

void
setshow_sysclk (int argc, char **argv)
{
    int result = 0;
    res_ptr = &result;
    printf("Placeholder for now...\n\n");
}
//******************************************************************************
