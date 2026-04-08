#include "board.h"

extern void enable_SRAMX(bool enable);
extern void enable_SRAM0(bool enable);
extern void enable_SRAM1(bool enable);
extern void enable_SRAM2(bool enable);		

void deep_sleep_measurement(){
	
	enable_SRAMX(true);
	enable_SRAM0(true);
	enable_SRAM1(true);
	enable_SRAM2(true);
	
	LPC_SYSCON->PDRUNCFGSET[0] = SYSCON_PDRUNCFG_PD_BOD_INTR | SYSCON_PDRUNCFG_PD_BOD_RST;
	
	__disable_irq();
	
	Chip_POWER_EnterPowerMode(POWER_DEEP_SLEEP,SYSCON_PDRUNCFG_PD_SRAM0 | SYSCON_PDRUNCFG_PD_SRAM1 | 
									SYSCON_PDRUNCFG_PD_SRAM2 | SYSCON_PDRUNCFG_PD_SRAMX);
	__enable_irq();
}
