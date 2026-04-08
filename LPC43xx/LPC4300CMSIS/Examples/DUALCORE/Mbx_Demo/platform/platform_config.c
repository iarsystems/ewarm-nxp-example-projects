#include "LPC43xx.h"
#include "platform_config.h"

#include "lpc43xx_scu.h"
#include "lpc_types.h"
#include "lpc43xx_emc.h"
//#include "reloc_irq_table.h"


/**********************************************************************
 ** Function prototypes
 **********************************************************************/
void vIOInit(void);
void clockInit(void);
void fpuInit(void);



/* this function initializes the platform with system level settings */
void platformInit(void) {

	SystemInit();	
	
	/* checks for presence of an FPU unit */
	fpuInit();
  
	clockInit();
  
	vIOInit();

	#if (USE_EXT_STATIC_MEM == YES) || (USE_EXT_DYNAMIC_MEM == YES)
	 
	EMC_Init();
	
	#endif
	
	#if (USE_EXT_FLASH == YES)
	
	// relocate vector table to internal ram
	// updates also VTOR
  
	//relocIrqTable(); 
	
	#endif

}

/*----------------------------------------------------------------------------
  Initialize board specific IO
 *----------------------------------------------------------------------------*/
void vIOInit(void)
{	
	#if (PLATFORM == NXP_VALIDATION_BOARD)
		// P9.2 : GPIO4_14: LD11 (LED)
		scu_pinmux(0x9 ,2 , MD_PDN, FUNC0); 	
		LPC_GPIO_PORT->DIR[4] |= (1UL << 14);
	#endif
	#if (PLATFORM == HITEX_A2_BOARD)
		// P9.3 : GPIO4_15
		scu_pinmux(0x9 , 3 , MD_PDN, FUNC0); 	
		LPC_GPIO_PORT->DIR[4] |= (1UL << 15);
	#endif	
}
	
/*----------------------------------------------------------------------------
  Initialize clocks
 *----------------------------------------------------------------------------*/
extern void Hitex_CGU_Init(void);

void clockInit(void)
{
	Hitex_CGU_Init();
}


