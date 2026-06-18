/******************************************************************************
 *   irq.h:  Interrupt related Header file for NXP LPC29xx Family 
 *   Microprocessors
 *
 *   Copyright(C) 2007, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __IRQ_H 
#define __IRQ_H

typedef  void (*HandlerAddr)(void);

#define I_Bit			0x80
#define F_Bit			0x40

#define SYS32Mode		0x1F
#define IRQ32Mode		0x12
#define FIQ32Mode		0x11

#define HIGHEST_PRIORITY	0x0F
#define LOWEST_PRIORITY		0x01

#define ACTIVE_HIGH		0x00
#define ACTIVE_LOW		0x01

#define	WDT_INT			1
#define TIMER0_INT		2
#define TIMER1_INT		3
#define	TIMER2_INT		4
#define TIMER3_INT		5
#define	UART0_INT		6
#define UART1_INT		7
#define SPI0_INT		8
#define	SPI1_INT		9
#define	SPI2_INT		10
#define FLASH_INT		11		
#define ICE_RX_INT		12
#define ICE_TX_INT		13
#define	MSCSS_TMR0_INT	14
#define	MSCSS_TMR1_INT	15
#define ADC0_INT		16
#define ADC1_INT		17
#define ADC2_INT		18
#define PWM0_INT		19
#define PWM0_CAP_INT	20
#define	PWM1_INT		21
#define PWM1_CAP_INT	22
#define PWM2_INT		23
#define PWM2_CAP_INT	24
#define PWM3_INT		25
#define PWM3_CAP_INT	26
#define EVT_INT			27

#define CAN_ALL_INT		36
#define CAN0_RX_INT		37
#define CAN1_RX_INT		38
#define CAN2_RX_INT		39
#define CAN3_RX_INT		40
#define CAN4_RX_INT		41
#define CAN5_RX_INT		42
#define CAN0_TX_INT		43
#define CAN1_TX_INT		44
#define CAN2_TX_INT		45
#define CAN3_TX_INT		46
#define CAN4_TX_INT		47
#define CAN5_TX_INT		48

#define VIC_SIZE		64

#define VECT_INT_REQ_INDEX	0x400

/* Be aware that, from compiler to compiler, nested interrupt will have to
be handled differently. More details can be found in Philips LPC2000
family app-note AN10381 */
 
/* unlike Keil CARM Compiler, in ARM's RealView compiler, don't save and 
restore registers into the stack in RVD as the compiler does that for you. 
See RVD ARM compiler Inline and embedded assemblers, "Rules for 
using __asm and asm keywords. */
static DWORD sysreg;		/* used as LR register */
#define IENABLE __asm { MRS sysreg, SPSR; MSR CPSR_c, #SYS32Mode }
#define IDISABLE __asm { MSR CPSR_c, #(IRQ32Mode|I_Bit); MSR SPSR_cxsf, sysreg }

extern void init_VIC( void );
extern void EnableIntReq_IRQ( DWORD IntNumber, DWORD Polarity, DWORD Priority );
extern void EnableIntReq_FIQ( DWORD IntNumber, DWORD Polarity, DWORD Priority );
extern void DisableIntReq_IRQ( DWORD IntNumber );
extern void DisableIntReq_FIQ( DWORD IntNumber );
extern DWORD install_irq( DWORD IntNumber, void *HandlerAddr, DWORD Priority );
extern DWORD install_fiq( DWORD IntNumber, void *HandlerAddr, DWORD Priority );
#endif /* end __IRQ_H */

/******************************************************************************
**                            End Of File
******************************************************************************/
