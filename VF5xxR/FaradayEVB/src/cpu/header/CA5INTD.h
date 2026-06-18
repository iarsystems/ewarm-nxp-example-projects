/*****************************************************************  
 * FILE        : CA5INTD.h 
 *  
 * DESCRIPTION : This is the header file describing the register 
 *               set for Vybrid
 * 
 ******************************************************************/ 
/**************************************************************************/ 
/*                           Module: CA5INTD                            */ 
/**************************************************************************/ 
/*        Header translated using script version 0.11 (13 Mar 2012)       */ 
/*                                                                        */ 
/*                                                                        */
  
#include "typedefs.h"
  
struct CA5INTD_tag { 
    union { 
        vuint32_t R; 
        struct { 
           vuint32_t ENABLE:1; 
           vuint32_t:31; 
        } B; 
    } ICDDCR; 
 
    union { 
        vuint32_t R; 
        struct { 
           vuint32_t ITLINESNUMBER:5; 
           vuint32_t CPUNUMBER:3; 
           vuint32_t:2; 
           vuint32_t SECURITYEXTN:1; 
           vuint32_t LSPI:5; 
           vuint32_t:16; 
        } B; 
    } ICDICTR; 
 
    union { 
        vuint32_t R; 
        struct { 
           vuint32_t IMPLEMENTER:12; 
           vuint32_t REVISION:4; 
           vuint32_t VARIANT:4; 
           vuint32_t:4; 
           vuint32_t PRODUCTID:8; 
        } B; 
    } ICDIIDR; 
 
    uint8_t CA5INTD_reserved0[116]; 
 
    union { 
        vuint32_t R; 
        struct { 
           vuint32_t STATUS:32; 
        } B; 
    } ICDISR[5]; 
 
    uint8_t CA5INTD_reserved1[108]; 
 
    union { 
        vuint32_t R; 
        struct { 
           vuint32_t SET:32; 
        } B; 
    } ICDISER[32]; 
 
       union { 
        vuint32_t R; 
        struct { 
           vuint32_t CLEAR:32; 
        } B; 
    } ICDICER[32]; 
 
       union { 
        vuint32_t R; 
        struct { 
           vuint32_t SET:32; 
        } B; 
    } ICDISPR[32]; 
 
       union { 
        vuint32_t R; 
        struct { 
           vuint32_t CLEAR:32; 
        } B; 
    } ICDICPR[32]; 

       union { 
        vuint32_t R; 
        struct { 
           vuint32_t ACTIVE:32; 
        } B; 
    } ICDABR[32]; 
 
    
    uint8_t CA5INTD_reserved2[128]; 
 
    union { 
        vuint32_t R; 
        struct { 
           vuint32_t PRIORITY0:8; 
           vuint32_t PRIORITY1:8; 
           vuint32_t PRIORITY2:8; 
           vuint32_t PRIORITY3:8; 
        } B; 
    } ICDIPR[255]; 
 
    uint8_t CA5INTD_reserved3[4]; 
 
    union { 
        vuint32_t R; 
        struct { 
           vuint32_t CPU0:8; 
           vuint32_t CPU1:8; 
           vuint32_t CPU2:8; 
           vuint32_t CPU3:8; 
        } B; 
    } ICDIPTR[8]; 
 
    uint8_t CA5INTD_reserved4[992]; 
 
    union { 
        vuint32_t R; 
        struct { 
           vuint32_t FIELD0:2; 
           vuint32_t FIELD1:2; 
           vuint32_t FIELD2:2; 
           vuint32_t FIELD3:2; 
           vuint32_t FIELD4:2; 
           vuint32_t FIELD5:2; 
           vuint32_t FIELD6:2; 
           vuint32_t FIELD7:2; 
           vuint32_t FIELD8:2; 
           vuint32_t FIELD9:2; 
           vuint32_t FIELD10:2; 
           vuint32_t FIELD11:2; 
           vuint32_t FIELD12:2; 
           vuint32_t FIELD13:2; 
           vuint32_t FIELD14:2; 
           vuint32_t FIELD15:2; 
        } B; 
    } ICDICFR[64]; 
  
    uint8_t CA5INTD_reserved5[512]; 
 
    union { 
        vuint32_t R; 
        struct { 
           vuint32_t SGIINTID:4; 
           vuint32_t:11; 
           vuint32_t SATT:1; 
           vuint32_t CPUTARGETLIST:8; 
           vuint32_t TLF:8; 
        } B; 
    } ICDSGIR; 
 
}; 
 
#define CA5INTD    (*(volatile struct CA5INTD_tag *)        0x40003000) 
