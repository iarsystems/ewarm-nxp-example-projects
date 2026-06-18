;
;; Copyright (c) 2012 ARM Ltd. All rights reserved.
;;

  AREA WRITE_VBAR, CODE, READONLY

  EXPORT write_vbar

  ;SECTION .noinit : CODE
write_vbar
  ;import __intvec_start
  ;ldr r1, =__intvec_start
  import vector_table
  ldr r1, =vector_table
  MCR p15, 0, R1, c12, c0, 0 ;      Program VBAR -> Vector Base Address Register
  bx lr
  END
  
  END
