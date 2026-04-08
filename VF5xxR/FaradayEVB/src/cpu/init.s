;;
;; Copyright (c) 2009 ARM Ltd. All rights reserved.
;;

;;  AREA init_section, CODE, READONLY
  SECTION VECTORS_SECTION : CODE (2)

  EXPORT init_start

  IMPORT  main

init_start

  ; Disable MMU
  MRC   p15, 0, r0, c1, c0, 0 ; Read Control Register configuration data
  BIC   r0, r0, #0x1
  MCR   p15, 0, r0, c1, c0, 0 ; Write Control Register configuration data

 
  LDR   r12, =main

  ; Branch to C
  BX    r12

  END
