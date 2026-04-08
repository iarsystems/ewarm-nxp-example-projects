#include "rom_drivers.h"

#define  ROMD_MISSING 0xFFFFFFFF 

#ifdef USBROMD_PRESENT
    #include "usbhw.h"

    extern void init_clk_usb_pins(void);

    const USB usb = {init_clk_usb_pins,USB_IRQHandler,USB_Init,USB_Connect};
    #define USBROMD &usb
#else
    #define USBROMD ROMD_MISSING
#endif /* USB_ROMD_PRESENT */

#ifdef CLIBROMD_PRESENT
    #define  CLIBROMD 0x1ffc0000
#else 
    #define  CLIBROMD ROMD_MISSING
#endif /* CLIB_ROMD_PRESENT */

#ifdef CANROMD_PRESENT
    #define  CANROMD 0xffffffff /* Add CAN driver table here */
#else 
    #define  CANROMD ROMD_MISSING
#endif /* CANROMD_PRESENT */

#ifdef PWRROMD_PRESENT
#include "power_control.h"
  const PWRD pwrd = {
    &set_pll_routine,
    &set_power_routine,
  };
    #define  PWRROMD &pwrd /* Add PWR driver table here */
#else 
    #define  PWRROMD ROMD_MISSING
#endif /* PWRROMD_PRESENT */

#ifdef DIVROMD_PRESENT

  const DIVD divd = {
    &sdiv,
    &udiv,
    &sdivmod,
    &udivmod,
  };

    #define  DIVROMD &divd /* Add DEV1 driver table here */
#else 
    #define  DIVROMD ROMD_MISSING
#endif /* DIVROMD_PRESENT */

#ifdef I2CROMD_PRESENT
    #define  I2CROMD &i2c_api /* Add DEV2 driver table here */
#else 
    #define  I2CROMD ROMD_MISSING
#endif /* DEV2ROMD_PRESENT */

#ifdef DEV3ROMD_PRESENT
    #define  DEV3ROMD 0xffffffff /* Add DEV3 driver table here */
#else 
    #define  DEV3ROMD ROMD_MISSING
#endif /* DEV3ROMD_PRESENT */

#ifdef DEV4ROMD_PRESENT
    #define  DEV4ROMD 0xffffffff /* Add DEV4 driver table here */
#else 
    #define  DEV4ROMD ROMD_MISSING
#endif /* DEV4ROMD_PRESENT */

const ROM rom_drivers = {USBROMD,CLIBROMD,CANROMD,PWRROMD,
                         DIVROMD,I2CROMD,DEV3ROMD,DEV4ROMD};

#pragma arm section rodata = "rom_drivers_ptr"
unsigned* rom_drivers_ptr = &rom_drivers;
/*changed from this line to the above to overcome illegal pointer cast. Compile command line uses --loose_implicit_cast to overcome this issue*/
/*const unsigned rom_drivers_ptr = (unsigned)&rom_drivers;*/
