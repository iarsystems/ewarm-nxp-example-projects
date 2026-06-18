;
;; Copyright (c) 2012 ARM Ltd. All rights reserved.
;;

;;  AREA vectors_section, CODE, READONLY

  PUBLIC write_vbar

  SECTION VBAR_SECTION : CODE
write_vbar:
  import __intvec_start
  ldr r1, =__intvec_start
  MCR p15, 0, R1, c12, c0, 0 ;      Program VBAR -> Vector Base Address Register
  bx lr
  END