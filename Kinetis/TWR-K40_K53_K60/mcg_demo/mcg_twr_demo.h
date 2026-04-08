/*
 * File:    mcg_twr_demo.h
 * Purpose: MCG demo header
 *
 * Notes:
 */

#ifndef _PMC_BASIC_H_
#define _PMC_BASIC_H_

/********************************************************************/
/*
 * Function Prototypes 
 */
void mainloop (void);
void cmd_rd (int, char **);
void cmd_wrt (int, char **);
void setshow_sysclk (int, char **);
void cmd_clr (int, char **);
void cmd_atc (int argc, char **argv);
void cmd_atc_loop (int argc, char **argv);
void cmd_adc(int argc, char **argv);
void cmd_blpi(int argc, char **argv);
void cmd_pee(int argc, char **argv);
void cmd_fei(int argc, char **argv);
void cmd_fei_pee(int argc, char **argv);
void cmd_cstm1(int argc, char **argv);
void cmd_cstm2(int argc, char **argv);
void cmd_blpi_pee(int argc, char **argv);
void cmd_fei_pee_demo(int argc, char **argv);
void cmd_rtc_as_refclk(int argc, char **argv);


extern void default_isr(void);
void ftm_enable(void);

void pee_pbe(void);
void pbe_blpe(void);
void blpe_fbe(void);
void fbe_fee(void);
void fee_fei(void);
void fei_fbi(void);
void fbi_blpi(void);
void blpi_fbi(void);
void fbi_fbe(void);
void fbe_pbe(void);
void pbe_pee(void);

extern void timer_setup(void);
extern void time_delay_ms(int);
void adc_test (void);
extern void Port_Init(void);


/********************************************************************/

#endif /* _PMC_BASIC_H_ */
