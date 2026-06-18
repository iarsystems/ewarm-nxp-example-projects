
#ifndef EA3250_SPINOR_H
#define EA3250_SPINOR_H

#include "lpc_types.h"

#ifdef __cplusplus
extern "C"
{
#endif




/***********************************************************************
 * NAND support functions
 **********************************************************************/

STATUS spinor_init(void);
void spinor_setTo512Mode(void);
STATUS spinor_write_page(UNS_8 *src, UNS_32 offset);
STATUS spinor_read(UNS_8 *dst, UNS_32 offset, UNS_32 bytecount);
void spinor_close(void);
STATUS spinor_block_erase(UNS_32 offset);
STATUS spinor_sector_erase(UNS_32 offset);


#ifdef __cplusplus
}
#endif

#endif /* EA3250_SPINOR_H */
