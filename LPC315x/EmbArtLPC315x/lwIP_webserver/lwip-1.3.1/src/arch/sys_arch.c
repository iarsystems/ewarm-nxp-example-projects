/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2009
 *
 *    File name   : sys_arch.c
 *    Description :
 *
 *    History :
 *    1. Date        : 27.8.2009
 *       Author      : Stanimir Bonev
 *       Description : initial revision.
 *
 *    $Revision: 28 $
 **************************************************************************/
#include <intrinsics.h>
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "arch/sys_arch.h"


#if SYS_LIGHTWEIGHT_PROT
/*
 * This function is used to lock access to critical sections when lwipopt.h
 * defines SYS_LIGHTWEIGHT_PROT. It disables interrupts and returns a value
 * indicating the interrupt enable state when the function entered. This
 * value must be passed back on the matching call to sys_arch_unprotect().
 *
 * @return the interrupt level when the function was entered.
 *
 */
sys_prot_t
sys_arch_protect(void)
{
sys_prot_t Ret = __get_interrupt_state();
  __disable_interrupt();
  return(Ret);
}

/*
 * This function is used to unlock access to critical sections when lwipopt.h
 * defines SYS_LIGHTWEIGHT_PROT. It enables interrupts if the value of the lev
 * parameter indicates that they were enabled when the matching call to
 * sys_arch_protect() was made.
 *
 * @param lev is the interrupt level when the matching protect function was
 * called
 *
 */
void
sys_arch_unprotect(sys_prot_t lev)
{
  __set_interrupt_state(lev);
}
#endif /* SYS_LIGHTWEIGHT_PROT */


