// -------------------------------------------------------------------
// -----------------------LPC2106 main routines-----------------------
// -------------------------------------------------------------------
// Written for TinyARM MB40 development board
// -------------------------------------------------------------------
// THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// -------------------------------------------------------------------

#include "usercode.h"   /* Usercode macros (see <template.h>) */
#include "ma_tgt.h"     /* Target specific header file */
#include "ma_sfr.h"     /* Special function register bitfield macros */
#include "NXP/iolpc210x.h"  /* Defines Special function registers */
#include <intrinsics.h>  

#include "ma_scb.h"     /* SCB Module driver header file */
#include "ma_gpio.h"    /* GPIO Module - I/O port */
#include "ma_pcb.h"     /* PCB  Module - Port Control Block */
#include "S1_Led.h"

int main(void)
{
	MA_Reset_SCB();
	MA_Reset_PCB();
	MA_Init_GPIO();
        Exec_BlinkLed();
}


