
#include "common.h"
#include "Settings.h"

enum usb_clock
{
  MCGPLL0,
  MCGFLL,
  CLKIN
};


/* Select Clock source */
#define USB_CLOCK   MCGPLL0
//#define USB_CLOCK   MCGFLL
//#define USB_CLOCK   CLKIN

/* USB Fractional Divider value for 96MHz input */
/** USB Clock = PLL x (FRAC +1) / (DIV+1)       */
/** USB Clock = 120MHz x (1+1) / (4+1) = 48 MHz  */

  #define USB_FRAC    1
  #define USB_DIV     SIM_CLKDIV2_USBDIV(4)
  
#define FLAG_SET(BitNumber, Register)        (Register |=(1<<BitNumber))
#define FLAG_CLR(BitNumber, Register)        (Register &=~(1<<BitNumber))
#define FLAG_CHK(BitNumber, Register)        (Register & (1<<BitNumber))

#define ENABLE_USB_5V   GPIOC_PSOR|=(1<<9);
#define DISABLE_USB_5V  GPIOC_PCOR|=(1<<9);

void vfnInitUSBClock (uint8 u8ClkOption);

