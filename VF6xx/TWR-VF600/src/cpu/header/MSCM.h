/*****************************************************************
 * FILE        : MSCM.h
 *
 * DESCRIPTION : This is the header file describing the register
 *               set for Vybrid
 *
 ******************************************************************/
/**************************************************************************/
/*                           Module: MSCM                            */
/**************************************************************************/
/*        Header translated using script version 0.14 (14 Mar 2012)       */
/*                                                                        */

struct MSCM_tag {
    union {
        vuint32_t R;
        struct {
           vuint32_t rYpZ:8;
           vuint32_t PERSONALITY:24;
        } B;
    } CPxTYPE;				/*00000000*/

    union {
        vuint32_t R;
        struct {
           vuint32_t CPN:1;
           vuint32_t:31;
        } B;
    } CPxNUM;				/*00000004*/

    union {
        vuint32_t R;
        struct {
           vuint32_t PPN:5;
           vuint32_t:27;
        } B;
    } CPxMASTER;				/*00000008*/

    union {
        vuint32_t R;
        struct {
           vuint32_t PCNT:1;
           vuint32_t:31;
        } B;
    } CPxCOUNT;				/*0000000C*/

    union {
        vuint32_t R;
        struct {
           vuint32_t DCWY:8;
           vuint32_t DCSZ:8;
           vuint32_t ICWY:8;
           vuint32_t ICSZ:8;
        } B;
    } CPxCFG0;				/*00000010*/

    union {
        vuint32_t R;
        struct {
           vuint32_t:16;
           vuint32_t L2WY:8;
           vuint32_t L2SZ:8;
        } B;
    } CPxCFG1;				/*00000014*/

    union {
        vuint32_t R;
        struct {
           vuint32_t:8;
           vuint32_t TMUSZ:8;
           vuint32_t:8;
           vuint32_t TMLSZ:8;
        } B;
    } CPxCFG2;				/*00000018*/

    union {
        vuint32_t R;
        struct {
           vuint32_t FPU:1;
           vuint32_t SIMD:1;
           vuint32_t JAZ:1;
           vuint32_t MMU:1;
           vuint32_t TZ:1;
           vuint32_t CMP:1;
           vuint32_t BB:1;
           vuint32_t:1;
           vuint32_t SBP:2;
           vuint32_t:22;
        } B;
    } CPxCFG3;				/*0000001C*/

    union {
        vuint32_t R;
        struct {
/*HEADER_ERROR: No bits identified for register CP0TYPE. Please make manual fixes to the generated header file.*/
           vuint32_t DUMMY_NOBIT:32;
        } B;
    } CP0TYPE;				/*00000020*/

    union {
        vuint32_t R;
        struct {
/*HEADER_ERROR: No bits identified for register CP0NUM. Please make manual fixes to the generated header file.*/
           vuint32_t DUMMY_NOBIT:32;
        } B;
    } CP0NUM;				/*00000024*/

    union {
        vuint32_t R;
        struct {
/*HEADER_ERROR: No bits identified for register CP0MASTER. Please make manual fixes to the generated header file.*/
           vuint32_t DUMMY_NOBIT:32;
        } B;
    } CP0MASTER;				/*00000028*/

    union {
        vuint32_t R;
        struct {
/*HEADER_ERROR: No bits identified for register CP0COUNT. Please make manual fixes to the generated header file.*/
           vuint32_t DUMMY_NOBIT:32;
        } B;
    } CP0COUNT;				/*0000002C*/

    union {
        vuint32_t R;
        struct {
/*HEADER_ERROR: No bits identified for register CP0CFG0. Please make manual fixes to the generated header file.*/
           vuint32_t DUMMY_NOBIT:32;
        } B;
    } CP0CFG[4];				/*00000030*/

    union {
        vuint32_t R;
        struct {
/*HEADER_ERROR: No bits identified for register CP1TYPE. Please make manual fixes to the generated header file.*/
           vuint32_t DUMMY_NOBIT:32;
        } B;
    } CP1TYPE;				/*00000040*/

    union {
        vuint32_t R;
        struct {
/*HEADER_ERROR: No bits identified for register CP1NUM. Please make manual fixes to the generated header file.*/
           vuint32_t DUMMY_NOBIT:32;
        } B;
    } CP1NUM;				/*00000044*/

    union {
        vuint32_t R;
        struct {
/*HEADER_ERROR: No bits identified for register CP1MASTER. Please make manual fixes to the generated header file.*/
           vuint32_t DUMMY_NOBIT:32;
        } B;
    } CP1MASTER;				/*00000048*/

    union {
        vuint32_t R;
        struct {
/*HEADER_ERROR: No bits identified for register CP1COUNT. Please make manual fixes to the generated header file.*/
           vuint32_t DUMMY_NOBIT:32;
        } B;
    } CP1COUNT;				/*0000004C*/

    union {
        vuint32_t R;
        struct {
/*HEADER_ERROR: No bits identified for register CP1CFG0. Please make manual fixes to the generated header file.*/
           vuint32_t DUMMY_NOBIT:32;
        } B;
    } CP1CFG[4];				/*00000050*/

    uint8_t MSCM_reserved0[928];

    union {
        vuint32_t R;
        struct {
           vuint32_t OCMC0:4;
           vuint32_t OCMC1:4;
           vuint32_t OCMC2:4;
           vuint32_t OCMPU:1;
           vuint32_t OCMTZ:1;
           vuint32_t OCMT:2;
           vuint32_t RO:1;
           vuint32_t OCMW:3;
           vuint32_t:4;
           vuint32_t OCMSZ:4;
           vuint32_t OCMSZH:1;
           vuint32_t:2;
           vuint32_t V:1;
        } B;
    } OCMDR[4];				/*00000400*/

    uint8_t MSCM_reserved1[16];

    union {
        vuint32_t R;
        struct {
           vuint32_t ERNCR:1;
           vuint32_t:7;
           vuint32_t ER1BR:1;
           vuint32_t:23;
        } B;
    } OCMDECR;				/*00000420*/

    uint8_t MSCM_reserved2[4];

    union {
        vuint32_t R;
        struct {
           vuint32_t ENC0:1;
           vuint32_t ENC1:1;
           vuint32_t ENC2:1;
           vuint32_t ENC3:1;
           vuint32_t ENC4:1;
           vuint32_t ENC5:1;
           vuint32_t ENC6:1;
           vuint32_t ENC7:1;
           vuint32_t EIB0:1;
           vuint32_t EIB1:1;
           vuint32_t EIB2:1;
           vuint32_t EIB3:1;
           vuint32_t EIB4:1;
           vuint32_t EIB5:1;
           vuint32_t EIB6:1;
           vuint32_t EIB7:1;
           vuint32_t:16;
        } B;
    } OCMDEIR;				/*00000428*/

    union {
        vuint32_t R;
        struct {
           vuint32_t ERR2BIT:8;
           vuint32_t ERR1BIT:8;
           vuint32_t FR1NC1:1;
           vuint32_t FRCNC1:1;
           vuint32_t:6;
           vuint32_t FR1IB1:1;
           vuint32_t FRCIB1:1;
           vuint32_t:2;
           vuint32_t OCMSEL:4;
        } B;
    } OCMDEGR;				/*0000042C*/

    union {
        vuint32_t R;
        struct {
/*HEADER_ERROR: No bits identified for register OCMFAR. Please make manual fixes to the generated header file.*/
           vuint32_t DUMMY_NOBIT:32;
        } B;
    } OCMFAR;				/*00000430*/

    union {
        vuint32_t R;
        struct {
           vuint32_t EFPRT:4;
           vuint32_t EFSIZ:3;
           vuint32_t EFW:1;
           vuint32_t EFMST:8;
           vuint32_t EFSYN:8;
           vuint32_t:8;
        } B;
    } OCMFTR;				/*00000434*/

    union {
        vuint32_t R;
        struct {
/*HEADER_ERROR: No bits identified for register OCMFDR. Please make manual fixes to the generated header file.*/
           vuint32_t DUMMY_NOBIT:32;
        } B;
    } OCMFDR;				/*00000438*/

    uint8_t MSCM_reserved3[964];

    union {
        vuint32_t R;
        struct {
           vuint32_t INT0:1;
           vuint32_t INT1:1;
           vuint32_t INT2:1;
           vuint32_t INT3:1;
           vuint32_t:28;
        } B;
    } IRCP0IR;				/*00000800*/

    union {
        vuint32_t R;
        struct {
           vuint32_t INT0:1;
           vuint32_t INT1:1;
           vuint32_t INT2:1;
           vuint32_t INT3:1;
           vuint32_t:28;
        } B;
    } IRCP1IR;				/*00000804*/

    uint8_t MSCM_reserved4[24];

    union {
        vuint32_t R;
        struct {
           vuint32_t INTID:2;
           vuint32_t:14;
           vuint32_t CPUTL:2;
           vuint32_t:6;
           vuint32_t TLF:2;
           vuint32_t:6;
        } B;
    } IRCPGIR;				/*00000820*/

    uint8_t MSCM_reserved5[92];

    union {
        vuint16_t R;
        struct {
           vuint16_t CP0:1;
           vuint16_t CP1:1;
           vuint16_t:13;
           vuint16_t RO:1;
        } B;
    } IRSPRC[112];				/*00000880*/

};

#define MSCM    (*(volatile struct MSCM_tag *)        0x40001000)
