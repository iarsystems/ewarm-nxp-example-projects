/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */

/*
 * Include the platform specific header file
 */
#if (defined(TWR_K40X256))
  #include <MK40N512VMD100.h>
#elif (defined(TWR_K53N512))
  #include <MK53N512CMD100.h>
#else
  #error "No valid platform defined"
#endif

#define _SERIAL_BRIDGE_

#define __MK_xxx_H__
#define USED_PIT0
