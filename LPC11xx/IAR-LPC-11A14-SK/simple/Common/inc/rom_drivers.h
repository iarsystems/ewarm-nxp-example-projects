#ifndef ROM_DRIVERS_H_
#define ROM_DRIVERS_H_

#define ROM_DRIVERS_PTR ((ROM *)(*((unsigned int *)0x1FFF1FF8)))

#ifdef DIVROMD_PRESENT
#include "divfuncs.h"

typedef struct _DIVD {
    int32_t (*sidiv) (int32_t, int32_t);
    uint32_t (*uidiv) (int32_t, int32_t);
    sidiv_t (*sidivmod) (int32_t, int32_t);
    uidiv_t (*uidivmod) (uint32_t, uint32_t);
}  DIVD;
#endif /* DIVROMD_PRESENT */

#ifdef PWRROMD_PRESENT
#include "power_api.h"
#endif /* PWRROMD_PRESENT */

#ifdef USBROMD_PRESENT
#include "mw_usbd_rom_api.h"
#endif

#ifdef I2CROMD_PRESENT
#include "hw_i2cd_rom_api.h"  
#endif

typedef struct _ROM {
#ifdef USBROMD_PRESENT
   const USBD_API_T * pUSBD;
#else
   const unsigned p_usbd;
#endif /* USBROMD_PRESENT */

   const unsigned p_clib;
   const unsigned p_cand;

#ifdef PWRROMD_PRESENT
   const PWRD * pPWRD;
#else
   const unsigned p_pwrd;
#endif /* PWRROMD_PRESENT */

#ifdef DIVROMD_PRESENT
   const DIVD * pDIVD;
#else
   const unsigned p_promd;
#endif /* DIVROMD_PRESENT */

#ifdef I2CROMD_PRESENT
   const I2CD_API_T * pI2CD;
#else
   const unsigned p_i2cd;
#endif /* I2CROMD_PRESENT */

   const unsigned p_dev3;
   const unsigned p_dev4; 
}  ROM;

#endif /*ROM_DRIVERS_H_*/
