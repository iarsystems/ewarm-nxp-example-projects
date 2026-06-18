#ifndef _LEDS_H
#define _LEDS_H

/* Define LED mapping based on board */
#include "common.h"

/*************************/
/* TWR-VF65GS10 Specific */
/*************************/

#define LED0_GPIO 22
#define LED1_GPIO 23
#define LED2_GPIO 24
#define LED3_GPIO 25

#define ENABLE_LED0  	IOMUXC->RGPIO[LED0_GPIO] = 0x000011A2; //enable GPIO output
#define ENABLE_LED1  	IOMUXC->RGPIO[LED1_GPIO] = 0x000011A2; //enable GPIO output
#define ENABLE_LED2  	IOMUXC->RGPIO[LED2_GPIO] = 0x000011A2; //enable GPIO output
#define ENABLE_LED3  	IOMUXC->RGPIO[LED3_GPIO] = 0x000011A2; //enable GPIO output
#define ENABLE_LEDS 	{ ENABLE_LED0; ENABLE_LED1; ENABLE_LED2; ENABLE_LED3; }
/*
  IOMUXC->SINGLE.PTB0 = 0x000011A2; //enable GPIO output for PTB0
  IOMUXC->SINGLE.PTB1 = 0x000011A2; //enable GPIO output for PTB1
  IOMUXC->SINGLE.PTB2 = 0x000011A2; //enable GPIO output for PTB2
  IOMUXC->SINGLE.PTB3 = 0x000011A2; //enable GPIO output for PTB3
*/

#define LED0_ON   { GPIO0->PDOR &= (~PIN(22)); }            /* PTB0 */
#define LED1_ON   { GPIO0->PDOR &= (~PIN(23)); }            /* PTB1 */
#define LED2_ON   { GPIO0->PDOR &= (~PIN(24)); }            /* PTB2 */
#define LED3_ON   { GPIO0->PDOR &= (~PIN(25)); }            /* PTB3 */
#define LED0_OFF  { GPIO0->PDOR |= PIN(22); }               /* PTB0 */
#define LED1_OFF  { GPIO0->PDOR |= PIN(23); }               /* PTB1 */
#define LED2_OFF  { GPIO0->PDOR |= PIN(24); }               /* PTB2 */
#define LED3_OFF  { GPIO0->PDOR |= PIN(25); }               /* PTB3 */
#define LEDS_ON   { LED0_ON; LED1_ON; LED2_ON; LED3_ON; }
#define LEDS_OFF  { LED0_OFF; LED1_OFF; LED2_OFF; LED3_OFF; }
#define LEDS_TOGGLE	{ GPIO0->PTOR |= (PIN(22) | PIN(23) | PIN(24) | PIN(25)); }
    
/* Define push-buttons on Tower board */
#define PUSH_BUTTON0_GPIO	38
#define PUSH_BUTTON1_GPIO	39
/* enable GPIO input for PTB16 (SW1), set IBE, enable pullup */
#define PUSH_BUTTON0_INIT 	{ IOMUXC->RGPIO[PUSH_BUTTON0_GPIO] = 0x000011AD;  }
/* enable GPIO input for PTB17 (SW1), set IBE, enable pullup */
#define PUSH_BUTTON1_INIT 	{ IOMUXC->RGPIO[PUSH_BUTTON1_GPIO] = 0x000011AD; }
//GPIO1->PDOR = 0x000000C0;
#define PUSH_BUTTONS_INIT	{ PUSH_BUTTON0_INIT; PUSH_BUTTON1_INIT; GPIO1->PDOR = 0; }

#endif //#ifndef _LEDS_H
