/******************************************************************************
 * (c) Copyright 2010-2016, Freescale Semiconductor Inc.
 * ALL RIGHTS RESERVED.
 ***************************************************************************//*!
 * @file      svcall_isr.s
 * @version   1.0.0.0
 * @date      Mar-31-2016
 * @brief     Module with interrupt handler for calling a Supervisor Call (SVC)
 *            callback function.
 ******************************************************************************/
      PUBWEAK svcall_isr
      EXTERN  pCallbackSVC
      SECTION .text:CODE:NOROOT
      THUMB
svcall_isr:
      movs  r0,#4
      mov   r1,lr
      tst   r0,r1
      beq   _msp
      mrs   r0,psp
      b     _end
_msp: mrs   r0,msp
_end: ldr   r0,[r0,#24]
      subs  r0,r0,#2
      ldrb  r0,[r0]
      ldr   r1,=pCallbackSVC
      ldr   r1,[r1]
      bx    r1
      END
/******************************************************************************
 * End of module                                                              *
 ******************************************************************************/