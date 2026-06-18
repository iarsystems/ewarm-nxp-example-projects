#ifndef _QUADSPI_BOOT_H_
#define _QUADSPI_BOOT_H_

#include "common.h"

/*************************************
 *  QuadSPI Configuration Information
 *************************************/
#define  COMAND_SEQ_LEN       128 //interms of instruction word size

/*Quad SPI configuration structures */
typedef struct __sflash_configuration_parameter
{
  uint32_t dqs_loopback;              //Sets DQS LoopBack Mode to enable Dummy Pad MCR[24]
  uint32_t rsvd[4];
  uint32_t cs_hold_time;              //CS hold time in terms of serial clock.(for example 1 serial clock cyle)
  uint32_t cs_setup_time;             //CS setup time in terms of serial clock.(for example 1 serial clock cyle)
  uint32_t sflash_A1_size;            //interms of Bytes
  uint32_t sflash_A2_size;            //interms of Bytes
  uint32_t sflash_B1_size;            //interms of Bytes
  uint32_t sflash_B2_size;            //interms of Bytes
  uint32_t sclk_freq;                 //In  00 - 22MHz, 01 - 66MHz, 10 - 80MHz, 11 - 104MHz (only for SDR Mode)
  uint32_t rsvd5;                     //Reserved for Future Use
  uint8_t sflash_type;                //0-Single,1--Dual 2--Quad
  uint8_t sflash_port;                //0--Only Port-A,1--Both PortA and PortB
  uint8_t ddr_mode_enable;            //Enable DDR mode if set to TRUE
  uint8_t dqs_enable;                 //Enable DQS mode if set to TRUE.
  uint8_t parallel_mode_enable;       //Enable Individual or parrallel mode.
  uint8_t portA_cs1;
  uint8_t portB_cs1;
  uint8_t fsphs;
  uint8_t fsdly;
  uint8_t ddrsmp;
  uint16_t  command_seq[COMAND_SEQ_LEN]; //set of seq to perform optimum read on SFLASH as as per vendor SFLASH

  /* added to make even 0x400 size */
  /* this is required to pad the memory space when using DS-5 fromelf parser to create C-array from elf file */
  uint8_t empty[0x2C0];
}SFLASH_CONFIGURATION_PARAM,*SFLASH_CONFIGURATION_PARAM_PTR;


/*************************************
 *  DCD Data
 *************************************/
#define DCD_TAG_HEADER            (0xD2)
#define DCD_TAG_HEADER_SHIFT      (24)
#define DCD_VERSION               (0x40)


/*************************************
 *  IVT Data
 *************************************/
typedef struct _ivt_ {
    /** @ref hdr with tag #HAB_TAG_IVT, length and HAB version fields
     *  (see @ref data)
     */
    uint32_t hdr;
    /** Absolute address of the first instruction to execute from the
     *  image
     */
    uint32_t entry;
    /** Reserved in this version of HAB: should be NULL. */
    uint32_t reserved1;
    /** Absolute address of the image DCD: may be NULL. */
    uint32_t dcd;
    /** Absolute address of the Boot Data: may be NULL, but not interpreted
     *  any further by HAB
     */
    uint32_t boot_data;
    /** Absolute address of the IVT.*/
    uint32_t self;
    /** Absolute address of the image CSF.*/
    uint32_t csf;
    /** Reserved in this version of HAB: should be zero. */
    uint32_t reserved2;
} ivt;

#define IVT_MAJOR_VERSION           0x4
#define IVT_MAJOR_VERSION_SHIFT     0x4
#define IVT_MAJOR_VERSION_MASK      0xF
#define IVT_MINOR_VERSION           0x1
#define IVT_MINOR_VERSION_SHIFT     0x0
#define IVT_MINOR_VERSION_MASK      0xF

#define IVT_VERSION(major, minor)   \
  ((((major) & IVT_MAJOR_VERSION_MASK) << IVT_MAJOR_VERSION_SHIFT) |  \
  (((minor) & IVT_MINOR_VERSION_MASK) << IVT_MINOR_VERSION_SHIFT))


#define IVT_TAG_HEADER        (0xD1)       /**< Image Vector Table */
#define IVT_SIZE              byte_swap16(sizeof(ivt))
#define IVT_PAR               IVT_VERSION(IVT_MAJOR_VERSION, IVT_MINOR_VERSION)

#define IVT_HEADER          (IVT_TAG_HEADER | (IVT_SIZE << 8) | (IVT_PAR << 24))
#define IVT_RSVD            (uint32_t)(0x00000000)


/*************************************
 *  Boot Data
 *************************************/
typedef struct _boot_data_ {
  void * start;           /* boot start location */
  int size;            /* size */
  uint32_t plugin;        /* plugin flag - 1 if downloaded application is plugin */
  uint32_t placeholder;		/* placehoder to make even 0x10 size */
}BOOT_DATA_T;

/* External Variables */
extern const uint32_t device_config_data[];
extern const BOOT_DATA_T boot_data;
extern void __iar_program_start(void);


#endif
