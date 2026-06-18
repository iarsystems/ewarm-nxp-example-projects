/******************************************************************************
 * (c) Copyright 2016, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ******************************************************************************
 * svc_test.c
 ******************************************************************************/
#include "drivers.h"

/* LEDs definition                                                            */
#define LED1    PIN3        /* PTJ3 */
#define LED2    PIN4        /* PTJ4 */

/* define supervisor service commands                                         */
#define SVC_TGL_LED1()  SVC_Call(0x01)
#define SVC_TGL_LED2()  SVC_Call(0x02)

/* supervisor service callback function definition; this function executes in */
/* handler (privileged) mode and it uses main stack. Note that modification   */
/* of any special registers within this mode is allowed                       */
static void svc_callback (uint8 imm)
{
  if (imm == 0x01) { GPIO_Tgl (FGPIOJ, LED1); }
  if (imm == 0x02) { GPIO_Tgl (FGPIOJ, LED2); }
}

/* user function definition; this function executes in the thread/user mode   */
/* where all attempts to modify any special register are ignored              */
static void user_function (void)
{
  arch_delay(100000);
  SVC_TGL_LED1();         /* LED1 toggle                                      */
  arch_delay(100000);
  SVC_TGL_LED2();         /* LED2 toggle                                      */
}

/* device enters thread/privileged execution mode after POR; main stack is    */
/* selected                                                                   */
void main (void)
{
  /* enable clocks to all on chip peripherals                                 */
  SIM_Init (SIM_MODULE_ALL_PERIPH_ON_CONFIG);

  /* initialize LEDs                                                          */
  PORT_Init (PORTJ , PORT_MODULE_LED_MODE, LED1|LED2);
  GPIO_Init (FGPIOJ, GPIO_OUT_LOGIC1_MODE, LED1|LED2);

  /* initialize supervisor callback function                                  */
  SVC_InstallCallback (PRI_LVL1, svc_callback);

  /* switch to process stack and enter thread/user execution mode; note that  */
  /* process stack as well as other memory sections shall be defined in linker*/
  /* file and protected by MPU on the contrary to this simple implementation  */
  SetPSP  (0x20000000);   /* setup process stack pointer                      */
  SelPSP  ();             /* select process stack                             */
  EnableInterrupts();     /* enable interrupts                                */
  UserMode();             /* switch into thread/user mode                     */

  /* main software loop executes in thread user mode with using process stack */
  while (1) { user_function(); }
}
