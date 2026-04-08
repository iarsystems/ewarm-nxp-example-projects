/*
 * File:        sysinit.h
 * Purpose:     Vybrid Configuration
 *              Initializes processor to a default state
 *
 * Notes:
 *
 */
/********************************************************************/
// function prototypes
void sysinit (void);
void clocks_init (void);
void interrupts_init(void);
void enable_interrupt(int vector_number, void (*f)());
void clk_out_setup(void);
/********************************************************************/
// Global Variables
typedef void (*int_hdlr_t)(void);
/********************************************************************/

// Definitions for various modules used at start-up 

/********General********/
#define ENABLED     1
#define DISABLED    0

/********IOMUX Controller********/
#define ALT0_GPIO_MODE  0x0 
#define DRV_STR_50_OHM  0x3

// Pad SPEED parameter options
#define LOW_50_MHZ      0x0
#define HIGH_200_MHZ    0x3

// Pad SRE (Slew Rate) parameter options
#define SLOW_SLEW_RATE  0x0
#define FAST_SLEW_RATE  0x1

// IOMUXC_PAD_32 and IOMUXC_PAD_33 CKO Alt6 mux definition
#define ALT6_CKO        0x6 

/********Clock Controller Module (CCM)******/

// CCM Clock Switcher Register (CCSR) bit definitions
#define SYS_CLK_SEL_FAST_CLK_SEL            0x00000000

// SYS_CLK_SEL
#define FAST_CLK_SELECT       0x0
#define SLOW_CLK_SELECT       0x1
#define PLL2_PFD_CLK_OUT      0x2
#define PLL2_MAIN_CLK         0x3
#define PLL1_PFD_CLK_OUT      0x4
#define PLL3_MAIN_CLK         0x5

// CA5_CLK_SEL
#define SYS_DIV_OUT_CLK       0x0
#define PLL1_PFD_OUT_CLK      0x1

// DDRC_CLK_SEL
#define DDRC_SYS_DIV_OUT_CLK  0x1

// FAST_CLK_SEL
#define IRC_CLK_24MHZ         0x0
#define FOSC_CLK_24MHZ        0x1

// PLL2 PFD CLK SEL
#define PLL2_PFD_OUT_MAIN_CLK         0x0
#define PLL2_PFD1_OUT_CLK     0x1
#define PLL2_PFD2_OUT_CLK     0x2 
#define PLL2_PFD3_OUT_CLK     0x3
#define PLL2_PFD4_OUT_CLK     0x4


// CCM ARM Clock Root Register (CACRR) bit definitions
#define ARM_CLK_DIV_VAL_1                   0x00000000
#define BUS_CLK_DIV_VAL_2                   0x00000008
#define DIVIDE_BY_1           0x0
#define DIVIDE_BY_2           0x1
#define DIVIDE_BY_3           0x2
#define DIVIDE_BY_4           0x3
#define DIVIDE_BY_5           0x4
#define DIVIDE_BY_6           0x5
#define DIVIDE_BY_7           0x6
#define DIVIDE_BY_8           0x7

// CCM Clock Gating Register (CCGRn) bit definitions
#define CG75_CCM_UNGATE_ALL_MODE_BUT_STOP   0x00C00000
#define CG78_SRC_UNGATE_ALL_MODE_BUT_STOP   0x30000000

#define CLK_OFF_ALL_MODES               0x0
#define CLK_ON_RUN_OFF_WAIT_STOP_MODES  0x1
#define CLK_ON_ALL_MODES                0x2
#define CLK_ON_ALL_MODES_EXCEPT_STOP    0x3

// CCM Clock Output Source Register (CCOSR) bit definitions
#define CKO1_PLL2_MAIN_CLK              0x12
#define CKO1_PLL1_PFD1_CLK              0x26
#define CKO1_FOSC_CLK                   0x28
#define CKO1_FIRC_CLK                   0x29
#define CKO1_CA5_ROOT_CLK               0x3D   
#define CKO1_BUS_ROOT_CLK               0x3E   
#define CKO1_CM4_ROOT_CLK               0x37    
#define CKO1_IPG_ROOT_CLK               0x1B  

#define CKO2_PLL2_PFD3_CLK              0x23
#define CKO2_PLL1_MAIN_CLK              0x2B
