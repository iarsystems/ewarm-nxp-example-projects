/**
 * @file	: SystemTick.c
 * @purpose	: A simple systemtick driver example
 * @version	: 1.0
 * @date	: 30. Nov. 2009
 * @author	: ThieuTrinh
 *----------------------------------------------------------------------------
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/

#include "lpc13xx_gpio.h"
#include "lpc13xx_libcfg.h"
#include "lpc13xx_nvic.h"
#include "lpc13xx.h"
#include "lpc13xx_pinsel.h"
volatile unsigned long SysTickCnt;      /* SysTick Counter                    */

void SysTick_Handler (void) {           /* SysTick Interrupt Handler (1ms)    */
  SysTickCnt++;
}

void Delay (unsigned long tick) {       /* Delay Function                     */
  unsigned long systickcnt;

  systickcnt = SysTickCnt;
  while ((SysTickCnt - systickcnt) < tick);
}

int main (void) {                       /* Main Program                       */

  SystemInit();

	//  Set Vector table offset value
#if (__RAM_MODE__==1)
	NVIC_SetVTOR(0x10000000);
#else
	NVIC_SetVTOR(0x00000000);
#endif


  SysTick_Config(SystemCoreClock/1000 - 1); /* Generate interrupt each 1 ms   */

  PINSEL_SetPinFunc(1,9,0);
  PINSEL_SetPinMode(1,9,2);
  GPIO_SetDir(3, 0, 1); //Green LED
  GPIO_SetDir(1, 9, 1); //Red LED

  GPIO_ClearValue(3, 0);
  GPIO_ClearValue(1, 9);

  for (;;) {                            /* Loop forever                       */
        GPIO_SetValue(3, 0);
        GPIO_SetValue(1, 9);
        Delay(100);                         /* Delay 500ms                        */
        GPIO_ClearValue(3, 0);
        GPIO_ClearValue(1, 9);
        Delay(100);                         /* Delay 500ms                        */
  }
}


/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int c_entry(void)
{
    return main();
}

#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif
